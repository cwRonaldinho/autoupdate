#include "stdafx.h"

#include <cacti/kernel/DllBase.h>

#include "cacti/kernel/SysError.h"

#include <errno.h>

#define new DEBUG_NEW

namespace cacti
{
#ifdef WIN32
	DllBase::DllBase(const char *dll, int flag)
	{
		m_handle = ::LoadLibraryA(dll);
		if(m_handle == 0)
			throw SyscallException(__FILE__, __LINE__);
	}
	DllBase::~DllBase()
	{
		::FreeLibrary(m_handle);
	}
	void* DllBase::getsym(const char* symbol)
	{
		return ::GetProcAddress(m_handle, symbol);
	}
	void DllBase::geterror(std::string &strError)
	{
		SysError::lastErrorMessage(strError);
	}
#else
	DllBase::DllBase(const char* dll, int flag)
	{
		m_handle = ::dlopen(dll, flag);
		if(m_handle == 0)
			throw SyscallException(__FILE__, __LINE__);
	}
	DllBase::~DllBase()
	{
		dlclose(m_handle);
	}
	void* DllBase::getsym(const char* symbol)
	{
		return ::dlsym(m_handle, symbol);
	}
	void DllBase::geterror(std::string &strError)
	{
		strError = ::dlerror();
	}
#endif
}
