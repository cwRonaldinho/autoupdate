#include "stdafx.h"

#include <cacti/util/Exception.h>

#define new DEBUG_NEW

namespace cacti 
{
	//////////////////////////////////////////////////////////////////////////
	
	Exception::Exception()
		: m_file(0)
		, m_line(0)
	{
	}

	Exception::Exception(const char* file, int line)
		: m_file(file)
		, m_line(line)
	{
	}

	Exception::~Exception() throw()
	{
	}

	const char* Exception::file() const 
	{
		return m_file;
	}
	int Exception::line() const
	{
		return m_line;
	}

	void Exception::print(std::ostream& os)
	{
		if(m_file && m_line>0)
			os << m_file << "(" << m_line << ") : " << what();
		else
			os << what();
	}
	//////////////////////////////////////////////////////////////////////////
	
	NullHandleException::NullHandleException(const char* file, int line)
		: Exception(file, line)
	{
	}

	NullHandleException::~NullHandleException() throw()
	{
	}

	const char* NullHandleException::what() const throw()
	{
		return "NullHandleException";
	}
} // namespace cacti
