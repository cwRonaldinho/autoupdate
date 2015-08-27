#include "stdafx.h"
#include "sessionobject.h"
#include "utmsenderSrvservice.h"
#include "twglobal.h"
#include "CDownloadCTL.h"
#include "cacti\util\FileSystem.h"
using namespace cacti;

extern "C"
{ 
#include "lua.h"  
#include "lualib.h"
#include "lauxlib.h"
}

// lua与c通用的全局变量名
#define LC_MSG_STATUS "_Status"
#define LC_RESULT "_Result"
#define lC_SESSION_ID "_SessionID"

extern USService *g_usservice;
extern void mprint(const char* fmt, ...);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]= __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////
/* 注册类成员函数到lua */
// 注册DataAccess、ResponseMessage等3个int输入参数和一个int输出参数
typedef int (TLuaSession::*Fun_iii_i)(int, int, int);

//函数指针相关数据会存到UpValue的第一个值中，此处从upvalue中取出
unsigned char* getFirstUpValue(lua_State *L)
{	
	unsigned char* buffer = (unsigned char*)lua_touserdata(L, lua_upvalueindex(1));	
	return buffer;	
}

/*
实现对class里面memeber function的调用  
参数cla 要调用的类的实例	
TLuaSession::*func 类的函数指针	  
*/
int callMemFunc(int sessionID, Fun_iii_i func, lua_State *L, int index)
{		
	int v1 = 0;
	int v2 = 0;
	int v3 = 0;

#ifdef _DEBUG
	// 输出L的相关信息
	//printf("stack top = 0x%08x\n", L->stack);
#endif

	// 从lua栈中取出实参
	if (lua_isnumber(L, 1))
	{ 
		v1 = lua_tointeger(L, 1);
	}
	if (lua_isnumber(L, 2))
	{ 
		v2 = lua_tointeger(L, 2);
	}
	if (lua_isnumber(L, 3))
	{ 
		v3 = lua_tointeger(L, 3);
	}

	// 从module中获取session对象
	TLuaSession* pSessionObject = NULL;
	pSessionObject = g_usservice->m_sessionManager.GetSessionObject(sessionID);
	if (pSessionObject)
	{
		return (pSessionObject->*func)(v1, v2, v3);
	}

	return 1;	
}

/*
实现callMemFunc的lua调用形式封装  
*/
int directCallMemFunc(lua_State *L)
{	
	//得到函数指针	
	unsigned char* buffer = getFirstUpValue(L);

	//转换成相应的函数调用	
	int ret = callMemFunc(*(int*)(buffer), *(Fun_iii_i*)(buffer + sizeof(int)), L, 1);	
	lua_pushnumber(L, ret);   // 将成员函数的返回结果入栈，以返回给lua
	return 1;
}

/*
将directCallMemFunc注册进lua  
*/
void lua_pushdirectmemclosure(lua_State *L, int sessionID, Fun_iii_i func, unsigned int nupvalues)
{	
	//创建userdata并把cla和func指针的值拷贝进去	
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(int) + sizeof(func));	
	memcpy(buffer, &sessionID, sizeof(int));
	memcpy(buffer + sizeof(int), &func, sizeof(func));	
	lua_pushcclosure(L, directCallMemFunc, nupvalues + 1);
}

#define lua_directregistry_memfunc_iii_i(L, name, sessionID, func) \
	lua_pushstring(L, name); \
	lua_pushdirectmemclosure(L, sessionID, func, 0); \
	lua_settable(L, LUA_GLOBALSINDEX);

// 注册 SetNextState  输入参数: int int string  输出参数: int
typedef int (TLuaSession::*Fun_sis_i)(string, int, string);

/*
实现对class里面memeber function的调用  
参数cla 要调用的类的实例	
TLuaSession::*func 类的函数指针	  
*/
int callMemFunc_sis_i(int sessionID, Fun_sis_i func, lua_State *L, int index)
{		
	string v1 = "";
	int v2 = 0;
	string v3 = "";

	// 从lua栈中取出实参
	if (lua_isstring(L, 1))
	{ 
		v1 = lua_tostring(L, 1);
	}
	if (lua_isnumber(L, 2))
	{ 
		v2 = lua_tointeger(L, 2);
	}
	if (lua_isstring(L, 3))
	{ 
		v3 = lua_tostring(L, 3);
	}

	// 从module中获取session对象
	TLuaSession* pSessionObject = NULL;
	pSessionObject = g_usservice->m_sessionManager.GetSessionObject(sessionID);
	if (pSessionObject)
	{
		return (pSessionObject->*func)(v1, v2, v3);
	}

	return 1;	
}

/*
实现callMemFunc的lua调用形式封装  
*/
int directCallMemFunc_sis_i(lua_State *L)
{	
	//得到函数指针	
	unsigned char* buffer = getFirstUpValue(L);

	//转换成相应的函数调用	
	int ret = callMemFunc_sis_i(*(int*)buffer, *(Fun_sis_i*)(buffer + sizeof(int)), L, 1);
	lua_pushinteger(L, ret);
	return 1;
}

/*
将directCallMemFunc注册进lua  
*/
void lua_pushdirectmemclosure_sis_i(lua_State *L, int sessionID, Fun_sis_i func, unsigned int nupvalues)
{	
	//创建userdata并把cla和func指针的值拷贝进去	
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(int) + sizeof(func));	
	memcpy(buffer, &sessionID, sizeof(int));
	memcpy(buffer + sizeof(int), &func, sizeof(func));

	lua_pushcclosure(L, directCallMemFunc_sis_i, nupvalues + 1);	
}

#define lua_directregistry_memfunc_sis_i(L, name, sessionID, func) \
	lua_pushstring(L, name); \
	lua_pushdirectmemclosure_sis_i(L, sessionID, func, 0); \
	lua_settable(L, LUA_GLOBALSINDEX);

// 注册 SendHttpReq  输入参数: string int int int  输出参数: int
typedef int (TLuaSession::*Fun_siii_i)(string, int, int, int);

/*
实现对class里面memeber function的调用  
参数cla 要调用的类的实例	
TLuaSession::*func 类的函数指针	  
*/
int callMemFunc_siii_i(int sessionID, Fun_siii_i func, lua_State *L, int index)
{		
	string v1 = "";
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;

	// 从lua栈中取出实参
	if (lua_isstring(L, 1))
	{ 
		v1 = lua_tostring(L, 1);
	}
	if (lua_isnumber(L, 2))
	{ 
		v2 = lua_tointeger(L, 2);
	}
	if (lua_isnumber(L, 3))
	{ 
		v3 = lua_tointeger(L, 3);
	}
	if (lua_isnumber(L, 4))
	{ 
		v4 = lua_tointeger(L, 4);
	}

	// 从module中获取session对象
	TLuaSession* pSessionObject = NULL;
	pSessionObject = g_usservice->m_sessionManager.GetSessionObject(sessionID);
	if (pSessionObject)
	{
		return (pSessionObject->*func)(v1, v2, v3, v4);
	}

	return 1;	
}

/*
实现callMemFunc的lua调用形式封装  
*/
int directCallMemFunc_siii_i(lua_State *L)
{	
	//得到函数指针	
	unsigned char* buffer = getFirstUpValue(L);

	//转换成相应的函数调用	
	int ret = callMemFunc_siii_i(*(int*)buffer, *(Fun_siii_i*)(buffer + sizeof(int)), L, 1);	
	lua_pushnumber(L, ret);   // 将成员函数的返回结果入栈，以返回给lua
	return 1;
}

/*
将directCallMemFunc注册进lua  
*/
void lua_pushdirectmemclosure_siii_i(lua_State *L, int sessionID, Fun_siii_i func, unsigned int nupvalues)
{	
	//创建userdata并把cla和func指针的值拷贝进去	
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(int) + sizeof(func));	
	memcpy(buffer, &sessionID, sizeof(int));
	memcpy(buffer + sizeof(int), &func, sizeof(func));

	lua_pushcclosure(L, directCallMemFunc_siii_i, nupvalues + 1);	
}

#define lua_directregistry_memfunc_siii_i(L, name, sessionID, func) \
	lua_pushstring(L, name); \
	lua_pushdirectmemclosure_siii_i(L, sessionID, func, 0); \
	lua_settable(L, LUA_GLOBALSINDEX);
///////////////////////////////////////////////////////////////////////////

// 注册 SendHttpReq  输入参数: string int int int  输出参数: int
typedef int (TLuaSession::*Fun_sss_i)(string, string, string);

/*
实现对class里面memeber function的调用  
参数cla 要调用的类的实例	
TLuaSession::*func 类的函数指针	  
*/
int callMemFunc_sss_i(int sessionID, Fun_sss_i func, lua_State *L, int index)
{		
	string v1 = "";
	string v2 = "";
	string v3 = "";

	// 从lua栈中取出实参
	if (lua_isstring(L, 1))
	{ 
		v1 = lua_tostring(L, 1);
	}
	if (lua_isstring(L, 2))
	{ 
		v2 = lua_tostring(L, 2);
	}
	if (lua_isstring(L, 3))
	{ 
		v3 = lua_tostring(L, 3);
	}

	// 从module中获取session对象
	TLuaSession* pSessionObject = NULL;
	pSessionObject = g_usservice->m_sessionManager.GetSessionObject(sessionID);
	if (pSessionObject)
	{
		return (pSessionObject->*func)(v1, v2, v3);
	}

	return 1;	
}

/*
实现callMemFunc的lua调用形式封装  
*/
int directCallMemFunc_sss_i(lua_State *L)
{	
	//得到函数指针	
	unsigned char* buffer = getFirstUpValue(L);

	//转换成相应的函数调用	
	int ret = callMemFunc_sss_i(*(int*)buffer, *(Fun_sss_i*)(buffer + sizeof(int)), L, 1);	
	lua_pushnumber(L, ret);   // 将成员函数的返回结果入栈，以返回给lua
	return 1;
}

/*
将directCallMemFunc注册进lua  
*/
void lua_pushdirectmemclosure_sss_i(lua_State *L, int sessionID, Fun_sss_i func, unsigned int nupvalues)
{	
	//创建userdata并把cla和func指针的值拷贝进去	
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(int) + sizeof(func));	
	memcpy(buffer, &sessionID, sizeof(int));
	memcpy(buffer + sizeof(int), &func, sizeof(func));

	lua_pushcclosure(L, directCallMemFunc_sss_i, nupvalues + 1);	
}

#define lua_directregistry_memfunc_sss_i(L, name, sessionID, func) \
	lua_pushstring(L, name); \
	lua_pushdirectmemclosure_sss_i(L, sessionID, func, 0); \
	lua_settable(L, LUA_GLOBALSINDEX);
///////////////////////////////////////////////////////////////////////////

// 调用lua函数
int call_lua_funcion(const UserTransferMessage& utm, TLuaSession* pSession, lua_State* L, const char* func)
{
// 	if (!pSession)
// 	{
// 		return 1;
// 	}
// 
// 	if (!L || !func || 0==strlen(func))
// 	{
// 		return 1;
// 	}
// 
// 	int narg;
// 	int nret;
// 	int nindex;
// 
// 	lua_getglobal(L, func);
// 	if(0 == lua_isfunction(L, -1))
// 	{
// 		lua_pop(L, 1);
// 		// lua函数不存在
// 		mprint("[0x%08X][Error](%s) is not a lua function", pSession->m_SessionID, func);
// 		return 1;
// 	}
// 
// 	va_list ap;
// 	va_start(ap, fmt);
// 	for(narg=0; *fmt; ++narg){
// 		luaL_checkstack(L, 1, "too many arguments");
// 
// 		switch(*fmt)
// 		{
// 		case 'd':
// 			lua_pushnumber(L, va_arg(ap, double));
// 			break;
// 		case 'i':
// 			lua_pushinteger(L, va_arg(ap, int));
// 			break;
// 		case 's':
// 			lua_pushstring(L, va_arg(ap, char*));
// 		case '>':
// 			goto end_in;
// 		default:
// 			break;
// 		}
// 	}
// end_in:
// 
// 	nret = strlen(fmt);	// 返回参数个数
// 	if(lua_pcall(L, narg, nret, 0) != 0)
// 	{
// 		// 调用lua函数失败
// 		mprint("[0x%08X][Error]Call lua function (%s) failed, msg=%s", pSession->m_SessionID, func, lua_tostring(L, -1));
// 		return 1;
// 	}
// 
// 	nindex = -nret;
// 
// 	// 取得lua函数返回值
// 	while(*fmt)
// 	{
// 		switch(*fmt)
// 		{
// 		case 'd':
// 			if (!lua_isnumber(L, nindex))
// 			{
// 				luaL_error(L, "wrong result type");
// 			}
// 			*va_arg(ap, double*) = lua_tonumber(L, nindex);
// 			break;
// 		case 'i':
// 			if (!lua_isnumber(L, nindex))
// 			{
// 				luaL_error(L, "wrong result type");
// 			}
// 			*va_arg(ap, int*) = lua_tointeger(L, nindex);
// 			break;
// 		case 's':
// 			if (!lua_isstring(L, nindex))
// 			{
// 				luaL_error(L, "wrong result type");
// 			}
// 			*va_arg(ap, const char**) = lua_tostring(L, nindex);
// 			break;
// 		default:
// 			luaL_error(L, "invalid option (%c)", *(fmt-1));
// 			break;
// 		}
// 		++nindex;
// 	}
// 
// 	lua_pop(L, nret);
// 
// 	va_end(ap);

	return 0;
}

TLuaSession::TLuaSession(USService* owner, int SessionID) : m_pOwner(owner)
{	
	m_L = luaL_newstate();
	m_SessionID = SessionID;
}

TLuaSession::~TLuaSession()
{
	// 	if(m_TimerOutID)
	// 		ClearTimer(m_TimerOutID);
	// 		
	if (m_L)
	{
		lua_close(m_L);
	}
}

TLuaSession& TLuaSession::operator=(TLuaSession& obj)
{
	return *this;
}



void TLuaSession::ClearTimer(int TimerID)
{
	//m_pOwner->ClearTimer(TimerID);
}

int TLuaSession::SetTimer(int owner, int interval, int attr)
{
	//return m_pOwner->SetTimer(owner, interval, attr);
	return 0;
}

// int TLuaSession::PostMessage(RTCS_CTpdu pdu)
// {
// 	m_DataAccessFlowID = GetTickCount();
// 	pdu.tranID = m_DataAccessFlowID;
// 
// 	return m_pOwner->PostMessage(pdu);
// }


///////////////////////////////////////////////////////////////////////////////
// 原子动作
///////////////////////////////////////////////////////////////////////////////
// Name: PrintLog
void TLuaSession::PrintLog(int nLevel, char *fmt, ...)
{
	// 	char szOut[LEN_LONG+1]= "";
	// 	va_list args;
	// 	va_start(args, fmt);
	// 	
	// 	vsprintf(szOut, fmt, args);
	// 
	// 	if(nLevel == 0)
	// 		PrintConsole(m_pOwner->m_ModuleName, szOut);
	// 
	// 	if(m_pOwner->logfile)
	// 		m_pOwner->logfile->Trace(nLevel, szOut);
	// 
	// 	va_end(args);
}

/* 
功能: 加载脚本
参数:
file: 脚本文件路径，规定使用相对路径
返回:
0: 成功
1: 失败
*/
int	TLuaSession::LoadScript(const char* file)
{
	//mprint("[0x%08X]BEGIN:", m_SessionID);

	if (!file || 0==strlen(file))
	{
		mprint("Load script failed, script file path is null\n");
		return 1;
	}

	// 加载lua文件
	int ret = luaL_loadfile(m_L, file);
	if (ret) 
	{                                  
		mprint("Load script failed, bad file %s, errno=%d, msg=%s\n", file, ret, lua_tostring(m_L, -1));
		return 1;
	}

	//函数加载Lua库
	luaL_openlibs(m_L);

	// 初始化 field
	InitGlobalParams();

	// 设置 package.path
	SetLuaPath(m_L, "./lua/");
	//SetLuaPath(m_L, "d:/Program Files (x86)/Lua/5.1/lua/");

	// 注册所有c函数
	RegisterAllFunction2Lua();

	// 同步参数到lua栈中
	SyncFieldC2Lua();

	return 0;
}

/* 
功能: 初始化所有全局变量
参数:
返回:
*/
int	TLuaSession::InitGlobalParams()
{
	// 	// 堆栈L里创建field table
	// 	lua_newtable(m_L);   
	// 	// 初始化所有field的值	
	// 	for (int i=1; i<=1000; i++)
	// 	{
	// 		lua_pushinteger(m_L, i);
	// 		lua_pushstring(m_L, m_Field[i-1]);
	// 		lua_rawset(m_L, -3);
	// 	} 
	// 	lua_setglobal(m_L, LC_FIELD);
	// 	
	// 	// 堆栈L里创建field num
	// 	lua_pushinteger(m_L, m_nFieldNum);
	// 	lua_setglobal(m_L, LC_NUM_FIELD);
	// 	
	// 堆栈L里创建 utm status
	lua_pushinteger(m_L, m_lastValidPDUStatus);
	lua_setglobal(m_L, LC_MSG_STATUS);
 
	// 堆栈L里创建 result
	lua_pushstring(m_L, "");
	lua_setglobal(m_L, LC_RESULT);

	// 堆栈L里创建 session id
	lua_pushinteger(m_L, m_SessionID);
	lua_setglobal(m_L, lC_SESSION_ID);

	return 0;
}

/* 
功能: 执行脚本
参数:
返回:
0: 成功
1: 失败
*/
int	TLuaSession::ExecScript()
{
	int ret = lua_pcall(m_L, 0, LUA_MULTRET, 0);
	if (ret)
	{
		mprint("[0x%08X]Execute script failed, errno=%d, msg=%s\n", m_SessionID, ret, lua_tostring(m_L, -1));
		return 1;
	}

	return 0;
}

/* 
功能: 查询匹配的规则
参数:
rule: 查询条件
retRule: 查询到的规则
返回:
0: 获取到匹配规则
1: 未获取到匹配规则
*/
int TLuaSession::GetMatchedRule(const Rule& rule, Rule& retRule)
{	
	int ret =  m_ruleManager.GetMatchedRule(rule, retRule);

	if (ret == 0)
	{
		//mprint("[0x%08X]%s:", m_SessionID, retRule.m_state.GetFuncName().c_str());		
	}
	else
	{
		mprint("[0x%08X][Error]Recv unexpected event (%s, %d)", m_SessionID, rule.m_sEvent.c_str(), rule.m_status);
	}
	return ret;
}

/* 
功能: 调用脚本函数
说明: 以当前模式，c中调用lua的函数时，不需要传递field之外的参数
参数:
func: 函数名
返回:
0: 成功
1: 失败
*/
int	TLuaSession::CallLuaFunc(const char* func)
{		
	if (!func || 0==strlen(func))
	{
		mprint("[0x%08X]Call function failed, function name is null\n", m_SessionID);
		return 1;
	}

	// 清空定时器
	StopTimer();

	// 清空跳转规则
	ClearRule();

	// 同步field到lua
	//m_lastValidPDUStatus = pdu.status;
	int traceFlag = 1;  // 输出日志标志  
	// 	if (pdu.messageID == EvtRTCS_IClientBase_Timer)		// 为时间消息时，则不打印field的值
	// 	{
	// 	  traceFlag = 0;
	// 	}
	SyncFieldC2Lua(traceFlag);

	// 调用lua函数
	lua_getglobal(m_L, func);
	if(0 == lua_isfunction(m_L, -1))
	{
		lua_pop(m_L, 1);
		// lua函数不存在
		mprint("[0x%08X][Error](%s) is not a lua function", m_SessionID, func);
		return 1;
	}

	luaL_checkstack(m_L, 1, "too many arguments");	

	if(lua_pcall(m_L, 0, 0, 0) != 0)	// All arguments and the function value are popped from the stack when the function is called. 
	{
		// 调用lua函数失败
		mprint("[0x%08X][Error]Call lua function (%s) failed, msg=%s", m_SessionID, func, lua_tostring(m_L, -1));

		// 脚本异常，结束本会话
		mprint("[Error]脚本异常，退出自动更新程序");
		exit(0);
		return 1;
	}

	// 因为调用的lua函数没有返回值，所以不用从栈中pop
	//lua_pop(m_L, nret);

	return 0;
}

/* 
功能: 注册所有函数到lua中
说明: 以目前方案，每注册一种类型的成员函数时，需要重要编写一套注册函数，后续考虑使用模板函数
参数:
返回:
*/
void TLuaSession::RegisterAllFunction2Lua()
{
	// 注册 SendMsg
	lua_directregistry_memfunc_iii_i(m_L, "C_SendMsg", m_SessionID, &TLuaSession::_LuaDataAccess);	
	// 	// 注册 ResponseMessage
	// 	lua_directregistry_memfunc_iii_i(m_L, "C_ResponseMessage", m_SessionID, &TLuaSession::_LuaResponseMessage);
	// 注册 StartTimer
	lua_directregistry_memfunc_iii_i(m_L, "C_StartTimer", m_SessionID, &TLuaSession::_LuaStartTimer);
	// 注册 StopTimer
	lua_directregistry_memfunc_iii_i(m_L, "C_StopTimer", m_SessionID, &TLuaSession::_LuaStopTimer);
	// 注册 Exit
	lua_directregistry_memfunc_iii_i(m_L, "C_Exit", m_SessionID, &TLuaSession::_LuaExit);
	// 注册 DriverReturn
	lua_directregistry_memfunc_iii_i(m_L, "C_DriverReturn", m_SessionID, &TLuaSession::_LuaDriverReturn);
	// 注册 GetSessionID
	lua_directregistry_memfunc_iii_i(m_L, "C_GetSessionID", m_SessionID, &TLuaSession::_LuaGetSessionID);
	//
	lua_directregistry_memfunc_iii_i(m_L, "C_SetStatus", m_SessionID, &TLuaSession::_LuaSetStatus);
	//
	lua_directregistry_memfunc_iii_i(m_L, "C_PostSelfMsg", m_SessionID, &TLuaSession::_LuaPostSelfMsg);
	//
	lua_directregistry_memfunc_iii_i(m_L, "C_PostMsg", m_SessionID, &TLuaSession::_LuaPostMsg);
	//
	lua_directregistry_memfunc_iii_i(m_L, "C_SetResAppRef", m_SessionID, &TLuaSession::_LuaSetResAppRef);
	//
	lua_directregistry_memfunc_iii_i(m_L, "C_GetInput", m_SessionID, &TLuaSession::_LuaGetInput);
	// 自动更新专用
	lua_directregistry_memfunc_iii_i(m_L, "C_IsHttpDownloadOK", m_SessionID, &TLuaSession::_LuaIsHttpDownloadOK);

	// 注册 SetNextState
	lua_directregistry_memfunc_sis_i(m_L, "C_SetNextState", m_SessionID, &TLuaSession::_LuaSetNextState);
	// 注册 PrintLog
	lua_directregistry_memfunc_sis_i(m_L, "C_PrintLog", m_SessionID, &TLuaSession::_LuaPrintLog);
	// 注册 PutInt
	lua_directregistry_memfunc_sis_i(m_L, "C_PutInt", m_SessionID, &TLuaSession::_LuaPutInt);

	// 注册 GetString
	lua_directregistry_memfunc_sss_i(m_L, "C_GetString", m_SessionID, &TLuaSession::_LuaGetString);
	// 注册 GetInt
	lua_directregistry_memfunc_sss_i(m_L, "C_GetInt", m_SessionID, &TLuaSession::_LuaGetInt);
	// 注册 GetNumber
	lua_directregistry_memfunc_sss_i(m_L, "C_GetNumber", m_SessionID, &TLuaSession::_LuaGetNumber);
	// 注册 CreateUtm
	lua_directregistry_memfunc_sss_i(m_L, "C_CreateUtm", m_SessionID, &TLuaSession::_LuaCreateUtm);
	// 注册 SetMsgID
	lua_directregistry_memfunc_sss_i(m_L, "C_SetMsgID", m_SessionID, &TLuaSession::_LuaSetMsgID);
	// 注册 PutString
	lua_directregistry_memfunc_sss_i(m_L, "C_PutString", m_SessionID, &TLuaSession::_LuaPutString);
	// 注册 PutStringArray
	lua_directregistry_memfunc_sss_i(m_L, "C_PutStringArray", m_SessionID, &TLuaSession::_LuaPutStringArray);
	// 注册 ReadConfig
	lua_directregistry_memfunc_sss_i(m_L, "C_ReadConfig", m_SessionID, &TLuaSession::_LuaReadConfig)
	//
	lua_directregistry_memfunc_sss_i(m_L, "C_HttpDownload", m_SessionID, &TLuaSession::_LuaHttpDownload);
	//
	lua_directregistry_memfunc_sss_i(m_L, "C_CopyFile", m_SessionID, &TLuaSession::_LuaCopyFile);
}

/* 
功能: 供lua调用的DataAccess函数
参数:
templateID: 模板号
dst: 数据源
timeout: 超时时间，单位毫秒
返回:
*/
int TLuaSession::_LuaDataAccess(int templateID, int dataSource, int timeout)
{		
	SyncFieldLua2C();
	mprint("[0x%08X] DataAccess(%d, %d, %d)", m_SessionID, templateID, dataSource, timeout);	// 因为用到了field，所以该行需要放在SyncFieldLua2C之后

	sprintf(m_sLastAction, "DataAccess()");

	// 	RTCS_CTpdu pdu;
	// 	char szFuncName[LEN_NORMAL+1]= "";
	// 	string _field[MAX_FIELD_NUM];
	// 	stringArray pField = _field;
	// 	int nTimerDelay = timeout;
	// 
	// 
	// 	strcpy(szFuncName, "TLuaSession::DataAccess()");
	// 
	// 	pdu.messageID = CmdRTCS_IDBP_DataAccess;
	// 	pdu.sender = m_pOwner->GetGOR();
	// 	pdu.senderObj = m_SessionID;
	// 
	// 	pdu.receiver =  m_pOwner->m_pDataSourceUINTMgr->GetDataSource(dataSource);
	// 	pdu.status = 0;
	// 	
	// 	if(pdu.receiver == INVALID_GOR)
	// 	{
	// 		mprint("[0x%08X][Error][%s]: DataAccess error, receiver is INVALID_GOR\n", m_SessionID, szFuncName);
	// 		PostAlarmMessage(ALARM_MDO_AppServer_ReqFailed, 0, 0, 0, "发送请求(DataAccess)失败，网关不在线!");
	// 
	// 		RTCS_CTpdu respPdu;		
	// 		respPdu.messageID   = RespRTCS_IDBP_DataAccess;
	// 		respPdu.sender      = m_pOwner->GetGOR();
	// 		respPdu.receiverObj = m_SessionID;
	// 		respPdu.senderObj = m_SessionID;
	// 		respPdu.receiver    = m_pOwner->GetGOR();
	// 		respPdu.status      = 1001;		
	// 		respPdu.PutString(RTCS_IDBP_SessionID, m_transSessionID);
	// 
	// 		PostMessage(respPdu);
	// 		
	// 		return RTCS_Error_FAIL;
	// 	}
	// 	
	// 	for(unsigned int i = 0; i < m_nFieldNum; i++)
	// 	{
	// 		_field[i]= m_Field[i];
	// 	}
	// 
	// 	pdu.PutUInt(RTCS_IDBP_DataAccessID, templateID);
	// 	pdu.PutUInt(RTCS_IDBP_FieldNum, m_nFieldNum);
	// 	pdu.PutStringArray(RTCS_IDBP_ParamData, pField, m_nFieldNum);
	// 
	// 	pdu.PutString(RTCS_IDBP_SPID, (string)m_SpID);
	// 	pdu.PutString(RTCS_IDBP_ServiceID, (string)m_ServiceID);
	// 	pdu.PutString(RTCS_IDBP_OperateID, (string)m_OperateID);
	// 	pdu.PutString(RTCS_IDBP_CallingNumber, (string)m_CallingNum);
	// 	pdu.PutString(RTCS_IDBP_CalledNumber, (string)m_CalledNum);
	// 
	// 	pdu.PutString(RTCS_IDBP_SessionID, m_transSessionID);
	// 	
	// 
	// 	
	// 	int nRet = PostMessage(pdu);
	// 	if(nRet != 0)
	// 	{
	// 		mprint("[0x%08X][Error]流程动作%s: PostMessage错误(Receiver=0x%08X)，请检查DataAccess的数据源是否正确, ret=%d！\n", 
	// 			m_SessionID, m_sLastAction, pdu.receiver, nRet);
	// 		PostAlarmMessage(ALARM_MDO_AppServer_ReqFailed, nRet, 0, 0, "发送请求(%s)失败，发包失败!", m_sLastAction);
	// 
	// 		pdu.messageID   = RespRTCS_IDBP_DataAccess;
	// 		pdu.sender      = m_pOwner->GetGOR();
	// 		pdu.receiverObj = m_SessionID;
	// 		pdu.senderObj = m_SessionID;
	// 		pdu.receiver    = m_pOwner->GetGOR();
	// 		pdu.status      = 1001;
	// 		
	// 		PostMessage(pdu);
	// 		
	// 		return RTCS_Error_FAIL;
	// 	}
	// 
	// 	if(m_TimerID)
	// 		ClearTimer(m_TimerID);
	// 	m_TimerID = SetTimer(m_SessionID, nTimerDelay);

	return 0;
}

/* 
功能: 供lua调用的ResponseMessage函数
说明: 原型与_LuaDataAccess一致，以便使用同样的代码能注册该函数到lua，第2、3个参数未使用
参数:
status: 回复值
返回:
*/
int TLuaSession::_LuaResponseMessage(int status, int empty1, int empty2)
{	
	sprintf(m_sLastAction, "ResponseMessage()");

	SyncFieldLua2C();

	// 	int nResult = status;
	// 	RTCS_CTpdu pdu;
	// 	string FieldArray[MAX_FIELD_NUM]; //字段赋值
	// 	stringArray pFieldArray = FieldArray;
	// 	char szFuncName[LEN_NORMAL+1]= "";
	// 
	// 	strcpy(szFuncName, "TLuaSession::ResponseMessage()");
	// 
	// 	pdu.sender = m_AppModuleGOR;
	// 	if(m_nOrgPDUSender != INVALID_GOR)
	// 	{
	// 		pdu.receiver = m_nOrgPDUSender;
	// 		pdu.receiverObj = m_nOrgPDUSenderObj;
	// 	}
	// 	else
	// 	{
	// 		pdu.receiver = m_nOrgSender;
	// 		pdu.receiverObj = m_nOrgSenderObj;
	// 	}
	// 	pdu.tranID = m_nOrgTranID;
	// 	pdu.messageID = m_nOrgMessageID | 0x20000000;
	// 	pdu.status = nResult;
	// 
	// 	for(unsigned int i = 0; i < m_nFieldNum; i++)
	// 	{
	// 		FieldArray[i]= m_Field[i];
	// 	}
	// 
	// 	pdu.PutUInt(RTCS_IDBP_DataAccessID, m_nFlowID); //流程号
	// 	pdu.PutUInt(RTCS_IDBP_FieldNum, m_nFieldNum);
	// 	pdu.PutStringArray(RTCS_IDBP_ParamData, pFieldArray, m_nFieldNum);
	// 	if(m_nHTTPStatus >= 0)
	// 		pdu.PutUInt(RTCS_ISPServer_HttpStatus, m_nHTTPStatus);
	// 
	// 	pdu.PutString(RTCS_IDBP_SessionID, m_transSessionID);
	// 
	// 	int ret = m_pOwner->PostMessage(pdu);
	// 	if(ret != 0)
	// 	{
	// 		PostAlarmMessage(ALARM_MDO_AppServer_RespFailed, ret, 0, 0, "回复IVR请求，发包失败!");
	// 		mprint("[0x%08X][Error][%s]: Send message error, Receiver=0x%08X, ret=%d\n", 
	// 			m_SessionID, szFuncName, pdu.receiver, ret);
	// 		
	// 		return 0;
	// 	}
	// 	
	// 	mprint("[0x%08X] ResponseMessage(%d)", m_SessionID, status);

	return 0;
}

/* 
功能: 设置定时器
说明: 原型与_LuaDataAccess一致，以便使用同样的代码能注册该函数到lua，第2、3个参数未使用
参数:
val: 时长，单位毫秒
返回:
*/
int TLuaSession::_LuaStartTimer(int val, int empty1, int empty2)
{
	//mprint("[0x%08X] StartTimer(%d)", m_SessionID, val);
	mprint("StartTimer(%d)", val);
	StartTimer(val);
	return 0;
}

/* 
功能: 关闭定时器
参数:
返回:
*/
int TLuaSession::_LuaStopTimer(int, int, int)
{
	mprint("[0x%08X] StopTimer()", m_SessionID);
	StopTimer();
	return 0;
}

/* 
功能: 结束session
说明: 原型与_LuaDataAccess一致，以便使用同样的代码能注册该函数到lua，第2、3个参数未使用
参数:
返回:
*/
int TLuaSession::_LuaExit(int, int, int)
{	
	mprint("[0x%08X] END()", m_SessionID);	

	StopTimer();

	UserTransferMessagePtr utm(new UserTransferMessage);
	utm->setMessageId(_EvtKillSession);
	(*utm)[_TagSessionID] = (u32)m_SessionID;
	m_pOwner->postSelfMessage(utm);
	return 0;
}

/* 
功能: 触发EvtReturn事件
参数:
receiverGor: 接收者gor
receiverObj: 
status: 状态
返回:
*/
int TLuaSession::_LuaDriverReturn(int receiverGor, int receiverObj, int status)
{
	char szFuncName[200+1]= "";	
	strcpy(szFuncName, "TLuaSession::DriverReturn()");

	SyncFieldLua2C();
	mprint("[0x%08X] DriverReturn(0x%08X, 0x%08X, %d)", m_SessionID, receiverGor, receiverObj, status);
	// 	if(receiverGor == INVALID_GOR)
	// 	{
	// 		mprint("[0x%08X][Error][%s]: DriverReturn error, receiver is INVALID_GOR\n", m_SessionID, szFuncName);
	// 		PostAlarmMessage(ALARM_MDO_AppServer_ReqFailed, 0, 0, 0, "发送请求(DriverReturn)失败!");
	// 		return RTCS_Error_FAIL;
	// 	}
	// 
	// 	RTCS_CTpdu pdu;
	// 	string _field[MAX_FIELD_NUM];
	// 	stringArray pField = _field;
	// 
	// 	pdu.messageID = EvtRTCS_DRIVER_Return;
	// 	pdu.sender = m_pOwner->GetGOR();
	// 	pdu.senderObj = m_SessionID;	
	// 	pdu.receiver =  receiverGor;
	// 	pdu.receiverObj = receiverObj;
	// 	pdu.status = status;	
	// 	
	// 	for(unsigned int i = 0; i < m_nFieldNum; i++)
	// 	{
	// 		_field[i]= m_Field[i];
	// 	}
	// 	
	// 	pdu.PutUInt(RTCS_IDBP_FieldNum, m_nFieldNum);
	// 	pdu.PutStringArray(RTCS_IDBP_ParamData, pField, m_nFieldNum);	
	// 	int nRet = PostMessage(pdu);
	// 	if(nRet != 0)
	// 	{
	// 		mprint("[0x%08X][Error]流程动作%s: DriverReturn错误(Receiver=0x%08X)，请检查数据是否正确, ret=%d！\n", 
	// 			m_SessionID, m_sLastAction, pdu.receiver, nRet);
	// 		PostAlarmMessage(ALARM_MDO_AppServer_ReqFailed, nRet, 0, 0, "发送请求(%s)失败，发包失败!", m_sLastAction);		
	// 		return RTCS_Error_FAIL;
	// 	}

	return 0;
}

/* 
功能: 获取Session ID
参数:
返回:
0: 成功, 输出结果放到_Result中
1: 失败
*/
int TLuaSession::_LuaGetSessionID(int , int , int)
{
	return m_SessionID;
}

/* 
功能: 设置发送utm的状态
参数:
	status ---- 状态
返回:
*/
int TLuaSession::_LuaSetStatus(int status, int, int)
{
	mprint("[0x%08X] SetStatus(%d)", m_SessionID, status);
	m_postUtm.setReturn(status);
	return 0;
}

/* 
功能: 向自己发送消息
参数:
返回:
*/
int TLuaSession::_LuaPostSelfMsg(int, int, int)
{
	mprint("[0x%08X] PostSelfMsg()", m_SessionID);
	if (m_postUtm.getMessageId() == INVALID_MESSID)
	{
		mprint("[0x%08X][Error]: CreateUtm action not done", m_SessionID);
		return 1;
	}

	m_pOwner->postSelfMessage(m_postUtm);

	return 0;
}

/* 
功能: 向指定appport对应的模块发送消息
参数:
	appport --
返回:
*/
int TLuaSession::_LuaPostMsg(int appport, int, int)
{
	mprint("[0x%08X] PostMsg(%d)", m_SessionID, appport);
	if (m_postUtm.getMessageId() == INVALID_MESSID)
	{
		mprint("[0x%08X][Error]: CreateUtm action not done", m_SessionID);
		return 1;
	}

	// 获取目标模块的服务ID
	ServiceIdentifier res;
	if (0 != m_pOwner->GetSIDByApport(appport, res))
	{
		mprint("[0x%08X] Post msg failed, dest program not online", m_SessionID);
		return 1;
	}

	res.m_appref = (m_selfSpecResAppref == -1) ? m_lastValidUtm.getReq().m_appref : m_selfSpecResAppref;		// 将session id存在此处
	m_postUtm.setRes(res);

	m_pOwner->postMessage(res, m_postUtm);
	return 0;
}

/* 
功能: 设置发送消息中的 res.m_appref 值，如果该值有设置，则优先使用该值，否则使用m_lastValidUtm.getReq().m_appref
参数:
返回:
*/
int TLuaSession::_LuaSetResAppRef(int appref, int, int)
{
	mprint("[0x%08X] SetResAppref(%d)", m_SessionID, appref);
	m_selfSpecResAppref = appref;
	return 0;
}

/* 
功能: 获取用户输入，结果放入全局变量 _Result 中
参数:
返回:
*/
int TLuaSession::_LuaGetInput(int, int, int)
{
	char sTmp[1024];
	scanf_s("%s", sTmp);

	lua_pushstring(m_L, sTmp);
	lua_setglobal(m_L, LC_RESULT);
	return 0;
}

// 临时结构体
// typedef struct TDHttpParam{
// 	string url;
// 	string savePath;
// 	string saveFileName;
// 
// 	TDHttpParam(string v1, string v2, string v3) { url=v1; savePath=v2; saveFileName=v3;}
// } TDHttpParam;

// http下载线程函数
unsigned int __stdcall HttpDownloadThreadFun(void *param)
{
	TLuaSession *pSession = (TLuaSession*)param;
	if (!pSession)
	{
		return 1;
	}

	DownloadCTL obj;
	obj.Init();
	obj.SetDownloadStatus(TRUE);		// release版本下必须设置该值，否则默认为0值
	//printf("-- begin: %s %s\n", pSession->m_httpUrl.c_str(), pSession->m_saveFullPath.c_str());
	obj.WriteToLocalFile((char*)pSession->m_httpUrl.c_str(), (char*)pSession->m_saveFullPath.c_str());
// 	char *url = "http://demo.zksr.cn/upload/priceTag/bbb.wmv";
// 	char *savefilename = "ddd.wmv";
//	obj.WriteToLocalFile(url,savefilename);
//	printf("end\n");
	return 0;
}

/* 
功能: 开始http下载
参数:
返回:
*/
 int TLuaSession::_LuaHttpDownload(string url,string savePath, string saveFileName)
 {
	 m_httpUrl = url;
	 m_saveFullPath = savePath;

	 unsigned threadID;
	 m_handleDownHttp = (HANDLE)_beginthreadex(0, 0, HttpDownloadThreadFun, (void*)this, 0, &threadID);
	 //printf("handle: 0x%08x threadid: %d\n", m_handleDownHttp, threadID);
	 return 0;
 }

 /* 
 功能: 拷贝文件
 参数:
 返回:
 */
 int TLuaSession::_LuaCopyFile(string src,string dst, string)
 {
	 mprint("CopyFile(%s, %s)", src.c_str(), dst.c_str());
	 return FileSystem::copyfile(src.c_str(), dst.c_str());
 }

 /* 
 功能: 判断http下载是否完成
 参数:
 返回:
	1: 已成功
	0: 未成功
 */
int TLuaSession::_LuaIsHttpDownloadOK(int, int, int)
{
	DWORD ret = WaitForSingleObject(m_handleDownHttp, 1000);
	if (ret == WAIT_OBJECT_0)
	{
		CloseHandle(m_handleDownHttp);
	}

	return (ret == WAIT_OBJECT_0) ? 1 : 0;
}

// 心跳线程函数
// unsigned int __stdcall HeartBeatThreadFun(void *param)
// {
// 	TLuaSession *pSession = (TLuaSession*)param;
// 	if (!pSession)
// 	{
// 		return 1;
// 	}
// 
// 	pSession->HeartBeatThread();
// 	return 0;
// }

/* 
功能: 创建自动更新的心跳线程
参数:
返回:
*/
// int TLuaSession::_LuaStartHeartBeat(int, int, int)
// {
// 	m_bHeartBeat = 1;
// 
// 	unsigned threadID;
// 	m_handleHeartBeat = (HANDLE)_beginthreadex(0, 0, HeartBeatThreadFun, this, 0, &threadID);
// // 	char *url = "http://demo.zksr.cn/gateway/pos_gateway!openAPI.action";
// // 	char *body = "method=heartBeat&key=201504291510112ABFBE";
// // 	char retMsg[1024];
// // 	CallLuaFunction(0, "HttpPost", "ss>s", url, body, retMsg);
// // 	printf("heart beat result: %s\n", retMsg);
// 	return 0;
// }

// 心跳线程
// int TLuaSession::HeartBeatThread()
// {
// 	// 读取url
// 	char url[1024];
// 	GetPrivateProfileString("port", "httpurl", "", url, sizeof(url), MAIN_CLIENT_CONFIG);
// 	printf("heart beat url: %s\n", url);
// 	
// 	// 读取客户端id
// 	char key[1024];
// 	GetPrivateProfileString("port", "key", "", key, sizeof(key), MAIN_CLIENT_CONFIG);
// 	printf("client id: %s\n", key);
// 
// 	char body[1024];
// 	sprintf_s(body, 1024, "method=heartbeat&key=%s", key);
// 	unsigned int nHeartBeatInterval = GetPrivateProfileInt("system", "heartbeatinterval", 1, SYSTEM_CFG_FILE);
// 	nHeartBeatInterval = nHeartBeatInterval<1 ? 1 : nHeartBeatInterval;
// 	printf("HeartBeatInterval=%d\n", nHeartBeatInterval);
// 	while(m_bHeartBeat != 0)
// 	{
// 		mprint("heart beat...");
// 		
// 		char retMsg[1024];
// 		CallLuaFunction(0, "HttpPost", "ss>s", url, body, retMsg);
// 		
// 		mprint("heart beat result: %s\n", retMsg);
// 		
// 		Sleep(nHeartBeatInterval * 1000);
// 	}
// 	mprint("heart beat thread over");
// 	return 0;
// }

/* 
功能: 创建自动更新的心跳线程
参数:
返回:
*/
// int TLuaSession::_LuaStopHeartBeat(int, int, int)
// {
// 	m_bHeartBeat = 0;
// 
// 	unsigned threadID;
// 	WaitForSingleObject(m_handleHeartBeat, INFINITE);
// 	// Destroy the thread object.
// 	CloseHandle(m_handleHeartBeat);
// 	return 0;
// }

/* 
功能: 设置下一个跳转状态
参数:
status: 回复值
返回:
*/
int TLuaSession::_LuaSetNextState(string msgID, int status, string nextFuncName)
{
	//mprint("[0x%08X] %s[%d]", m_SessionID, msgID.c_str(), status);
	//mprint("[0x%08X]   -> %s", m_SessionID, nextFuncName.c_str());
	
	// 创建rule，并加入rule管理器中
	// 根据字串型的msgID找到其值 TODO:   需要sID到iID的映射
	int iID = m_pOwner->GetIIDBySID(msgID);
	if (iID == INVALID_MESSID)
	{
		mprint("[0x%08X][Error]: SetNextState failed, unknown msgID=%s\n", m_SessionID, msgID.c_str());
		return 1;
	}
	
	Rule *rule = new Rule(iID, msgID.c_str(), (int)status, nextFuncName);
	if (0 != m_ruleManager.AddRule(rule))	// 成功
	{
		mprint("[0x%08X][Error]: SetNextState failed", m_SessionID);
		return 1;
	}

	return 0;
}

/* 
功能: 输出日志
参数:
s: 日志内容
返回:
*/
int TLuaSession::_LuaPrintLog(string s, int, string)
{
	//mprint("[0x%08X]%s", m_SessionID, s.c_str());	
	mprint("%s", s.c_str());	
	return 0;
}

/* 
功能: 设置utm的整型值
参数:
	key ---- 
	value --
返回:
*/
int TLuaSession::_LuaPutInt(string key, int value, string)
{
	mprint("[0x%08X] PutInt(%s, %d)", m_SessionID, key.c_str(), value);

	int iID = m_pOwner->GetIIDBySID(key);
	if (iID == INVALID_MESSID)
	{
		mprint("[0x%08X][Error] Unknown key: %s", m_SessionID, key.c_str());
		return 1;
	}

	m_postUtm[iID] = (u32)value;
	return 0;
}

/* 
功能: 获取utm包中key对应的字串值
参数:
	key ---- 键值
返回:
	获取的值放到全局变量 _result 中，值的类型可能为 string 或 strarray，strarray转化为以特殊字符分隔的字串，如"s1&|&|s2&|&|s3"
*/
int TLuaSession::_LuaGetString(string key, string, string)
{  
	mprint("[0x%08X] GetString(%s)", m_SessionID, key.c_str());

	if (m_lastValidUtm.getMessageId() == INVALID_MESSID)
	{
		mprint("[0x%8x][Error] No valid utm", m_SessionID);
		return 1;
	}

	// 找到key的整型值
	int iID = m_pOwner->GetIIDBySID(key);
	if (iID == INVALID_MESSID)
	{
		mprint("[0x%08X][Error] Unknown key: %s", m_SessionID, key.c_str());
		return 1;
	}
	
	string value = "";

	value = m_lastValidUtm[iID];
	
	// 结果赋值给L内全局变量
	lua_pushstring(m_L, value.c_str());
	lua_setglobal(m_L, LC_RESULT);

	return 0;
}

/* 
功能: 获取utm包中key对应的整型值
参数:
	key ---- 键值
返回:
	获取的值放到全局变量 _result 中
*/
int TLuaSession::_LuaGetInt(string key, string, string)
{  
	mprint("[0x%08X] GetInt(%s)", m_SessionID, key.c_str());

	if (m_lastValidUtm.getMessageId() == INVALID_MESSID)
	{
		mprint("[0x%8x][Error] No valid utm", m_SessionID);
		return 1;
	}

	// 找到key的整型值
	int iID = m_pOwner->GetIIDBySID(key);
	if (iID == INVALID_MESSID)
	{
		mprint("[0x%08X][Error] Unknown key: %s", m_SessionID, key.c_str());
		return 1;
	}
	
	u32 value = 0;

	value = m_lastValidUtm[iID];
	
	// 结果赋值给L内全局变量
	lua_pushinteger(m_L, value);
	lua_setglobal(m_L, LC_RESULT);

	return 0;
}

/* 
功能: 获取utm包中key对应的double值 (TODO: 取不出number，先不管)
参数:
	key ---- 键值
返回:
	获取的值放到全局变量 _result 中
*/
int TLuaSession::_LuaGetNumber(string key, string, string)
{  
	mprint("[0x%08X] GetNumber(%s)", m_SessionID, key.c_str());

	if (m_lastValidUtm.getMessageId() == INVALID_MESSID)
	{
		mprint("[0x%08x][Error] No valid utm", m_SessionID);
		return 1;
	}

	// 找到key的整型值
	int iID = m_pOwner->GetIIDBySID(key);
	if (iID == INVALID_MESSID)
	{
		mprint("[0x%08X][Error] Unknown key: %s", m_SessionID, key.c_str());
		return 1;
	}
	
	float value = 0;

	value = m_lastValidUtm[iID];
	
	// 结果赋值给L内全局变量
	lua_pushnumber(m_L, value);
	lua_setglobal(m_L, LC_RESULT);

	return 0;
}

/* 
功能: 创建用于发送的utm包
参数:
返回:
*/
int TLuaSession::_LuaCreateUtm(string, string, string)
{
	mprint("[0x%08X] CreateUtm()", m_SessionID);

	ClearPostUtm();

	return 0;
}

/* 
功能: 设置utm包的消息ID
参数:
	msgID ----
返回:
*/
int TLuaSession::_LuaSetMsgID(string msgID, string, string)
{
	mprint("[0x%08X] SetMsgID(%s)", m_SessionID, msgID.c_str());

	// 获取整型消息ID
	int iID = m_pOwner->GetIIDBySID(msgID);
	if (iID == INVALID_MESSID)
	{
		mprint("[0x%08X][Error]: SetMsgID failed, unknown msgID=%s\n", m_SessionID, msgID.c_str());
		return 1;
	}

	m_postUtm.setMessageId(iID);

	return 0;
}

/* 
功能: 设置utm包的字串值
参数:
	key ----
	value --
返回:
*/
int TLuaSession::_LuaPutString(string key, string value, string)
{
	mprint("[0x%08X] PutString(%s, \"%s\")", m_SessionID, key.c_str(), value.c_str());

	// 获取整型消息ID
	int iID = m_pOwner->GetIIDBySID(key);
	if (iID == INVALID_MESSID)
	{
		mprint("[0x%08X][Error] Unknown key: %s", m_SessionID, key.c_str());
		return 1;
	}

	m_postUtm[iID] = value;

	return 0;
}

/* 
功能: 设置utm包的字串数组值
参数:
	key ----
	value -- 字串组
返回:
*/
int TLuaSession::_LuaPutStringArray(string key, string value, string)
{
	mprint("[0x%08X] PutStringArray(%s, \"%s\")", m_SessionID, key.c_str(), value.c_str());

	// 获取整型消息ID
	int iID = m_pOwner->GetIIDBySID(key);
	if (iID == INVALID_MESSID)
	{
		mprint("[0x%08X][Error] Unknown key: %s", m_SessionID, key.c_str());
		return 1;
	}

	// 解析出每个字串	
	strarray values = split(value, ";");
	m_postUtm[iID] = values;

	return 0;
}

/************************************************************************/
/* 
功能: 读取配置文件
参数:
返回:
0为成功，1为失败
*/
/************************************************************************/
int TLuaSession::_LuaReadConfig(string section,string key,string filename)
{
	char value[1024] = "";
	GetPrivateProfileString(section.c_str(), key.c_str(), "", value, 1024, filename.c_str());
	lua_pushstring(m_L, value);
	lua_setglobal(m_L, LC_RESULT);
	return 0;
}

/* 
功能: 同步C中相关数据到lua堆栈
说明: 目前同步数据包括: _status
参数:
traceFlag: 是否输出日志标志  0:不输出 其它:输出
返回:
*/
int TLuaSession::SyncFieldC2Lua(int traceFlag)
{	
	// 	设置lua栈中全局变量LC_MSG_STATUS的值
	lua_pushinteger(m_L, m_lastValidUtm.getReturn());
	lua_setglobal(m_L, LC_MSG_STATUS);

	return 0;
}

/* 
功能: 同步lua中field到C
说明: 目前同步数据包括: _FieldNum _Field
参数:
返回:
*/
int TLuaSession::SyncFieldLua2C()
{
	return 0;
}

/* 
功能: 清除跳转规则
说明: 在每次c调用lua函数之前必须调用该方法
参数:
返回:
*/
int TLuaSession::ClearRule()
{
	m_ruleManager.ClearRule();
	return 0;
}

/* 
功能: 设置 package.path
参数:
返回:
*/
int TLuaSession::SetLuaPath(lua_State* L, const char* path)
{
	lua_getglobal( L, "package" );
	lua_getfield( L, -1, "path" ); 								// get field "path" from table at top of stack (-1)
	std::string cur_path = lua_tostring( L, -1 );					// grab path string from top of stack
	cur_path.append( ";" ); 										// do your path magic here
	cur_path.append( path );
	cur_path.append( "?.lua;" );
	cur_path.append( path );
	cur_path.append( "?/init.lua;" );
	lua_pop( L, 1 ); 												// get rid of the string on the stack we just pushed on line 5
	lua_pushstring( L, cur_path.c_str() ); 						// push the new one
	lua_setfield( L, -2, "path" ); 								// set the field "path" in table at -2 with value at top of stack
	lua_pop( L, 1 ); 												// get rid of package table from top of stack
	return 0;
}

/************************************************************************/
/* 
功能：设置定时器
参数：val ---- 时间间隔，单位秒
返回：
*/
/************************************************************************/
void TLuaSession::StartTimer(int tval)
{
	UserTransferMessagePtr utm(new UserTransferMessage);
	utm->setMessageId(_EvtTimeOut);
	(*utm)[_TagSessionID] = (u32)m_SessionID;
	
	m_timerID = m_pOwner->setTimer(tval * 1000, utm);
}

/************************************************************************/
/* 
功能：关闭定时器
参数
返回：
*/
/************************************************************************/
void TLuaSession::StopTimer()
{
	if (m_timerID > 0)
	{
		m_pOwner->killTimer(m_timerID);
		m_timerID = 0;
	}
}

/************************************************************************/
/* 
功能：清空最近的有效utm包
参数：
返回：
*/
/************************************************************************/
void TLuaSession::ClearLastValidUtm()
{
	// 因为utm没有清空操作，替换方法为将 utm的消息ID置为 IVALID
	UserTransferMessage tUtm;
	m_lastValidUtm = tUtm;	// 用该法能将Utm中数据清空
	m_lastValidUtm.setMessageId(INVALID_MESSID);
}

/************************************************************************/
/* 
功能：清空post utm包
参数：
返回：
*/
/************************************************************************/
void TLuaSession::ClearPostUtm()
{
	// 因为utm没有清空操作，替换方法为将 utm的消息ID置为 IVALID
	UserTransferMessage tUtm;
	m_postUtm = tUtm;	// 用该法能将Utm中数据清空
	m_postUtm.setMessageId(INVALID_MESSID);
 	ServiceIdentifier req = m_pOwner->myIdentifier(m_SessionID);
// 	req.m_appref = m_SessionID;
 	m_postUtm.setReq(req);		// 将session id放在req的appref字段中
 	//m_postUtm.setRes(req);

	m_selfSpecResAppref = -1;
}

/************************************************************************/
/* 
功能：字符串分割函数
参数：
	str------源串
	pattern--分隔字串
返回：
	以指定分隔符分隔后的字串数组
*/
/************************************************************************/
std::vector<std::string> TLuaSession::split(std::string str,std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str+=pattern;//扩展字符串以方便操作
	int size=str.size();

	for(int i=0; i<size; i++)
	{
		pos=str.find(pattern,i);
		if(pos<size)
		{
			std::string s=str.substr(i,pos-i);
			result.push_back(s);
			i=pos+pattern.size()-1;
		}
	}
	return result;
}

/************************************************************************/
/* 
名字：CallLuaFunction
功能：执行lua文件中指定函数
参数：
	lp ---- 扩展参数
	func -- 函数名
	vfmt -- 输入输出参数格式 "[ids]*[>[ids]*]"
	... --- 实参
返回：
	0 ----- 成功
	其它 -- 失败
*/
/************************************************************************/
int TLuaSession::CallLuaFunction(void *lp, const char* func, char *vfmt, ...)
{	
	int retcode = 0;

	if (!m_L || !func || 0==strlen(func))
	{
		return 1;
	}
	
	int narg;
	int nret;
	int nindex;

	lua_getglobal(m_L, func);
	if(0 == lua_isfunction(m_L, -1))
	{
		lua_pop(m_L, 1);
		// lua函数不存在
		mprint("[Error](%s) is not a lua function\n", func);
		return 1;
	}
	
	char *fmt = vfmt;
	va_list ap;
	va_start(ap, vfmt);
	for(narg=0; *fmt; ++narg, ++fmt){
		luaL_checkstack(m_L, 1, "too many arguments");
		
		switch(*fmt)
		{
		case 'd':
			lua_pushnumber(m_L, va_arg(ap, double));
			break;
		case 'i':
			lua_pushinteger(m_L, va_arg(ap, int));
			break;
		case 's':
			lua_pushstring(m_L, va_arg(ap, char*));
			break;
		case '>':
			++fmt;
			goto end_in;
		default:
			break;
		}
	}
end_in:
	
	nret = strlen(fmt);	// 返回参数个数
	if(lua_pcall(m_L, narg, nret, 0) != 0)
	{
		// 调用lua函数失败
		mprint("[Error]Call lua function (%s) failed, msg=%s\n", func, lua_tostring(m_L, -1));
		return 1;
	}
	
	nindex = -nret;
	
	// 取得lua函数返回值
	while(*fmt)
	{
		switch(*fmt)
		{
		case 'd':
			if (!lua_isnumber(m_L, nindex))
			{
				mprint("[ERROR] Wrong result type\n");
				retcode = 1;
				goto lua_end;
			}
			*va_arg(ap, double*) = lua_tonumber(m_L, nindex);
			break;
		case 'i':
			if (!lua_isnumber(m_L, nindex))
			{
				mprint("[ERROR] Wrong result type\n");
				retcode = 1;
				goto lua_end;
			}
			*va_arg(ap, int*) = lua_tointeger(m_L, nindex);			
			break;
		case 's':
			if (!lua_isstring(m_L, nindex))
			{
				mprint("[ERROR] Wrong result type\n");
				retcode = 1;
				goto lua_end;
			}
			strcpy((char*)va_arg(ap, const char**), lua_tostring(m_L, nindex));
			break;
		default:
			mprint("[ERROR] Invalid option(%c)\n", *fmt);
			retcode = 1;
			goto lua_end;
		}
		++fmt;
		++nindex;
	}

lua_end:
	lua_pop(m_L, nret);
	
	va_end(ap);
	
	return retcode;
}