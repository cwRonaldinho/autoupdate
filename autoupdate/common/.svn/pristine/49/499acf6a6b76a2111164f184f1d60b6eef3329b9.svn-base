/*
 * DbConnection Object defining the wrapper class for all the database drivers.
 * Copyright (C) 2003 Johnathan Ingram, jingram@rogueware.org
 *
 * This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US
 *
 */

#ifndef __DBCONNECTION_H__
#define __DBCONNECTION_H__

#ifdef WIN32
   // Windows
   // warning C4503: '  ' : decorated name length exceeded, name was truncated
   #pragma warning( disable:4503 )  

   // DLL Exports
#	ifndef	STATIC_DBC
   	struct __declspec( dllexport ) DbConnectionDriverInfo;
   	class  __declspec( dllexport ) DbConnection;
#	endif
#endif   


#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#include <dbconnect/base/smartPointer.h>
#include <dbconnect/base/dlLoader.h>

#include <dbconnect/base/baseConnection.h>
#include <dbconnect/base/baseQuery.h>
#include <dbconnect/base/baseException.h>
#include <dbconnect/base/dbconnectTypes.h>

#if	defined( STATIC_DBC )
//	-----------------------------------------------------------------------
//	
//	declaration of interface functions.	
//
extern	"C"	{
extern	void* 		createDriverInstance(int , const char**);
extern	void* 		destroyDriverInstance(void*);
extern	const char* 	getAuthor();
extern	const char* 	getVendor();
extern	const char* 	getCopyright();
extern	const char* 	getDriverType();
extern	const char* 	getDriverName();
extern	const char* 	getDriverDesc();
extern	const char* 	getDbConnectVersion();
};
//	-----------------------------------------------------------------------
#endif


using namespace std;

class DbConnection;

/* Define the smart pointer types
 */
typedef SmartPointer <DbConnection> DbConnectionVar;

typedef BaseQuery* DbQuery;
typedef SmartPointer <BaseQuery> DbQueryVar;

typedef BaseFieldDescription* DbFieldInfo;


/* Define the DbConnectionDriverInfo struct
 * Holds information returned by the currently loaded driver.
 */
struct DbConnectionDriverInfo
{
   string  author;
   string  vendor;
   string  copyright;
   string  driverType;
   string  driverName;
   string  driverDescription;
   string  dbConnectVersion;
};


/* Define the DbConnection class
 *
 */
class DbConnection
{
private: 
   DlLoader  dlLoader;                    

   // Library Function pointers
   void* (*ptr_createDriverInstance)(int , const char**);
   void* (*ptr_destroyDriverInstance)(void*);
   const char* (*ptr_getAuthor)();
   const char* (*ptr_getVendor)();
   const char* (*ptr_getCopyright)();
   const char* (*ptr_getDriverType)();
   const char* (*ptr_getDriverName)();
   const char* (*ptr_getDriverDesc)();
   const char* (*ptr_getDbConnectVersion)();

   // Store the compatibility versions for libraries.
   map<string, vector<string> >  compatibilityTable;

   // Store the driver information.
   DbConnectionDriverInfo driverInfo;
            
   /* Define the driver library file names 
    * These entries in the array correspond to the value defined by enum Driver.
    */
   static const char* driverPaths[];

   /* Define the driver string names 
    * These entries in the array correspond to the value defined by enum Driver.
    */
   static const char* driverNames[];

   /* Pointer to the instance of a driver object */
   BaseConnection* drvRef;  
   

   /* Builds the internal compatibility table for version checking.
    *
    */
   void 
   _buildCompatibilityTable();


public:  
   /* Define the driver types 
    */
   enum Driver
   {
      NONE,
      MYSQL,
      MSQL,
      POSTGRESQL,
      DB2,
      ORACLE,
      ODBC,
	  SYBASE,
	  ORACLE10,
   };



   /* Constructor.
    *
    * @param driver           The database driver to use
    * @param configFile       The config file. 
    *                         If blank, will use default searching for dbconnect.[ini | cfg]
    */
   DbConnection(
         Driver driver, 
         const string &configFile="");


   /* Destructor.
    *
    */
   ~DbConnection();

   
   /* Create the connection pool and connect required connections to the database.
    *
    * @param username         Database username.
    * @param password         Password used for username.
    * @param databaseName     Name of the database connecting to.
    * @param host             Host the database resides on. Default "localhost"
    * @param maxConnections   Maximum number of connections the object can pool at any one time. Default 1
    * @param optParam1        Optional parameter that can be used by the driver.
    * @param optParam2        Optional parameter that can be used by the driver.
    */
   void 
   connect(
      const string &username, 
      const string &password="", 
      const string &databaseName="", 
      const string &host="localhost",
      int   maxConnections=1,
      int   minConnections=1,
      const string &optParam1="",
      const string &optParam2="");


   /* Destroy all connections to the database.
    *
    * @param timeout          Timeout in seconds to wait for connections to become idle. Default 120.
    */
   void 
   disconnect(
      time_t timeout=120);
   

   /* Set the time interval that a connection will be checked to make sure it is alive.
    *
    * @param pingInterval     Interval in seconds. Minimum of 60 seconds
    */
   void 
   setPingInterval(
      time_t pingInterval);
   

   /* Obtain a connection that is attached to a query object.
    *
    * @return            Returns a void pointer to a BaseQuery object. (The pointer needs to be type casted.
    */
   BaseQuery*
   requestQueryConnection();

   /* release a connection that is attached to a query object.
   *
   * @param queryConnection   Pointer of the query connection to release.
   *
   */
   void 
   releaseQueryConnection(
	   void* queryObject);


   /* Checks if a library version is compatible with the current version of dbConnect.
    *
    * @return        Returns true if version compatible or false if not.
    *
    * @param ver     String representing the version of the library.
    */
   bool 
   checkCompatibility(
      const string &ver);


   /* Returns a pointer to the information about the current driver that is loaded.
    *
    * @return        Returns a pointer to the driver information struct.
    *
    */
   DbConnectionDriverInfo* 
   getDriverInformation();
};


#endif
