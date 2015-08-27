#ifndef INCLUDED_CACTI_UTIL_EXCEPTION_H
#define INCLUDED_CACTI_UTIL_EXCEPTION_H

#if defined(HAS_PRAGMA_ONCE)
#pragma PRAGMA_ONCE_DECLARE
#endif

#include <stdexcept>
#include <iostream>

namespace cacti 
{
	//////////////////////////////////////////////////////////////////////////
	
	class Exception : public std::exception
	{
	public:
		Exception();
		Exception(const char* file, int line);
		~Exception() throw();
		
		const char*file() const;
		int line() const;

		void print(std::ostream& os);
        
	private:
		const char* m_file;	// just the reference to __FILE__
		int m_line;			// just the value of __LINE__
	};

	//////////////////////////////////////////////////////////////////////////
	
	class NullHandleException : public Exception
	{
	public:
		NullHandleException(const char* file, int line);
		~NullHandleException() throw();

	public:
		const char* what() const throw();
	};
} // namespace cacti

#endif // INCLUDED_EXCEPTION_H
