#ifdef _DB_SYBASE_
#include "stdafx.h"

#include "dbconnect/driver_sybase/sybaseConnection.h"
#include "dbconnect/driver_sybase/sybaseBindParam.h"
#include "dbconnect/driver_sybase/sybaseQuery.h"


// -----------------------------------------------------------------------------
// PRIVATE:
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
SybaseHandle::~SybaseHandle()
{
	if(queryObject)
	{
		SybaseQuery* sq = (SybaseQuery*)queryObject;
		delete sq;
	}
}


// SybaseConnection::_sybaseConnection
// -----------------------------------------------------------------------------

void 
SybaseConnection::_sybaseConnect(
         volatile int index, const string &charset ,const string &application)
{
	// Make sure the handle is valid and we are not already connected.
	if (index > (int) _numHandles || index < 0)
	  throw Error("_sybaseConnection(): Invalid index to database handle.");


	if (_handles[index]->status != BaseHandle::DISCONNECTED)
	  throw AlreadyConnected("_sybaseConnection(): "
		 "The database connection is already connected to the database.");

	LOGINREC *login;
	login = dblogin();

	if( !login ) 
	{	
      string err = "_sybaseConnection(): ";
      err += "dblogin failed";
      throw ErrorConnecting(err);
	}

	DBSETLUSER(login, (char *)username.c_str());
	DBSETLPWD(login, (char *)password.c_str());
	DBSETLAPP(login, (char*)application.c_str());
	DBSETLCHARSET(login, (char*)charset.c_str());
//TODO charset
	
	_handles[index]->dbproc = dbopen(login, (char *)host.c_str());	
	
	if(NULL == _handles[index]->dbproc) 
	{		
		dbloginfree( login );
		string err = "_sybaseConnection(): ";
		err += "dbopen failed";
		throw ErrorConnecting(err);
	}
	
	dbloginfree( login );

	DBPROCESS *dbproc = _handles[index]->dbproc;

	if( dbuse(dbproc, (char *)databaseName.c_str()) == FAIL )
	{
		string err = "_sybaseConnection(): ";
		err += "dbuse " + databaseName + "failed";
		throw ErrorConnecting(err);
	}
	g_dbclog.print("[%04d] _sybaseConnect ok, 0x%X \n", index, _handles[index]->dbproc );
   _handles[index]->status = BaseHandle::CONNECTED;
   _handles[index]->lastUsed = time(NULL);

} // SybaseConnection::_sybaseConnection

// -----------------------------------------------------------------------------
// SybaseConnection::_sybaseDisconnect
// -----------------------------------------------------------------------------
void 
SybaseConnection::_sybaseDisconnect(
         int index)
{
   // Make sure the handle is valid and we are not already connected.
   if (index > (int) _numHandles || index < 0)
      throw Error("_sybaseDisconnect(): Invalid index to database handle.");   

   if (_handles[index]->status == BaseHandle::DISCONNECTED || 
       _handles[index]->status == BaseHandle::UNKNOWN)
      throw NotConnected("_sybaseDisconnect(): "
         "The database handle does not have a valid connection to the database.");

   // Disconnect from the server.
   dbclose(_handles[index]->dbproc);
   _handles[index]->dbproc = NULL;
   
   _handles[index]->status = BaseHandle::DISCONNECTED;
   _handles[index]->lastUsed = time(NULL);
   g_dbclog.print("[%04d] _sybaseDisconnect ok, 0x%08x\n", 
		index, _handles[index]->dbproc ); 
} // SybaseConnection::_sybaseDisconnect



// -----------------------------------------------------------------------------
// SybaseConnection::_sybasePing
// -----------------------------------------------------------------------------
void 
SybaseConnection::_sybasePing(
      int index)
{
   // Make sure the handle is valid.
   if (index > (int) _numHandles || index < 0)
      throw Error("_sybasePing(): Invalid index to database handle.");

   // Only ping the connection if the ping interval has expired since the
   //   connection was last used.
	time_t currentTime = time(NULL);
	if (currentTime - _handles[index]->lastUsed >= pingInterval)
	{
      // Do a dummy query
		dbsettime(5);
		g_dbclog.print("[%04d] LastUsed Great interval %d,Ping\n",
			index, pingInterval);
		string query = "select 1";
		dbcmd(_handles[index]->dbproc, (char *)query.c_str());
		g_dbclog.print("[%04d] cmd end\n", index);
		if(FAIL == dbsqlexec(_handles[index]->dbproc)) 
		{
			g_dbclog.print("[%04d] exec end\n", index);
			_sybaseReconnect(index);
			g_dbclog.print("[%04d] dbsettime\n", index);
		}
		else
		{
			while((dbresults(_handles[index]->dbproc)) != NO_MORE_RESULTS)
				while (dbnextrow(_handles[index]->dbproc) != NO_MORE_ROWS);
		}
   }
   dbsettime(0);
} // SybaseConnection::_sybasePing


// -----------------------------------------------------------------------------
// SybaseConnection::_sybaseReconnect
// -----------------------------------------------------------------------------
void
SybaseConnection::_sybaseReconnect(int index)
{
	// Connection is stale, need to reconnect
	// Synchronize the function as we dont want this connection taken from us
	g_dbclog.print("[%04d] _SybaseReconnect Begin\n", index);
	SimpleThread_Synchronize sync(classMutex);         
	g_dbclog.print("[%04d] _sybaseReconnect\n", index );
	//1. Disconnect ingnoring errors
	try         
	{
		_sybaseDisconnect(index);
	}
	catch(...)
	{ }  // Ignore any errors from the disconnect.

	//2. Try get a new connection
	try
	{
		_sybaseConnect(index,BaseConnection::optParam1,BaseConnection::optParam2);

		// Force the connection to be in use again
		_handles[index]->status = BaseHandle::CONNECTED_USED;
	}
	catch(...)
	{
		string err = "_sybaseReconnect(): Unable to establish new connection, ";
		throw ErrorPingingConnection(err);            
	}  
}


// -----------------------------------------------------------------------------
// SybaseConnection::_sybaseQuery
// -----------------------------------------------------------------------------
void 
SybaseConnection::_sybaseQuery(
      int           index,
      const string& sqlStatement)
{
   // Make sure the handle is in the connected used state.
	if (_handles[index]->status != BaseHandle::CONNECTED_USED) 
	{
		g_dbclog.print("[%04d] _sybaseQuery dbproc in invalid status (%d) \n",index,_handles[index]->status );
		throw ErrorQuerying("_sybaseQuery(): The database connection is not valid. May have been terminated by the connection object?");
	}
	g_dbclog.print("[%04d] _sybaseQuery( %s )\n", index, sqlStatement.c_str() );

   //Execute the query
	dbcmd(_handles[index]->dbproc, (char *)sqlStatement.c_str());
	if(FAIL == dbsqlexec(_handles[index]->dbproc)) 
	{
      // Something went wrong while trying to execute the sql statement, so throw an exception.
      string err = "_sybaseQuery(): SQL statement: " + sqlStatement;
      throw ErrorQuerying(err);
	}
   // Set the last used
   _handles[index]->lastUsed = time(NULL);
} // SybaseConnection::_sybaseQuery
      

//------------------------------------------------------------------------------
// SybaseConnection::_freeCollection
//------------------------------------------------------------------------------
void SybaseConnection::_freeCollection(
      CollectionType type)
{
	g_dbclog.print("_freeCollection\n");
   unsigned int i;
   switch (type)
   {
      case CONNECTION_HANDLES:
         if (_handles)
         {
            for ( i=0; i<_numHandles; i++)
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
} // SybaseConnection::_freeCollection


// -----------------------------------------------------------------------------
// PUBLIC:
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// SybaseConnection::SybaseConnection
// -----------------------------------------------------------------------------
SybaseConnection::SybaseConnection(
      int argc, 
      const char** argv)
:  
   BaseConnection(SYBASE_DRIVERNAME),
   _numHandles(0),
   _handles(NULL)
{
   // Store any arguments in a easy to use structure. (Must be even number)
   // All argument names will be in lowercase.
	   
} // SybaseConnection::SybaseConnection



// -----------------------------------------------------------------------------
// SybaseConnection::~SybaseConnection
// -----------------------------------------------------------------------------
SybaseConnection::~SybaseConnection()
{
	// Make sure we disconnect from the database.
	if (isConnected)
	{
		disconnect(120);  // 2 Minute timeout.
	}

   // Free any handles.   
   _freeCollection(CONNECTION_HANDLES);   
} // SybaseConnection::~SybaseConnection



// -----------------------------------------------------------------------------
// SybaseConnection::connect
// -----------------------------------------------------------------------------
void 
SybaseConnection::connect(
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
	g_dbclog.print("connect()\n");
   SimpleThread_Synchronize sync(classMutex);
	dbsetmaxprocs(100);
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
   _handles = (SybaseHandle**)malloc(_numHandles * sizeof(SybaseHandle*));
   for (unsigned int i=0; i<_numHandles; i++)
   {
      _handles[i] = new SybaseHandle();
      _handles[i]->status = BaseHandle::DISCONNECTED;
   }
      
   // Connect the required specified minimum connections. The rest will be on demand.
   for (int j=0; j<minConnections; j++)
      _sybaseConnect(j,optParam1,optParam2);

   isConnected = true;
   
}  // SybaseConnection::connect
                           


// -----------------------------------------------------------------------------
// SybaseConnection::disconnect
// -----------------------------------------------------------------------------
void
SybaseConnection::disconnect(
      time_t timeout)
{
   // Synchronize the function.
   SimpleThread_Synchronize sync(classMutex);

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
         _sybaseDisconnect(i);
      }
   }
           
   isConnected = false;
} // SybaseConnection::disconnect



// -----------------------------------------------------------------------------
// SybaseConnection::requestQueryConnection
// -----------------------------------------------------------------------------


void*
SybaseConnection::requestQueryConnection()
{
   // This function must act as a fifo stack. The first thread in must get the first 
   //   available connection.
   // Synchronize the function.
   g_dbclog.print("requestQueryConnection()\n");
   SimpleThread_Synchronize sync(classMutex);
   
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
      for ( i=0; i<_numHandles; i++)
      {
         // We have an available handle thats already connected.
         if (_handles[i]->status == BaseHandle::CONNECTED)
         {
            _handles[i]->queryObject = new SybaseQuery(this, i);
            _handles[i]->status = BaseHandle::CONNECTED_USED;

            isRequestQueryConnectionOccuring = false;
			g_dbclog.print("requestQueryConnnection, return 0x%08x\n", _handles[i]->dbproc );
            return _handles[i]->queryObject;
         }
         
         // We have an available handle that needs to be connected.
         if (_handles[i]->status == BaseHandle::DISCONNECTED)
         {
            _sybaseConnect(i,BaseConnection::optParam1,BaseConnection::optParam2);
            _handles[i]->queryObject = new SybaseQuery(this, i);
            _handles[i]->status = BaseHandle::CONNECTED_USED;

            isRequestQueryConnectionOccuring = false;
			g_dbclog.print("requestQueryConnection, return 0x%08x\n", _handles[i]->dbproc );
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

} // SybaseConnection::requestQueryConnection



// -----------------------------------------------------------------------------
// SybaseConnection::releaseQueryConnection
// -----------------------------------------------------------------------------
void
SybaseConnection::releaseQueryConnection(
   void* queryObject)
{
   // Don't synchronize as we may need to release a connection to allow
   //  requestQueryConnection to get an available one
   g_dbclog.print("releaseQueryConnection\n");
   // Find the handle that has the query connection instance
   unsigned int i;
   for ( i=0; i<_numHandles; i++)
   {
      if (_handles[i]->queryObject == queryObject)
      {
		  delete (SybaseQuery*)queryObject;
         _handles[i]->queryObject = NULL;
         _handles[i]->status = BaseHandle::CONNECTED;
         
         // Check if we need to release the connection.
         // We release the connection if the connection is outside of the minimum
         // connections and there is no current request for a connection waiting.
         if (i >= (unsigned int) minConnections && !isRequestQueryConnectionOccuring)
         {
            _sybaseDisconnect(i);            
         }
        
         break;
      }
   }
} // SybaseConnection::releaseQueryConnection
#endif