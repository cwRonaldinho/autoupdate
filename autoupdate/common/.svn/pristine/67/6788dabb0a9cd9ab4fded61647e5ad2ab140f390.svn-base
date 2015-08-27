#ifndef INCLUDED_LOGGING_PORTABILITYIMPL_H
#define INCLUDED_LOGGING_PORTABILITYIMPL_H

#ifndef INCLUDED_LOGGING_PORTABILITY_H
#endif

#ifdef CACTI_CSTDLIB_NOT_IN_STD
#include <cstdlib>
namespace std 
{
    static inline char *getenv(const char *name) { return ::getenv(name); };
    static inline int atoi(const char *nptr) { return ::atoi(nptr); };
    static inline unsigned long int
        strtoul(const char *nptr, char **endptr, int base) { 
        return ::strtol(nptr, endptr, base); 
    };
}
#endif
    
#ifdef CACTI_CSTRING_NOT_IN_STD
#include <cstring>
namespace std 
{
    static inline void *memmove(void *dest, const void *src, size_t n) 
	{
        return ::memmove(dest, src, n);
    };
}
#endif

#ifdef CACTI_CTIME_NOT_IN_STD
#include <ctime>
namespace std {
    static inline size_t strftime(char *strDest, size_t maxsize, const char *format, const struct tm *timeptr ) {
        return ::strftime(strDest,maxsize,format,timeptr);
    }
    static inline struct tm *localtime( const time_t *timer ) { return ::localtime(timer); }
}
#endif

#ifdef CACTI_CMATH_NOT_IN_STD
#include <cmath>
namespace std {
    static inline int abs(int i) { return ::abs(i); }
}
#endif

#ifdef CACTI_BOOST_SWAP_NOT_IN_STD
#include "boost/shared_ptr.hpp"
namespace std {
	template<class T> inline void swap(boost::shared_ptr<T> & a, boost::shared_ptr<T> & b)
	{
		a.swap(b);
	}
}
#endif
#ifndef CACTI_HAVE_SSTREAM
#include <strstream>
namespace std 
{
	class ostringstream : public ostrstream 
	{
	public:
		std::string str();
	};
}
#else
#include <sstream>
#endif


#endif // INCLUDED_LOGGING_PORTABILITYIMPL_H
