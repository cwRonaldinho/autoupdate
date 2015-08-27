#ifdef _DB_SYBASE_
#ifndef __SYBASE_CONNECTION_H__
#define __SYBASE_CONNECTION_H__

#define SYBASE_DRIVERNAME    "Sybase"

#include <stdio.h>
#ifdef WIN32
	#include <windows.h>
#endif

#include "dbconnect/driver_sybase/sybase_inc/sybfront.h"
#include "dbconnect/driver_sybase/sybase_inc/sybdb.h"
#include "dbconnect/driver_sybase/sybase_inc/syberror.h"

#include <map>
#include <string>
#include <time.h>

#include "dbconnect/base/dbconnectTypes.h"
#include "dbconnect/base/baseConnection.h"
#include "dbconnect/base/baseException.h"
#include "dbconnect/base/simpleThreads.h"



using namespace std;

class SybaseQuery; // Need to be friends with.

class SybaseHandle : public BaseHandle
{
public:
   //Sybase Connection Handles;
   DBPROCESS*	dbproc;   
   void     *queryObject;             // Query Object bound to the connection

   // Constructor
   SybaseHandle() 
   : 
   dbproc(NULL),   
   queryObject(NULL) 
   { };  
   ~SybaseHandle();
};

class SybaseConnection : public BaseConnection
{

private: 


   // Collection types that can be freed within this object
   enum CollectionType
   {
      CONNECTION_HANDLES
   };

   // Thread mutexes
   SimpleThread_Mutex classMutex;


   // Handles used by the driver
   DBULONG       _numHandles;
   SybaseHandle **_handles;

   /* Internal method to connect the handle to a MySQL database.
    *
    * @param handleIndex     Index to the connection handle that must be connected.
    */
   void 
   _sybaseConnect(
         int index,const string& charset,const string& application);

   
   /* Internal method to disconnect the handle from a MySQL database.
    *
    * @param handleIndex     Index to the connection handle that must be disconnected.
    */
   void 
   _sybaseDisconnect(
         int index);


   /* Internal method to make sure the connection handle is still alive and reconnect if timed out.
    *
    * @param handleIndex     Index to the connection handle that must be pinged.
    */
   void 
   _sybasePing(
         int index);

	/*
	 * Internal method to reconnect to SQL server
	 */
   void
	   _sybaseReconnect(int index);

   /* Internal method to query MySQL. All query objects will use this.
    *
    * @param handleIndex     Index to the connection handle to use for the query.
    * @param sqlStatement    Sql statment to query against the database.
    */
   void 
   _sybaseQuery(
         int           index,
         const string& sqlStatement);


   /* Internal method to free internally allocated memory
    *
    * @param type    The type representing which value to deallocate.
    *
    */
   void _freeCollection(
         CollectionType type);


public:

   /* Constructor.
    *
    */
   SybaseConnection(
         int argc, 
         const char** argv);


   /* Destructor.
    *
    */
   ~SybaseConnection();


   /* Create connections to the database.
    *
    * @param username         Database username.
    * @param password         Password used for username.
    * @param databaseName     Name of the database connecting to.
    * @param host             Host the database resides on. Default "localhost"
    * @param maxConnections   Maximum number of connections the object can pool at any one time. Default 1
    * @param optParam1        Optional parameter that can be used by the driver.
    * @param optParam2        Optional parameter that can be used by the driver.
    *
    */
   void 
   connect(
         const string &username, 
         const string &password="", 
         const string &databaseName="", 
         const string &host="localhost", 
         int          maxConnections=1,
         int          minConnections=1,
         const string &optParam1="iso_1",
         const string &optParam2="dbconnect");

   /* Destroy all connections to the database.
    *
    * @param timeout          Timeout in seconds to wait for connections to become idle. Default 120.
    */
   void 
   disconnect(
         time_t timeout=120);
     

   /* Obtain a connection that is attached to a query object.
    *
    * @param timeout     Interval in seconds that the function should block for before throwing an exception.
    *
    * @return            Returns a void pointer to a BaseQuery object. (The pointer needs to be type casted)
    */
   void*
   requestQueryConnection();
   

   /* release a connection that is attached to a query object.
    *
    * @param queryConnection   Pointer of the query connection to release.
    *
    */
   void
   releaseQueryConnection(
      void* queryObject);
  
  
   // Define all friend classes
   friend class SybaseQuery;
};

#endif
#endif