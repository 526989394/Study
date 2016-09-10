/*++

Module Name:

	CallbackDetect.c


Abstract:

	��ݴ���������Ǽ�� ObRegisterCallbacks ע��Ĺ��Ӻ�����

	�����еĹؼ����ݽṹ�Ǵ� ObRegisterCallbacks ��������������ģ�ˮƽ���޳������⣬ϣ�����ָ����


Author:

	Fypher ( http://xiaonieblog.com/ )

	2013/12/20


--*/
#include <ntifs.h>

#include "CallbackDetect.h"


//
//	�������
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
	//	Step 1. bypass MmVerifyCallbackFunction��13 * sizeof(void*) ��Ϊ�˼���32λ��64λ��
	//
	//	Win8��ͬ�����������ƹ���http://xiaonieblog.com/?post=117
	//
	//  ��Ȼ���Լ���ǩ�����
	//
	*(PULONG)((PCHAR)DriverObject->DriverSection + 13 * sizeof(void*)) |= 0x20;

	//
	//	Step 2. ע��ٻص���Ŀ����ȡ�ûص������ϵ�һ���ڵ��ַ
	//
	//	MSDN �ϵ����Ӳ���http://code.msdn.microsoft.com/windowshardware/ObCallback-Sample-67a47841
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
	//	Step 3. ���ˣ�����ȡ�� pFakeNode �ˣ���������������
	//
	DumpObCallbacks(pFakeNode);

	//
	//	Step 4. �չ�
	//
	ObUnRegisterCallbacks(pFakeNode);
	
	KdPrintEx((DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, "[CallbackDetect] over!\r\n"));

	//	����ʧ�ܣ�����дDriverUnload
	return STATUS_UNSUCCESSFUL;

}



//
//	���ǵļٻص�����������ȡ������
//
OB_PREOP_CALLBACK_STATUS
FakeCallBack (
	PVOID RegistrationContext,
	POB_PRE_OPERATION_INFORMATION PreInfo
)
{
	// ֱ�ӷ��ؼ���
	return OB_PREOP_SUCCESS;
}




VOID
DumpObCallbacks (
	IN struct CALLBACK_NODE* pAnyCallbackNode
)
/*++

Routine Description:

    ����ϵͳ�� ObRegisterCallbacks ע��ĺ���


Arguments:
	
    pAnyCallbackNode - ����һ�� Callback �ڵ�


Return Value:
   
    None.


Comments:

    �ú�����������û�п��Ǽ��������⣬��ʵ��ʹ������Ҫע�⡣


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
			// ����ͷ�ڵ�
			pCallbackBody = CONTAINING_RECORD(pCallbackBody->ListEntry.Flink, struct CALLBACK_BODY, ListEntry);
			continue;
		}
		
		// ȡ�� Altitude �ַ�������Ϊ pCallbackNode->wcsAltitude ��һ���� 0 ��β
		wcsncpy(wzAltitude, pCallbackNode->wcsAltitude, sizeof(wzAltitude) / sizeof(WCHAR) - 1);
		wzAltitude[sizeof(wzAltitude) / sizeof(WCHAR) - 1] = 0;
		//
		//	��� pCallbackNode �ڵ���Ϣ
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
			
			// ȡ�� p->pObjectType �����ƣ�
			RtlInitEmptyUnicodeString(&ObjectName, wzObjectNameBuffer, sizeof(wzObjectNameBuffer));
			status = MyGetObjectName(p->pObjectType, &ObjectName);
			if (!NT_SUCCESS(status)) {
				ObjectName.Length = 0;
			}

			// ȡ�� p->pPreCallbackRoutine ���ڵ�ģ����
			bRet = GetModuleNameByAddr( p->pPreCallbackRoutine, 
										szPreModulePath,
										sizeof(szPreModulePath)
									);
			if (!bRet) {
				szPreModulePath[0] = 0;
			}
			
			// ȡ�� p->pPostCallbackRoutine ���ڵ�ģ����
			bRet = GetModuleNameByAddr( p->pPostCallbackRoutine, 
										szPostModulePath,
										sizeof(szPostModulePath)
									);
			if (!bRet) {
				szPostModulePath[0] = 0;
			}
			
			//
			//	��� CallbackBody ��Ϣ
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
//  һЩ�������������Ի�ȡģ�����Ƶ�
//

BOOLEAN
GetModuleNameByAddr(
	IN PVOID pAddr,
	OUT PCHAR strModuleName,
	IN	ULONG ulModuleNameSize
)
/*++

Routine Description:

	��ȡ��ַ���ڵ�ģ����


Arguments:

	pAddr - ����һ����ַ

	strModuleName - ��ȡ��ַ���ڵ�ģ����

	ulModuleNameSize - strModuleName �������ĳ���


Return Value:
	
	�ɹ�����TRUE��ʧ�ܷ���FALSE��


Comments:

	strModuleName �ɵ��÷����䣬���С����ulModuleNameSize����ô��ڻ����256��


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

	��ȡϵͳģ����Ϣ


Arguments:

	None.


Return Value:
	
	�ɹ��򷵻�ϵͳģ����Ϣ��ʧ�ܷ��� NULL ��


Comments:

	�ú������ص�ϵͳģ����Ϣʹ����Ϻ���Ҫ�ɵ����߽����ͷţ�ExFreePool����


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
	UCHAR buf[128];	// �������ȵĶ������Ͳ�ȡ�ˣ��Ͼ�ȡ���� ObjectType ���ַ���
	POBJECT_NAME_INFORMATION pNameInfo = (POBJECT_NAME_INFORMATION)buf;
	ULONG ulRet;

	status = ObQueryNameString(pObject, pNameInfo, sizeof(buf), &ulRet);
	
	if (NT_SUCCESS(status)) {    // ������ STATUS_INFO_LENGTH_MISMATCH ���������
		RtlCopyUnicodeString(pObjectName, &pNameInfo->Name);
	}

	return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////