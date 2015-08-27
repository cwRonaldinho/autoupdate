#ifndef INCLUDED_CACTI_KERNEL_EXCEPTION_H
#define INCLUDED_CACTI_KERNEL_EXCEPTION_H

#include <string>
#include <cacti/util/Exception.h>

namespace cacti 
{
	//////////////////////////////////////////////////////////////////////////

	class SyscallException : public Exception
	{
	public:
		SyscallException(const char* file, int line);
		~SyscallException() throw();

		const char* what() const throw();

	private:
		int m_errCode;
		mutable std::string m_errMessage;
	};

} // namespace cacti

#endif // INCLUDED_CACTI_KERNEL_EXCEPTION_H
