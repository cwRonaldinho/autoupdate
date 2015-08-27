#ifndef INCLUDED_CACTI_KERNEL_SYSERROR_H
#define INCLUDED_CACTI_KERNEL_SYSERROR_H

#pragma once

#include <string>

namespace cacti
{
	struct SysError
	{
		static int lastError();
		static const char* lastErrorMessage(std::string& message);
		static const char* lastErrorMessage(int error, std::string& message);
	};
}
#endif // #ifndef INCLUDED_SYSOBJ_H
