//����������������������������������������������������������������������������
// Copyright (c) 2001-2002
// XStudio Technology All Right Reserved.
// Author: Tony Zhu
// 2001-7-10 Create
// summary:
//			This Program to demo the Intermediate Miniport NDIS Driver.
//����������������������������������������������������������������������������
// ���������Ϣ��
//
#define DBGPRINT(Fmt)												\
{																	\
	DbgPrint("MinDriver debug: %s (%d)", __FILE__, __LINE__);		\
	DbgPrint (Fmt);													\
}

//����������������������������������������������������������������������������
//Driver's Function
//����������������������������������������������������������������������������
// ����������ں���
//
NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
);

//����������������������������������������������������������������������������
// ��������ж�غ���
//
VOID 
PacketUnload(
	IN PDRIVER_OBJECT		DriverObject
);

