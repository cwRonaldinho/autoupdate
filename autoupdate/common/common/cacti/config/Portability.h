#ifndef INCLUDED_LOGGING_PORTABILITY_H
#define INCLUDED_LOGGING_PORTABILITY_H

#if defined (_MSC_VER) || defined(__BORLANDC__)
	#include <cacti/config/detail/config-win32.h>
	#include <windows.h>
#else
#    include <cacti/config/detail/config.h>
#endif

#include <cacti/config/detail/PortabilityImpl.h>

#endif //INCLUDED_LOGGING_PORTABILITY_H
