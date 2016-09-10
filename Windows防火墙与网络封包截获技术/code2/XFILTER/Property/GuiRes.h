//=============================================================================================
/*
	GUI Resurce, GuiRes.h

	Project	: Xfilter 1.0
	Author	: Tony Zhu
	Create Date	: 2001/08/03
	Email	: xstudio@xfilt.com
	URL		: http://www.xfilt.com

	Copyright (c) 2001-2002 XStudio Technology.
	All Rights Reserved.

	WARNNING: 
*/

#ifndef	GUIRES_H
#define GUIRES_H

#define GUI_LANGUAGE				_T("RPC")

//---------------------------------------------------------------------------------------------
//app's resource

#define GUI_APP_CLASS_NAME			_T("Xfilter")
#define	GUI_APP_CAPTION				_T("Xfilter ���˷���ǽ")
#define	GUI_APP_LOGO				_T("XSTUDIO")

//---------------------------------------------------------------------------------------------
//menu's resource

#define GUI_MENU_FILE				_T("�ļ�(&F)")
#define GUI_MENU_ABOUT				_T("����(&A)")
#define GUI_MENU_CONTROL_FRAME		_T("������(&C)")
#define GUI_MENU_PACKET_MONITOR		_T("�������(&M)")
#define GUI_MENU_LOG_QUERY			_T("��־��ѯ(&Q)")
#define GUI_MENU_ACL				_T("�عܹ���(&L)")
#define GUI_MENU_SYSTEM_SET			_T("ϵͳ����(&S)")
#define GUI_MENU_STOP_FILTER		_T("ֹͣ����(&T)")
#define GUI_MENU_START_FILTER		_T("��ʼ����(&F)")
#define GUI_MENU_EXIT				_T("�˳�(&X)")

//---------------------------------------------------------------------------------------------
//button's resource

#define GUI_BUTTON_OK				_T("ȷ��")
#define GUI_BUTTON_CANCEL			_T("ȡ��")
#define GUI_BUTTON_CANCEL_EX		_T("ȡ��(&C)")
#define GUI_BUTTON_APPLY			_T("Ӧ��(&A)")
#define GUI_BUTTON_HELP				_T("����")

//---------------------------------------------------------------------------------------------
//tab's resource

#define GUI_TAB_PACKET_MONITOR		_T("�������")
#define GUI_TAB_LOG_QUERY			_T("��־��ѯ")
#define GUI_TAB_ACL					_T("�عܹ���")
#define GUI_TAB_SYSTEM_SETTING		_T("ϵͳ����")
#define GUI_TAB_ABOUT				_T("����")

//---------------------------------------------------------------------------------------------
//packet monitor resource

#define	GUI_PACKET_MONITOR_TITLE						_T("�������")
#define GUI_PACKET_MONITOR_LABLE						_T("��������б�")
#define GUI_PACKET_MONITOR_TOOLBAR_CLEAR				_T("���")
#define GUI_PACKET_MONITOR_TOOLBAR_STOP_SCROLL			_T("ֹͣ����")
#define GUI_PACKET_MONITOR_TOOLBAR_START_SCROLL			_T("�Զ�����")
#define GUI_PACKET_MONITOR_TOOLBAR_STOP_MONITOR			_T("ֹͣ����")
#define GUI_PACKET_MONITOR_TOOLBAR_START_MONITOR		_T("��ʼ����")
#define GUI_PACKET_MONITOR_LIST_ACTION					_T("����")
#define GUI_PACKET_MONITOR_LIST_ACTION_SIZE				\
		sizeof(GUI_PACKET_MONITOR_LIST_ACTION)			* 8
#define GUI_PACKET_MONITOR_LIST_STARTTIME_ENDTIME		_T("��ʼʱ�� - ����ʱ��")
#define GUI_PACKET_MONITOR_LIST_STARTTIME_ENDTIME_SIZE	\
		sizeof(GUI_PACKET_MONITOR_LIST_STARTTIME_ENDTIME) * 8
#define GUI_PACKET_MONITOR_LIST_PROTOCOL				_T("Э��")
#define GUI_PACKET_MONITOR_LIST_PROTOCOL_SIZE		\
		sizeof(GUI_PACKET_MONITOR_LIST_PROTOCOL)		* 8
#define GUI_PACKET_MONITOR_LIST_IN_DATA					_T("������")
#define GUI_PACKET_MONITOR_LIST_IN_DATA_SIZE			\
		sizeof(GUI_PACKET_MONITOR_LIST_IN_DATA)			* 8
#define GUI_PACKET_MONITOR_LIST_OUT_DATA				_T("������")
#define GUI_PACKET_MONITOR_LIST_OUT_DATA_SIZE			\
		sizeof(GUI_PACKET_MONITOR_LIST_OUT_DATA)		* 8
#define GUI_PACKET_MONITOR_LIST_IP_PORT					_T("���� IP : �˿� - Զ�� IP : �˿�")
#define GUI_PACKET_MONITOR_LIST_IP_PORT_SIZE			\
		sizeof(GUI_PACKET_MONITOR_LIST_IP_PORT)			* 8
#define GUI_PACKET_MONITOR_LIST_APPLICATION				_T("Ӧ�ó��� : ��ע")
#define GUI_PACKET_MONITOR_LIST_APPLICATION_SIZE		\
		sizeof(GUI_PACKET_MONITOR_LIST_APPLICATION)		* 8

//---------------------------------------------------------------------------------------------
//log query resource

#define	GUI_LOG_QUERY_SET_LABLE				_T("��ѯ��������")
#define GUI_LOG_QUERY_SET_START_TIME_LABLE	_T("��ʼ����/ʱ�䣺")
#define GUI_LOG_QUERY_SET_END_TIME_LABLE	_T("��������/ʱ�䣺")
#define GUI_LOG_QUERY_RESULT_LABLE			_T("��ѯ���")
#define GUI_LOG_QUERY_RESULT_LIST_LABLE		_T("�� %u ҳ,�� %u ҳ,ÿҳ %u ��,��ǰ %u ��,�� %u ��")
#define GUI_LOG_QUERY_BUTTON_QUERY			_T("��ʼ��ѯ(&Q)")
#define GUI_LOG_QUERY_BUTTON_BACK			_T("��һҳ(&B)")
#define GUI_LOG_QUERY_BUTTON_NEXT			_T("��һҳ(&N)")
#define GUI_LOG_QUERY_BUTTON_DELETE			_T("���(&D)")

//---------------------------------------------------------------------------------------------
//ACL RESOURCE

#define GUI_ACL_APPLICATION_SET_LABLE		_T("Ӧ�ó�������")
#define GUI_ACL_RADIO_PASS					_T("��������")
#define GUI_ACL_RADIO_QUERY					_T("ѯ��")
#define GUI_ACL_RADIO_DENY					_T("�ܾ�����")
#define GUI_ACL_SET_LABLE					_T("�عܹ�������")
#define GUI_ACL_BUTTON_ADD					_T("���(&A)")
#define GUI_ACL_BUTTON_EDIT					_T("�޸�(&E)")
#define GUI_ACL_BUTTON_DELETE				_T("ɾ��(&D)")
#define GUI_ACL_LIST_ACTION					_T("����")
#define GUI_ACL_LIST_ACTION_SIZE			sizeof(GUI_ACL_LIST_ACTION)	* 8
#define GUI_ACL_LIST_APPLICATION			_T("Ӧ�ó���")
#define GUI_ACL_LIST_APPLICATION_SIZE		sizeof(GUI_ACL_LIST_APPLICATION) * 8
#define GUI_ACL_LIST_DIRECTION				_T("����")
#define GUI_ACL_LIST_DIRECTION_SIZE			sizeof(GUI_ACL_LIST_DIRECTION) * 8
#define GUI_ACL_LIST_REMOTE_NET				_T("Զ������")
#define GUI_ACL_LIST_REMOTE_NET_SIZE		sizeof(GUI_ACL_LIST_REMOTE_NET)	* 8
#define GUI_ACL_LIST_ACCESS_TIME			_T("����ʱ��")
#define GUI_ACL_LIST_ACCESS_TIME_SIZE		sizeof(GUI_ACL_LIST_ACCESS_TIME) * 8
#define GUI_ACL_LIST_TYPE_PORT				_T("����/�˿�")
#define GUI_ACL_LIST_TYPE_PORT_SIZE			sizeof(GUI_ACL_LIST_TYPE_PORT) * 8
#define GUI_ACL_LIST_APPLICATION_PATH		_T("Ӧ�ó���·��")
#define GUI_ACL_LIST_APPLICATION_PATH_SIZE	sizeof(GUI_ACL_LIST_APPLICATION_PATH) * 8
#define GUI_ACL_LIST_MEMO					_T("˵��")
#define GUI_ACL_LIST_MEMO_SIZE				sizeof(GUI_ACL_LIST_MEMO) * 8

//---------------------------------------------------------------------------------------------
//ACL SET RESOURCE

#define GUI_ACL_SET_CAPTION					_T("Xfilter ���˷���ǽ - �عܹ���")
#define GUI_ACL_BASE_SET_LABLE				_T("��������")
#define GUI_ACL_QUERY_INFO					_T("Ҫ�������磬������")
#define GUI_ACL_APPLICATION_LABLE			_T("Ӧ�ó���")
#define GUI_ACL_REMOTE_NET_LABLE			_T("Զ�����磺")
#define GUI_ACL_ACCESS_TIME_LABLE			_T("����ʱ�䣺")
#define GUI_ACL_ACTION_LABLE				_T("���ƶ�����")
#define GUI_ACL_SUPER_SET_LABLE				_T("�߼����ã�����˿�Ϊ 0 ��ʾ���ж˿ڡ�")
#define GUI_ACL_DERECTION_LABLE				_T("��������")
#define GUI_ACL_SERVICE_TYPE_LABLE			_T("�������ͣ�")
#define GUI_ACL_SERVICE_PORT_LABLE			_T("����˿ڣ�")
#define GUI_ACL_MEMO_LABLE					_T("��ע")
#define GUI_ACL_ACTION_0					_T("����")
#define GUI_ACL_ACTION_1					_T("�ܾ�")
#define GUI_ACL_DERECTION_0					_T("��")
#define GUI_ACL_DERECTION_1					_T("��")
#define GUI_ACL_DERECTION_2					_T("˫��")
#define GUI_ACL_SERVICE_TYPE_0				_T("ȫ��")
#define GUI_ACL_SERVICE_TYPE_1				_T("TCP")
#define GUI_ACL_SERVICE_TYPE_2				_T("UDP")
#define GUI_ACL_SERVICE_TYPE_3				_T("FTP")
#define GUI_ACL_SERVICE_TYPE_4				_T("TELNET")
#define GUI_ACL_SERVICE_TYPE_5				_T("HTTP")
#define GUI_ACL_SERVICE_TYPE_6				_T("NNTP")
#define GUI_ACL_SERVICE_TYPE_7				_T("POP3")
#define GUI_ACL_SERVICE_TYPE_8				_T("SMTP")

//---------------------------------------------------------------------------------------------
//net and time set sheet resource

#define GUI_NET_TIME_SHEET_CAPTION			_T("������ʱ������")

//---------------------------------------------------------------------------------------------
//set time resource

#define GUI_SET_TIME_WEEK_TIME_LABLE		_T("������ʱ��")
#define GUI_SET_TIME_CHECK_SUNDAY			_T("������")
#define GUI_SET_TIME_CHECK_MONDAY			_T("����һ")
#define GUI_SET_TIME_CHECK_TUESDAY			_T("���ڶ�")
#define GUI_SET_TIME_CHECK_WEDNESDAY		_T("������")
#define GUI_SET_TIME_CHECK_THURSDAY			_T("������")
#define GUI_SET_TIME_CHECK_FRIDAY			_T("������")
#define GUI_SET_TIME_CHECK_SATURDAY			_T("������")
#define GUI_SET_TIME_TIME_LABLE				_T("ʱ�䣺")
#define GUI_SET_TIME_LABLE_START_TIME		_T("��ʼʱ��")
#define GUI_SET_TIME_LABLE_END_TIME			_T("����ʱ��")
#define GUI_SET_TIME_TREE_0					_T("�κ�ʱ��")
#define GUI_SET_TIME_TREE_1					_T("����ʱ��")
#define GUI_SET_TIME_TREE_2					_T("������ҵ��ʱ��")
#define GUI_SET_TIME_TREE_3					_T("��ĩ")
#define GUI_SET_TIME_TREE_4					_T("Լ��ʱ��")
#define GUI_SET_TIME_TREE_5					_T("����ʱ��")
#define GUI_SET_TIME_TREE_6					_T("�Զ���ʱ��")

//---------------------------------------------------------------------------------------------
//net ip aria resource

#define GUI_NET_IP_ARIA_CAPTION				_T("IP ��ַ��")
#define GUI_NET_IP_ARIA_LABLE				_T("Զ������ IP ��ַ��")
#define GUI_NET_IP_ARIA_LABLE_START_IP		_T("��ʼ IP��")
#define GUI_NET_IP_ARIA_LABLE_END_IP		_T("���� IP��")
#define GUI_NET_IP_ARIA_ALL					_T("*")

//---------------------------------------------------------------------------------------------
//set remote net resource

#define GUI_SET_NET_LABLE					_T("Զ��������Ϣ")
#define GUI_SET_NET_NET_LABLE				_T("������Ϣ")
#define GUI_SET_NET_BUTTON_ADD				_T("���(&A)")
#define GUI_SET_NET_BUTTON_EDIT				_T("�޸�(&E)")
#define GUI_SET_NET_BUTTON_DELETE			_T("ɾ��(&D)")
#define GUI_SET_NET_LIST_START_IP			_T("��ʼIP")
#define GUI_SET_NET_LIST_END_IP				_T("����IP")
#define GUI_SET_NET_TREE_0					_T("��������")
#define GUI_SET_NET_TREE_1					_T("������")
#define GUI_SET_NET_TREE_2					_T("��Լ��������")
#define GUI_SET_NET_TREE_3					_T("���ε�����")
#define GUI_SET_NET_TREE_4					_T("�Զ��������")

//---------------------------------------------------------------------------------------------
//system set resource

#define GUI_SYSTEM_SET_COMMON_SET_LABLE		_T("��������")
#define GUI_SYSTEM_SET_CHECK_LOG			_T("��¼��־����־�ļ���СΪ")
#define GUI_SYSTEM_SET_UNIT_LABLE			_T("M��")
#define GUI_SYSTEM_SET_CHECK_AUTOSTART		_T("Windows ����ʱ�Զ����� Xfilter��")
#define GUI_SYSTEM_SET_CHECK_SPLASH			_T("Xfilter ����ʱ��ʾ��ӭ���档")
#define GUI_SYSTEM_SET_ALERT_SET_LABLE		_T("��������")
#define GUI_SYSTEM_SET_CHECK_ALERT_PCSPEAKER	_T("����ʱ PC ���ȷ�������������")
#define GUI_SYSTEM_SET_CHECK_ALERT_DIALOG	_T("����ʱ��˸�������ϵ�ͼ�ꡣ")

//---------------------------------------------------------------------------------------------
//about resource

#define GUI_ABOUT_LABLE_TITLE			_T("���� Xfilter ���˷���ǽ")
#define GUI_ABOUT_LABLE_COPYRIGHT1		_T("Xfilter ���˷���ǽ %u.%u.%u ��")
#define GUI_ABOUT_LABLE_COPYRIGHT2		_T("Copyright (C) 2001-2002 Xstudio Technology")
#define GUI_ABOUT_LABLE_COPYRIGHT3		_T("All rights reserved.")
#define GUI_ABOUT_LABLE_ACCREDIT_TO		_T("�������Ȩ�� %s ʹ��")
#define GUI_ABOUT_LABLE_WEB_ADDRESS_LABLE	_T("��վ��ַ��")
#define GUI_ABOUT_LABLE_EMAIL_LABLE			_T("�����ʼ���")
#define GUI_ABOUT_LABLE_WEB_ADDRESS		_T("http://www.xfilt.com/")
#define GUI_ABOUT_LABLE_EMAIL			_T("mailto:xstudio@xfilt.com")
#define GUI_ABOUT_LABLE_WARNING			_T("���棺�����������Ȩ���ı������������������ɴ��������������ƻ�����������Ի�����Ϊӯ��Ŀ�ģ���δ����Ȩ������Ӫ���������һ����ҵ��Ϊ�����⵽�����⳥�����µĴ�������������������̷�����׷�ߡ�")
#define GUI_ABOUT_LABLE_INFO			_T("���������Դ���룬��ϸ��Ϣ������������ַ����վ���г��˲�Ʒ�۸�͸��ʽ������ֱ�ӷ��� Email ������ѯ��")
#define GUI_ABOUT_LABLE_AUTHOR			_T("���ߣ�    ����ԡ������")
#define GUI_ABOUT_LABLE_COPYRIGHT		_T("��Ȩ����(C) 2001-2002 X ������")

//---------------------------------------------------------------------------------------------
// Register Dialog Resource

#define GUI_REG_CAPTION				_T("Xfilter �û�ע��")
#define GUI_REG_LABEL_INFO_1		_T("���������Դ���룬��ϸ��Ϣ������������ַ�������������ȫ��ѵģ�ֻҪ���� Email ��ַ�����ͳ�Ϊ������ĺϷ��û���")
#define GUI_REG_LABEL_INFO_3		_T("��ȷ�� Email ����ʵ�ԣ��⽫��Ϊ����֧�ֵ���Ҫ;����")
#define GUI_REG_LABEL_INFO_2		_T("ע��������Ϣ���������ڵ�һʱ��õ���������������򣬲��п��ܳ�Ϊ�����û���ѻ��Դ���롣 ")
#define GUI_REG_LABEL_EMAIL			_T("Email��*")
#define GUI_REG_LABEL_NAME			_T("������")
#define GUI_REG_LABEL_GENDER		_T("�Ա�")
#define GUI_REG_LABEL_ID_TYPE		_T("֤�����ͣ�")
#define GUI_REG_LABEL_ID			_T("֤�����룺")
#define GUI_REG_LABEL_BIRTHDAY		_T("���գ�")
#define GUI_REG_LABEL_DEGREE		_T("ѧ����")
#define GUI_REG_LABEL_METIER		_T("ְҵ��")
#define GUI_REG_LABEL_DUTY			_T("ְ��")
#define GUI_REG_LABEL_ZIP			_T("�ʱࣺ")
#define GUI_REG_LABEL_SALARY		_T("��н��")
#define GUI_REG_LABEL_ADDRESS		_T("��ַ��")
#define GUI_REG_LABEL_QQ			_T("QQ ���룺")
#define GUI_REG_LABEL_INC			_T("��λ��")

//---------------------------------------------------------------------------------------------
// Popup Message

#define GUI_REMOVE_CAPTION				_T("ж�� Xfilter ���˷���ǽ")
#define GUI_REMOVE_LABEL_INFO			_T("ж�س��򽫴�����������Ƴ� Xfilter ���˷���ǽ�������Զ���Ҫж�صĸ����������ȷ����ť��ʼж�أ���ȡ����ťȡ��ж�ء�")
#define GUI_REMOVE_CHECK_MAIN			_T("ж�� Xfilter ���˷���ǽ������")
#define GUI_REMOVE_CHECK_LOG			_T("ж��ʱɾ����־�ļ�")
#define GUI_REMOVE_CHECK_ACL			_T("ж��ʱɾ���عܹ����ļ�")
#define GUI_REMOVE_LABEL_LOGO			_T("XSTUDIO")

//---------------------------------------------------------------------------------------------
// Popup Message

#define GUI_ACL_MESSAGE_INAVALID_PORT					_T("�˿�ֵ��Ч����Ч��ΧΪ 0 - 65535�����������롣")
#define GUI_ACL_MESSAGE_APP_PATH_ERROR					_T("��������Ӧ�ó���·�������ƣ����������롣")
#define GUI_ACL_MESSAGE_APP_NOT_EXSITS					_T("Ӧ�ó��򲻴��ڣ�����·�������ơ�")
#define GUI_ACL_MESSAGE_ONLY_PATH						_T("��������Ӧ�ó���������·�����ļ�����")
#define GUI_ACL_MESSAGE_ADD_ACL_IP_ERROR				_T("��� ACL IP��Χ����")
#define GUI_ACL_MESSAGE_SAVE_ACL_IP_TIME_ERROR			_T("���� ACL IP��Χ����ʱ�䷶Χ����")
#define GUI_ACL_MESSAGE_ADD_ACL_ERROR					_T("��� ACL �������")
#define GUI_ACL_MESSAGE_SAVE_ACL_ERROR					_T("���� ACL �������")
#define GUI_ACL_MESSAGE_ACL_FILE_SET_ERROR				_T("ACL �ļ�ϵͳ���ô���")
#define GUI_ACL_MESSAGE_ACL_READ_ERROR					_T("��ȡ ACL �ļ�����")
#define GUI_ACL_MESSAGE_ACL_ASK_SAVE					_T("Ҫ�����������޸���")
#define GUI_ACL_MESSAGE_PLEASE_CLOSE_SUB_WINDOW			_T("�����ȹر����е��Ӵ��ڡ�")
#define GUI_ACL_MESSAGE_MAX_ACL							_T("���� ACL ����������Ŀ�����ܹ�����ӡ�")
#define GUI_ACL_MESSAGE_MAX_IP_ARIA						_T("���� IP ��Χ�������Ŀ�����ܹ�����ӡ�")
#define GUI_ACL_MESSAGE_DLL_NOT_FOUND					_T("�����ҵ� Xfilter.dll ���߷��ʱ��ܾ���")
#define GUI_ACL_MESSAGE_FUNCTION_NOT_FOUNT				_T("Xfilter.dll ��û���ҵ���Ա������")
#define GUI_ACL_MESSAGE_INIT_DLL_DATA_ERROR				_T("��ʼ�� Xfilter.dll ����������")
#define GUI_ACL_MESSAGE_INIT_CALLBACK_FUNC_ERROR		_T("��ʼ���ص���������")
#define GUI_NET_IP_ARIA_MESSAGE_IP_NULL					_T("IP��ַ����Ϊ�գ����������롣")
#define GUI_NET_IP_ARIA_MESSAGE_INVALID_IP_ARIA			_T("��ʼ IP ����С�ڵ��ڽ��� IP�����������롣")
#define GUI_ACL_MESSAGE_START_TIME_MIN_END_TIME			_T("��ʼʱ�����С�ڽ���ʱ�䡣")
#define GUI_ACL_MESSAGE_CAN_NOT_FIND_RECORD				_T("û���ҵ����������ļ�¼��")
#define GUI_ACL_MESSAGE_SET_WORK_MODE_ERROR				_T("���ù���ģʽ����")
#define GUI_REG_MESSAGE_MUST_INPUT_EMAIL				_T("��������ȷ�� Email ��ַ��")
#define GUI_REG_MESSAGE_INVALID_PASSWORD				_T("��Ч�����룬���������롣")
#define GUI_REG_MESSAGE_MUST_INPUT_NAME					_T("��������������")
#define GUI_REG_MESSAGE_MUST_INPUT_ID					_T("��������֤�����롣")
#define GUI_ACL_MESSAGE_NO_WINSOCK2						_T("ϵͳû�� Winsock 2 ֧�֡�")
#define GUI_ACL_MESSAGE_INSTALL_FAILED					_T("��װʧ�ܡ�")
#define GUI_ACL_MESSAGE_PROCESS_IS_RUNNING				_T("Xfilter �������У�ж��ǰ�����ȹر� Xfilter��")
#define GUI_MESSAGE_REMOVE_PROVIDER_FAILED				_T("ж�� Xfilter ����������Xfilterû�а�װ���߶�дע������")
#define GUI_MESSAGE_REMOVE_SUCCESS						_T("ж�سɹ���")
#define GUI_MESSAGE_REMOVE_HAVE_FILE_USING				_T("ж�سɹ����������ļ�����ʹ�ã��������������ֹ�ɾ����")
#define GUI_MESSAGE_OPEN_HELP_FAILED					_T("�򿪰����ļ�����")
#define GUI_MESSAGE_HYPER_LINK_ERROR					_T("�򿪳������ӳ���")

//---------------------------------------------------------------------------------------------
// some resource arrays

static TCHAR *GUI_ACTION[]		= {
	GUI_ACL_ACTION_0,
	GUI_ACL_ACTION_1
};

static TCHAR *GUI_DIRECTION[]	= {
	GUI_ACL_DERECTION_0,
	GUI_ACL_DERECTION_1,
	GUI_ACL_DERECTION_2
};

static TCHAR *GUI_DIRECTION_EX[]= {
	_T("<-"),
	_T("->"),
	_T("<->")
};

static TCHAR *GUI_SERVICE_TYPE[]= {
	 GUI_ACL_SERVICE_TYPE_0,	
	 GUI_ACL_SERVICE_TYPE_1,						
	 GUI_ACL_SERVICE_TYPE_2,						
	 GUI_ACL_SERVICE_TYPE_3,						
	 GUI_ACL_SERVICE_TYPE_4,						
	 GUI_ACL_SERVICE_TYPE_5,						
	 GUI_ACL_SERVICE_TYPE_6,						
	 GUI_ACL_SERVICE_TYPE_7,						
	 GUI_ACL_SERVICE_TYPE_8	
};

static TCHAR *GUI_TIME[]		= {
	GUI_SET_TIME_TREE_0,
	GUI_SET_TIME_TREE_1,
	GUI_SET_TIME_TREE_2,
	GUI_SET_TIME_TREE_3,
	GUI_SET_TIME_TREE_4,
	GUI_SET_TIME_TREE_5,
	GUI_SET_TIME_TREE_6
};

static TCHAR *GUI_NET[]			= {
	GUI_SET_NET_TREE_0,
	GUI_SET_NET_TREE_1,
	GUI_SET_NET_TREE_2,
	GUI_SET_NET_TREE_3,
	GUI_SET_NET_TREE_4
};

static TCHAR *GUI_GENDER[]		= {
	_T("��"),
	_T("Ů")
};

static TCHAR *GUI_ID_TYPE[]		= {
	_T("����"),
	_T("���֤"),
	_T("ѧ��֤"),
	_T("����֤")
};

static TCHAR *GUI_DEGREE[]		= {
	_T("����"),
	_T("Сѧ"),
	_T("����"),
	_T("����"),
	_T("��ר"),
	_T("��ר"),
	_T("��ѧ"),
	_T("˶ʿ"),
	_T("��ʿ")
};

static TCHAR *GUI_METIER[]		= {
	_T("����"),
	_T("�����ҵ"),
	_T("ѧ��"),
	_T("��ʦ"),
	_T("����"),
	_T("����ҵ"),
	_T("��ҵ"),
	_T("ũҵ"),
	_T("����ҵ"),
	_T("����ҵ"),
	_T("����ҵ"),
	_T("���ҵ"),
	_T("����"),
	_T("����"),
	_T("��������"),
	_T("ҽ��")
};

static TCHAR *GUI_SALARY[]		= {
	_T("��"),
	_T("1000 ����"),
	_T("1000 - 2500"),
	_T("2500 - 4000"),
	_T("4000 - 6000"),
	_T("6000 - 8000"),
	_T("8000 - 10000"),
	_T("10000 ����")
};

#endif

