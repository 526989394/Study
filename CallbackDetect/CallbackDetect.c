/*++

Module Name:

	CallbackDetect.c


Abstract:

	这份代码的作用是检测 ObRegisterCallbacks 注册的钩子函数。

	代码中的关键数据结构是从 ObRegisterCallbacks 函数中逆向出来的，水平有限出错难免，希望大家指正！


Author:

	Fypher ( http://xiaonieblog.com/ )

	2013/12/20


--*/
#include <ntifs.h>

#include "CallbackDetect.h"


//
//	驱动入口
//
NTSTATUS DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS status;

	OB_CALLBACK_REGISTRATION ObCallbackReg;
	OB_OPERATION_REGISTRATION ObOperationReg;

	struct CALLBACK_NODE* pFakeNode;

	KdPrintEx((DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, "[CallbackDetect] start!\r\n\r\n"));

	//
	//	Step 1. bypass MmVerifyCallbackFunction，13 * sizeof(void*) 是为了兼容32位和64位。
	//
	//	Win8中同样可以这样绕过：http://xiaonieblog.com/?post=117
	//
	//  当然，自己有签名最好
	//
	*(PULONG)((PCHAR)DriverObject->DriverSection + 13 * sizeof(void*)) |= 0x20;

	//
	//	Step 2. 注册假回调，目的是取得回调链表上的一个节点地址
	//
	//	MSDN 上的例子不错：http://code.msdn.microsoft.com/windowshardware/ObCallback-Sample-67a47841
	//
	memset(&ObCallbackReg, 0, sizeof(ObCallbackReg));
	memset(&ObOperationReg, 0, sizeof(ObOperationReg));

	ObCallbackReg.Version = OB_FLT_REGISTRATION_VERSION;
	ObCallbackReg.OperationRegistrationCount = 1;
	RtlInitUnicodeString (&ObCallbackReg.Altitude, L"1000");
	ObCallbackReg.RegistrationContext = NULL;
	ObCallbackReg.OperationRegistration = &ObOperationReg;

	ObOperationReg.ObjectType = PsProcessType;
	ObOperationReg.Operations = OB_OPERATION_HANDLE_CREATE; 
	ObOperationReg.PreOperation = (POB_PRE_OPERATION_CALLBACK)FakeCallBack;

	status = ObRegisterCallbacks(&ObCallbackReg, &pFakeNode);
	if (!NT_SUCCESS(status)) {
		KdPrintEx((DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, "[CallbackDetect] ObRegisterCallbacks failed! errcode: 0x%x\r\n", status));
		return status;
	}

	//
	//	Step 3. 好了，我们取得 pFakeNode 了，接下来遍历链表
	//
	DumpObCallbacks(pFakeNode);

	//
	//	Step 4. 收工
	//
	ObUnRegisterCallbacks(pFakeNode);
	
	KdPrintEx((DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, "[CallbackDetect] over!\r\n"));

	//	返回失败，懒得写DriverUnload
	return STATUS_UNSUCCESSFUL;

}



//
//	我们的假回调函数，用来取得链表
//
OB_PREOP_CALLBACK_STATUS
FakeCallBack (
	PVOID RegistrationContext,
	POB_PRE_OPERATION_INFORMATION PreInfo
)
{
	// 直接返回即可
	return OB_PREOP_SUCCESS;
}




VOID
DumpObCallbacks (
	IN struct CALLBACK_NODE* pAnyCallbackNode
)
/*++

Routine Description:

    遍历系统中 ObRegisterCallbacks 注册的函数


Arguments:
	
    pAnyCallbackNode - 任意一个 Callback 节点


Return Value:
   
    None.


Comments:

    该函数遍历链表并没有考虑加锁的问题，在实际使用中需要注意。


--*/
{
	struct CALLBACK_BODY* pCallbackBody = pAnyCallbackNode->CallbackBodies;
	do {
		USHORT i;
		WCHAR wzAltitude[7];
		struct CALLBACK_NODE* pCallbackNode = pCallbackBody->pCallbackNode;
		
		if (!MmIsAddressValid(pCallbackNode)                 ||
			pCallbackNode->usVersion != ObGetFilterVersion() ||
			pCallbackNode->usCallbackBodyCount < 1             )
		{
			// 跳过头节点
			pCallbackBody = CONTAINING_RECORD(pCallbackBody->ListEntry.Flink, struct CALLBACK_BODY, ListEntry);
			continue;
		}
		
		// 取得 Altitude 字符串，因为 pCallbackNode->wcsAltitude 不一定以 0 结尾
		wcsncpy(wzAltitude, pCallbackNode->wcsAltitude, sizeof(wzAltitude) / sizeof(WCHAR) - 1);
		wzAltitude[sizeof(wzAltitude) / sizeof(WCHAR) - 1] = 0;
		//
		//	输出 pCallbackNode 节点信息
		//
		DbgPrintEx(	DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL,
					"[CallbackDetect] Found a Callback Node at 0x%p, context: 0x%p, altitude: %ws, %d Callback Bodies:\r\n",
					pCallbackNode, 
					pCallbackNode->pContext,
					wzAltitude,
					pCallbackNode->usCallbackBodyCount
				);

		for (i = 0; i < pCallbackNode->usCallbackBodyCount; ++i) {
			NTSTATUS status;
			UNICODE_STRING ObjectName;
			WCHAR wzObjectNameBuffer[64];
			CHAR szPreModulePath[256], szPostModulePath[256];
			BOOLEAN bRet;

			struct CALLBACK_BODY* p = pCallbackNode->CallbackBodies + i;
			
			// 取得 p->pObjectType 的名称：
			RtlInitEmptyUnicodeString(&ObjectName, wzObjectNameBuffer, sizeof(wzObjectNameBuffer));
			status = MyGetObjectName(p->pObjectType, &ObjectName);
			if (!NT_SUCCESS(status)) {
				ObjectName.Length = 0;
			}

			// 取得 p->pPreCallbackRoutine 所在地模块名
			bRet = GetModuleNameByAddr( p->pPreCallbackRoutine, 
										szPreModulePath,
										sizeof(szPreModulePath)
									);
			if (!bRet) {
				szPreModulePath[0] = 0;
			}
			
			// 取得 p->pPostCallbackRoutine 所在地模块名
			bRet = GetModuleNameByAddr( p->pPostCallbackRoutine, 
										szPostModulePath,
										sizeof(szPostModulePath)
									);
			if (!bRet) {
				szPostModulePath[0] = 0;
			}
			
			//
			//	输出 CallbackBody 信息
			//
			DbgPrintEx(	DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL,
						"[CallbackDetect]     No.%d Callback Body at: 0x%p\r\n"
						"[CallbackDetect]         ObjectType: %wZ, operations: %x\r\n"
						"[CallbackDetect]         PreCallbackRoutine: 0x%p (%s)\r\n"
						"[CallbackDetect]         PostCallbackRoutine: 0x%p (%s)\r\n\r\n",
						i + 1, p, &ObjectName, p->ulOperations,
						p->pPreCallbackRoutine, szPreModulePath,
						p->pPostCallbackRoutine, szPostModulePath
					);
		}
		
		pCallbackBody = CONTAINING_RECORD(pCallbackBody->ListEntry.Flink, struct CALLBACK_BODY, ListEntry);
	
	} while ( pCallbackBody != pAnyCallbackNode->CallbackBodies);
}



////////////////////////////////////////////////////////////////////////////////////////////////
//
//  一些辅助函数，用以获取模块名称等
//

BOOLEAN
GetModuleNameByAddr(
	IN PVOID pAddr,
	OUT PCHAR strModuleName,
	IN	ULONG ulModuleNameSize
)
/*++

Routine Description:

	获取地址所在地模块名


Arguments:

	pAddr - 输入一个地址

	strModuleName - 获取地址所在的模块名

	ulModuleNameSize - strModuleName 缓冲区的长度


Return Value:
	
	成功返回TRUE，失败返回FALSE。


Comments:

	strModuleName 由调用方分配，其大小（即ulModuleNameSize）最好大于或等于256。


--*/
{
	PRTL_PROCESS_MODULES pSysMods;
	PRTL_PROCESS_MODULE_INFORMATION pModInfo;
	ULONG i, ulModNum;
	BOOLEAN bRet = FALSE;
	
	PAGED_CODE();

	if (pAddr == 0) {
		return FALSE;
	}

	pSysMods = GetSystemModules();
	if (!pSysMods)
		return FALSE;

	ulModNum = pSysMods->NumberOfModules;
	pModInfo = pSysMods->Modules;

	for (i = 0; i < ulModNum; ++i) {
		if (pModInfo->ImageBase <= pAddr && 
			(PCHAR)pModInfo->ImageBase + pModInfo->ImageSize >= (PCHAR)pAddr)
		{
			strncpy(strModuleName, pModInfo->FullPathName, ulModuleNameSize - 1);
			strModuleName[ulModuleNameSize - 1] = 0;
			bRet = TRUE;
			break;
		}
		pModInfo += 1;
	}

	ExFreePool(pSysMods);

	return bRet;
}


PRTL_PROCESS_MODULES GetSystemModules() 
/*++

Routine Description:

	获取系统模块信息


Arguments:

	None.


Return Value:
	
	成功则返回系统模块信息，失败返回 NULL 。


Comments:

	该函数返回的系统模块信息使用完毕后，需要由调用者进行释放（ExFreePool）。


--*/
{
	PRTL_PROCESS_MODULES pSysMods = NULL;
	ULONG ulSize = 512;
	NTSTATUS status;
 	
	// PAGED_CODE();
	
	while ( TRUE )
	{
		pSysMods = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(PagedPool, ulSize, ALLOC_TAG);
		if ( !pSysMods )
			return NULL;

		status = ZwQuerySystemInformation(SystemModuleInformation, pSysMods, ulSize, &ulSize);
		if ( status != STATUS_INFO_LENGTH_MISMATCH )
			break;

		ExFreePool(pSysMods);
	}
	
	if ( !NT_SUCCESS(status) ) {
		ExFreePool(pSysMods);
		pSysMods = NULL;
	}

	return pSysMods;
}

NTSTATUS MyGetObjectName (IN PVOID pObject, OUT PUNICODE_STRING pObjectName)
{
	NTSTATUS status;
	UCHAR buf[128];	// 超出长度的对象名就不取了，毕竟取得是 ObjectType 的字符串
	POBJECT_NAME_INFORMATION pNameInfo = (POBJECT_NAME_INFORMATION)buf;
	ULONG ulRet;

	status = ObQueryNameString(pObject, pNameInfo, sizeof(buf), &ulRet);
	
	if (NT_SUCCESS(status)) {    // 不考虑 STATUS_INFO_LENGTH_MISMATCH 这种情况了
		RtlCopyUnicodeString(pObjectName, &pNameInfo->Name);
	}

	return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////