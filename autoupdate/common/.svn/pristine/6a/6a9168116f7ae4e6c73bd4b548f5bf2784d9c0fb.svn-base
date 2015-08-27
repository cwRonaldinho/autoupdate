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

#include "baseConnection.h"
#include "dbconnectTypes.h"

cacti::LazyLog		g_dbclog( "dbc" );

//------------------------------------------------------------------------------
// BaseConnection::BaseConnection
//------------------------------------------------------------------------------
BaseConnection::BaseConnection(
   const string &drvName)
   :
    driverName(drvName)
{
   username = "";
   password = "";
   databaseName = "";
   host = "";
   maxConnections = 0;
   minConnections = 0;
   optParam1 = "";
   optParam2 = "";
   pingInterval = 60;    //Default to 1 min
   isConnected = false;
   isRequestQueryConnectionOccuring = false;
}  // BaseConnection::BaseConnection


//------------------------------------------------------------------------------
// BaseConnection::~BaseConnection
//------------------------------------------------------------------------------
BaseConnection::~BaseConnection()
{
   
}  // BaseConnection::~BaseConnection



//------------------------------------------------------------------------------
// BaseConnection::connect
//------------------------------------------------------------------------------
void 
BaseConnection::connect(
   const string &username, 
   const string &password, 
   const string &databaseName, 
   const string &host, 
   int   maxConnections,
   int   minConnections,
   const string &optParam1,
   const string &optParam2)
{
   //Make a copy of the values.
   BaseConnection::username = username.c_str();
   BaseConnection::password = password.c_str();
   BaseConnection::databaseName = databaseName.c_str();
   BaseConnection::host = host.c_str();
   if (maxConnections < 1)
      BaseConnection::maxConnections = 1;
   else
      BaseConnection::maxConnections = maxConnections;

   if (minConnections < 1)
      BaseConnection::minConnections = 1;
   if (minConnections > maxConnections)
      BaseConnection::minConnections = maxConnections;
   else
      BaseConnection::minConnections = minConnections;


   BaseConnection::optParam1 = optParam1.c_str();
   BaseConnection::optParam2 = optParam2.c_str();       
} // BaseConnection::connect


//------------------------------------------------------------------------------
// BaseConnection::setPingInterval
//------------------------------------------------------------------------------
void 
BaseConnection::setPingInterval(
   time_t pingInterval)
{
   // Minimum of 10 seconds
   if (pingInterval < 10)
      BaseConnection::pingInterval = 10;
   else
      BaseConnection::pingInterval = pingInterval;
}  // BaseConnection::setPingInterval
