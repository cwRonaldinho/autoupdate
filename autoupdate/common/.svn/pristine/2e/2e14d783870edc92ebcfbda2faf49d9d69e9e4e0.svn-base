/*
 * BaseConnection Object Defining the base connection that all drivers inherit from
 * Copyright (C) 2002 Johnathan Ingram, jingram@rogue-order.net
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


#ifndef __BASE_CONNECTION_H__
#define __BASE_CONNECTION_H__

#ifdef WIN32
   // Windows
   // DLL Exports
#pragma warning(disable: 4251)
   class  __declspec( dllexport ) BaseHandle;
   class  __declspec( dllexport ) BaseConnection;
#endif   


#include <string>
#include <vector>
#include <time.h>
#include "cacti/logging/LazyLog.h"
extern	cacti::LazyLog		g_dbclog;

using namespace std;

class BaseHandle
{
public:     
   // Status changes in order that the enum is declared. 
   // (ie goes from DISCONNECTED to CONNECTED etc)
   enum ConnStatus   
   {
      UNKNOWN,
      DISCONNECTED,
      CONNECTED,
      CONNECTED_USED
   };

   time_t      lastUsed;
   ConnStatus  status;

   BaseHandle() 
      : lastUsed(0), status(UNKNOWN) { };
};


class BaseConnection
{
protected:  
   string     username;
   string     password; 
   string     databaseName;
   string     host;
   int        maxConnections;
   int        minConnections;
   string     optParam1;
   string     optParam2;
   
   bool       isConnected;
   time_t     pingInterval;   

   bool       isRequestQueryConnectionOccuring;   // Used when trying to obtain a query connection.

          
public:  
   string     driverName;


   /* Constructor.
    *
    */
   BaseConnection(
      const string &drvName="Unknown");

   /* Destructor.
    *
    */
   virtual 
   ~BaseConnection();

   /* Create connections to the database.
    *
    * @param username         Database username.
    * @param password         Password used for username.
    * @param databaseName     Name of the database connecting to.
    * @param host             Host the database resides on. Default "localhost"
    * @param maxConnections   Maximum number of connections the object can pool at any one time. Default 1
    * @param optParam1        Optional parameter that can be used by the driver.
    * @param optParam2        Optional parameter that can be used by the driver.
    */
   virtual void 
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
   virtual void 
   disconnect(
      time_t timeout=120) = 0;
   

   /* Set the time interval that a connection will be checked to make sure it is alive.
    *
    * @param pingInterval     Interval in seconds. Minimum of 60 seconds
    *
    */
   virtual void 
   setPingInterval(
      time_t pingInterval);

   
   /* Obtain a connection that is attached to a query object.
    *
    * @return            Returns a void pointer to a BaseQuery object. (The pointer needs to be type casted.
    */
   virtual void*
   requestQueryConnection() = 0;
   
   
   /* release a connection that is attached to a query object.
    *
    * @param queryConnection   Pointer of the query connection to release.
    *
    */
   virtual void
   releaseQueryConnection(
      void* queryObject) = 0;
};


#endif
