#include "stdafx.h"
#include "utmsenderSrvservice.h"
#include "Service.h"
#include "cacti/util/IniFile.h"
#include "cacti/logging/LazyLog.h"

#include "sessionobject.h"
#include "twglobal.h" 
#include "rule.h"
#include "TLuaExec.h"
#include "../../timeschedule/timestrategy.h"

cacti::LazyLog g_serviceLog("module");

// 全局函数
// 打印日志
void mprint(const char* fmt, ...)
{
	static char	msg[1024];
	va_list args;
	va_start(args,fmt);
	_vsnprintf_s(msg, 1022, fmt, args);
	va_end(args);

	// 时间
	time_t t = time(NULL);
	struct tm tm;
	localtime_s(&tm, &t);

	static char line[1048];
	sprintf_s(line, "[%02d:%02d:%02d] %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, msg);

	// 暂时使用printf打印
	//printf("%s", line);

	g_serviceLog.print(line);
}


MTL_BEGIN_MESSAGE_MAP(USService)
MTL_ON_MESSAGE(_EvtStartSession, onCreateSession)
MTL_ON_MESSAGE(_EvtTimeOut, onTimeOut)
MTL_ON_MESSAGE(_EvtKillSession, onKillSession)
MTL_ON_MESSAGE(_EvtStartWork,onStartWork)
//MTL_ON_MESSAGE(_EvtRecognizeResult, onRecognizeResult)		// 测试该消息
MTL_END_MESSAGE_MAP

USService::USService(int appport, MessageDispatcher * dispatcher )
:ServiceSkeleton(appport, dispatcher)
{
	g_serviceLog.open();

	//m_pStrategy = NULL;
 }

USService::~USService()
{
	uninit();
	Service::printConsole("AlarmServer Service exit\n");
}

bool USService::init()
{
	Service::printConsole("AlarmServer Service init OK \n");
	m_testUnit.setOwner(this);

	// 加载lua，供后续函数调用
	if (0 != TLuaExec::LoadAndExecLua("./system/systemop.lua"))
	{
		g_serviceLog.print("[Error] Load lua fail\n");
	}

	// 加载宏定义
	LoadAllMacroes();

	// 加载peer信息
	//LoadPeers();

	// 开启时间调度
// 	unsigned int day = GetPrivateProfileIntEx("repeat", "day", 1, SYSTEM_CFG_FILE);
// 	if (day <=0 || day >= 31)
// 	{
// 		day = 1;
// 	}
	
	// 不需要定时执行
// 	char szTime[1024];
// 	GetPrivateProfileString("repeat", "time", "00:00:00", szTime, 1024, SYSTEM_CFG_FILE);
// 	unsigned int hour = 0;
// 	unsigned int minute = 0;
// 	unsigned int second = 0;
// 	// 解析出时、分、秒
// 	sscanf_s(szTime, "%d:%d:%d", &hour, &minute, &second);
// 	hour =  (hour > 23) ? 0 : hour;
// 	minute = (minute > 59) ? 0 : minute;
// 	second = (second > 59) ? 0 : second;
// 	
// 	m_pStrategy = new MonthStrategy(day, hour, minute, second);
// 	m_timeSchedule.Register(this, m_pStrategy);
// 	m_timeSchedule.Start();

	// 启动后，开启自动更新定时器
	UserTransferMessagePtr utm(new UserTransferMessage);
	utm->setMessageId(_EvtStartWork);
	
	unsigned int nWaitTimeAfterInit = GetPrivateProfileIntEx("system", "waittimeafterinit", 5, SYSTEM_CFG_FILE);
	printf("nWaitTimeAfterInit=%d\n", nWaitTimeAfterInit);
	setTimer(nWaitTimeAfterInit * 1000, utm);
	return true;
}

void USService::uninit()
{
// 	if (m_pStrategy)
// 	{
// 		delete m_pStrategy;
// 		m_pStrategy = NULL;
// 	}

	m_sessionManager.ClearAll();
	return;
}


void USService::onDefaultMessage(const ServiceIdentifier &sender,UserTransferMessage& utm)
{
	// 判断该消息是否为当前状态下，某个session正在等待的消息，如果是，则触发该session继续走流程
	if (IsSessionMsg(sender, utm))
	{
		return;
	}	

	string sID = GetSIDByIID(utm.getMessageId());

	switch(utm.getMessageId())
	{
	case _EvtModuleActive:
		onModuleActive(sender,utm);
		break;
	case _EvtModuleDeactive:
		onModuleDeactive(sender,utm);
		break;
	default:
		if (sID == "")
		{
			g_serviceLog.print("[INFO] Recv Message (0x%08X) but no handler\n", utm.getMessageId());
		}
		else
		{
			g_serviceLog.print("[INFO] Recv Message (%s) but no handler\n", sID.c_str());
		}
		break;
	}

}

void USService::onModuleActive(const ServiceIdentifier &sender, UserTransferMessage &utm)
{
	printf("Module active: %s\n", sender.toString2().c_str());
	m_dataSource.addServiceIDByApport(sender.m_appport, sender);	
}

void USService::onModuleDeactive(const ServiceIdentifier &sender, UserTransferMessage &utm)
{
	printf("Module deactive: %s\n", sender.toString2().c_str());
	m_dataSource.removeServiceIDByApport(sender.m_appport, sender);
}

void USService::ResponseUTM(const ServiceIdentifier& sender, UserTransferMessage& utm, int status)
{
	// TODO
}

void USService::onCreateSession(const ServiceIdentifier& sender, UserTransferMessage& utm)
{
	int sessionID = m_sessionManager.GenerateSessionID();
	TLuaSession *pSessionObject = new TLuaSession(this, sessionID);
	if (!pSessionObject)
	{
		ResponseUTM(sender, utm, 1);
		return;
	}

	// 添加到管理器
	m_sessionManager.AddSessionObject(pSessionObject);

	// 取参数
	int flowID = utm[_TagFlowId];

	// 加载流程，目前只使用到2个流程，所以直接定死
	char sLuaFile[1024];
	strcpy_s(sLuaFile, "./lua/autoupdate.lua");
// 	if (flowID == 1)
// 	{
// 		strcpy_s(sLuaFile, "./lua/autoupdate.lua");
// 	}
// 	else if (flowID == 2)
// 	{
// 		strcpy_s(sLuaFile, "./lua/heartbeat.lua");
// 	}
// 	else
// 		return;

// 	char sKey[100];
// 	sprintf_s(sKey, 100, "lua.%d", flowID);
// 	GetPrivateProfileString("lua", sKey, "./lua/autoupdate.lua", sLuaFile, sizeof(sLuaFile), SYSTEM_CFG_FILE);
// 	printf("lua source: %s\n", sLuaFile);
	if (pSessionObject->LoadScript(sLuaFile)) // TODO: 暂时写定
	{
		ResponseUTM(sender, utm, 2);
		m_sessionManager.DeleteSessionObject(pSessionObject->m_SessionID);
		return;
	}

	// 执行流程
	if (pSessionObject->ExecScript())			// 执行脚本失败
	{
		ResponseUTM(sender, utm, 2);
		m_sessionManager.DeleteSessionObject(pSessionObject->m_SessionID);
		return;
	}
}

/************************************************************************/
/* 
功能: 根据字串型消息ID得到整型消息ID
参数: 
	sID ---- 字串型消息ID
返回: 
	整型消息ID，如不存在，返回 IVALID_MESSAGEID
*/
/************************************************************************/
int USService::GetIIDBySID(const string& sID)
{
	map<string, int>::iterator it = m_strID2IID.find(sID);
	if (it != m_strID2IID.end())
	{
		return it->second;
	}

	return INVALID_MESSID;
}

/************************************************************************/
/* 
功能: 根据整型消息ID得到字串型消息ID
参数: 
	iID ---- 整型消息ID
返回: 
	整型消息ID，如不存在，返回空串
*/
/************************************************************************/
string USService::GetSIDByIID(int iID)
{
	map<int, string>::iterator it = m_iID2SID.find(iID);
	if (it != m_iID2SID.end())
	{
		return it->second;
	}

	return "";
}

/************************************************************************/
/* 
功能: 从头文件中加载并解析所有宏
参数: 
返回: 
*/
/************************************************************************/
// #define ADDIDMAP(sID, iID) m_strID2IID[#sID] = iID; \
// 	m_iID2SID[iID] = #sID;
void USService::LoadAllMacroes()
{
	// 解析出字串到整型的映射

	// 处理每个头文件
	LoadOneFile(".\\common\\evtmsg.h");
	//LoadOneFile(".\\common\\reqmsg.h");
	//LoadOneFile(".\\common\\resmsg.h");
	
	// tag.h的宏值全大于等于 0x80000000，使用特殊方法解析
	//LoadBig8File(".\\common\\tag.h");

	// 大于等于 0x80000000的情况已解决
	//LoadOneFile(".\\common\\tag.h");
}

/************************************************************************/
/* 
功能: 解析一个头文件
参数: 
返回: 
*/
/************************************************************************/
void USService::LoadOneFile(const string& filename)
{
	FILE *pf = fopen(filename.c_str(), "r");
	if (!pf)
	{
		g_serviceLog.print("[Error] Load macro file failed, open failed: %s\n", filename.c_str());
		return;
	}

	int intValue;
	char sKeyName[100];
	char line[1024];
	// 遍历文件每一行
	while (fgets(line, 1024, pf))
	{	
		int ret = 1;
		if (0 != TLuaExec::CallLuaFunction(0, "ParseLine", "s>isi", line, &ret, sKeyName, &intValue))
		{
			g_serviceLog.print("[Error] Call lua function failed\n");
			continue;
		}

		// 判断解析行结果
		if (0 == ret)
		{
			m_strID2IID[sKeyName] = intValue;
			m_iID2SID[intValue] = sKeyName;

			//g_serviceLog.print("%-30s = 0x%08X\n", sKeyName, intValue);
		}
	}

	fclose(pf);
}

/************************************************************************/
/* 
功能: 专门解析值大于等于0x80000000的宏
参数: 
返回: 
*/
/************************************************************************/
void USService::LoadBig8File(const string& filename)
{
	FILE *pf = fopen(filename.c_str(), "r");
	if (!pf)
	{
		g_serviceLog.print("[Error] Load macro file failed, open failed: %s\n", filename.c_str());
		return;
	}

	char intValue[20];
	char sKeyName[100];
	char line[1024];
	// 遍历文件每一行
	while (fgets(line, 1024, pf))
	{
		int ret = 1;
		if (0 != TLuaExec::CallLuaFunction(0, "ParseLineBig8", "s>iss", line, &ret, sKeyName, intValue))
		{
			g_serviceLog.print("[Error] Call lua function failed\n");
			continue;
		}

		// 判断长度，最长为10个字节 "0xffffffff"
		if (strlen(intValue) > 10)
		{
			continue;
		}

		// 将十六进制形式的字串转换为整数
		//char *str;
		//unsigned int tIntValue = (int)strtol(intValue, &str, 16);
		int tIntValue = 0;
		sscanf(intValue, "%x", &tIntValue);   

		// 判断解析行结果
		if (0 == ret)
		{
			m_strID2IID[sKeyName] = tIntValue;
			m_iID2SID[tIntValue] = sKeyName;
		}
	}

	fclose(pf);
}

// /************************************************************************/
// /* 
// 功能: 加载peers信息
// 参数: 
// 返回: 
// */
// /************************************************************************/
// void USService::LoadPeers()
// {
// // 	int appid = 0;
// // 	int appport = 0;
// // 	int id = 0;
// // 	char key[100];
// // 	char url[1024];
// // 	int luaRet = -1;
// // 
// // 	for (id = 1; ; id++;)
// // 	{
// // 		sprintf_s(key, "peers%d", id);
// // 		GetPrivateProfileString("peers", key, url, 1024, SYSTEM_CFG_FILE);
// // 		if (0 == strcmp(url, ""))
// // 		{
// // 			break;
// // 		}	
// // 	}
// }

/************************************************************************/
/* 
功能: 定时器触发函数
参数: 
功能: 
*/
/************************************************************************/
void USService::onTimeOut(const ServiceIdentifier& sender, UserTransferMessage& utm)
{
	// TODO: 特殊定时器判断

	// 查找session
	u32 sessionID = utm[_TagSessionID];
	TLuaSession *pSessionObject = m_sessionManager.GetSessionObject(sessionID);
	if (!pSessionObject)
	{
		mprint("[Error]: SessionObject not exist, SessionID=0x%08x", sessionID);
		return;
	}

	// 状态跳转
	// 根据整型消息ID得到字串型消息ID
	int iID = utm.getMessageId();
	string sID = GetSIDByIID(iID);
	if (sID == "")
	{
		mprint("[Error] OnTimer: No str message id according to msgID(0x%08x)", iID);
		return;
	}

	// 跳转规则匹配
	Rule matchedRule;	// 匹配规则
	int ret = pSessionObject->GetMatchedRule(Rule(iID, sID, utm.getReturn(), ""), matchedRule);
	if (ret != 0)
	{
		mprint("[Info] OnTimer: No matched rule");
		return;
	}

	// 停止定时器
	pSessionObject->StopTimer();

	// 跳转到下一状态
	pSessionObject->CallLuaFunc(matchedRule.m_state.GetFuncName().c_str());
}

/************************************************************************/
/* 
功能: 关闭session
参数: 
功能: 
*/
/************************************************************************/
void USService::onKillSession(const ServiceIdentifier& sender, UserTransferMessage& utm)
{
	u32 sessionID = utm[_TagSessionID];
	m_sessionManager.DeleteSessionObject(sessionID);
}

/************************************************************************/
/* 
功能: 处理识别结果消息
参数: 
功能: 
*/
/************************************************************************/
void USService::onRecognizeResult(const ServiceIdentifier& sender, UserTransferMessage& utm)
{
	// 处理过程与 onTimeOut 类似，但需要将utm中所有参数正确传递给lua

	// 将utm中所有参数正确传递给lua

	// 方案: 
	// 在处理每个消息时，在c中将其所有key-value放入

	// utm数据处理
	// 堆栈L里创建临时table
// 	lua_newtable(m_L);  
// 	// 将utm中所有数据放入临时table对象中
// 	for (int i=1; i<=MAX_FIELD_COUNT; i++)
// 	{
// 		lua_pushinteger(m_L, i);
// 		lua_pushstring(m_L, m_Field[i-1]);
// 		lua_rawset(m_L, -3);
// 	} 
// 	lua_setglobal(m_L, LC_FIELD);


}

/************************************************************************/
/*
功能: 判断utm是否为某session正在等待的消息
参数: 
返回: 
	true ---- 是
	false --- 不是
*/
/************************************************************************/
bool USService::IsSessionMsg(const ServiceIdentifier& sender, UserTransferMessage& utm)
{
	// 查找session
	u32 sessionID = utm.getRes().m_appref; 
	TLuaSession *pSessionObject = m_sessionManager.GetSessionObject(sessionID);
	if (!pSessionObject)
	{
		return false;
	}

	// 注意: 即使能找到session，也不能证明该utm就肯定是需要送给该session的，需要继续判断

	// 判断该utm是否为该session正在等待的消息，通过规则匹配判断

	// 状态跳转
	// 根据整型消息ID得到字串型消息ID
	int iID = utm.getMessageId();
	string sID = GetSIDByIID(iID);
	if (sID == "")
	{
		mprint("[Error] IsSessionMsg: No str message id according to msgID(0x%08x)", iID);
		return false;
	}

	// 跳转规则匹配
	Rule matchedRule;	// 匹配规则
	int ret = pSessionObject->GetMatchedRule(Rule(iID, sID, utm.getReturn(), ""), matchedRule);
	if (ret != 0)
	// 找不到匹配规则，说明该utm不是该session正在等待的消息
	{
		return false;
	}

	// 停止定时器
	pSessionObject->StopTimer();

	// 让sessi on保存最近一次接收到的这个utm
	pSessionObject->SetLastValidUtm(utm);

	// 跳转到下一状态
	pSessionObject->CallLuaFunc(matchedRule.m_state.GetFuncName().c_str());

	return 1;
}

/************************************************************************/
/*
功能: 获取appport对应程序的service id
参数: 
返回: 
	0 ---- 成功
	其它 - 失败
*/
/************************************************************************/
int USService::GetSIDByApport(int appport, ServiceIdentifier& sid)
{
	return m_dataSource.getServiceIDByApport(appport, sid);
}

/************************************************************************/
/*
功能: 输出内存快照
参数: 
返回: 
*/
/************************************************************************/
void USService::SnapShot()
{
	m_sessionManager.Snapshot(NULL);
}

/************************************************************************/
/*
功能: 时间周期到期时的回调
参数: 
返回: 
*/
/************************************************************************/
void USService::Notify()
{
	printf("on time to execute\n");

	// 触发流程
	UserTransferMessagePtr utm(new UserTransferMessage);
	utm->setMessageId(_EvtStartSession);
	//utm->setReq(ServiceIdentifier(4, AppPort::_apAlarmProxy, 0));
	//utm->setRes(m_owner->myIdentifier());
	//utm->setReturn(99);

	//(*utm)[_TagFlowId] = (u32)999;
	//(*utm)[_TagSessionID] = "kkkk";

	postSelfMessage(utm);
}

/************************************************************************/
/*
功能: 开始触发脚本
参数: 
返回: 
*/
/************************************************************************/
void USService::onStartWork(const ServiceIdentifier& sender, UserTransferMessage& utm)
{
	UserTransferMessagePtr startSessionUtm(new UserTransferMessage);
	(*startSessionUtm)[_TagFlowId] = (u32)1;
	// 设置请求需要的相关参数
	startSessionUtm->setMessageId(_EvtStartSession);
	startSessionUtm->setReq(ServiceIdentifier(4, AppPort::_apAlarmProxy, 0));
	startSessionUtm->setRes(myIdentifier());
		
	postSelfMessage(startSessionUtm);
}