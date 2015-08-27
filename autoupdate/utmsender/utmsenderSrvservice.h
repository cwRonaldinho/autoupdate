#ifndef __ALARMSRV_SERVICE_H
#define __ALARMSRV_SERVICE_H

#include <string>
#include <cacti/mtl/ServiceSkeleton.h>

#include "sessionmanager.h"
#include "TestUnit.h"

//#include "peerinfo.h"
#include "DataSource.h"
#include "../../timeschedule/timeobserver.h"
#include "../../timeschedule/timeschedule.h"

using namespace std;
using namespace cacti;

#define  SYSTEM_CFG_FILE		"./autoupdate.ini"

#define  MAIN_CLIENT_CONFIG     "../Update_cfg.dat"

// key定义，暂时放在此处
#define _EvtStartSession 0x999000
#define _EvtKillSession	0x999001

#define _EvtStartWork	0x999002

#define _TagSessionID    0x998000

class USService : public ServiceSkeleton, TimeObserver
{
public:
	USService(int appport, MessageDispatcher * dispatcher);
	~USService();

	void SnapShot();

	void Notify();

protected:
	virtual bool init();
	virtual void uninit();

private:
	virtual void onMessage(const ServiceIdentifier& sender, UserTransferMessage& utm);
	void	onDefaultMessage(const ServiceIdentifier& sender, UserTransferMessage& utm);
	void	onModuleActive(const ServiceIdentifier& sender, UserTransferMessage& utm);
	void	onModuleDeactive(const ServiceIdentifier& sender, UserTransferMessage& utm);

	// 内部函数
	void	ResponseUTM(const ServiceIdentifier& sender, UserTransferMessage& utm, int status);

	// 消息函数
	void	onCreateSession(const ServiceIdentifier& sender, UserTransferMessage& utm);
	void	onTimeOut(const ServiceIdentifier& sender, UserTransferMessage& utm);
	void	onKillSession(const ServiceIdentifier& sender, UserTransferMessage& utm);

	void	onStartWork(const ServiceIdentifier& sender, UserTransferMessage& utm);

	// 测试方法
	void	onRecognizeResult(const ServiceIdentifier& sender, UserTransferMessage& utm);

	int		GetIIDBySID(const string& sID);
	string	GetSIDByIID(const int iID);

	void	LoadAllMacroes();
	void	LoadOneFile(const string& filename);
	void	LoadBig8File(const string& filename);

	//void	LoadPeers();

	bool	IsSessionMsg(const ServiceIdentifier& sender, UserTransferMessage& utm);

	int		GetSIDByApport(int appport, ServiceIdentifier& sid);

public:
	TASTestUnit	m_testUnit;

	TSessionManager m_sessionManager;		// session管理器

	map<string, int> m_strID2IID;			// 字串型消息ID到整型
	map<int, string> m_iID2SID;				// 整型消息ID到字串型消息ID

	//map<int, TPeerInfo*> m_peerTable;		// 保存所有peer信息
	TDataSource m_dataSource;

	friend class TASTestUnit;
	friend class TLuaSession;

private:
	//TimeSchedule m_timeSchedule;			// 时间调度器
	//TimeStrategy *m_pStrategy;
};


#endif //__ALARMSRV_SERVICE_H