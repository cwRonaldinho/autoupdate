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
*/
#include "stdafx.h"

#include "odbcconnection.h"
#include "odbcquery.h"
#include <stdio.h>

// -----------------------------------------------------------------------------
// PRIVATE:
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// ODBCConnection::_odbcConnect
// -----------------------------------------------------------------------------
void 
ODBCConnection::_odbcConnect(
							 int index)
{
	g_dbclog.print("[%04d] _odbcConnec\n", index );
	// Make sure the handle is valid and we are not already connected.
	if (index > (int)_numHandles || index < 0)
		throw Error("_odbcConnect(): Invalid index to database handle.");

	if (_handles[index]->status != BaseHandle::DISCONNECTED)
		throw AlreadyConnected("_odbcConnect(): "
		"The database connection is already connected to the database.");   

	SQLRETURN cliRC = SQL_SUCCESS;

	// Allocate the connection handle
	cliRC = SQLAllocHandle(SQL_HANDLE_DBC, _henv, &_handles[index]->_hdbc);
	if (cliRC != SQL_SUCCESS)
		throw Error("_odbcConnect(): "
		"Unable to allocate the connection handle.");

	// Connect to the ODBC database.   
	cliRC = SQLConnect(_handles[index]->_hdbc, (SQLCHAR*)host.c_str(), SQL_NTS, (SQLCHAR*)username.c_str(), SQL_NTS, (SQLCHAR*)password.c_str(), SQL_NTS);

	// Check if an error occured while connecting to the database.
	if (cliRC != SQL_SUCCESS)
	{
		SQLCHAR		sqlState[16]	= { 0 };
		SQLINTEGER	sqlCode;
		SQLCHAR		message[SQL_MAX_MESSAGE_LENGTH]	= { 0 };
		SQLSMALLINT	length;

		SQLGetDiagRec(SQL_HANDLE_DBC, _handles[index]->_hdbc, 1, sqlState, &sqlCode, message, SQL_MAX_MESSAGE_LENGTH - 1, &length);

		string err = "_odbcConnect(): ";
		err += (const char*)message;

		if ( cliRC != SQL_SUCCESS_WITH_INFO ){
			// Free the connection handle allocated
			SQLFreeHandle(SQL_HANDLE_DBC, _handles[index]->_hdbc);
			_handles[index]->_hdbc = SQL_NULL_HDBC;
			g_dbclog.print("[%04d] ERRO: _odbcConnect: %s\n", index, message );
			throw ErrorConnecting(err);
		}
		else{
			g_dbclog.print("[%04d] INFO: _odbcConnect: %s\n", index, message );
		};
	}

	//	Set the connection autocommit to off
	//	cliRC = SQLSetConnectAttr(_handles[index]->_hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_NTS);
	//
	//	do not switch AUTO_COMMIT off, it may cause some SQL Server locked
	//

	/* Check if an error occured setting the autocommit to false
	if (cliRC != SQL_SUCCESS)
	{
		SQLCHAR		sqlState[16]	= { 0 };
		SQLINTEGER	sqlCode			= 0;
		SQLCHAR		message[SQL_MAX_MESSAGE_LENGTH]	= { 0 };
		SQLSMALLINT	length			= 0;

		SQLGetDiagRec(SQL_HANDLE_DBC, _handles[index]->_hdbc, 1, sqlState, &sqlCode, message, SQL_MAX_MESSAGE_LENGTH -1, &length);

		string err = "_odbcConnect(): Unable to set autocommit off, ";      
		err += (const char*)message;

		// Disconnect from the server.
		SQLDisconnect(_handles[index]->_hdbc);

		// Free the connection handle allocated
		SQLFreeHandle(SQL_HANDLE_DBC, _handles[index]->_hdbc);
		_handles[index]->_hdbc = SQL_NULL_HDBC;
		throw ErrorConnecting(err);
	}
	*/

	_handles[index]->status = BaseHandle::CONNECTED;
	_handles[index]->lastUsed = time(NULL);      
} // ODBCConnection::_odbcConnect


// -----------------------------------------------------------------------------
// ODBCConnection::_odbcDisconnect
// -----------------------------------------------------------------------------
void 
ODBCConnection::_odbcDisconnect(
								int index)
{
	g_dbclog.print("[%04d] _odbcDisconnect\n", index );
	// Make sure the handle is valid and we are not already connected.
	if (index > (int)_numHandles || index < 0)
		throw Error("_odbcDisconnect(): Invalid index to database handle.");   

	if (_handles[index]->status == BaseHandle::DISCONNECTED || 
		_handles[index]->status == BaseHandle::UNKNOWN)
		throw NotConnected("_odbcDisconnect(): "
		"The database handle does not have a valid connection to the database.");

	// Disconnect from the server.
	SQLDisconnect(_handles[index]->_hdbc);

	// Free the connection handle allocated
	SQLFreeHandle(SQL_HANDLE_DBC, _handles[index]->_hdbc);
	_handles[index]->_hdbc = SQL_NULL_HDBC;


	_handles[index]->status = BaseHandle::DISCONNECTED;
	_handles[index]->lastUsed = time(NULL);
} // ODBCConnection::_odbcDisconnect


// -----------------------------------------------------------------------------
// ODBCConnection::_odbcPing
// -----------------------------------------------------------------------------
void 
ODBCConnection::_odbcReconnect( 
						  int index)
{
	/*
	g_dbclog.print("[%04d] _odbcPing\n", index );
	// Make sure the handle is valid.
	if (index > _numHandles || index < 0)
		throw Error("_odbcPing(): Invalid index to database handle.");

	// Only ping the connection if the ping interval has expired since the
	//   connection was last used.
	time_t currentTime = time(NULL);
	if (currentTime - _handles[index]->lastUsed >= pingInterval)
	{
		// Do a dummy query
		SQLRETURN cliRC = SQL_SUCCESS;
		SQLHANDLE hstmt;
		bool		needreconnect	= false;

		// Directly execute the statement after allocating the statement handle
		cliRC = SQLAllocHandle(SQL_HANDLE_STMT, _handles[index]->_hdbc, &hstmt);
		if (cliRC == SQL_SUCCESS){
			//cliRC = SQLExecDirect(hstmt, (SQLCHAR*)"SELECT CURRENT DATE FROM SYSIBM.SYSDUMMY1", SQL_NTS);
			cliRC	= SQLExecDirect(hstmt, (SQLCHAR*)"select * from NEVER_EXIST_TABLE_FOR_TEST", SQL_NTS );
			if ( cliRC != SQL_SUCCESS ){
				SQLCHAR		sqlState[16]	= { 0 };
				SQLINTEGER	sqlCode			= 0;
				SQLCHAR		message[SQL_MAX_MESSAGE_LENGTH]	= { 0 };
				SQLSMALLINT	length			= 0;

				SQLGetDiagRec(SQL_HANDLE_DBC, _handles[index]->_hdbc, 1, sqlState, &sqlCode, message, SQL_MAX_MESSAGE_LENGTH -1, &length);
				if ( strcmp( (const char*)sqlState, "S0002") != 0 ){
					needreconnect	= true;
				}
			}
		};

		// Directly execute the statement      
		if ( needreconnect )
		{*/


	g_dbclog.print("[%04d] _odbcPing, reconnect\n", index );
	//SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

	// Connection is stale, need to reconnect
	// Synchronize the function as we dont want this connection taken from us
	SimpleThread_Synchronize sync(classMutex);         

	//1. Disconnect ingnoring errors
	try         
	{
		_odbcDisconnect(index);
	}
	catch(...)
	{ }  // Ignore any errors from the disconnect.

	//2. Try get a new connection
	try
	{
		_odbcConnect(index);

		// Force the connection to be in use again
		_handles[index]->status = BaseHandle::CONNECTED_USED;
	}
	catch(...)
	{
		string err = "_odbcPing(): Unable to establish new connection, ";
		throw ErrorPingingConnection(err);            
	}            

		/*}
		else
		{
			// Ping was OK
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		}
	}
	*/
} // ODBCConnection::_odbcPing


//------------------------------------------------------------------------------
// ODBCConnection::_freeCollection
//------------------------------------------------------------------------------
void ODBCConnection::_freeCollection(
									 CollectionType type)
{
	unsigned int i;
	switch (type)
	{
	case CONNECTION_HANDLES:
		if (_handles)
		{
			for (i=0; i<_numHandles; i++)
			{
				if (_handles[i])
				{
					delete _handles[i];
					_handles[i] = NULL;         
				}
			}
			free(_handles);
			_handles = NULL;
			_numHandles = 0;
		}

		break;      
	}
} // ODBCConnection::_freeCollection


// -----------------------------------------------------------------------------
// PUBLIC:
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// ODBCConnection::ODBCConnection
// -----------------------------------------------------------------------------
ODBCConnection::ODBCConnection(
							   int argc, 
							   const char** argv)
							   :  
BaseConnection(ODBC_DRIVERNAME),
_henv(SQL_NULL_HENV),
_numHandles(0),
_handles(NULL)
{
	// Store any arguments in a easy to use structure. (Must be even number)
	// All argument names will be in lowercase.

	// -- No argument processing at present



	// Create the ODBC environment handle
	SQLRETURN cliRC = SQL_SUCCESS;

	cliRC = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_henv);

	if (cliRC != SQL_SUCCESS)
	{
		printf("Unable to initialize ODBC environment. Is ODBCINSTANCE=xxx exported correctly\n");
		throw Error("Unable to allocate ODBC Environment handle");
	}

	// Set attribute to enable application to run as ODBC 3.0 application 
	if( argc > 0 )
		cliRC = SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC2, 0);
	else
		cliRC = SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

} // ODBCConnection::ODBCConnection


// -----------------------------------------------------------------------------
// ODBCConnection::~ODBCConnection
// -----------------------------------------------------------------------------
ODBCConnection::~ODBCConnection()
{
	// Make sure we disconnect from the database.
	if (isConnected)
	{
		disconnect(120);  // 2 Minute timeout.
	}

	// Free any handles.   
	_freeCollection(CONNECTION_HANDLES);

	// Free the ODBC environment handle
	SQLFreeHandle(SQL_HANDLE_ENV, _henv);
} // ODBCConnection::~ODBCConnection


// -----------------------------------------------------------------------------
// ODBCConnection::connect
// -----------------------------------------------------------------------------
void 
ODBCConnection::connect(
						const string &username, 
						const string &password, 
						const string &databaseName, 
						const string &host, 
						int          maxConnections,
						int          minConnections,
						const string &optParam1,
						const string &optParam2)
{
	// Synchronize the function.
	SimpleThread_Synchronize sync(classMutex);
	g_dbclog.print("connect()\n");
	// Make sure we are not already connected.
	if (isConnected)
		throw ErrorConnecting("connect(): Already connected to the database.");

	// Check and set the parameters. (Use the base class for this)
	BaseConnection::connect(username, password, databaseName, host,
		maxConnections, minConnections, optParam1, optParam2);

	// Free any handles used before.
	_freeCollection(CONNECTION_HANDLES);   

	// Create the collection to hold the needed handle information.
	_numHandles = maxConnections;
	_handles = (ODBCHandle**)malloc(_numHandles * sizeof(ODBCHandle*));
	for (unsigned int i=0; i<_numHandles; i++)
	{
		_handles[i] = new ODBCHandle();
		_handles[i]->status = BaseHandle::DISCONNECTED;
	}

	// Connect the required specified minimum connections. The rest will be on demand.
	for (int j=0; j<minConnections; j++)
		_odbcConnect(j);

	isConnected = true;      
} // ODBCConnection::connect


// -----------------------------------------------------------------------------
// ODBCConnection::disconnect
// -----------------------------------------------------------------------------
void
ODBCConnection::disconnect(
						   time_t timeout)
{
	// Synchronize the function.
	SimpleThread_Synchronize sync(classMutex);
	g_dbclog.print("disconnect()\n");
	bool doneTimeout = false;

	// Only disconnect if we are connected.
	if (!isConnected)
		throw NotConnected("disconnect(): Not connected to the database.");

	// Disconnect all the open connections.
	for (unsigned int i=0; i<_numHandles; i++)
	{
		// Free the query object if it is still attached.
		// TODO

		if (_handles[i]->status == BaseHandle::CONNECTED || 
			_handles[i]->status == BaseHandle::CONNECTED_USED)
		{
			// If the handle is USED wait for the timeout.
			// Only wait once for the timeout for all connections.
			if (_handles[i]->status == BaseHandle::CONNECTED_USED && !doneTimeout)
			{
				SimpleThread::sleep((unsigned int)timeout * 1000);
				doneTimeout = true;
			}

			_odbcDisconnect(i);
		}
	}

	isConnected = false;   
} // ODBCConnection::disconnect


// -----------------------------------------------------------------------------
// ODBCConnection::requestQueryConnection
// -----------------------------------------------------------------------------
void*
ODBCConnection::requestQueryConnection()
{
	// This function must act as a fifo stack. The first thread in must get the first 
	//   available connection.
	// Synchronize the function.
	SimpleThread_Synchronize sync(classMutex);
	g_dbclog.print("requestQueryConnection()\n");
	// Make sure we are connected.
	if (!isConnected)
		throw NotConnected("requestQueryConnection(): Not connected to the database.");

	// Flag that a request is occuring.
	isRequestQueryConnectionOccuring = true;


	// Loop until we have a valid connection or an error.
	// Built in timeout of 1 min per thread to retrieve a connection.
	unsigned int i;
	time_t now = time(NULL);
	while (time(NULL) <= now + 60)
	{
		// Try and obtain a connection
		for (i=0; i<_numHandles; i++)
		{
			// We have an available handle thats already connected.
			if (_handles[i]->status == BaseHandle::CONNECTED)
			{
				_handles[i]->queryObject = new ODBCQuery(this, i);
				_handles[i]->status = BaseHandle::CONNECTED_USED;

				isRequestQueryConnectionOccuring = false;
				g_dbclog.print("requestQueryConnection, return 0x%08x\n", 
					_handles[i]->_hdbc );
				return _handles[i]->queryObject;
			}

			// We have an available handle that needs to be connected.
			if (_handles[i]->status == BaseHandle::DISCONNECTED)
			{
				_odbcConnect(i);
				_handles[i]->queryObject = new ODBCQuery(this, i);
				_handles[i]->status = BaseHandle::CONNECTED_USED;

				isRequestQueryConnectionOccuring = false;
				g_dbclog.print("requestQueryConnection, return 0x%08x\n", 
					_handles[i]->_hdbc );
				return _handles[i]->queryObject;
			}
		}

		// Sleep for a second to conserve resources
		SimpleThread::sleep(1000);
	}


	// If we got this far a timeout occured.
	isRequestQueryConnectionOccuring = false;
	throw QueryConnectionTimeout("requestQueryConnection(): A timout occured "
		"while trying to obtain a query connection.");
} // ODBCConnection::requestQueryConnection


// -----------------------------------------------------------------------------
// ODBCConnection::releaseQueryConnection
// -----------------------------------------------------------------------------
void
ODBCConnection::releaseQueryConnection(
									   void* queryObject)
{
	// Don't synchronize as we may need to release a connection to allow
	//  requestQueryConnection to get an available one
	g_dbclog.print("releaseQueryConnection\n");
	// Find the handle that has the query connection instance
	unsigned int i;
	for (i=0; i<_numHandles; i++)
	{
		if (_handles[i]->queryObject == queryObject)
		{
			_handles[i]->queryObject = NULL;
			_handles[i]->status = BaseHandle::CONNECTED;

			// Check if we need to release the connection.
			// We release the connection if the connection is outside of the minimum
			// connections and there is no current request for a connection waiting.
			if (i >= (unsigned int) minConnections && !isRequestQueryConnectionOccuring)
			{
				_odbcDisconnect(i);
			}

			break;
		}
	}
} // ODBCConnection::releaseQueryConnection
