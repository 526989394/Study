//=============================================================================================
/*
	�ļ���		SystemTray.cpp

	˵��:
	---------------------------------------------------
		��ϵͳ����������ʾͼ�ꡣ
	---------------------------------------------------

	���̣�		Xfilter ���˷���ǽ
	���ߣ�		����ԣ������
	�������ڣ�	2001/08/12
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

#include "stdafx.h"
#include "SystemTray.h"

CSystemTray::CSystemTray()
{
    Initialise();
}

CSystemTray::CSystemTray(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szToolTip, 
                         HICON icon, UINT uID)
{
    Initialise();
    Create(pParent, uCallbackMessage, szToolTip, icon, uID);
}

void CSystemTray::Initialise()
{
    memset(&m_tnd, 0, sizeof(m_tnd));
    m_bEnabled				= FALSE;
    m_bHidden				= FALSE;
    m_DefaultMenuItemID		= 0;
    m_DefaultMenuItemByPos	= TRUE;
}

BOOL CSystemTray::Create(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szToolTip, 
                         HICON icon, UINT uID)
{
    m_bEnabled = (GetVersion() & 0xff) >= 4;
    if (!m_bEnabled) 
    {
        ASSERT(FALSE);
        return FALSE;
    }

    ASSERT(uCallbackMessage >= WM_USER);

    ASSERT(_tcslen(szToolTip) <= 64);

    m_tnd.cbSize = sizeof(NOTIFYICONDATA);
    m_tnd.hWnd   = pParent->GetSafeHwnd();
    m_tnd.uID    = uID;
    m_tnd.hIcon  = icon;
    m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    m_tnd.uCallbackMessage = uCallbackMessage;
    _tcscpy(m_tnd.szTip, szToolTip);

    m_bEnabled = Shell_NotifyIcon(NIM_ADD, &m_tnd);
    ASSERT(m_bEnabled);
    return m_bEnabled;
}

CSystemTray::~CSystemTray()
{
    RemoveIcon();
}

void CSystemTray::RemoveIcon()
{
    if (!m_bEnabled) return;

    m_tnd.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &m_tnd);
    m_bEnabled = FALSE;
}

void CSystemTray::HideIcon()
{
    if (m_bEnabled && !m_bHidden) {
        m_tnd.uFlags = NIF_ICON;
        Shell_NotifyIcon (NIM_DELETE, &m_tnd);
        m_bHidden = TRUE;
    }
}

BOOL CSystemTray::SetIcon(UINT nIDResource)
{
    HICON hIcon = AfxGetApp()->LoadIcon(nIDResource);

    if (!m_bEnabled) return FALSE;

    m_tnd.uFlags = NIF_ICON;
    m_tnd.hIcon = hIcon;

    return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL CSystemTray::SetMenuDefaultItem(UINT uItem, BOOL bByPos)
{
    if ((m_DefaultMenuItemID == uItem) && (m_DefaultMenuItemByPos == bByPos)) 
        return TRUE;

    m_DefaultMenuItemID = uItem;
    m_DefaultMenuItemByPos = bByPos;   

    CMenu menu, *pSubMenu;

    if (!menu.LoadMenu(m_tnd.uID))
        return FALSE;

    pSubMenu = menu.GetSubMenu(0);
    if (!pSubMenu)
        return FALSE;

    ::SetMenuDefaultItem(pSubMenu->m_hMenu, m_DefaultMenuItemID, m_DefaultMenuItemByPos);

    return TRUE;
}

LRESULT CSystemTray::OnTrayNotification(UINT wParam, LONG lParam) 
{
    if (wParam != m_tnd.uID)
        return 0;

    CMenu menu, *pSubMenu;
    CWnd* pTarget			= AfxGetMainWnd();
	static BOOL IsShowed	= FALSE;

    if (LOWORD(lParam) == WM_RBUTTONUP)
    {    
        if (!menu.LoadMenu(m_tnd.uID))
            return 0;
        
        pSubMenu = menu.GetSubMenu(0);
        if (!pSubMenu)
            return 0;

        ::SetMenuDefaultItem(pSubMenu->m_hMenu, m_DefaultMenuItemID, m_DefaultMenuItemByPos);

        CPoint pos;
        GetCursorPos(&pos);

        pTarget->SetForegroundWindow();  
		IsShowed = TRUE;
        ::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, 
                         pTarget->GetSafeHwnd(), NULL);
 		IsShowed = FALSE;
        pTarget->PostMessage(WM_NULL, 0, 0);

        menu.DestroyMenu();
    } 
    else if (LOWORD(lParam) == WM_LBUTTONDBLCLK) 
    {
		if(IsShowed)
			return 0;

        pTarget->SetForegroundWindow();  

        UINT uItem;
        if (m_DefaultMenuItemByPos)
        {
           if (!menu.LoadMenu(m_tnd.uID))
                return 0;
            
            pSubMenu = menu.GetSubMenu(0);
            if (!pSubMenu)
                return 0;
            
            uItem = pSubMenu->GetMenuItemID(m_DefaultMenuItemID);
        }
        else
            uItem = m_DefaultMenuItemID;
        
        pTarget->SendMessage(WM_COMMAND, uItem, 0);

        menu.DestroyMenu();
    }

    return 1;
}
