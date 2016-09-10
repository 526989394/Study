//=============================================================================================
/*
	�ļ���		XLogFile.cpp

	˵��:
	---------------------------------------------------
		��־�ļ����������������־�ļ��Ĵ�������־��¼
	����Ӻ���־�Ĳ�ѯ������
	---------------------------------------------------

	���̣�		Xfilter ���˷���ǽ
	���ߣ�		����ԣ������
	�������ڣ�	2001/08/23
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
#include "XLogFile.h"

CXLogFile::CXLogFile()
{
	InitializeCriticalSection(&gCriticalSectionLog);

	m_sPathName			= CAclFile::GetAppPath() + LOG_FILE_NAME;
	m_LogFile.m_hFile	= NULL;
}

CXLogFile::~CXLogFile()
{
	CloseLog();
}

//=============================================================================================
// Private log file operator function

int CXLogFile::CreateLog(const TCHAR *sPathName)
{
	TRY
	{
		m_LogFile.Open(	sPathName,
						CFile::modeCreate	| 
						CFile::modeWrite	| 
						CFile::typeBinary	| 
						CFile::shareExclusive
						);

		_tcscpy(m_LogHeader.Singnature, ACL_HEADER_SIGNATURE);
		m_LogHeader.RecordCount		= 0;
		m_LogHeader.CurrentPosition	= LOG_HEADER_LENTH;

		int		iRet;

		if(iRet = WriteHeader() != XERR_SUCCESS)
			return iRet;

		ODS2(_T("XFILE: Success create the file "),sPathName);

		CloseLog();
	}
	CATCH( CFileException, e )
	{
		ODS2(_T("XFILE: Can't create the file:"), sPathName);

		return XERR_FILE_CREATE_FAILURE;
	}
	END_CATCH

	return XERR_SUCCESS;
}

int CXLogFile::WriteHeader()
{
	TRY
	{
		m_LogFile.SeekToBegin();
		m_LogFile.Write(&m_LogHeader, LOG_HEADER_LENTH);
	}
	CATCH( CFileException, e )
	{
		return XERR_FILE_WRITER_HEADER_ERROR;
	}
	END_CATCH

	return XERR_SUCCESS;
}

int CXLogFile::OpenLog()
{
	if(_taccess(m_sPathName, 0) == -1)	// file not exists & create
	{
		DP1(_T("XFILE: Log file %s not exists.\n"), LOG_FILE_NAME);

		int		iRet;
		if((iRet = CreateLog(m_sPathName)) != XERR_SUCCESS)
			return iRet;
	}

	TRY												// open file
	{
		m_LogFile.Open(	m_sPathName,
						CFile::modeReadWrite   	|
						CFile::typeBinary		| 
						CFile::shareDenyNone   
						);

		m_LogFile.Read(&m_LogHeader, LOG_HEADER_LENTH);

		if(_tcscmp(m_LogHeader.Singnature, ACL_HEADER_SIGNATURE) != 0)
			return XERR_FILE_INVALID_SIGNATURE;

		if(!m_LogFile.GetStatus(m_FileStatus))
			return XERR_GET_FILE_STATUS_ERROR;

		if(m_FileStatus.m_attribute == 0x01)
			return XERR_FILE_READ_ONLY;

		DP1(_T("XFILE: Success open the file of %s\n"), LOG_FILE_NAME);
	}
	CATCH( CFileException, e )
	{
		DP1(_T("XFILE: Can't open the file of %s\n"), LOG_FILE_NAME);
		return XERR_FILE_CAN_NOT_OPEN;
	}
	END_CATCH

	return XERR_SUCCESS;
}

int CXLogFile::CloseLog()
{
	m_LogFile.Close();
	m_LogFile.m_hFile = NULL;

	return XERR_SUCCESS;
}

//=============================================================================================
// Public log file operator function

int CXLogFile::AddLog(SESSION *session, long MaxLogSize)
{
	int		iRet;

	if(m_LogFile.m_hFile == NULL)
	{
		if((iRet = OpenLog()) != XERR_SUCCESS)
			return iRet;
	}

#if _DEBUG
	CString a;
	a.Format("file size: %u; max log size: %u; one record lenth: %u; \
		after add file size: %u; Current Position: %u; Log Header Lenth: %u"
		, m_FileStatus.m_size
		, MaxLogSize * FILE_SIZE_1M_BYTES
		, RESULT_ONE_RECORD_LENTH
		, MaxLogSize * FILE_SIZE_1M_BYTES - RESULT_ONE_RECORD_LENTH
		, m_LogHeader.CurrentPosition
		, LOG_HEADER_LENTH
		);
	OutputDebugString(a);
#endif

	DWORD PreAddFileSize = MaxLogSize * FILE_SIZE_1M_BYTES - RESULT_ONE_RECORD_LENTH;
	if(m_LogHeader.CurrentPosition > PreAddFileSize)
	{
		m_LogHeader.CurrentPosition = LOG_HEADER_LENTH;

		if(iRet = WriteHeader() != XERR_SUCCESS)
			return iRet;
	}

	TRY												
	{
		session->s = m_LogHeader.CurrentPosition;

		m_LogFile.Seek(m_LogHeader.CurrentPosition, CFile::begin);
		m_LogFile.Write(session, SESSION_LENTH);

		m_LogHeader.CurrentPosition = m_LogFile.GetPosition();
		m_LogHeader.RecordCount ++;

		if(iRet = WriteHeader() != XERR_SUCCESS)
			return iRet;

		DP1(_T("XFILE: Success add a record to the file of %s\n"), LOG_FILE_NAME);
	}
	CATCH( CFileException, e )
	{
		DP1(_T("XFILE: Can't add record to the file of %s\n"), LOG_FILE_NAME);

		return XERR_FILE_ADD_ERROR;
	}
	END_CATCH

	return XERR_SUCCESS;
}

int CXLogFile::FindLog(LOG_FIND *logfind, long MaxLogSize)
{
	if(m_LogFile.m_hFile == NULL)
	{
		int		iRet;
		if((iRet = OpenLog()) == XERR_FILE_CAN_NOT_OPEN)
			return iRet;
	}

	m_LogFile.SeekToBegin();
	m_LogFile.Read(&m_LogHeader, LOG_HEADER_LENTH);

	SESSION		session;
	DWORD		ulLenth				= m_LogFile.GetLength();
	DWORD		ulMaxLenth			= MaxLogSize * FILE_SIZE_1M_BYTES;
	DWORD		ulCurrentPosition	= 0;
	int			iIsChangeDirection	= 0;
	
	logfind->ulChangeDirectionIndex = 0;
	logfind->ulRecordCount			= 0;
	logfind->ulStartPosition		= 0;

	TRY												
	{
		m_LogFile.Seek(LOG_HEADER_LENTH, CFile::begin);
		ulCurrentPosition = m_LogFile.GetPosition();

		while(ulCurrentPosition < ulLenth && ulCurrentPosition < ulMaxLenth)
		{
			m_LogFile.Read(&session, SESSION_LENTH);

			ulCurrentPosition = m_LogFile.GetPosition();
			
			if(session.tStartTime	>= logfind->tStartTime
				&& session.tEndTime <= logfind->tEndTime)
			{
				if(logfind->ulRecordCount != 1 
					&& m_LogHeader.CurrentPosition == (ulCurrentPosition - SESSION_LENTH))
				{
					iIsChangeDirection				= 2;
					logfind->ulChangeDirectionIndex = logfind->ulRecordCount;
					logfind->ulStartPosition		= ulCurrentPosition - SESSION_LENTH;
				}

				logfind->ulRecordCount ++;
				
				if(logfind->ulRecordCount == 1 || iIsChangeDirection == 1)
					logfind->ulStartPosition = ulCurrentPosition - SESSION_LENTH;

				if(iIsChangeDirection == 1)
					iIsChangeDirection = 2;
			}
			else if(logfind->ulRecordCount > 0 && iIsChangeDirection != 1)
			{
				iIsChangeDirection				= 1;
				logfind->ulChangeDirectionIndex = logfind->ulRecordCount;
			}
		}

		logfind->ulChangeDirectionIndex = logfind->ulRecordCount - logfind->ulChangeDirectionIndex;
	}
	CATCH( CFileException, e )
	{
		ODS(_T("XFILE: Find Log Error."));

		return XERR_FILE_RECORD_CAN_NOT_FIND;
	}
	END_CATCH

	return XERR_SUCCESS;
}