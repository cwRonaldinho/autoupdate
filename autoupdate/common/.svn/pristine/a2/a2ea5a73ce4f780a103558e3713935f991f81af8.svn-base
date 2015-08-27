#include "stdafx.h"

#ifdef WIN32

#include <cacti/logging/NTEventHandler.h>

#include <cacti/config/Portability.h>

namespace {

	bool FreeSid(SID* pSid) 
	{
		return ::HeapFree(GetProcessHeap(), 0, (LPVOID)pSid) != 0;
	}


	bool CopySid(SID** ppDstSid, SID* pSrcSid) 
	{
		bool bSuccess = false;

		DWORD dwLength = ::GetLengthSid(pSrcSid);
		*ppDstSid = (SID *) ::HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY, dwLength);

		if(::CopySid(dwLength, *ppDstSid, pSrcSid)) {
			bSuccess = true;
		}
		else {
			FreeSid(*ppDstSid);
		}

		return bSuccess;
	}



	bool GetCurrentUserSID(SID** ppSid) 
	{
		bool bSuccess = false;

		// Pseudohandle so don't need to close it
		HANDLE hProcess = ::GetCurrentProcess();
		HANDLE hToken = NULL;
		if(::OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
			// Get the required size
			DWORD tusize = 0;
			GetTokenInformation(hToken, TokenUser, NULL, 0, &tusize);
			TOKEN_USER* ptu = (TOKEN_USER*)new BYTE[tusize];

			if(GetTokenInformation(hToken, TokenUser, (LPVOID)ptu, tusize, &tusize)) {
				bSuccess = CopySid(ppSid, (SID *)ptu->User.Sid);
			}

			CloseHandle(hToken);
			delete [] ptu;
		}

		return bSuccess;
	}

	HKEY regGetKey(const std::string& subkey, DWORD* disposition)
	{
		HKEY hkey = 0;
		RegCreateKeyExA(HKEY_LOCAL_MACHINE, 
			subkey.c_str(), 
			0, 
			NULL, 
			REG_OPTION_NON_VOLATILE, 
			KEY_SET_VALUE, 
			NULL, 
			&hkey, 
			disposition);
		return hkey;
	}



	void regSetString(HKEY hkey, const std::string& name, const std::string& value)
	{
		RegSetValueExA(hkey, 
			name.c_str(), 
			0, 
			REG_SZ, 
			(LPBYTE)value.c_str(), 
			static_cast<DWORD>(value.length() * sizeof(char)));
	}



	void regSetDword(HKEY hkey, const std::string& name, DWORD value)
	{
		RegSetValueExA(hkey, 
			name.c_str(), 
			0, 
			REG_DWORD, 
			(LPBYTE)&value, 
			sizeof(DWORD));
	}

}

namespace cacti
{
	NTEventHandler::NTEventHandler(
		const std::string& server, 
		const std::string& log, 
		const std::string& source)

		: m_server(server)
		, m_log(log)
		, m_source(source)
		, m_hEventLog(NULL)
		, m_pCurrentUserSID(NULL)

	{
		assert(!m_source.empty());

		if(m_log.length() == 0) 
		{
			m_log = ("Application");
		}

		// current user security identifier
		GetCurrentUserSID(&m_pCurrentUserSID);

		addRegistryInfo();

		m_hEventLog = ::RegisterEventSourceA(m_server.c_str(), m_source.c_str());
	}

	NTEventHandler::~NTEventHandler()
	{
		if(m_hEventLog != NULL) 
		{
			::DeregisterEventSource(m_hEventLog);
			m_hEventLog = NULL;
		}

		if(m_pCurrentUserSID != NULL) 
		{
			FreeSid(m_pCurrentUserSID);
			m_pCurrentUserSID = NULL;
		}

	}

	void NTEventHandler::doPublish(const LogRecord& rec)
	{
		std::ostringstream buf;
		m_formatter->format(buf, rec);
		std::string sz = buf.str();
		const char* s = sz.c_str();

		::ReportEventA(
			m_hEventLog,
			getEventType(rec),
			getEventCategory(rec),
			0x1000,
			m_pCurrentUserSID,
			1,
			0,
			&s,
			NULL);
	}

	WORD NTEventHandler::getEventType(const LogRecord& rec)
	{	
		if(rec.level == LogLevel::FATAL || rec.level == LogLevel::ERR)
			return EVENTLOG_ERROR_TYPE;
		else if(rec.level == LogLevel::WARN)
			return EVENTLOG_WARNING_TYPE;
		else 
			return EVENTLOG_INFORMATION_TYPE;
	}



	WORD NTEventHandler::getEventCategory(const LogRecord& rec)
	{
		if(rec.level == LogLevel::FATAL)
			return 1;
		else if(rec.level == LogLevel::ERR)
			return 2;
		else if(rec.level == LogLevel::WARN)
			return 3;
		else if(rec.level == LogLevel::INFO)
			return 4;
		else
			return 5;
	}

	void NTEventHandler::addRegistryInfo()
	{
		DWORD disposition;
		HKEY hkey = 0;
		std::string subkey = ("SYSTEM\\CurrentControlSet\\Services\\EventLog\\")
			+ m_log 
			+ ("\\") 
			+ m_source;

		hkey = regGetKey(subkey, &disposition);
		if(disposition == REG_CREATED_NEW_KEY) 
		{
			regSetString(hkey, "EventMessageFile", "NTEventLogAppender.dll");
			regSetString(hkey, "CategoryMessageFile", "NTEventLogAppender.dll");
			regSetDword(hkey, "TypesSupported", (DWORD)7);
			regSetDword(hkey, "CategoryCount", (DWORD)5);
		}

		RegCloseKey(hkey);
		return;
	}

}
#endif
