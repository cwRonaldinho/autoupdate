#include "stdafx.h"

#include <cstring>
#include <string>

#ifdef WIN32
#   include "Windows.h"
# else
#   include <errno.h> // for POSIX error codes
#   include <stdio.h>
# endif

#include <cacti/kernel/KernelException.h>
#include "cacti/kernel/SysError.h"

#define new DEBUG_NEW

namespace cacti 
{
	//////////////////////////////////////////////////////////////////////////
	SyscallException::SyscallException(const char* file, int line)
		: Exception(file, line)
		, m_errCode(SysError::lastError())
	{
	}

	SyscallException::~SyscallException() throw()
	{
	}

	const char* SyscallException::what() const throw()
	{
		if(m_errMessage.empty())
			SysError::lastErrorMessage(m_errCode, m_errMessage);
		return m_errMessage.c_str();
	}
}
