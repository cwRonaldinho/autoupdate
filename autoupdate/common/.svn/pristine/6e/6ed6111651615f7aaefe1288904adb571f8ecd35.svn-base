#ifdef _DB_SYBASE_
#include "stdafx.h"


#include <string.h>
#include <string>

#include "dbconnect/driver_sybase/sybaseConnection.h"
#include <iostream>
#include <ctype.h>

using namespace std;

#define DBCONNECTVER "0.3.5"

#ifdef __linux__
	__const unsigned short int *__ctype_b;
	__const __int32_t *__ctype_tolower;
	__const __int32_t *__ctype_toupper;

	void ctSetup()
	{
		__ctype_b = *(__ctype_b_loc());
		__ctype_toupper = *(__ctype_toupper_loc());
		__ctype_tolower = *(__ctype_tolower_loc());
	}
#else	
	void ctSetup(){};
#endif

//Stop the C++ name mangling
extern "C"
{

#ifdef WIN32
   // Windows
   // DLL Exports
   __declspec( dllexport ) void* createDriverInstance(int argc, const char** argv);
   __declspec( dllexport ) void* destroyDriverInstance(void* driver);
   __declspec( dllexport ) const char* getAuthor();
   __declspec( dllexport ) const char* getVendor();
   __declspec( dllexport ) const char* getDriverName();
   __declspec( dllexport ) const char* getDriverDesc();
   __declspec( dllexport ) const char* getDbConnectVersion();
   __declspec( dllexport ) const char* getCopyright();
   __declspec( dllexport ) const char* getDriverType();
#endif


namespace 
{
	static int refcount = 0;
}

int syb_msg_handler (DBPROCESS * dbproc, DBINT msgno,
					 int msgstate, int severity, char *msgtext, char *srvname,
					 char *procname, int line);
int syb_err_handler (DBPROCESS * dbproc, int severity, int dberr, int oserr,
					 char *dberrstr, char *oserrstr);


/* Create an instance of the driver.
 
 * @param  argc     Number of parameters and values.
 * @param  argv     Parameters that can be used to instantiate the driver with.
 *
 * @return          Return the instance or NULL if an error occured.
 */
void*
createDriverInstance(int argc, const char** argv)
{   
	try
	{
		ctSetup();
		if (dbinit() == FAIL)
		{
			return NULL;
		}
		dberrhandle(( EHANDLEFUNC)syb_err_handler);
		dbmsghandle((MHANDLEFUNC) syb_msg_handler);

		refcount++;
		SybaseConnection *sybaseConnection = NULL;
		sybaseConnection = new SybaseConnection(argc, argv);
		return (void*)sybaseConnection;
	}
	catch(...)
	{
		return NULL;
	}
}


/* Destroy an instance of the driver.
 *
 * @return        Return NULL for success or the the pointer to the instance for failure.
 *
 * @param  driver The pointer to the driver instance.
 */
void*
destroyDriverInstance(
   void* driver)
{
   SybaseConnection *sybaseConnection = (SybaseConnection*)driver;
   
   if (sybaseConnection)
   {
      // Make sure we have the correct driver.     
      if (strcmp( sybaseConnection->driverName.c_str(), SYBASE_DRIVERNAME) != 0)
         return driver;

      // Free the driver.
      delete sybaseConnection;
	  refcount--;

	  if( !refcount)
			dbexit();
   }
   return NULL;
}

   
/* Get the author of the driver.
 *
 * @return          Return the author.
 */
const char*
getAuthor()
{
   return "cool, boentel";
}


/* Get the vendor information of the driver.
 *
 * @return          Return the vendor.
 */
const char*
getVendor()
{
   return "boentel, GNU";
}  


/* Get the copyright information of the driver.
 *
 * @return          Return the copyright information.
 */
const char*
getCopyright()
{
   return "Copyright (C) 2006 boentel";
}  


/* Get the driver type
 *
 * @return          Return the driver type.
 */
const char*
getDriverType()
{
   return "RDMS";
}  


/* Get the name of the driver.
 *
 * @return          Return the driver name.
 */
const char*
getDriverName()
{  
   return SYBASE_DRIVERNAME;
}


/* Get the description of the driver.
 *
 * @return          Return the driver description.
 */
const char*
getDriverDesc()
{  
   return "Sybase Database Driver";
}


/* Returns the version of DbConnect this driver was compiled with.
 *
 * @return          Return the DbConnect version.
 */
const char*
getDbConnectVersion()
{
   return DBCONNECTVER;
}


int syb_msg_handler(DBPROCESS * dbproc, DBINT msgno, int msgstate, int severity,
				char *msgtext, char *srvname, char *procname, int line)
{
	g_dbclog.print(" dbproc = 0x%X msg = %s srvname = %s\n",dbproc,msgtext ? msgtext : "null" , 
		srvname ? srvname : "null" );
	return 0;
}

int	syb_err_handler (DBPROCESS * dbproc, int severity, int dberr, int oserr,
					 char *dberrstr, char *oserrstr)
{
	if (dberr == SYBESMSG)
		return INT_CANCEL;

	g_dbclog.print(	"dbproc = 0x%X error msg (severity %d, dberr %d, oserr %d, dberrstr %s, oserrstr	%s):\n",
			dbproc,severity, dberr, oserr, dberrstr ? dberrstr : "(null)",
			oserrstr ? oserrstr : "(null)");
	return INT_CANCEL;
}

} // extern "C"
#endif