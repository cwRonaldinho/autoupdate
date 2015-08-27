#ifndef __TSESSIONOBJECT_H_
#define __TSESSIONOBJECT_H_

#include <vector>
#include <string>
#include "rule.h"
#include "cacti/util/Timer.h"
#include "cacti/message/TransferMessage.h"

using namespace cacti;


class USService;
struct lua_State;

class TLuaSession
{
public:
	int		m_SessionID;
	
	USService *m_pOwner;
	
	int m_nFieldNum; //字段数量
	char m_Field[100][1000];

	int m_nOrgSender; //pdu包发送者的sender
	int m_nOrgSenderObj;
	int m_nOrgReceiverObj;
	int m_nOrgTranID;
	int m_nCurrResult;
	int m_nOrgMessageID;
	int m_nFlowID;
	int	m_nDataAccessID;
	int	m_nHTTPStatus;

	int m_nOrgPDUSender;		//代表RouteGW发过来的请求的原始发送者，区别于m_nOrgSender，该值一般为RouteGW
	int	m_nOrgPDUSenderObj;		//同上

	int m_TimerOutID; //自动删除Session定时器ID

	char  m_sLastAction[256];	
	
	// 为lua功能添加的变量
	RuleManager		m_ruleManager;		// 路由管理器
	
	lua_State*		m_L;				      // lua state指针
	
	int				m_lastValidPDUStatus;			// 接收到的最后一个有效pdu包的状态值

public:
	TLuaSession(USService* Owner, int SessionID=0);
	~TLuaSession();
	TLuaSession& operator=(TLuaSession& obj);

	void Snapshot(FILE *fp);
	string GetSnapshot();
	void PrintLog(int nLevel, char *fmt, ...);
	void PrintErrorLog(int nLevel, char *fmt, ...);
	int GetID(){return m_SessionID;}

	void ClearTimer(int TimerID);
	int SetTimer(int owner, int interval, int attr=0);
	//int PostMessage(RTCS_CTpdu pdu);

	//common
	void      StartTimer(int tval);
	void      StopTimer();
	
	// 为lua功能添加的方法
	int				LoadScript(const char* file);
	int				InitGlobalParams();
	int				ExecScript();
	int				GetMatchedRule(const Rule& rule, Rule& retRule);
	void			RegisterAllFunction2Lua();
	int				CallLuaFunc(const char* func);
	
	int				SyncFieldC2Lua(int traceFlag=1);
	int				SyncFieldLua2C();
	
	int				ClearRule();
	
	int 			SetLuaPath(lua_State* L, const char* path);
	
	// 注册到lua的成员函数
	int _LuaDataAccess(int templateID, int dataSource, int timeout);
	int _LuaResponseMessage(int status, int empty1, int empty2);
	int _LuaStartTimer(int val, int empty1, int empty2);
	int _LuaStopTimer(int, int, int);
	int _LuaExit(int empty1=0, int empty2=0, int empty3=0);
	int _LuaDriverReturn(int receiverGor, int receiverObj, int status);
	int _LuaGetSessionID(int empty1=0, int empty2=0, int empty3=0);
	int _LuaSetStatus(int status, int, int);
	int _LuaPostSelfMsg(int, int, int);
	int _LuaPostMsg(int appport, int, int);
	int _LuaSetResAppRef(int appref, int, int);
	int _LuaGetInput(int, int, int);
	int _LuaIsHttpDownloadOK(int, int, int);
	
	int _LuaSetNextState(string msgID, int status, string nextFuncName);
	int _LuaPrintLog(string s, int, string);
	int _LuaPutInt(string key, int value, string);

	int _LuaGetString(string key, string, string);
	int _LuaGetInt(string key, string, string);
	int _LuaGetNumber(string key, string, string);
	int _LuaCreateUtm(string, string, string);
	int _LuaSetMsgID(string msgID, string, string);
	int _LuaPutString(string key, string value, string);
	int _LuaPutStringArray(string key, string value, string);
	int _LuaReadConfig(string section,string key,string filename);
	int _LuaHttpDownload(string url,string savePath, string saveFileName);
	int _LuaCopyFile(string src,string dst, string);

	// 
	void SetLastValidUtm(const UserTransferMessage& utm) { m_lastValidUtm = utm; }
	void ClearLastValidUtm();

	void ClearPostUtm();

	// 辅助功能函数
	std::vector<std::string> split(std::string str,std::string pattern);

	int CallLuaFunction(void *lp, const char* func, char *vfmt, ...);

	// 心跳线程
	//int HeartBeatThread();

	string m_httpUrl;							// http下载路径
	string m_saveFullPath;						// 本地保存全路径，包含文件名

private:
	TimerID		m_timerID;						// 定时器ID
	UserTransferMessage m_lastValidUtm;			// 最近的有效utm消息包
	UserTransferMessage m_postUtm;				// 即将发送的utm

	u32 m_selfSpecResAppref;					// 自定义的发送消息中的 res.m_appref 值

	//u32 m_bHeartBeat;							// 是否触发心跳 0:否 1:是
	HANDLE m_handleDownHttp;					// http下载线程句柄
};

#endif