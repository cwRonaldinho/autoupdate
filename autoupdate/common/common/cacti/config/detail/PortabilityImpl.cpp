#include "stdafx.h"

#include <cacti/config/Portability.h>

#include "PortabilityImpl.h"

#ifndef CACTI_HAVE_SSTREAM
namespace std {
    std::string ostringstream::str() { 
        (*this) << '\0'; 
        std::string msg(ostrstream::str()); 
        ostrstream::freeze(false);			//unfreeze stream 
        return msg;         
    } 
}
#endif

#ifndef CACTI_HAVE_SNPRINTF
	extern "C" 
	{
		#include "snprintf.c"
	}

#endif
