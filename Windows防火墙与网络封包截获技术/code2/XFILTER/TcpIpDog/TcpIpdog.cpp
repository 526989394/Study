/*=============================================================================================

	�ļ���		TcpIpDog.cpp

	˵��:
	---------------------------------------------------
		Winsock ���ӣ������ػ� Winsock ���ôӶ�����
	TCP/IP�����������Ӧ����
	---------------------------------------------------

	���̣�		Xfilter ���˷���ǽ
	���ߣ�		����ԣ������
	�������ڣ�	2001/08/21
	��ַ��		http://www.xfilt.com
	�����ʼ���	xstudio@xfilt.com
	��Ȩ���� (c) 2001-2002 X ������

	����:
	---------------------------------------------------
		�����Գ���������Ȩ���ı�����δ����Ȩ������ʹ��
	���޸ı����ȫ���򲿷�Դ���롣�����Ը��ơ����û�ɢ
	���˳���򲿷ֳ�������������κ�ԽȨ��Ϊ�����⵽��
	���⳥�����µĴ�������������������̷�����׷�ߡ�
	
		��ͨ���Ϸ�;���������Դ������û���Ĭ����Ȩ
	�������Լ��ĳ�����ʹ�ñ�����Ĳ��ִ��룬�����߲���
	����������κκ������
	
		ʹ���˱��������ĳ���ֻ���Կ�ִ���ļ���ʽ������
	δ���ر���ɣ����ܽ����б����Դ�����Դ�������κ�
	��ʽ������
	---------------------------------------------------	
*/
//=============================================================================================
// include header file and global variables

#include "stdafx.h"
#include "TcpIpDog.h"
#include "CheckAcl.h"

// v1.0.2 2001-12-24 add for overlapped io
#include "Overlapped.h"

#pragma data_seg(".inidata")
	int				gAclChangeCount = INIT_ACL_CHANGE_COUNT;
	int				m_iDllCount		= 0;
#pragma data_seg()

#pragma bss_seg(".uinidata")
	QUERY_SESSION		m_QuerySession[MAX_QUERY_SESSION];
#pragma bss_seg()

CRITICAL_SECTION	gCriticalSection;
CCheckAcl			m_CheckAcl;
WSPPROC_TABLE		NextProcTable   ;
TCHAR				m_sProcessName[MAX_PATH];

// 1.0.2 add 2001-12-22
BOOL				m_IsFirstStarup = TRUE;
WSPUPCALLTABLE		m_UpCallTable;

// v1.0.2 2001-12-24 add for overlapped io
COverlapped			m_Overlapped;

//=============================================================================================
//DllMain Procedure

BOOL WINAPI DllMain(
	HINSTANCE	hModule, 
    DWORD		ul_reason_for_call, 
    LPVOID		lpReserved
)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
 		GetModuleFileName(NULL, m_sProcessName, MAX_PATH);
		InitializeCriticalSection(&gCriticalSection);

		EnterCriticalSection(&gCriticalSection);
		{
			m_iDllCount ++;

			if(m_iDllCount == 1)
				m_CheckAcl.SetWindowsVersion();
		}
		LeaveCriticalSection(&gCriticalSection);

		ODS2(m_sProcessName,_T(" Loading ..."));
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		EnterCriticalSection(&gCriticalSection);
		{
			m_iDllCount -- ;
		}
		LeaveCriticalSection(&gCriticalSection);

		ODS2(m_sProcessName,_T(" Exit ..."));
	}

	return TRUE;
}

//=============================================================================================
//Exported Functions

int WSPAPI WSPStartup(
	WORD				wVersionRequested,
	LPWSPDATA			lpWSPData,
	LPWSAPROTOCOL_INFOW	lpProtocolInfo,
	WSPUPCALLTABLE		upcallTable,
	LPWSPPROC_TABLE		lpProcTable
)
{
	EnterCriticalSection(&gCriticalSection);

	__try
	{
		ODS(_T("WSPStartup..."));
		ODS(m_CheckAcl.IsWin9x() ? _T("IsWin9x") : _T("IsWinNT or Win2000 ..."));
    
		//v1.0.2 add 2001-12-22
		if(!m_IsFirstStarup)
		{
			upcallTable = m_UpCallTable;
			memcpy(lpProcTable, &NextProcTable, sizeof(WSPPROC_TABLE));
			return 0;
		}

		//
		// v1.0.2 2001-12-25 remove
		//
		//if(!m_CheckAcl.IsWin9x() && m_CheckAcl.CheckStartup() == XF_QUERY && !QueryAccess())
		//	return SOCKET_ERROR;

		TCHAR				sLibraryPath[512];
		LPWSPSTARTUP        WSPStartupFunc      = NULL;
		HMODULE				hLibraryHandle		= NULL;
		INT                 ErrorCode           = 0; 

		if (!GetHookProvider(lpProtocolInfo, sLibraryPath)
			|| (hLibraryHandle = LoadLibrary(sLibraryPath)) == NULL
			|| (WSPStartupFunc = (LPWSPSTARTUP)GetProcAddress(hLibraryHandle, "WSPStartup")) == NULL
			)
			return WSAEPROVIDERFAILEDINIT;

		if ((ErrorCode = WSPStartupFunc(wVersionRequested, lpWSPData, lpProtocolInfo, upcallTable, lpProcTable)) != ERROR_SUCCESS)
			return ErrorCode;
		
		if( !lpProcTable->lpWSPAccept              ||
			!lpProcTable->lpWSPAddressToString     ||        
			!lpProcTable->lpWSPAsyncSelect         ||   
			!lpProcTable->lpWSPBind                ||   
			!lpProcTable->lpWSPCancelBlockingCall  || 
			!lpProcTable->lpWSPCleanup             ||     
			!lpProcTable->lpWSPCloseSocket         ||    
			!lpProcTable->lpWSPConnect             ||
			!lpProcTable->lpWSPDuplicateSocket     ||
			!lpProcTable->lpWSPEnumNetworkEvents   ||
			!lpProcTable->lpWSPEventSelect         ||
			!lpProcTable->lpWSPGetOverlappedResult ||
			!lpProcTable->lpWSPGetPeerName         ||
			!lpProcTable->lpWSPGetSockName         ||
			!lpProcTable->lpWSPGetSockOpt          ||   
			!lpProcTable->lpWSPGetQOSByName        ||
			!lpProcTable->lpWSPIoctl               ||
			!lpProcTable->lpWSPJoinLeaf            ||  
			!lpProcTable->lpWSPListen              || 
			!lpProcTable->lpWSPRecv                ||     
			!lpProcTable->lpWSPRecvDisconnect      ||     
			!lpProcTable->lpWSPRecvFrom            ||     
			!lpProcTable->lpWSPSelect              ||   
			!lpProcTable->lpWSPSend                ||      
			!lpProcTable->lpWSPSendDisconnect      ||      
			!lpProcTable->lpWSPSendTo              ||      
			!lpProcTable->lpWSPSetSockOpt          ||       
			!lpProcTable->lpWSPShutdown            ||      
			!lpProcTable->lpWSPSocket              ||       
			!lpProcTable->lpWSPStringToAddress )
			return WSAEINVALIDPROCTABLE;

		m_UpCallTable = upcallTable;
		NextProcTable = *lpProcTable;

		lpProcTable->lpWSPSocket				= WSPSocket;
		lpProcTable->lpWSPCloseSocket			= WSPCloseSocket;
		lpProcTable->lpWSPConnect				= WSPConnect;
		lpProcTable->lpWSPAccept				= WSPAccept;
		lpProcTable->lpWSPSend					= WSPSend;
		lpProcTable->lpWSPSendTo				= WSPSendTo;
		lpProcTable->lpWSPRecv					= WSPRecv;
		lpProcTable->lpWSPRecvFrom				= WSPRecvFrom;

		//v1.0.2 add 2001-12-22
		m_IsFirstStarup = FALSE;
	}
	__finally
	{
		LeaveCriticalSection(&gCriticalSection);
	}

	return 0;
}

int WINAPI XfIoControl(
	int					iControlType, 
	XFILTER_IO_CONTROL	*ioControl
)
{
	if(iControlType == IO_CONTROL_SET_WORK_MODE)
		return m_CheckAcl.SetWorkMode(ioControl->iWorkMode);
	
	else if(iControlType == IO_CONTROL_GET_WORK_MODE)
		return m_CheckAcl.GetWorkMode();

	else if(iControlType == IO_CONTROL_SET_ACL)
		return m_CheckAcl.SetAcl(ioControl->AclFile);

	else if(iControlType == IO_CONTROL_GET_ACL_CHANGE_COUNT)
		return gAclChangeCount;

	else if(iControlType == IO_CONTROL_SET_GUI_INSTANCE)
	{
		m_CheckAcl.SetGuiWnd(ioControl->hwnd);
		m_CheckAcl.SetGuiProcessName(m_sProcessName);
	}

	else if(iControlType == IO_CONTROL_GET_SESSION)
	{
		m_CheckAcl.GetSessionAndSetSessionNull(ioControl->session, ioControl->isession);
	}

	else if(iControlType == IO_CONTROL_GET_QUERY_SESSION)
	{
		_tcscpy(ioControl->sPathName, m_QuerySession[ioControl->isession].sPathName);
	}

	else if(iControlType == IO_CONTROL_SET_QUERY_SESSION)
	{
		m_QuerySession[ioControl->isession].status = 0;
		m_QuerySession[ioControl->isession].ReturnCode = ioControl->iWorkMode;
	}

	return XERR_SUCCESS;
}

//=============================================================================================
//Socket Private functions

BOOL QueryAccess()
{
	ODS(_T("Query Access ..."));

	int		i;
	for(i = 0; i < MAX_QUERY_SESSION; i++)
	{
		//
		// 2001-12-22 repaired the bug's here, _tcscpy changed to _tcscmp.
		//
		if(m_QuerySession[i].status == 1 
			&& _tcscmp(m_QuerySession[i].sPathName, m_sProcessName) == 0)
		{
			ODS("Xfilter.dll found this process is querying...");
			while(m_QuerySession[i].status == 1)
			{
				static int sec = 0;
				sec ++;
				if(sec > 6000)
					return FALSE;
				Sleep(100);
			}

			//
			// v1.0.2 2001-12-25 add 
			//
			if(m_QuerySession[i].ReturnCode == 0)
			{
				return FALSE;
			}

			return TRUE;
		}
	}

	for(i = 0; i < MAX_QUERY_SESSION; i++)
	{
		if(m_QuerySession[i].status == 0)
		{
			m_QuerySession[i].status = 1;
			_tcscpy(m_QuerySession[i].sPathName, m_sProcessName);

			if(!::PostMessage(m_CheckAcl.GetGuiWnd(), WM_QUERY_ACL_NOTIFY, i, NULL))
			{
				//
				// 2001-12-25 add set status to free
				//
				m_QuerySession[i].status = 0;
				//
				// 2001-12-25 modify chage
				// return FALSE;
				// to stop deny the service
				//
				return TRUE;
			}
			ODS(_T("Query Access SendMessage ..."));
			while(m_QuerySession[i].status == 1)
			{
				static int sec = 0;
				sec ++;
				if(sec > 600)
					return FALSE;
				Sleep(1000);
			}
			
			//
			// v1.0.2 2001-12-25 add 
			//
			if(m_QuerySession[i].ReturnCode == 0)
			{
				return FALSE;
			}

			return TRUE;
		}
	}

	if(i >= MAX_QUERY_SESSION)
		return FALSE;

	return TRUE;
}

BOOL GetHookProvider(
	IN	WSAPROTOCOL_INFOW	*pProtocolInfo, 
	OUT	TCHAR				*sPathName
)
{
	TCHAR sItem[21];
	GetRightEntryIdItem(pProtocolInfo, sItem);

	HKEY	hSubkey;
	DWORD	ulDateLenth	= MAX_PATH;
	TCHAR	sTemp[MAX_PATH];


	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_INSTALL_KEY, 0, KEY_ALL_ACCESS, &hSubkey) != ERROR_SUCCESS)
		return FALSE;

	if (RegQueryValueEx(hSubkey, sItem, 0, NULL, (BYTE*)sTemp, &ulDateLenth)
		|| ExpandEnvironmentStrings(sTemp, sPathName, ulDateLenth) == 0)
		return FALSE;

	if(sPathName[0] == '\0' && sTemp[0] != '\0')
		_tcscpy(sPathName, sTemp);

	RegCloseKey(hSubkey);

	return TRUE;
}

void GetRightEntryIdItem(
	IN	WSAPROTOCOL_INFOW	*pProtocolInfo, 
	OUT	TCHAR				*sItem
)
{
	if(pProtocolInfo->ProtocolChain.ChainLen <= 1)
		_stprintf(sItem, _T("%u"), pProtocolInfo->dwCatalogEntryId);
	else
		_stprintf(sItem, _T("%u"), pProtocolInfo->ProtocolChain.ChainEntries[pProtocolInfo->ProtocolChain.ChainLen - 1]);
}

void XfShutdown(SOCKET s)
{
	int		iError;
	if(NextProcTable.lpWSPShutdown(s, SD_BOTH, &iError) != 0)
		WSASetLastError(iError);
}

//=============================================================================================
//Winsock 2 service provider hook functions

SOCKET WSPAPI WSPSocket(
	int			af,                               
	int			type,                             
	int			protocol,                         
	LPWSAPROTOCOL_INFOW lpProtocolInfo,   
	GROUP		g,                              
	DWORD		dwFlags,                        
	LPINT		lpErrno
)
{
	ODS(_T("XFILTER.DLL: WSPSocket ..."));

	SOCKET	s = NextProcTable.lpWSPSocket(af, type, protocol, lpProtocolInfo, g, dwFlags, lpErrno);

	if(s == INVALID_SOCKET)
		return s;

	if (af == FROM_PROTOCOL_INFO)
		af = lpProtocolInfo->iAddressFamily;
	if (type == FROM_PROTOCOL_INFO)
		type = lpProtocolInfo->iSocketType;
	if (protocol == FROM_PROTOCOL_INFO)
		protocol = lpProtocolInfo->iProtocol;
	m_CheckAcl.CheckSocket(s, af, type, protocol);

	return s;
}

int WSPAPI WSPCloseSocket(
	SOCKET		s,
	LPINT		lpErrno
)
{
	ODS(_T("XFILTER.DLL: WSPCloseSocket ..."));

	m_CheckAcl.CheckCloseSocket(s);
	return NextProcTable.lpWSPCloseSocket(s, lpErrno);
}

int WSPAPI WSPConnect(
	SOCKET			s,
	const struct	sockaddr FAR * name,
	int				namelen,
	LPWSABUF		lpCallerData,
	LPWSABUF		lpCalleeData,
	LPQOS			lpSQOS,
	LPQOS			lpGQOS,
	LPINT			lpErrno
)
{
	ODS(_T("XFILTER.DLL: WSPConnect ..."));

	if(m_CheckAcl.CheckConnect(s, name, namelen) != XF_PASS)
	{
		ODS2(_T("Deny the application "), m_sProcessName);
		*lpErrno = WSAECONNREFUSED;
		return SOCKET_ERROR;
	}
 
	return NextProcTable.lpWSPConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS, lpErrno);
}

SOCKET WSPAPI WSPAccept(
	SOCKET			s,
	struct sockaddr FAR *addr,
	LPINT			addrlen,
	LPCONDITIONPROC	lpfnCondition,
	DWORD			dwCallbackData,
	LPINT			lpErrno
)
{
	ODS(_T("XFILTER.DLL: WSPAccept ..."));

	SOCKET	news	= NextProcTable.lpWSPAccept(s, addr, addrlen, lpfnCondition, dwCallbackData, lpErrno);

	if (news != INVALID_SOCKET && m_CheckAcl.CheckAccept(s, news) != XF_PASS)
	{
		int iError;
		if(NextProcTable.lpWSPCloseSocket(news, &iError) != 0)
			WSASetLastError(iError);
		// v1.0.2 add 2001-12-22
		*lpErrno = 0;
	}

	return news;
}

int WSPAPI WSPSend(
	SOCKET			s,
	LPWSABUF		lpBuffers,
	DWORD			dwBufferCount,
	LPDWORD			lpNumberOfBytesSent,
	DWORD			dwFlags,
	LPWSAOVERLAPPED	lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID	lpThreadId,
	LPINT			lpErrno
)
{
	ODS(_T("XFILTER.DLL: WSPSend ..."));

	if (m_CheckAcl.CheckSend(s, lpBuffers[0].buf, lpBuffers[0].len, lpNumberOfBytesSent) != XF_PASS)
	{
		XfShutdown(s);
		*lpErrno = WSAECONNABORTED;
		return SOCKET_ERROR;
	}

	return NextProcTable.lpWSPSend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped
				, lpCompletionRoutine, lpThreadId, lpErrno);
}

int WSPAPI WSPSendTo(
	SOCKET			s,
	LPWSABUF		lpBuffers,
	DWORD			dwBufferCount,
	LPDWORD			lpNumberOfBytesSent,
	DWORD			dwFlags,
	const struct sockaddr FAR * lpTo,
	int				iTolen,
	LPWSAOVERLAPPED	lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID	lpThreadId,
	LPINT			lpErrno
)
{
	ODS(_T("XFILTER.DLL: WSPSendTo ..."));

	if (m_CheckAcl.CheckSendTo(s, lpTo, lpBuffers[0].buf, lpBuffers[0].len, lpNumberOfBytesSent) != XF_PASS)
	{
		XfShutdown(s);
		*lpErrno = WSAECONNABORTED;
		return SOCKET_ERROR;
	}

	return NextProcTable.lpWSPSendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo
			, iTolen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);

}

int WSPAPI WSPRecv(
	SOCKET			s,
	LPWSABUF		lpBuffers,
	DWORD			dwBufferCount,
	LPDWORD			lpNumberOfBytesRecvd,
	LPDWORD			lpFlags,
	LPWSAOVERLAPPED	lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID	lpThreadId,
	LPINT			lpErrno
)
{
	ODS(_T("XFILTER.DLL: WSPRecv ..."));

	//
	// 2001-12-24 add, ������ص������������˻ص����������������Զ��庯��
	// AddOverlapped����ԭ���Ĳ�����Ϣ��Ȼ�����Լ��Ļص������ӹ�ԭ���Ļ�
	// �������������ñ�־ΪIsSetCompletionRoutine
	//
	BOOL IsSetCompletionRoutine = FALSE;
	if(lpOverlapped && lpCompletionRoutine && m_Overlapped.AddOverlapped(s
		, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags
		, lpOverlapped, lpCompletionRoutine, NULL, NULL, 0/*WSPRecv*/)
		)
	{
		lpCompletionRoutine		= CompletionRoutine;
		IsSetCompletionRoutine	= TRUE;
	}

	//
	// 2001-12-22 �޸ģ�������һ��BUG��ԭ������dwBufferCountΪ 1
	// �������¸���ΪdwBufferCount
	//
	int	iRet = NextProcTable.lpWSPRecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped
				, lpCompletionRoutine, lpThreadId, lpErrno);

	//
	// ����SOCKET_ERROR�����������
	// 1. �д�����
	// 2. û���������أ�û���������ؿ����������ص�������ɣ����lpErrnoΪ
	//	  WSA_IO_PENDING ���ʾ�ص������ɹ����Ժ����������ɡ�
	// �������SOCKET_ERROR�����������Լ��Ļص�������ֱ�ӷ����ɻص�����
	// ���������ɺ�Ĺ�����
	//
	if(iRet == SOCKET_ERROR || IsSetCompletionRoutine == TRUE)
	{
		return iRet;
	}

	if (m_CheckAcl.CheckRecv(s, lpBuffers[0].buf, lpBuffers[0].len, lpNumberOfBytesRecvd) != XF_PASS)
	{
		XfShutdown(s);
		*lpErrno = WSAECONNABORTED;
		return SOCKET_ERROR;
	}

	return iRet;
}

int WSPAPI WSPRecvFrom (
	SOCKET			s,
	LPWSABUF		lpBuffers,
	DWORD			dwBufferCount,
	LPDWORD			lpNumberOfBytesRecvd,
	LPDWORD			lpFlags,
	struct sockaddr FAR * lpFrom,
	LPINT			lpFromlen,
	LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID	lpThreadId,
	LPINT			lpErrno
)
{
	ODS(_T("XFILTER.DLL: WSPRecvFrom ..."));

	//
	// 2001-12-24 add, ���ʱ�ص������������˻ص����������������Զ��庯��
	// AddOverlapped����ԭ���Ĳ�����Ϣ��Ȼ�����Լ��Ļص������ӹ�ԭ���Ļ�
	// �������������ñ�־ΪIsSetCompletionRoutine
	//
	BOOL IsSetCompletionRoutine = FALSE;
	if(lpOverlapped && lpCompletionRoutine && m_Overlapped.AddOverlapped(s
		, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags
		, lpOverlapped, lpCompletionRoutine, lpFrom, lpFromlen, 1/*WSPRecvFrom*/)
		)
	{
		lpCompletionRoutine		= CompletionRoutine;
		IsSetCompletionRoutine	= TRUE;
	}

	//
	// 2001-12-22 �޸ģ�������һ��BUG��ԭ������dwBufferCountΪ 1
	// �������¸���ΪdwBufferCount
	//
	int	iRet= NextProcTable.lpWSPRecvFrom(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpFrom
			, lpFromlen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);

	//
	// ����SOCKET_ERROR�����������
	// 1. �д�����
	// 2. û���������أ�û���������ؿ����������ص�������ɣ����lpErrnoΪ
	//	  WSA_IO_PENDING ���ʾ�ص������ɹ����Ժ����������ɡ�
	// �������SOCKET_ERROR�����������Լ��Ļص�������ֱ�ӷ����ɻص�����
	// ���������ɺ�Ĺ�����
	//
	if(iRet == SOCKET_ERROR || IsSetCompletionRoutine == TRUE)
	{
		return iRet;
	}

	if (m_CheckAcl.CheckRecvFrom(s, lpFrom, lpBuffers[0].buf, lpBuffers[0].len, lpNumberOfBytesRecvd) != XF_PASS)
	{
		XfShutdown(s);
		*lpErrno = WSAECONNABORTED;
		return SOCKET_ERROR;
	}

	return iRet;
}

// v1.0.2 2001-12-24 add for overlapped io
void CALLBACK CompletionRoutine (
	IN    DWORD				dwError, 
	IN    DWORD				cbTransferred, 
	IN    LPWSAOVERLAPPED	lpOverlapped, 
	IN    DWORD				dwFlags 
)
{
	ODS(_T("XFILTER.DLL: CompletionRoutine ..."));
	int iIndex = m_Overlapped.FindOverlapped(lpOverlapped);
	if(iIndex < 0)
		return;

	if(m_Overlapped.m_OverlappedRecorder[iIndex].FunctionType == 1)//WSPRecvFrom
	{
		if( m_CheckAcl.CheckRecvFrom(
				m_Overlapped.m_OverlappedRecorder[iIndex].s
				, m_Overlapped.m_OverlappedRecorder[iIndex].lpFrom
				, m_Overlapped.m_OverlappedRecorder[iIndex].lpBuffers[0].buf
				, m_Overlapped.m_OverlappedRecorder[iIndex].lpBuffers[0].len
				, &cbTransferred
				) != XF_PASS
		)
		{
			XfShutdown(m_Overlapped.m_OverlappedRecorder[iIndex].s);
			dwError = WSAECONNABORTED;
		}
	}
	else if(m_Overlapped.m_OverlappedRecorder[iIndex].FunctionType == 0) //WSPRecv
	{
		if( m_CheckAcl.CheckRecv(
				m_Overlapped.m_OverlappedRecorder[iIndex].s
				, m_Overlapped.m_OverlappedRecorder[iIndex].lpBuffers[0].buf
				, m_Overlapped.m_OverlappedRecorder[iIndex].lpBuffers[0].len
				, &cbTransferred
				) != XF_PASS
		)
		{
			XfShutdown(m_Overlapped.m_OverlappedRecorder[iIndex].s);
			dwError = WSAECONNABORTED;
		}
	}

	if(m_Overlapped.m_OverlappedRecorder[iIndex].lpCompletionRoutine != NULL)
	{
		m_Overlapped.m_OverlappedRecorder[iIndex].lpCompletionRoutine(dwError
			, cbTransferred, lpOverlapped, dwFlags);
	}
	m_Overlapped.DeleteOverlapped(iIndex);
}
 