/*
 * ODBCConnection object defines the needed connection functions for the dbConnect ODBC driver
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
 * Notes:
 *  1)  
 *
 */

#ifndef __ODBC_CONNECTION_H__
#define __ODBC_CONNECTION_H__

#define ODBC_DRIVERNAME    "ODBC"


#include <windows.h>
#include <sql.h>
#include <sqlext.h>

#include <string>
#include <time.h>

#include "dbconnectTypes.h"
#include "baseConnection.h"
#include "baseException.h"
#include "simpleThreads.h"

using namespace std;

class ODBCHandle : public BaseHandle
{
public:
   // ODBC Connection Handles;
   SQLHANDLE   _hdbc;
   void        *queryObject;   // Query Object bound to the connection

   // Constructor
   ODBCHandle() 
   :
      _hdbc(SQL_NULL_HDBC),
      queryObject(NULL) 
   { };
};


class ODBCConnection : public BaseConnection
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
   SQLHANDLE    _henv;          // Environment handle   
   DBULONG      _numHandles;
   ODBCHandle  **_handles;


   /* Internal method to connect the handle to an ODBC database.
    *
    * @param handleIndex     Index to the connection handle that must be connected.
    */
   void 
   _odbcConnect(
         int index);

   
   /* Internal method to disconnect the handle from an ODBC database.
    *
    * @param handleIndex     Index to the connection handle that must be disconnected.
    */
   void 
   _odbcDisconnect(
         int index);
   
   
   
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
   ODBCConnection(
         int argc, 
         const char** argv);


   /* Destructor.
    *
    */
   ~ODBCConnection();


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
         const string &optParam1="",
         const string &optParam2="");


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
    * @return            Returns a void pointer to a BaseQuery object. (The pointer needs to be type casted.
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

   /* Internal method to make sure the connection handle is still alive and reconnect if timed out.
    *
    * @param handleIndex     Index to the connection handle that must be pinged.
    */
   void 
   _odbcReconnect(
         int index);


   friend class ODBCQuery;
};


#endif
