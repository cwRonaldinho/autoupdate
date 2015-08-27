//提供动态连接库的基本操作，可以屏蔽WIN32和UNIX的不一致。

#ifndef INCLUDED_CACTI_KERNEL_DLLBASE_H
#define INCLUDED_CACTI_KERNEL_DLLBASE_H

#pragma once

#include <cacti/kernel/KernelException.h>

#ifdef WIN32
#include <Windows.h>
#define RTLD_LAZY 0
#else
#include <dlfcn.h>
#endif

#include <string>

namespace cacti
{
	class DllBase
	{
	public:
		DllBase(const char* dll, int flag = RTLD_LAZY);
		virtual ~DllBase();
		void* getsym(const char* symbol);
		void geterror(std::string &strError);
	private:
#ifdef WIN32
		HMODULE m_handle;
#else
		void* m_handle;
#endif
	};
};
#endif // INCLUDED_DLLBASE_H
