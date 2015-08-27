#pragma once

//#include "ntservmsg.h" // Event message ids

#define SERVICE_CONTROL_USER 128

class NTService
{
public:
    NTService(const char* serviceName, const char* versionInfo, const char* desc);
    virtual ~NTService();
    bool ParseStandardArgs(int argc, char* argv[]);
    bool IsInstalled();
    bool Install();
    bool Uninstall();
    void LogEvent(WORD wType, DWORD dwID,
                  const char* pszS1 = NULL,
                  const char* pszS2 = NULL,
                  const char* pszS3 = NULL);
    bool StartService();
    void SetStatus(DWORD dwState);
    bool Initialize();
    virtual void Run();
	virtual bool OnInit();
    virtual void OnStop();
    virtual void OnInterrogate();
    virtual void OnPause();
    virtual void OnContinue();
    virtual void OnShutdown();
    virtual bool OnUserControl(DWORD dwOpcode);
    void DebugMsg(const char* pszFormat, ...);
    
    // static member functions
    static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    static void WINAPI Handler(DWORD dwOpcode);

    // data members
	std::string m_serviceName;
	std::string m_versionInfo;
	std::string m_desc;
    SERVICE_STATUS_HANDLE m_serviceStatus;
    SERVICE_STATUS m_status;
    volatile bool m_isRunning;

    // static data
    static NTService* m_this; // nasty hack to get object ptr

private:
    HANDLE m_eventSource;
};
