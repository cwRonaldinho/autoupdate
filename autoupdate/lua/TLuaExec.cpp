#include "TLuaExec.h"
#include "time.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

extern void mprint(const char* fmt, ...);

extern "C"
{ 
#include "lua.h"  
#include "lualib.h"
#include "lauxlib.h" 
}

struct lua_State* TLuaExec::m_L = luaL_newstate();

/************************************************************************/
/* 
名字：ExecLua
功能：加载并执行lua文件
参数：
	luafile ---- lua文件名 .lua或.out格式均可
*/
/************************************************************************/
int TLuaExec::LoadAndExecLua(const char* luafile)
{
	// 加载lua文件
	int ret = luaL_loadfile(m_L, luafile);
	if (ret) 
	{                                  
		mprint("[ERROR] Load script failed, bad file %s, errno=%d, msg=%s", luafile, ret, lua_tostring(m_L, -1));
		return 1;
	}

	//函数加载Lua库
	luaL_openlibs(m_L);

	ret = lua_pcall(m_L, 0, -1, 0);
	if (ret)
	{
		mprint("Execute script failed, errno=%d, msg=%s", ret, lua_tostring(m_L, -1));
		return 2;
	}

	return 0;
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
int TLuaExec::CallLuaFunction(void *lp, const char* func, char *vfmt, ...)
{	
	//
	//printf("sizeof(lua_Integer) = %d\n", sizeof(lua_Integer));

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
		mprint("[Error](%s) is not a lua function", func);
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
		mprint("[Error]Call lua function (%s) failed, msg=%s", func, lua_tostring(m_L, -1));
		return 1;
	}
	
	nindex = -nret;
	
	// 取得lua函数返回值
	while(*fmt)
	{
		//int t;
		switch(*fmt)
		{
		case 'd':
			if (!lua_isnumber(m_L, nindex))
			{
				mprint("[ERROR] Wrong result type");
				retcode = 1;
				goto lua_end;
			}
			*va_arg(ap, double*) = lua_tonumber(m_L, nindex);
			break;
		case 'i':
			if (!lua_isnumber(m_L, nindex))
			{
				mprint("[ERROR] Wrong result type");
				retcode = 1;
				goto lua_end;
			}
			//*va_arg(ap, int*) = lua_tonumber(m_L, nindex);			
			*va_arg(ap, int*) = (long long)lua_tonumber(m_L, nindex);	// 用long long转换解决lua传入数据大于0x80000000的问题			
			//t = lua_tonumber(m_L, nindex);
			//t = (long long)lua_tonumber(m_L, nindex);
			break;
		case 's':
			if (!lua_isstring(m_L, nindex))
			{
				mprint("[ERROR] Wrong result type");
				retcode = 1;
				goto lua_end;
			}
			strcpy((char*)va_arg(ap, const char**), lua_tostring(m_L, nindex));
			break;
		default:
			mprint("[ERROR] Invalid option(%c)", *fmt);
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