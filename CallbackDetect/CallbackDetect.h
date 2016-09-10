/*++

Module Name:

	CallbackDetect.h


Abstract:

	��ݴ���������Ǽ�� ObRegisterCallbacks ע��Ĺ��Ӻ�����

	�����еĹؼ����ݽṹ�Ǵ� ObRegisterCallbacks ��������������ģ�ˮƽ���޳������⣬ϣ�����ָ����


Author:

	Fypher ( http://xiaonieblog.com/ )

	2013/12/20


--*/
#ifndef _CALLBACKDETECT_H_
#define _CALLBACKDETECT_H_

struct CALLBACK_BODY {

	// ���е� CALLBACK_NODE ͨ�����������һ��
	LIST_ENTRY	ListEntry;

	// ע��ص�ʱ�趨�� OB_OPERATION_REGISTRATION.Operations��
	// �� OB_OPERATION_HANDLE_CREATE ��Щ����
	ULONG		ulOperations;

	// �����ƺ�ʼ���� 1
	ULONG		ulXXX_1;

	// ָ��� CallbackBody ��Ӧ�� CallbackNode
	struct CALLBACK_NODE* pCallbackNode;

	// ע��ص�ʱ�趨�� OB_OPERATION_REGISTRATION.pObjectType
	// �� PsProcessType��PsThreadType ��Щ����
	PVOID		pObjectType;

	// �����ע��ص�ʱ�趨�� PreCallbackRoutine ������ַ
	PVOID		pPreCallbackRoutine;

	// �����ע��ص�ʱ�趨�� PostCallbackRoutine ������ַ
	PVOID		pPostCallbackRoutine;

	// �����ƺ��Ǹ����ü���
	ULONG		ulRefCount;

};

struct CALLBACK_NODE {
	// �汾�ţ�Ŀǰ�� 0x100, ��ͨ�� ObGetFilterVersion ��ȡ��ֵ
	USHORT		usVersion;

	// ���ڵ����ж��ٸ� CallbackOperator�������ٸ� CallbackBodies
	USHORT		usCallbackBodyCount;

	// ע��ص�ʱ�趨�� OB_CALLBACK_REGISTRATION.RegistrationContext
	// �� PreCallbackRoutine �� PostCallbackRoutine �Ĳ���
	PVOID		pContext;

	// �����ƺ��� 0x000C000C/0x00080008 һ���ֵ
	ULONG		ulXXX_000X000X;

	// ָ�� Altitude �ַ���
	// ע������ַ�����Ϊ��󳤶��Ǻ㶨�ģ����Բ�һ���� NULL ��β
	PWCHAR		wcsAltitude;

	// CALLBACK_BODY ���飬��Ԫ�ظ���Ϊ ulCallbackCount
	struct CALLBACK_BODY CallbackBodies[1];
	
	//
	// CALLBACK_BODY����󣬽����� Altitude �ַ�����һЩ��������
	//
};



//
//	���ǵļٻص�����������ȡ������
//
OB_PREOP_CALLBACK_STATUS
FakeCallBack (
	PVOID RegistrationContext,
	POB_PRE_OPERATION_INFORMATION PreInfo
);


//
//	����Callbacks�ĺ���
//
VOID
DumpObCallbacks (
	IN struct CALLBACK_NODE* pAnyCallbackNode
);


//
//	�������
//
NTSTATUS
DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath
);

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  һЩ�����͸������������Ի�ȡģ�����Ƶ�
//
#define ALLOC_TAG 'tdbC'

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemNotImplemented1,
	SystemProcessesAndThreadsInformation,
	SystemCallCounts,
	SystemConfigurationInformation,
	SystemProcessorTimes,
	SystemGlobalFlag,
	SystemNotImplemented2,
	SystemModuleInformation,
	SystemLockInformation,
	SystemNotImplemented3,
	SystemNotImplemented4,
	SystemNotImplemented5,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPagefileInformation,
	SystemInstructionEmulationCounts,
	SystemInvalidInfoClass1,
	SystemCacheInformation,
	SystemPoolTagInformation,
	SystemProcessorStatistics,
	SystemDpcInformation,
	SystemNotImplemented6,
	SystemLoadImage,
	SystemUnloadImage,
	SystemTimeAdjustment,
	SystemNotImplemented7,
	SystemNotImplemented8,
	SystemNotImplemented9,
	SystemCrashDumpInformation,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemLoadAndCallImage,
	SystemPrioritySeparation,
	SystemNotImplemented10,
	SystemNotImplemented11,
	SystemInvalidInfoClass2,
	SystemInvalidInfoClass3,
	SystemTimeZoneInformation,
	SystemLookasideInformation,
	SystemSetTimeSlipEvent,
	SystemCreateSession,
	SystemDeleteSession,
	SystemInvalidInfoClass4,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemAddVerifier,
	SystemSessionProcessesInformation
}SYSTEM_INFORMATION_CLASS;

typedef struct _RTL_PROCESS_MODULE_INFORMATION {
    HANDLE Section;                 // Not filled in
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR  FullPathName[ 256 ];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES {
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[ 1 ];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;

NTSTATUS 
NTAPI 
ZwQuerySystemInformation (
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
);

PRTL_PROCESS_MODULES GetSystemModules();


BOOLEAN
GetModuleNameByAddr(
	IN PVOID pAddr,
	OUT PCHAR strModuleName,
	IN	ULONG ulModuleNameSize
);

NTSTATUS MyGetObjectName (IN PVOID pObject, OUT PUNICODE_STRING pObjectName);
////////////////////////////////////////////////////////////////////////////////////////////////

#endif
