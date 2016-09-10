/*����������������������������������������������������������������������������
	�ļ���xpassthru.c
	���̣�xpassthru
	��������ʾNDIS�м����������������ṹ��Ϣ
	��Ȩ����(c) 2001-2002 X ������
	http://www.xfilt.com
	xstudio@xfilt.com
*/

#include "prexsim.h"
#pragma hdrstop

/*����������������������������������������������������������������������������
	��NDIS_INIT_FUNCTION���ʶDriverEntry����ֻ����һ��
*/
#pragma NDIS_INIT_FUNCTION(DriverEntry)

NDIS_PHYSICAL_ADDRESS	HighestAcceptableMax \
						= NDIS_PHYSICAL_ADDRESS_CONST(-1, -1);
NDIS_HANDLE				ProtHandle		= NULL;
NDIS_HANDLE				DriverHandle	= NULL;
NDIS_MEDIUM				MediumArray[3]	=
						{
							NdisMedium802_3,	// Ethernet
							NdisMedium802_5,	// Token-ring
							NdisMediumFddi		// Fddi
						};
PADAPT					pAdaptList		= NULL;
NDIS_HANDLE				WrapperHandle	= NULL;

NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
)
{
	NDIS_STATUS						Status;
	PNDIS_CONFIGURATION_PARAMETER	Param;

	DBGPRINT("==> DriverEntry...\n");

	NdisMInitializeWrapper(&WrapperHandle, DriverObject, RegistryPath, NULL);

	Status = MPRegisterAsMiniport(DriverObject, RegistryPath);
	ASSERT(Status == NDIS_STATUS_SUCCESS);

	NdisMRegisterUnloadHandler(WrapperHandle, PtUnload);

	Status = PtRegisterAsProtocol(DriverObject, RegistryPath);
	ASSERT(Status == NDIS_STATUS_SUCCESS);

	NdisIMAssociateMiniport(DriverHandle, ProtHandle);
	DBGPRINT("<== NdisIMAssociateMiniport.\n");

	return(Status);
}
