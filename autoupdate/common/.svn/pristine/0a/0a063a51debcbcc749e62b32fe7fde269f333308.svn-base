#ifndef INCLUDED_STD_INC
#define INCLUDED_STD_INC

//c-runtime
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ctime>
#include <cstring>
#include <cassert>
#include <sys/stat.h>
#include <sys/types.h>

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

// process hash_map
#if defined(_MSC_VER) 
#	include <direct.h>
	#if defined(_STLP_STRING) 
		// we believe if _STLP_STRING included it mean using STLPORT
        #include <hash_map>
	#elif (_MSC_VER >= 1300)
	    // hasp_map not standard container
	    #pragma warning(disable: 4996)
	    #include <hash_map>    
    #else
    	// no hash_map, use map as hash_map
        #define hash_map map
    #endif
#endif

#include <memory>
#include <iterator>
#include <algorithm>
#include <functional>

#include "tag.h"
#include "reqmsg.h"
#include "resmsg.h"
#include "evtmsg.h"
#include "sxcode.h"
#include "sxconst.h"

#endif // 
