#include "stdafx.h"

#include "NTService.h"
#include <iostream>

using std::cout;
using std::endl;

#pragma comment(lib, "Advapi32.lib")

//
// MessageId: EVMSG_INSTALLED
//
// MessageText:
//
//  The %1 service was installed.
//
#define EVMSG_INSTALLED                  0x00000064L

//
// MessageId: EVMSG_REMOVED
//
// MessageText:
//
//  The %1 service was removed.
//
#define EVMSG_REMOVED                    0x00000065L

//
// MessageId: EVMSG_NOTREMOVED
//
// MessageText:
//
//  The %1 service could not be removed.
//
#define EVMSG_NOTREMOVED                 0x00000066L

//
// MessageId: EVMSG_CTRLHANDLERNOTINSTALLED
//
// MessageText:
//
//  The control handler could not be installed.
//
#define EVMSG_CTRLHANDLERNOTINSTALLED    0x00000067L

//
// MessageId: EVMSG_FAILEDINIT
//
// MessageText:
//
//  The initialization process failed.
//
#define EVMSG_FAILEDINIT                 0x00000068L

//
// MessageId: EVMSG_STARTED
//
// MessageText:
//
//  The service was started.
//
#define EVMSG_STARTED                    0x00000069L

//
// MessageId: EVMSG_BADREQUEST
//
// MessageText:
//
//  The service received an unsupported request.
//
#define EVMSG_BADREQUEST                 0x0000006AL

//
// MessageId: EVMSG_DEBUG
//
// MessageText:
//
//  Debug: %1
//
#define EVMSG_DEBUG                      0x0000006BL

//
// MessageId: EVMSG_STOPPED
//
// MessageText:
//
//  The service was stopped.
//
#define EVMSG_STOPPED                    0x0000006CL


// static variables
NTService* NTService::m_this = NULL;

NTService::NTService(const char* serviceName, const char* versionInfo, const char* desc)
{
	assert(serviceName);
	assert(versionInfo);
	assert(desc);
    // copy the address of the current object so we can access it from
    // the static member callback functions. 
    // WARNING: This limits the application to only one NTService object. 
    m_this = this;
	
	m_serviceName = serviceName;
	m_versionInfo = versionInfo;
	m_desc = desc;

    m_eventSource = NULL;

    // set up the initial service status 
    m_serviceStatus = NULL;
    m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_status.dwCurrentState = SERVICE_STOPPED;
    m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    m_status.dwWin32ExitCode = 0;
    m_status.dwServiceSpecificExitCode = 0;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;
    m_isRunning = false;
}

NTService::~NTService()
{
	DebugMsg("NTService::~NTService()");
	if (m_eventSource) 
	{
		::DeregisterEventSource(m_eventSource);
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Default command line argument parsing

// Returns true if it found an arg it recognised, false if not
// Note: processing some arguments causes output to stdout to be generated.
bool NTService::ParseStandardArgs(int argc, char* argv[])
{
    // See if we have any command line args we recognise
    if (argc <= 1) return false;

    if (_stricmp(argv[1], "-v") == 0) 
	{
		cout << m_serviceName << " " << m_versionInfo << endl;
		cout << "The service is " << (IsInstalled() ? "currently" : "not") << " installed" << endl;
        return true; // say we processed the argument
    } 
	else if (_stricmp(argv[1], "-i") == 0) 
	{
        // Request to install.
        if (IsInstalled()) 
		{
            cout << m_serviceName << " is already installed" << endl;
        } 
		else 
		{
            // Try and install the copy that's running
            if (Install()) 
			{
				cout << m_serviceName << " installed" << endl;
            } 
			else 
			{
				cout << m_serviceName << " failed to install, error:" << ::GetLastError() << endl;
            }
        }
        return true; // say we processed the argument
    } 
	else if (_stricmp(argv[1], "-u") == 0) 
	{
        // Request to uninstall.
        if (!IsInstalled()) 
		{
			cout << m_serviceName << " is not installed" << endl;
        } 
		else 
		{
            // Try and remove the copy that's installed
            if (Uninstall()) 
			{
				cout << m_serviceName << " removed." << endl;
            } 
			else 
			{
				cout << "Could not remove " << m_serviceName << " error:" << ::GetLastError() << endl;
            }
        }
        return true; // say we processed the argument
    }
         
    // Don't recognize the args
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////
// Install/uninstall routines

// Test if the service is currently installed
bool NTService::IsInstalled()
{
    bool bResult = false;

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (hSCM) {

        // Try to open the service
        SC_HANDLE hService = ::OpenService(hSCM,
                                           m_serviceName.c_str(),
                                           SERVICE_QUERY_CONFIG);
        if (hService) {
            bResult = true;
            ::CloseServiceHandle(hService);
        }

        ::CloseServiceHandle(hSCM);
    }
    
    return bResult;
}

bool NTService::Install()
{
    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) 
		return false;

    // Get the executable file path
    char szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));

    // Create the service
    SC_HANDLE hService = ::CreateService(hSCM,
                                         m_serviceName.c_str(),
                                         m_serviceName.c_str(),
                                         SERVICE_ALL_ACCESS,
                                         SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, 
										 SERVICE_AUTO_START,        // start condition
                                         SERVICE_ERROR_NORMAL,
                                         szFilePath,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL);
    if (!hService) 
	{
        ::CloseServiceHandle(hSCM);
        return false;
    }

	SERVICE_DESCRIPTION sd;
	sd.lpDescription = (LPSTR)m_desc.c_str();
	ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sd);
    // make registry entries to support logging messages
    // Add the source name as a subkey under the Application
    // key in the EventLog service portion of the registry.
    HKEY hKey = NULL;
	std::string keystr = "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\";
	keystr += m_serviceName;
    if (::RegCreateKey(HKEY_LOCAL_MACHINE, keystr.c_str(), &hKey) != ERROR_SUCCESS) 
	{
        ::CloseServiceHandle(hService);
        ::CloseServiceHandle(hSCM);
        return false;
    }

    // Add the Event ID message-file name to the 'EventMessageFile' subkey.
    ::RegSetValueEx(hKey,
                    "EventMessageFile",
                    0,
                    REG_EXPAND_SZ, 
                    (CONST BYTE*)szFilePath,
                    (DWORD)strlen(szFilePath) + 1);     

    // Set the supported types flags.
    DWORD dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
    ::RegSetValueEx(hKey,
                    "TypesSupported",
                    0,
                    REG_DWORD,
                    (CONST BYTE*)&dwData,
                     sizeof(DWORD));
    ::RegCloseKey(hKey);

    LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_INSTALLED, m_serviceName.c_str());

    // tidy up
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return true;
}

bool NTService::Uninstall()
{
    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) 
		return false;

    bool bResult = false;
    SC_HANDLE hService = ::OpenService(hSCM,
                                       m_serviceName.c_str(),
                                       DELETE);
    if (hService) 
	{
        if (::DeleteService(hService)) 
		{
            LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_REMOVED, m_serviceName.c_str());
            bResult = true;
        } else 
		{
            LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_NOTREMOVED, m_serviceName.c_str());
        }
        ::CloseServiceHandle(hService);
    }
    
    ::CloseServiceHandle(hSCM);
    return bResult;
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions

// This function makes an entry into the application event log
void NTService::LogEvent(WORD wType, DWORD dwID,
                          const char* pszS1,
                          const char* pszS2,
                          const char* pszS3)
{
    const char* ps[3];
    ps[0] = pszS1;
    ps[1] = pszS2;
    ps[2] = pszS3;

    int iStr = 0;
    for (int i = 0; i < 3; i++) {
        if (ps[i] != NULL) iStr++;
    }
        
    // Check the event source has been registered and if
    // not then register it now
    if (!m_eventSource) {
        m_eventSource = ::RegisterEventSource(NULL,  // local machine
                                               m_serviceName.c_str()); // source name
    }

    if (m_eventSource) {
        ::ReportEvent(m_eventSource,
                      wType,
                      0,
                      dwID,
                      NULL, // sid
                      iStr,
                      0,
                      ps,
                      NULL);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration

bool NTService::StartService()
{
    SERVICE_TABLE_ENTRY st[] = {
        {(LPSTR)m_serviceName.c_str(), ServiceMain},
        {NULL, NULL}
    };

    DebugMsg("Calling StartServiceCtrlDispatcher()");
    BOOL b = ::StartServiceCtrlDispatcher(st);
    DebugMsg("Returned from StartServiceCtrlDispatcher()");
	return b == TRUE ? true : false;
}

// static member function (callback)
void NTService::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    // Get a pointer to the C++ object
    NTService* pService = m_this;
    
    pService->DebugMsg("Entering NTService::ServiceMain()");
    // Register the control request handler
    pService->m_status.dwCurrentState = SERVICE_START_PENDING;
    pService->m_serviceStatus = RegisterServiceCtrlHandler(pService->m_serviceName.c_str(),
                                                           Handler);
    if (pService->m_serviceStatus == NULL) 
	{
        pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_CTRLHANDLERNOTINSTALLED);
        return;
    }

    // Start the initialisation
    if (pService->Initialize()) {

        // Do the real work. 
        // When the Run function returns, the service has stopped.
        pService->m_isRunning = true;
        pService->m_status.dwWin32ExitCode = 0;
        pService->m_status.dwCheckPoint = 0;
        pService->m_status.dwWaitHint = 0;
        pService->Run();
    }

    // Tell the service manager we are stopped
    pService->SetStatus(SERVICE_STOPPED);

    pService->DebugMsg("Leaving NTService::ServiceMain()");
}

///////////////////////////////////////////////////////////////////////////////////////////
// status functions

void NTService::SetStatus(DWORD dwState)
{
    DebugMsg("NTService::SetStatus(%lu, %lu)", m_serviceStatus, dwState);
    m_status.dwCurrentState = dwState;
    ::SetServiceStatus(m_serviceStatus, &m_status);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Service initialization

bool NTService::Initialize()
{
    DebugMsg("Entering NTService::Initialize()");

    // Start the initialization
    SetStatus(SERVICE_START_PENDING);
    
    // Perform the actual initialization
    bool bResult = OnInit(); 
    
    // Set final state
    m_status.dwWin32ExitCode = GetLastError();
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;
    if (!bResult) 
	{
        LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_FAILEDINIT);
        SetStatus(SERVICE_STOPPED);
        return false;    
    }
    
    LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_STARTED);
    SetStatus(SERVICE_RUNNING);

    DebugMsg("Leaving NTService::Initialize()");
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// main function to do the real work of the service

// This function performs the main work of the service. 
// When this function returns the service has stopped.
void NTService::Run()
{
    DebugMsg("Entering NTService::Run()");

    while (m_isRunning) 
	{
        DebugMsg("Sleeping...");
		for(int i=0; i<10 && m_isRunning; i++)
			Sleep(1000);
    }

    // nothing more to do
    DebugMsg("Leaving NTService::Run()");
}

//////////////////////////////////////////////////////////////////////////////////////
// Control request handlers

// static member function (callback) to handle commands from the
// service control manager
void NTService::Handler(DWORD dwOpcode)
{
    // Get a pointer to the object
    NTService* pService = m_this;
    
    pService->DebugMsg("NTService::Handler(%lu)", dwOpcode);

    switch (dwOpcode) 
	{
    case SERVICE_CONTROL_STOP: // 1
        pService->SetStatus(SERVICE_STOP_PENDING);
        pService->OnStop();
        pService->m_isRunning = false;
        pService->LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_STOPPED);
        break;

    case SERVICE_CONTROL_PAUSE: // 2
        pService->OnPause();
        break;

    case SERVICE_CONTROL_CONTINUE: // 3
        pService->OnContinue();
        break;

    case SERVICE_CONTROL_INTERROGATE: // 4
        pService->OnInterrogate();
        break;

    case SERVICE_CONTROL_SHUTDOWN: // 5
        pService->OnShutdown();
        break;

    default:
        if (dwOpcode >= SERVICE_CONTROL_USER)
		{
            if (!pService->OnUserControl(dwOpcode)) 
			{
                pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
            }
        } 
		else 
		{
            pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
        }
        break;
    }

    // Report current status
    pService->DebugMsg("Updating status (%lu, %lu)",
                       pService->m_serviceStatus,
                       pService->m_status.dwCurrentState);
    ::SetServiceStatus(pService->m_serviceStatus, &pService->m_status);
}
        
// Called when the service is first initialized
bool NTService::OnInit()
{
    DebugMsg("NTService::OnInit()");
	return true;
}

// Called when the service control manager wants to stop the service
void NTService::OnStop()
{
    DebugMsg("NTService::OnStop()");
}

// called when the service is interrogated
void NTService::OnInterrogate()
{
    DebugMsg("NTService::OnInterrogate()");
}

// called when the service is paused
void NTService::OnPause()
{
    DebugMsg("NTService::OnPause()");
}

// called when the service is continued
void NTService::OnContinue()
{
    DebugMsg("NTService::OnContinue()");
}

// called when the service is shut down
void NTService::OnShutdown()
{
    DebugMsg("NTService::OnShutdown()");
}

// called when the service gets a user control message
bool NTService::OnUserControl(DWORD dwOpcode)
{
    DebugMsg("NTService::OnUserControl(%8.8lXH)", dwOpcode);
    return false; // say not handled
}


////////////////////////////////////////////////////////////////////////////////////////////
// Debugging support

void NTService::DebugMsg(const char* pszFormat, ...)
{
#ifdef DEBUG
    char buf[4096];
    sprintf(buf, "[%s](%lu): ", m_serviceName.c_str(), GetCurrentThreadId());
	va_list arglist;
	va_start(arglist, pszFormat);
    vsprintf(&buf[strlen(buf)], pszFormat, arglist);
	va_end(arglist);
    strcat(buf, "\n");
    OutputDebugString(buf);
#endif
}
