#include "stdafx.h"

#include "cacti/config/Portability.h"
#include "cacti/kernel/SysError.h"

#include <errno.h>

#define new DEBUG_NEW

namespace cacti
{
	const char* SysError::lastErrorMessage(std::string& message)
	{
		return lastErrorMessage(lastError(), message);
	}

#ifdef WIN32
	int SysError::lastError()
	{
		return (int)GetLastError();
	}
	const char* SysError::lastErrorMessage(int error, std::string& message)
	{
		LPVOID buf = NULL;
		BOOL ok = FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &buf,
			0,
			NULL 
			);
		if(!ok)
		{
			//try to load network error message library
			HMODULE dll = LoadLibraryEx(TEXT("netmsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);
			if(dll)
			{
				FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					dll,
					error,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &buf,
					0,
					NULL 
					);
				FreeLibrary(dll);
			}
		}
		if(buf)
		{
			//format :"error code : description"
			char temp[64];
			snprintf(temp, sizeof(temp)-1, "<syserr:%d>", error);
			message = temp;
			message += " : ";
			message += (const char*)buf;
			//remove the "\n"--0x0D0A
			message[message.size() - 2] = '\0';
			LocalFree(buf);
		}

		return message.c_str();
	}
#else
	int SysError::lastError()
	{
		return errno;
	}
	const char* SysError::lastErrorMessage(int error, std::string& message)
	{
		char temp[64];
		snprintf(temp, sizeof(temp)-1, "<syserr:%d>", error);
		message = temp;
		message += " : ";
		message += std::strerror(error);

		return message.c_str();
	}
#endif
}
