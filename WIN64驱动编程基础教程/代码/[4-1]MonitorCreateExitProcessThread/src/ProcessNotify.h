
NTKERNELAPI PCHAR PsGetProcessImageFileName(PEPROCESS Process);
NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(HANDLE ProcessId, PEPROCESS *Process);

/*typedef struct _PS_CREATE_NOTIFY_INFO {
  SIZE_T              Size;
  union {
    ULONG  Flags;
    struct {
      ULONG FileOpenNameAvailable  :1;
      ULONG Reserved  :31;
    };
  };
  HANDLE              ParentProcessId;
  CLIENT_ID           CreatingThreadId;
  struct _FILE_OBJECT  *FileObject;
  PCUNICODE_STRING    ImageFileName;
  PCUNICODE_STRING    CommandLine;
  NTSTATUS            CreationStatus;
} PS_CREATE_NOTIFY_INFO, *PPS_CREATE_NOTIFY_INFO;*/

/*VOID MyCreateProcessNotifyEx	//������
(
	__inout   PEPROCESS Process,
	__in      HANDLE ProcessId,
	__in_opt  PPS_CREATE_NOTIFY_INFO CreateInfo
)
{
	if(CreateInfo==NULL)
		DbgPrint("[monitor_create_process_x64] Process exit: %s",PsGetProcessImageFileName(Process));
	else
		DbgPrint("[monitor_create_process_x64] Process create: %wZ",CreateInfo->CommandLine);
}*/

PCHAR GetProcessNameByProcessId(HANDLE ProcessId)
{
	NTSTATUS st=STATUS_UNSUCCESSFUL;
	PEPROCESS ProcessObj=NULL;
	PCHAR string=NULL;
	st = PsLookupProcessByProcessId(ProcessId, &ProcessObj);
	if(NT_SUCCESS(st))
	{
		string = PsGetProcessImageFileName(ProcessObj);
		ObfDereferenceObject(ProcessObj);
	}
	return string;
}

VOID MyCreateProcessNotifyEx
(
	__inout   PEPROCESS Process,
	__in      HANDLE ProcessId,
	__in_opt  PPS_CREATE_NOTIFY_INFO CreateInfo
)
{
	NTSTATUS st=0;
	HANDLE hProcess=NULL;
	OBJECT_ATTRIBUTES oa={0};
	CLIENT_ID ClientId={0};
	char xxx[16]={0};
	if(CreateInfo!=NULL)	//���̴����¼�
	{
		DbgPrint("[monitor_create_process_x64][%ld]%s��������: %wZ",
				CreateInfo->ParentProcessId,
				GetProcessNameByProcessId(CreateInfo->ParentProcessId),
				CreateInfo->ImageFileName);
		strcpy(xxx,PsGetProcessImageFileName(Process));
		if(!_stricmp(xxx,"calc.exe"))
		{
			DbgPrint("��ֹ�������������̣�");
			CreateInfo->CreationStatus=STATUS_UNSUCCESSFUL;	//��ֹ��������
		}
	}
	else
	{
		DbgPrint("[monitor_create_process_x64]�����˳�: %s",PsGetProcessImageFileName(Process));
	}
}

VOID MyCreateThreadNotify
(
	IN HANDLE  ProcessId,
	IN HANDLE  ThreadId,
	IN BOOLEAN  Create
)
{
	if(Create)
		DbgPrint("[monitor_create_process_x64]�̴߳���! PID=%ld;TID=%ld",ProcessId,ThreadId);
	else
		DbgPrint("[monitor_create_process_x64]�߳��˳�! PID=%ld;TID=%ld",ProcessId,ThreadId);
}