/*
* OracleConnection object defines the needed connection functions for the dbConnect Oracle driver
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
#ifdef _DB_ORACLE_
#include "dbconnect/driver_oracle/oracleconnection.h"
#include "dbconnect/driver_oracle/oraclequery.h"
#include <stdio.h>

#define new DEBUG_NEW
// -----------------------------------------------------------------------------
// PRIVATE:
// -----------------------------------------------------------------------------
OracleHandle::~OracleHandle()
{
	if(_stmtHandle) 
	{
		OCIHandleFree(_stmtHandle,OCI_HTYPE_STMT);
		_stmtHandle = NULL;
	}

	if (_svcCtx && _errHandle)
	{
		OCILogoff(_svcCtx,_errHandle);
		_svcCtx = NULL;
	}

	if (_errHandle)
	{
		OCIHandleFree(_errHandle,OCI_HTYPE_ERROR);
		_errHandle = NULL;
	}

	if (_evtHandle)
	{
		OCIHandleFree (_evtHandle,	OCI_HTYPE_ENV);
		_evtHandle = NULL;
	}

	if(queryObject)
	{
		OracleQuery* p = (OracleQuery*)queryObject;
		delete p;
	}
}

// -----------------------------------------------------------------------------
// OracleConnection::_oracleConnect
// -----------------------------------------------------------------------------
void 
OracleConnection::_oracleConnect(
							 int index)
{
	g_dbclog.print("[%04d] _oracleConnect\n", index );
	// Make sure the handle is valid and we are not already connected.
	if (index > (int)_numHandles || index < 0)
		throw Error("_oracleConnect(): Invalid index to database handle.");

	if (_handles[index]->status != BaseHandle::DISCONNECTED)
		throw AlreadyConnected("_oracleConnect(): "
		"The database connection is already connected to the database.");   

	OciFunctionCheck( index,"OCIEnvCreate",_handles[index]->_errHandle,OCIEnvCreate(&_handles[index]->_evtHandle,
		OCI_THREADED|OCI_NO_MUTEX|OCI_OBJECT, 0,0,0,0, (size_t) 0, 0));
		//OCI_THREADED|OCI_NO_MUTEX|OCI_OBJECT, 0,0,0,0, (size_t) 0, 0));

	OciFunctionCheck( index,"Alloc Error Handle",_handles[index]->_errHandle,OCIHandleAlloc ((dvoid *)_handles[index]->_evtHandle, 
		(dvoid **)&_handles[index]->_errHandle,OCI_HTYPE_ERROR, 0, (dvoid **) 0));

	OciFunctionCheck(index,"Logon",_handles[index]->_errHandle,OCILogon(_handles[index]->_evtHandle,_handles[index]->_errHandle,
		&_handles[index]->_svcCtx,(text*)username.c_str(),(ub4)username.length(),
		(text*)password.c_str(),(ub4)password.length(),(text*)host.c_str(),(ub4)host.length())) ;

	g_dbclog.print("[%04d] _oracleConnect ok, 0x%X \n", index, _handles[index]->queryObject );

	_handles[index]->status = BaseHandle::CONNECTED;
	_handles[index]->lastUsed = time(NULL);      

} // OracleConnection::_oracleConnect

void
OracleConnection::_setPingTime(int index)
{
	_handles[index]->lastUsed = time(NULL);
}

// -----------------------------------------------------------------------------
// OracleConnection::_oracleDisconnect
// -----------------------------------------------------------------------------
void 
OracleConnection::_oracleDisconnect(
								int index)
{
	g_dbclog.print("[%04d] _oracleDisconnect\n", index );
	// Make sure the handle is valid and we are not already connected.
	if (index > (int)_numHandles || index < 0)
		throw Error("_oracleDisconnect(): Invalid index to database handle.");   

	if (_handles[index]->status == BaseHandle::DISCONNECTED || 
		_handles[index]->status == BaseHandle::UNKNOWN)
		throw NotConnected("_oracleDisconnect(): "
		"The database handle does not have a valid connection to the database.");

	// Disconnect from the server.
	// Free the connection handle allocated
	if(_handles[index]->_stmtHandle) 
	{
		OCIHandleFree(_handles[index]->_stmtHandle,OCI_HTYPE_STMT);
		_handles[index]->_stmtHandle = NULL;
	}
	OCILogoff(_handles[index]->_svcCtx,_handles[index]->_errHandle);
	OCIHandleFree(_handles[index]->_errHandle,OCI_HTYPE_ERROR);
	OCIHandleFree (_handles[index]->_evtHandle,	OCI_HTYPE_ENV);
	_handles[index]->_errHandle = NULL;
	_handles[index]->_evtHandle = NULL;
	

	_handles[index]->status = BaseHandle::DISCONNECTED;
	_handles[index]->lastUsed = time(NULL);
} // OracleConnection::_oracleDisconnect


// -----------------------------------------------------------------------------
// OracleConnection::_oraclePing
// -----------------------------------------------------------------------------
void 
OracleConnection::_oraclePing( 
						  int index)
{   
#ifdef __ORACLE_X__
	// Make sure the handle is valid.
	if (index > (int)_numHandles || index < 0)
		throw Error("_oraclePing(): Invalid index to database handle.");

	// Only ping the connection if the ping interval has expired since the
	//   connection was last used.
	time_t currentTime = time(NULL);

	sb4 code = 0;
	OCIErrorGet(_handles[index]->_errHandle,1,NULL,&code,NULL,0,OCI_HTYPE_ERROR) ;
	g_dbclog.print("[%04d] The Error code is %d,must Reconnect\n",	index, code);//打印错误码
	if(code == 3114 || code == 12571 || code == 24324)//can not catch "03114" anomalous,change it to 3114
	{
		g_dbclog.print("[%04d] error code(%d) Reconnect\n",	index, code);
		_oracleReconnect(index);
		string err = "_oracleReconnect(): reconnect OK, current command abort!";
		throw ErrorPingingConnection(err);     
	}
	else if (currentTime - _handles[index]->lastUsed >= pingInterval)
	{
		// Do a dummy query
		g_dbclog.print("[%04d] LastUsed Great interval %d,Ping\n",	index, pingInterval);

		OCIStmt * statement;
		char sql[] ="select 1 from dual";
		OCIHandleAlloc(_handles[index]->_evtHandle,(dvoid **)&statement,OCI_HTYPE_STMT, 0, (dvoid **) 0);
		OCIStmtPrepare(statement,_handles[index]->_errHandle,(text*)sql,sizeof(sql)+1,OCI_NTV_SYNTAX,OCI_DEFAULT);
		sword ret = OCIStmtExecute(_handles[index]->_svcCtx,statement,
			_handles[index]->_errHandle,0,0,NULL,NULL,OCI_COMMIT_ON_SUCCESS);
		
		OCIHandleFree(statement,OCI_HTYPE_STMT);
		//sword ret = OCIPing(_handles[index]->_svcCtx,_handles[index]->_errHandle,OCI_DEFAULT);
		g_dbclog.print("[%04d] Ping ret=%d\n",	index, ret);
		if ( ret != OCI_SUCCESS )
		{
			g_dbclog.print("[%04d] Ping error Reconnect\n",	index, pingInterval);
			_oracleReconnect(index);
			string err = "_oracleReconnect(): reconnect OK, current command abort!";
			throw ErrorPingingConnection(err);     
		}
		_handles[index]->lastUsed  = time(NULL);
	}
#endif
} // OracleConnection::_oraclePing

// -----------------------------------------------------------------------------
// OracleConnection::_oracleReconnect
// -----------------------------------------------------------------------------
void
OracleConnection::_oracleReconnect(int index)
{
	// Connection is stale, need to reconnect
	// Synchronize the function as we dont want this connection taken from us
	g_dbclog.print("[%04d] _oracleReconnect Begin\n", index);
	SimpleThread_Synchronize sync(classMutex);         
	g_dbclog.print("[%04d] _oracleReconnect\n", index );
	//1. Disconnect ingnoring errors
	try         
	{
		_oracleDisconnect(index);
	}
	catch(...)
	{ }  // Ignore any errors from the disconnect.

	//2. Try get a new connection
	try
	{
		_oracleConnect(index);

		// Force the connection to be in use again
		_handles[index]->status = BaseHandle::CONNECTED_USED;
	}
	catch(...)
	{
		string err = "_oracleReconnect(): Unable to establish new connection, ";
		throw ErrorPingingConnection(err);            
	}  
}


//------------------------------------------------------------------------------
// OracleConnection::_freeCollection
//------------------------------------------------------------------------------
void OracleConnection::_freeCollection(
									 CollectionType type)
{
	int i;
	switch (type)
	{
	case CONNECTION_HANDLES:
		if (_handles)
		{
			for (i=0; i<(int)_numHandles; i++)
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
} // OracleConnection::_freeCollection


// -----------------------------------------------------------------------------
// PUBLIC:
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// OracleConnection::OracleConnection
// -----------------------------------------------------------------------------
OracleConnection::OracleConnection(
							   int argc, 
							   const char** argv)
							   :  
BaseConnection(ORACLE10_DRIVERNAME),
_numHandles(0),
_handles(NULL)
{


} // OracleConnection::OracleConnection


// -----------------------------------------------------------------------------
// OracleConnection::~OracleConnection
// -----------------------------------------------------------------------------
OracleConnection::~OracleConnection()
{
	// Make sure we disconnect from the database.
	if (isConnected)
	{
		disconnect(120);  // 2 Minute timeout.
	}

	// Free any handles.   
	_freeCollection(CONNECTION_HANDLES);
	OCITerminate(OCI_DEFAULT);

} // OracleConnection::~OracleConnection


// -----------------------------------------------------------------------------
// OracleConnection::connect
// -----------------------------------------------------------------------------
void 
OracleConnection::connect(
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
	g_dbclog.print("Connect: host is %s, username is %s \n",host.c_str(),username.c_str());
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
	_handles = (OracleHandle**)malloc(_numHandles * sizeof(OracleHandle*));
	for (int i=0; i<(int)_numHandles; i++)
	{
		_handles[i] = new OracleHandle();
		_handles[i]->status = BaseHandle::DISCONNECTED;
	}

	// Connect the required specified minimum connections. The rest will be on demand.
	for (int j=0; j<minConnections; j++)
		_oracleConnect(j);

	isConnected = true;      
} // OracleConnection::connect


// -----------------------------------------------------------------------------
// OracleConnection::disconnect
// -----------------------------------------------------------------------------
void
OracleConnection::disconnect(
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
	for (int i=0; i<(int)_numHandles; i++)
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

			_oracleDisconnect(i);
		}
	}

	isConnected = false;   
} // OracleConnection::disconnect


// -----------------------------------------------------------------------------
// OracleConnection::requestQueryConnection
// -----------------------------------------------------------------------------
void*
OracleConnection::requestQueryConnection()
{
	// This function must act as a fifo stack. The first thread in must get the first 
	//   available connection.
	// Synchronize the function.
	SimpleThread_Synchronize sync(classMutex);
	g_dbclog.print("[XXXX] requestQueryConnection()\n");
	// Make sure we are connected.
	if (!isConnected)
		throw NotConnected("requestQueryConnection(): Not connected to the database.");

	// Flag that a request is occuring.
	isRequestQueryConnectionOccuring = true; 

	// Loop until we have a valid connection or an error.
	// Built in timeout of 1 min per thread to retrieve a connection.
	int i;
	time_t now = time(NULL);
	while (time(NULL) <= now + 60)
	{
		// Try and obtain a connection
		for (i=0; i<(int)_numHandles; i++)
		{
			// We have an available handle thats already connected.
			if (_handles[i]->status == BaseHandle::CONNECTED)
			{
				_handles[i]->queryObject = new OracleQuery(this, i);
				_handles[i]->status = BaseHandle::CONNECTED_USED;

				isRequestQueryConnectionOccuring = false;
				g_dbclog.print("[%04d] requestQueryConnnection, return 0x%08x\n",i,_handles[i]->queryObject);
				return _handles[i]->queryObject;
			}

			// We have an available handle that needs to be connected.
			if (_handles[i]->status == BaseHandle::DISCONNECTED)
			{
				_oracleConnect(i);
				_handles[i]->queryObject = new OracleQuery(this, i);
				_handles[i]->status = BaseHandle::CONNECTED_USED;

				isRequestQueryConnectionOccuring = false;
				g_dbclog.print("requestQueryConnection, return 0x%08x\n", _handles[i]->queryObject );
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
} // OracleConnection::requestQueryConnection


// -----------------------------------------------------------------------------
// OracleConnection::releaseQueryConnection
// -----------------------------------------------------------------------------
void
OracleConnection::releaseQueryConnection(
									   void* queryObject)
{
	// Don't synchronize as we may need to release a connection to allow
	//  requestQueryConnection to get an available one
	g_dbclog.print("releaseQueryConnection,numhandles=%d\n", _numHandles);
	// Find the handle that has the query connection instance
	int i;
	for (i=0; i<(int)_numHandles; i++)
	{
		if (_handles[i]->queryObject == queryObject)
		{
			g_dbclog.print("release index=%d\n", i);
			delete (OracleQuery*)queryObject;
			_handles[i]->queryObject = NULL;
			_handles[i]->status = BaseHandle::CONNECTED;
			// Check if we need to release the connection.
			// We release the connection if the connection is outside of the minimum
			// connections and there is no current request for a connection waiting.
			if (i >= minConnections && !isRequestQueryConnectionOccuring)
			{
				_oracleDisconnect(i);
			}

			break;
		}
	}
} // OracleConnection::releaseQueryConnection


void OciFunctionCheck(int index,string message,OCIError *errhandle,sword ret)
{
	char buf[1024] = { 0}; 
	sb4 code = 0;
	switch(ret)
	{
	case OCI_NO_DATA:
	case OCI_SUCCESS_WITH_INFO:
	case OCI_SUCCESS:
			break;

	case OCI_ERROR:
		{
			OCIErrorGet(errhandle,1,NULL,&code,(text *)buf,sizeof(buf),OCI_HTYPE_ERROR) ;
			g_dbclog.print("[%04d] %s : %s\n",index, message.c_str(),buf );
			//throw Error(message+" : "+buf) ;
			switch(code)
			{
			case 3114: //连接断开
			case 3113: //非正常结束
			case 3135://连接失去联系
			case 12560:	//协议适配器错误
			case 12514: //无监听器
			case 12528:
			case 1089:
			case 530:
				throw Error(message+" : "+buf) ;
				break;
			default:
				throw CommandError(message+" : "+buf) ;//zhanglq modified 
				break;
			}
		}
		break;

	case OCI_INVALID_HANDLE:
		g_dbclog.print("[%04d] %s : Invalid handle\n",index, message.c_str());
		throw Error(message+ " : invalid handle") ;
		break;

	default:
		g_dbclog.print("[%04d] %s : unknown error code %d\n",index, message,ret );
		throw Error(message+" : unknown exception") ;
	}
	return;
}

#endif