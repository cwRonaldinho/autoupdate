#ifndef INCLUDED_CACTI_CONFIG_WIN32_H
#define INCLUDED_CACTI_CONFIG_WIN32_H
 
/* Define if you have the syslog function.  */
/* #undef CACTI_HAVE_SYSLOG */

/* Define if you have the `ftime' function. */
#ifndef CACTI_HAVE_FTIME 
#define CACTI_HAVE_FTIME  1 
#endif

/* Define if you have the <io.h> header file.  */
#ifndef CACTI_HAVE_IO_H
#define CACTI_HAVE_IO_H 1
#endif

/* Define if you have the <unistd.h> header file.  */
/* #undef CACTI_HAVE_UNISTD_H */

/* Define if you have the idsa library (-lidsa).  */
/* #undef CACTI_HAVE_LIBIDSA */

/* Define if you have the `strcasecmp' function. */
/* #undef CACTI_HAVE_STRCASECMP */

/* Name of package */
#ifndef CACTI_PACKAGE
#define CACTI_PACKAGE  "cacti"
#endif

/* Version number of package */
#ifndef CACTI_VERSION
#define CACTI_VERSION  "0.1.1"
#endif

/* define if the compiler implements namespaces */
#ifndef CACTI_HAVE_NAMESPACES
#define CACTI_HAVE_NAMESPACES 1
#endif

/* define if the compiler has stringstream */
#ifndef CACTI_HAVE_SSTREAM
#define CACTI_HAVE_SSTREAM 1
#endif

/* define if the C library has snprintf */
#ifndef CACTI_HAVE_SNPRINTF
#define CACTI_HAVE_SNPRINTF 1
	#if defined(_MSC_VER)
		#define vsnprintf _vsnprintf
		#define snprintf  _snprintf
	#endif
#endif

/* define to get around problems with ERROR in windows.h */
#ifndef CACTI_FIX_ERROR_COLLISION
#define CACTI_FIX_ERROR_COLLISION 1
#endif


/* boost::share_ptr's swap method not in std namesapce */
#ifndef CACTI_BOOST_SWAP_NOT_IN_STD
#define CACTI_BOOST_SWAP_NOT_IN_STD
#endif

/* define WIN32 for Borland */
#ifndef WIN32
#define WIN32
#endif

/* use threads */
#ifndef CACTI_HAVE_THREADING
#define CACTI_HAVE_THREADING
#endif

/* use ms threads */
#ifndef CACTI_USE_MSTHREADS
#define CACTI_USE_MSTHREADS
#endif

/* supply DLL main */
#ifndef CACTI_SUPPLY_DLLMAIN
#define CACTI_SUPPLY_DLLMAIN
#endif

/* MSVCs <cstdlib> and <cstring> headers are broken in the sense that they
   put functions in the global namespace instead of std::
   The #defines below enable a workaround for MSVC 6 and lower. If MSVC 7
   is still broken please adjust the _MSC_VER version check and report it.
   See also bug report #628211.
*/
#if defined(_MSC_VER) && _MSC_VER < 1300

#ifndef CACTI_CSTDLIB_NOT_IN_STD
#define CACTI_CSTDLIB_NOT_IN_STD
#endif

#ifndef CACTI_CSTRING_NOT_IN_STD
#define CACTI_CSTRING_NOT_IN_STD
#endif

#ifndef CACTI_CTIME_NOT_IN_STD
#define CACTI_CTIME_NOT_IN_STD
#endif

#ifndef CACTI_CMATH_NOT_IN_STD
#define CACTI_CMATH_NOT_IN_STD
#endif

#endif

#endif // INCLUDED_CACTI_CONFIG_WIN32_H
