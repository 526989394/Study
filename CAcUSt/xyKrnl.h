#pragma  once
//xy��ddk�Ļ�������ͷ�ļ�
#ifdef __cplusplus
extern "C"
{
#endif
	//////////////////////////////////Ҫ������ͷ�ļ�/////////////////////////////////////////
#include <ntddk.h>

	/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
/////////////////////////////////////�����ض���//////////////////////////////////////
typedef UCHAR BYTE,byte;
typedef unsigned int UINT,uint;
typedef ULONG DWORD,dword;
//////////////////////////////////////�궨��/////////////////////////////////////////
#define PAGEDCODE code_seg("PAGE")
#define NONPAGEDCODE code_seg()     //Ĭ��λ�ڷǷ�ҳ�������
#define INITCODE code_seg("INIT")

#define PAGEDATA data_seg("PAGE")
#define NONPAGEDATA data_seg()		//Ĭ��λ�ڷǷ�ҳ���ݽ���
#define INITDATA data_seg("INIT")

#define FAILED(status)     ((status)<0)
#define SUCCEEDED(status)  ((status)>=0)
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 
#define ArrayCnt(arrname) (sizeof((arrname))/sizeof((arrname)[0]))
#define  ASSERT_SUPPORT_PAGE_FAULT   PAGED_CODE()   //ȷ������IRQL֧��ȱҳ�ж�
#define  DEVICE_IOCONTROLCODE_USER 0X800 //����WM_USER
#define  MakeIoCtrlCode(ctlcode)	CTL_CODE(FILE_DEVICE_UNKNOWN,ctlcode,METHOD_BUFFERED,FILE_ANY_ACCESS)  //����һ��ϵͳ���巽ʽ���豸������

#define		IRP_MJ_OPEN		IRP_MJ_CREATE   //CreateFile���豸ʱ������������һ���豸������
#define	  	IRP_MJ_CANCEL_PENDING_IRPS				IRP_MJ_CLEANUP	//CancelIO������ȡ������δ��ɵ�IRP����
#define		xyKrnlGetMdlSva(pMDL)	MmGetSystemAddressForMdlSafe(pMDL,NormalPagePriority); //��ȡMDL�ڴ���������ֱ��ӳ���ϵͳ��ַ
#define		xyKrnlGetMdlUva(pMDL)	MmGetMdlVirtualAddress(pMDL)	//��ȡMDL�ڴ��������е��û��ռ��е������ַ������ȫ
#define		xyKrnlGetMdlLength(pMDL)	MmGetMdlByteCount(pMDL)	//��ȡMDL�ڴ��������еĳ���
#define     xyKrnlGetCurProcessHandle()   NtCurrentProcess()

//����������о����Ƿ������������ϻص��ķ���ֵ
#define  STATUS_CONTINUE_UP_CALLBACK(pirp)    ((pirp)->PendingReturned?IoMarkIrpPending(pirp):NULL,STATUS_SUCCESS)    //������־λ��������ϻص��������
#define  STATUS_NEED_MORE_IoCompleteRequest  STATUS_MORE_PROCESSING_REQUIRED   //��Ҫ�����irp�����Ȼ�����IoCompleteRequest�ټ������ϻص�
#define  STATUS_NEED_MORE_PROCESSS    STATUS_MORE_PROCESSING_REQUIRED     //��Ҫ�����irp�����Ȼ�����IoCompleteRequest�ټ������ϻص�
#define  STATUS_MARK_AND_PENDING(pIrp)   (IoMarkIrpPending(pIrp),STATUS_PENDING)  //����ǲ�����б귴����״̬��һ��������̬����

typedef IO_STACK_LOCATION	IRP_STACK_LOCATION;
typedef LIST_ENTRY LIST_NODE;
typedef LIST_ENTRY32 LIST_NODE32;
typedef LIST_ENTRY64 LIST_NODE64;

///////////////////////////////////�Զ���ĳ����ṹ������////////////////////////////////////////
// �Զ�����豸��չ��Ϣ�ṹ��
// typedef struct _DEVICE_EXTENSION
// {
// 	DEVICE_OBJECT* pDevice;//�豸����ĵ�ַ
// 	UNICODE_STRING ustrDeviceName;//�豸���������
// 	UNICODE_STRING ustrSymLinkName;//�豸����ķ�����������
// 	DEVICE_OBJECT* NextStackDevice;//�豸ջ�и��ӵ��²��豸
// }DEVICE_EXTENSION,*PDEVICE_EXTENSION;


///////////////////////////////////DDKδ�����ĺ�������////////////////////////////////////////
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














