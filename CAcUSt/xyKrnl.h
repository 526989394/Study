#pragma  once
//xy的ddk的基础公共头文件
#ifdef __cplusplus
extern "C"
{
#endif
	//////////////////////////////////要包含的头文件/////////////////////////////////////////
#include <ntddk.h>

	/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
/////////////////////////////////////类型重定义//////////////////////////////////////
typedef UCHAR BYTE,byte;
typedef unsigned int UINT,uint;
typedef ULONG DWORD,dword;
//////////////////////////////////////宏定义/////////////////////////////////////////
#define PAGEDCODE code_seg("PAGE")
#define NONPAGEDCODE code_seg()     //默认位于非分页代码节中
#define INITCODE code_seg("INIT")

#define PAGEDATA data_seg("PAGE")
#define NONPAGEDATA data_seg()		//默认位于非分页数据节中
#define INITDATA data_seg("INIT")

#define FAILED(status)     ((status)<0)
#define SUCCEEDED(status)  ((status)>=0)
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 
#define ArrayCnt(arrname) (sizeof((arrname))/sizeof((arrname)[0]))
#define  ASSERT_SUPPORT_PAGE_FAULT   PAGED_CODE()   //确保当期IRQL支持缺页中断
#define  DEVICE_IOCONTROLCODE_USER 0X800 //类似WM_USER
#define  MakeIoCtrlCode(ctlcode)	CTL_CODE(FILE_DEVICE_UNKNOWN,ctlcode,METHOD_BUFFERED,FILE_ANY_ACCESS)  //生成一个系统缓冲方式的设备控制码

#define		IRP_MJ_OPEN		IRP_MJ_CREATE   //CreateFile打开设备时会生成这样的一个设备打开请求
#define	  	IRP_MJ_CANCEL_PENDING_IRPS				IRP_MJ_CLEANUP	//CancelIO会请求取消所有未完成的IRP请求
#define		xyKrnlGetMdlSva(pMDL)	MmGetSystemAddressForMdlSafe(pMDL,NormalPagePriority); //获取MDL内存描述符中直接映射的系统地址
#define		xyKrnlGetMdlUva(pMDL)	MmGetMdlVirtualAddress(pMDL)	//获取MDL内存描述符中的用户空间中的虚拟地址，不安全
#define		xyKrnlGetMdlLength(pMDL)	MmGetMdlByteCount(pMDL)	//获取MDL内存描述符中的长度
#define     xyKrnlGetCurProcessHandle()   NtCurrentProcess()

//再完成例程中决定是否立即继续向上回调的返回值
#define  STATUS_CONTINUE_UP_CALLBACK(pirp)    ((pirp)->PendingReturned?IoMarkIrpPending(pirp):NULL,STATUS_SUCCESS)    //传播标志位后继续向上回调完成例程
#define  STATUS_NEED_MORE_IoCompleteRequest  STATUS_MORE_PROCESSING_REQUIRED   //需要更多的irp处理后，然后调用IoCompleteRequest再继续向上回调
#define  STATUS_NEED_MORE_PROCESSS    STATUS_MORE_PROCESSING_REQUIRED     //需要更多的irp处理后，然后调用IoCompleteRequest再继续向上回调
#define  STATUS_MARK_AND_PENDING(pIrp)   (IoMarkIrpPending(pIrp),STATUS_PENDING)  //在派遣例程中标反阻塞状态，一步以阻塞态返回

typedef IO_STACK_LOCATION	IRP_STACK_LOCATION;
typedef LIST_ENTRY LIST_NODE;
typedef LIST_ENTRY32 LIST_NODE32;
typedef LIST_ENTRY64 LIST_NODE64;

///////////////////////////////////自定义的常见结构体类型////////////////////////////////////////
// 自定义的设备扩展信息结构体
// typedef struct _DEVICE_EXTENSION
// {
// 	DEVICE_OBJECT* pDevice;//设备对象的地址
// 	UNICODE_STRING ustrDeviceName;//设备对象的名称
// 	UNICODE_STRING ustrSymLinkName;//设备对象的符号链接名称
// 	DEVICE_OBJECT* NextStackDevice;//设备栈中附加的下层设备
// }DEVICE_EXTENSION,*PDEVICE_EXTENSION;


///////////////////////////////////DDK未公开的函数声明////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

extern POBJECT_TYPE IoDriverObjectType;
extern POBJECT_TYPE IoDeviceObjectType;
extern POBJECT_TYPE* IoFileObjectType;

extern POBJECT_TYPE IoAdapterObjectType;
extern POBJECT_TYPE IoDeviceHandlerObjectType;

extern POBJECT_TYPE* ExEventObjectType;
extern POBJECT_TYPE* ExSemaphoreObjectType;

extern POBJECT_TYPE ExDesktopObjectType;
extern POBJECT_TYPE ExWindowStationObjectType;
extern POBJECT_TYPE MmSectionObjectType;
extern POBJECT_TYPE SeTokenObjectType;
extern POBJECT_TYPE LpcPortObjectType;
NTKERNELAPI  NTSTATUS ObReferenceObjectByName(PUNICODE_STRING ObjName,ULONG attr,PACCESS_STATE PassedAccessState OPTIONAL,ACCESS_MASK AccessMask OPTIONAL,POBJECT_TYPE ObjType,KPROCESSOR_MODE AccessMode,OUT PVOID Context OPTIONAL,OUT PVOID* ppObj);
	
#ifdef __cplusplus
}
#endif














