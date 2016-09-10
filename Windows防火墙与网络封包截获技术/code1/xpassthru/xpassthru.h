/*����������������������������������������������������������������������������
	file: xpassthru.h
*/

#define MAX_BUNDLEID_LENGTH 50		
#define TAG 'ImPa'

/*����������������������������������������������������������������������������
	Ԥ����
*/
typedef struct _ADAPT ADAPT, *PADAPT;

extern
NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT			DriverObject,
	IN	PUNICODE_STRING			RegistryPath
	);

#define DBGPRINT(Fmt)										\
	{														\
		DbgPrint("*** XPASSHTRU.SYS ***");					\
		DbgPrint (Fmt);										\
	}

/*����������������������������������������������������������������������������
	Protocol ����ı�������
*/
typedef struct _ProtRsvd
{
	PNDIS_PACKET	OriginalPkt;
} RSVD, *PRSVD;

typedef enum 
{
	Passthru_Invalid,
	Passthru_SetPower,
	Passthru_Unbind

} PASSSTHRU_EVENT_CODE, *PPASTHRU_EVENT_CODE; 

typedef struct _PASSTHRU_EVENT
{
	NDIS_EVENT Event;
	PASSSTHRU_EVENT_CODE Code;

} PASSTHRU_EVENT, *PPASSTHRU_EVENT;


/*����������������������������������������������������������������������������
	adapter ���ݽṹ
*/
typedef struct _ADAPT
{
	PADAPT			Next;
	
	NDIS_HANDLE		BindingHandle;	// ָ��ײ�miniport�豸
	NDIS_HANDLE		MiniportHandle;	// ָ��passthru��miniport�豸
	NDIS_HANDLE		SendPacketPoolHandle;
	NDIS_HANDLE		RecvPacketPoolHandle;
	NDIS_STATUS		Status;			// ��״̬
	NDIS_EVENT		Event;			// ��������Open/Close��bind/halt����ͬ��
	NDIS_MEDIUM		Medium;
	NDIS_REQUEST	Request;		// ����
	PULONG			BytesNeeded;
	PULONG			BytesReadOrWritten;
	BOOLEAN			IndicateRcvComplete;
	
	BOOLEAN			OutstandingRequests;  	
	BOOLEAN			QueuedRequest;		    
	BOOLEAN			StandingBy;				

	NDIS_DEVICE_POWER_STATE 	MPDeviceState;	// Miniport���豸״̬ 
	NDIS_DEVICE_POWER_STATE 	PTDeviceState;	// Protocol���豸״̬ 

	BOOLEAN			isSecondary;		// miniport�Ƿ���һ���󶨵ĵڶ����豸
	NDIS_STRING		BundleUniString;	// �����ID
	PADAPT			pPrimaryAdapt;		// ָ�����豸
	PADAPT			pSecondaryAdapt;	// ָ��ڶ����豸
	KSPIN_LOCK		SpinLock;			// ͬ����

} ADAPT, *PADAPT;

extern	NDIS_PHYSICAL_ADDRESS			HighestAcceptableMax;
extern	NDIS_HANDLE						ProtHandle, DriverHandle;
extern	NDIS_MEDIUM						MediumArray[3];
extern	PADAPT							pAdaptList;
extern  NDIS_HANDLE						WrapperHandle;

/*����������������������������������������������������������������������������
	�Զ�����ж��豸״̬�Ƿ�ΪON
*/
#define IsIMDeviceStateOn(_pP)					\
	((_pP)->MPDeviceState == NdisDeviceStateD0	\
	&& (_pP)->PTDeviceState == NdisDeviceStateD0 ) 

