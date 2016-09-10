/*++

Module Name:

	CallbackDetect.h


Abstract:

	这份代码的作用是检测 ObRegisterCallbacks 注册的钩子函数。

	代码中的关键数据结构是从 ObRegisterCallbacks 函数中逆向出来的，水平有限出错难免，希望大家指正！


Author:

	Fypher ( http://xiaonieblog.com/ )

	2013/12/20


--*/
#ifndef _CALLBACKDETECT_H_
#define _CALLBACKDETECT_H_

struct CALLBACK_BODY {

	// 所有的 CALLBACK_NODE 通过这个链表串在一起
	LIST_ENTRY	ListEntry;

	// 注册回调时设定的 OB_OPERATION_REGISTRATION.Operations，
	// 即 OB_OPERATION_HANDLE_CREATE 这些东西
	ULONG		ulOperations;

	// 这里似乎始终是 1
	ULONG		ulXXX_1;

	// 指向该 CallbackBody 对应的 CallbackNode
	struct CALLBACK_NODE* pCallbackNode;

	// 注册回调时设定的 OB_OPERATION_REGISTRATION.pObjectType
	// 即 PsProcessType，PsThreadType 这些东西
	PVOID		pObjectType;

	// 这就是注册回调时设定的 PreCallbackRoutine 函数地址
	PVOID		pPreCallbackRoutine;

	// 这就是注册回调时设定的 PostCallbackRoutine 函数地址
	PVOID		pPostCallbackRoutine;

	// 这里似乎是个引用计数
	ULONG		ulRefCount;

};

struct CALLBACK_NODE {
	// 版本号，目前是 0x100, 可通过 ObGetFilterVersion 获取该值
	USHORT		usVersion;

	// 本节点上有多少个 CallbackOperator，即多少个 CallbackBodies
	USHORT		usCallbackBodyCount;

	// 注册回调时设定的 OB_CALLBACK_REGISTRATION.RegistrationContext
	// 即 PreCallbackRoutine 和 PostCallbackRoutine 的参数
	PVOID		pContext;

	// 这里似乎是 0x000C000C/0x00080008 一类的值
	ULONG		ulXXX_000X000X;

	// 指向 Altitude 字符串
	// 注意这个字符串因为最大长度是恒定的，所以不一定以 NULL 结尾
	PWCHAR		wcsAltitude;

	// CALLBACK_BODY 数组，其元素个数为 ulCallbackCount
	struct CALLBACK_BODY CallbackBodies[1];
	
	//
	// CALLBACK_BODY数组后，紧接着 Altitude 字符串和一些其它数据
	//
};



//
//	我们的假回调函数，用来取得链表
//
OB_PREOP_CALLBACK_STATUS
FakeCallBack (
	PVOID RegistrationContext,
	POB_PRE_OPERATION_INFORMATION PreInfo
);


//
//	遍历Callbacks的函数
//
VOID
DumpObCallbacks (
	IN struct CALLBACK_NODE* pAnyCallbackNode
);


//
//	驱动入口
//
NTSTATUS
DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath
);

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  一些声明和辅助函数，用以获取模块名称等
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
