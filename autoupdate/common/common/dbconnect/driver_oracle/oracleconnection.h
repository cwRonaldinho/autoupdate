/*
 * OracleConnection object defines the needed connection functions for the dbConnect OracleConnection driver
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
#ifdef _DB_ORACLE_
#ifndef __ORACLE_CONNECTION_H__
#define __ORACLE_CONNECTION_H__

#define ORACLE10_DRIVERNAME    "ORACLE10"

#ifdef WIN32
#include <windows.h>
#endif



#include <string>
#include <time.h>

#include "dbconnect/driver_oracle/oracle_inc/oci.h"
#include "dbconnect/base/dbconnectTypes.h"
#include "dbconnect/base/baseConnection.h"
#include "dbconnect/base/baseException.h"
#include "dbconnect/base/simpleThreads.h"

using namespace std;


void OciFunctionCheck(int index,string message,OCIError *errhandle,sword ret);
class OracleHandle : public BaseHandle
{
public:
	// Oracle Connection Handles;
   OCIError*	_errHandle;
   OCIEnv*		_evtHandle;         // Environment handle
   OCISvcCtx*	_svcCtx;
   OCIStmt *    _stmtHandle;
   void			*queryObject;
	
	// Constructor
   OracleHandle() :_errHandle(NULL),queryObject(NULL),_stmtHandle(NULL)
	{ };
	~OracleHandle();
};


class OracleConnection : public BaseConnection
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

   DBULONG		_numHandles;
   OracleHandle **_handles;


   /* Internal method to connect the handle to an Oracle database.
    *
    * @param handleIndex     Index to the connection handle that must be connected.
    */
   void 
   _oracleConnect(
         int index);

   
   /* Internal method to disconnect the handle from an Oracle database.
    *
    * @param handleIndex     Index to the connection handle that must be disconnected.
    */
   void 
   _oracleDisconnect(
         int index);
   
   
   /* Internal method to make sure the connection handle is still alive and reconnect if timed out.
    *
    * @param handleIndex     Index to the connection handle that must be pinged.
    */
   void 
   _oraclePing(
         int index);


   /*
   * Internal method to reconnect to SQL server
   */
   void
   _oracleReconnect(int index);


   /* Internal method to free internally allocated memory
    *
    * @param type    The type representing which value to deallocate.
    *
    */
   void 
   _freeCollection(
         CollectionType type);

   void _setPingTime( int index);

   
public:
   /* Constructor.
    *
    */
   OracleConnection(
         int argc, 
         const char** argv);


   /* Destructor.
    *
    */
   ~OracleConnection();


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

   
   friend class OracleQuery;
};


#endif
#endif