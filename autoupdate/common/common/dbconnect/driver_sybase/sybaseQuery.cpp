#ifdef _DB_SYBASE_
#include "stdafx.h"

#include "dbconnect/driver_sybase/sybaseQuery.h"

#include <time.h>
#include <string.h>

#include "dbconnect/driver_sybase/sybaseConnection.h"
#include "dbconnect/base/dbconnectTypes.h"

// -----------------------------------------------------------------------------
// PRIVATE:
// -----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// SybaseQuery::_sybaseGetFieldsInformation
//------------------------------------------------------------------------------
void 
SybaseQuery::_sybaseGetFieldsInformation()
{
	// Clear any previous field information just incase.
	_freeCollection(FIELD_INFORMATION);

	// Internal method used to build the field information.


	// Allocate the information pointers
	_numFieldInformation = _fieldCount;
	_fieldInformation = (SybaseFieldDescription**)malloc(_numFieldInformation * sizeof(SybaseFieldDescription*));

	DBPROCESS* dbproc = _parentConnection->_handles[_index]->dbproc;
	DBCOLINFO* p = dbproc->dbcols;
	if (p == NULL)
	{
		string err = "_sybaseGetFieldsInformation(): ";      
		err += "DBCOLINFO is null";

		throw ErrorQuerying(err);
	}

	// Assign the data to the corresponding fields.
	for (unsigned int i=0; i<_numFieldInformation; i++)
	{
		// Get the field information
		if (p)
		{
			// Field Properties. 
			string fName = p->colname;

			DBINT colSize = 0;
			if (p->coltype == SYBVARCHAR || p->coltype == SYBCHAR ||
				p->coltype == SYBLONGCHAR || p->coltype == SYBTEXT)
			{
				colSize = p->collen + 1; // +1 for NULL terminating char
			}
			else
			{
				colSize = p->collen;
			}

			// Work out the field type.
			FieldType _type = _sybaseResolveFieldType(p->coltype);
			long int  _scale = p->coltypeinfo.scale;
			long int  _precision = p->coltypeinfo.precision;
			bool     _isIncrement = false;    // Not supported. Will always return false. Future versions of CLI will support this.
			bool     _isPriKey    = false;    // Not supported 
			bool     _isUnique    = false;    // Not supported 
			bool     _isNotNull   = (p->colhasnull ==0) ;

			// Add the field properties to the vector.
			_fieldInformation[i] = new SybaseFieldDescription(fName, i, _type, _isIncrement, _isPriKey, _isUnique, 
				_isNotNull, _precision, _scale, p->coltype, colSize);

			p = p->colnext;
		}
		else
		{
			break;
		}
	}
}  // SybaseQuery::_sybaseGetFieldsInformation


//------------------------------------------------------------------------------
// SybaseQuery::_sybaseGetResultSetRow
//------------------------------------------------------------------------------
void
SybaseQuery::_sybaseGetResultSetRow()
{
	// NOTE: This function always assumes fetchNext has been called before the start   
	// Clear any previous field information just incase.
	_freeCollection(FIELD_VALUES);
	//g_dbclog.print("[%04d] _sybaseGetResultSetRow()\n", this->_index );

	// Allocate the value pointers
	_numRecordValues = _fieldCount;
	_recordValues = (SybaseValue**)malloc(_numRecordValues * sizeof(SybaseValue*));

	// Get the data :)  
	DBPROCESS* dbproc = _parentConnection->_handles[_index]->dbproc;

	for (unsigned int i=0; i<_fieldCount; i++)
	{
		// Allocate the field
		_recordValues[i] = new SybaseValue(_fieldInformation[i]->name().c_str());

		// Get the field value according to the field type
		void *buffer = NULL;
		long  bufferSize = 0;
		void *bufferOffset = NULL;

		JDate dVal(0.0);   // Epoch
		int datalen = dbdatlen(dbproc,i+1);
		switch(_fieldInformation[i]->sybaseFieldType())
		{         
			// Obtain the data as an 64bit integer value            
		case SYBBIT:
		case SYBINT1:
		case SYBINT2:
		case SYBINT4:
		case SYBINTN:
			{
				DBINT integer = 0;
				if(datalen)
					integer = *((DBINT *)dbdata(dbproc, i+1));
				_recordValues[i]->setLong(integer);
			}

			break;       


			// Obtain the data as an double floating point value            
		case SYBDECIMAL:
		case SYBFLT8:
		case SYBREAL:
		case SYBMONEY4:
		case SYBMONEY:

		case SYBFLTN:
		case SYBMONEYN:
		case SYBNUMERIC:
			{
				DBFLT8 float8 = 0;
				if(datalen)
					float8 =*((DBFLT8 *)dbdata(dbproc, i+1));
				_recordValues[i]->setFloat(float8);
			}
			break;                 


			// Obtain the data as character data (Allocate enough for field size)
		case SYBVARCHAR:
		case SYBCHAR:
		case SYBLONGCHAR:
		case SYBTEXT:
			{
				bufferSize = _fieldInformation[i]->sybaseFieldSize();  
				buffer = malloc(bufferSize);
				memset(buffer, 0, bufferSize);

				memcpy(buffer, dbdata(dbproc, i+1), dbdatlen(dbproc, i+1));
				_recordValues[i]->setString((char*)buffer);
			}
			break;                


			// Obtain the data as timestamp value
		case SYBDATETIME4:
		case SYBDATETIME:
		case SYBDATETIMN:
			{
				BYTE* tmp = dbdata(dbproc, i+1);
				if(tmp)
				{
					DBDATETIME datetime8 = *((DBDATETIME *)tmp);
					DBDATEREC dstValue;
					dbdatecrack(dbproc, &dstValue, &datetime8);
					dVal.setDate((dstValue.dateyear),
						(dstValue.datemonth + 1),
						dstValue.datedmonth,
						dstValue.datehour,
						dstValue.dateminute,
						dstValue.datesecond);
				}
				else
				{
					dVal.setDate(1970, 1, 1, 0,0,0);
				}
				_recordValues[i]->setDateTime(dVal);
			}
			break;


			// Obtain the data as binary value (Obtain the data block by block)
		case SYBBINARY:
		case SYBVARBINARY:
		case SYBLONGBINARY:
			{
				bufferSize = _fieldInformation[i]->sybaseFieldSize();  
				buffer = malloc(bufferSize);
				memset(buffer, 0, bufferSize);

				memcpy(buffer, dbdata(dbproc, i+1), dbdatlen(dbproc, i+1));
				_recordValues[i]->setBinary(buffer, bufferSize);
			}
			break;       


		default:
			char buf[20] = {0};
			snprintf(buf, sizeof(buf)-1, "%d", _fieldInformation[i]->sybaseFieldType());

			string err = "_sybaseGetResultSetRow(): Unknown SYBASE SQL field type ";         
			err += buf;
			throw ResultSetError(err);
			break;
		}

		// Free the buffer
		if (buffer)
		{
			free(buffer);
			buffer = NULL;         
		}
	}	
}  // SybaseQuery::_sybaseGetResultSetRow


//------------------------------------------------------------------------------
// SybaseQuery::_sybaseResolveFieldType
//------------------------------------------------------------------------------
FieldType
SybaseQuery::_sybaseResolveFieldType(
									 BYTE type)
{
	FieldType res;

	switch(type)
	{
	case SYBBIT:
	case SYBINT1:
	case SYBINT2:
	case SYBINT4:
	case SYBINTN:
		res = FT_LONG;
		break;

	case SYBVARCHAR:
	case SYBCHAR:
	case SYBLONGCHAR:
	case SYBTEXT:
		res = FT_STRING;
		break;

	case SYBBINARY:
	case SYBVARBINARY:
	case SYBLONGBINARY:
		res = FT_STRING;
		break;

	case SYBDECIMAL:
	case SYBFLT8:
	case SYBFLTN:
	case SYBREAL:
	case SYBMONEY4:
	case SYBMONEY:
	case SYBMONEYN:
	case SYBNUMERIC:
		res = FT_DOUBLE;
		break;


	case SYBDATETIME4:
	case SYBDATETIME:
	case SYBDATETIMN:
		res = FT_DATETIME;
		break;

	default:
		res = FT_UNKNOWN;
		break;      
	}

	return res;
}  // SybaseQuery::_sybaseResolveFieldType


//------------------------------------------------------------------------------
// SybaseQuery::_sybaseParseBindParameters
//------------------------------------------------------------------------------
string
SybaseQuery::_sybaseParseBindParameters(
										const string& originalSqlStatement)

{
	// Try and substitute the parameters checking to make sure they exist.
	string res = originalSqlStatement; 
	size_t pos;

	for (unsigned int i =0; i<_numParameters; i++)
	{
		string paramName = ":" + _parameters[i]->name();
		if ((pos = res.find(paramName)) == string::npos)
		{
			// We have a parameter that is not present in our sql statement
			string err = "_sybaseParseBindParameters(): The specified bind parameter, ";
			err += paramName;
			err += ", is not present in the SQL statement: ";
			err += originalSqlStatement;
			throw BindParameterNotPresent(err);
		}

		// Substitute the parameter with the correct value according to the parameter type.
		res.replace(pos, paramName.length(), _parameters[i]->paramToSybaseString());
	}

	// Make sure there are no parameters left
	if (((pos = res.find(" :")) != string::npos) || 
		((pos = res.find("(:")) != string::npos) || 
		((pos = res.find(",:")) != string::npos) )
	{
		// We have a parameter that has not been set.
		pos += 1;
		size_t pos2 = res.find(" ", pos);
		size_t pos3 = res.find(")", pos);
		size_t pos4 = res.find(",", pos);
		if (pos2 == string::npos) pos2 = 65535;
		if (pos3 == string::npos) pos3 = 65535;
		if (pos4 == string::npos) pos4 = 65535;         

		size_t endPos;      
		if (pos2 < pos3 && pos2 < pos4)
			endPos = pos2;
		if (pos3 < pos2 && pos3 < pos4)
			endPos = pos3;
		if (pos4 < pos2 && pos4 < pos3)
			endPos = pos4;     

		string unknownParam = res.substr(pos, endPos-pos);

		string err = "_sybaseParseBindParameters(): The bind parameter, ";
		err += unknownParam;
		err += ", in the SQL statement: ";
		err += originalSqlStatement;
		err += " has not been set.";
		throw BindParameterNotSet(err);
	}

	return res;
}  // SybaseQuery::_sybaseParseBindParameters


//------------------------------------------------------------------------------
// SybaseQuery::_freeCollection
//------------------------------------------------------------------------------
void 
SybaseQuery::_freeCollection(
							 CollectionType type)
{
	unsigned int i;
	switch (type)
	{
	case FIELD_INFORMATION:
		if (_fieldInformation)
		{
			for ( i=0; i<_numFieldInformation; i++)
			{
				if (_fieldInformation[i])
				{
					delete _fieldInformation[i];
					_fieldInformation[i] = NULL;         
				}
			}
			free(_fieldInformation);
			_fieldInformation = NULL;
			_numFieldInformation = 0;
			_fieldCount = 0;            
		}

		break;


	case FIELD_VALUES:
		if (_recordValues)
		{
			for ( i=0; i<_numRecordValues; i++)
			{
				if (_recordValues[i])
				{
					delete _recordValues[i];
					_recordValues[i] = NULL;         
				}
			}
			free(_recordValues);
			_recordValues = NULL;
			_numRecordValues = 0;
		}

		break;

	case BIND_PARAMETERS:
		if (_parameters)
		{
			for ( i=0; i<_numParameters; i++)
			{
				if (_parameters[i])
				{
					delete _parameters[i];
					_parameters[i] = NULL;         
				}
			}
			free(_parameters);
			_parameters = NULL;
			_numParameters = 0;
		}

		break;      
	}
}  // SybaseQuery::_freeCollection


// -----------------------------------------------------------------------------
// PUBLIC:
// -----------------------------------------------------------------------------


//------------------------------------------------------------------------------
// SybaseQuery::SybaseQuery
//------------------------------------------------------------------------------
SybaseQuery::SybaseQuery(
						 SybaseConnection* parentConnection,
						 int              index)
						 : 
_parentConnection(parentConnection),
_index(index),
_isTransaction(false),
_sybaseNumRows(0),
_sybaseCurrentRow(0),
_recordValues(NULL),
_numRecordValues(0),
_fieldInformation(NULL),
_numFieldInformation(0),
_parameters(NULL),
_numParameters(0)
{

}  // SybaseQuery::SybaseQuery



//------------------------------------------------------------------------------
// SybaseQuery::~SybaseQuery
//------------------------------------------------------------------------------
SybaseQuery::~SybaseQuery()
{   
	// If there is still a transaction, rollback
	if (_isTransaction)
		rollback();

	//Make sure any stored query results are freed.
	//Todo

	// Free any parameters
	_freeCollection(BIND_PARAMETERS);

	// Free the current field values
	_freeCollection(FIELD_VALUES);

	// Clear any previous field information.
	_freeCollection(FIELD_INFORMATION);

}  // SybaseQuery::~SybaseQuery


//------------------------------------------------------------------------------
// SybaseQuery::clearBindParams
//------------------------------------------------------------------------------
void 
SybaseQuery::clearBindParams()
{
	// Free all the bind parameters.
	_freeCollection(BIND_PARAMETERS);   
}  // SybaseQuery::clearBindParams


//------------------------------------------------------------------------------
// SybaseQuery::command
//------------------------------------------------------------------------------
void
SybaseQuery::command(
					 const string& sqlStatement)
{
	// Clear any bind parameters as we now have a new query
	clearBindParams();

	// Call the base query to store a copy of the query.
	BaseQuery::command(sqlStatement);   
} // SybaseQuery::command


//------------------------------------------------------------------------------
// SybaseQuery::bindParam
//------------------------------------------------------------------------------
BaseValue*
SybaseQuery::bindParam(
					   const string& paramName)
{
	// Make sure the name has not already been added. If it has, return the instance to it
	for (unsigned int i=0; i<_numParameters; i++)
		if (strcasecmp(_parameters[i]->bindName.c_str(), paramName.c_str()) == 0)
			return _parameters[i];

	// Make sure the parameter is present in the query
	if (!_isBindParameterPresent(paramName))
	{
		string err = "bindParam(): The specified bind parameter, ";
		err += paramName;
		err += ", is not present in the SQL statement: ";
		err += _sqlStatement;      
		throw BindParameterNotPresent(err);
	}

	// Add the value to the parameters array.
	_numParameters++;
	_parameters = (SybaseBindParam**)realloc((void*)_parameters, _numParameters * sizeof(SybaseBindParam*));
	_parameters[_numParameters-1] = new SybaseBindParam(paramName);        
	return _parameters[_numParameters-1];   
}  // SybaseQuery::bindParam


//------------------------------------------------------------------------------
// SybaseQuery::execute
//------------------------------------------------------------------------------
void
SybaseQuery::execute()
{
	DBPROCESS*	dbproc = _parentConnection->_handles[_index]->dbproc;
	g_dbclog.print("[%04d] **** Execute SQL with proc=%X ****\n", this->_index, dbproc);
	//Make sure any stored query results are freed.
	RETCODE dbret = NO_MORE_RESULTS;
	if(dbproc)
	{
		dbclrbuf(dbproc, 10000);
		dbret = dbresults(dbproc);
	}
	// if reconnect FAILED, dbproc will still in NULL value,
	// so if dbproc is NULL, reconnect it!
	if(dbproc == NULL || (dbret == FAIL && dbdead(dbproc) == TRUE) )
	{
		// DBPROCESS is dead or not enabled
		// 
		g_dbclog.print("[%04d] execute, reconnect\n", this->_index );
		_parentConnection->_sybaseReconnect(_index);
		// reset the DBPROCESS
		dbproc = _parentConnection->_handles[_index]->dbproc;
		g_dbclog.print("[%04d] reconnect,new proc=%X\n",
			_index, dbproc);
	}

	while (dbret != NO_MORE_RESULTS && dbret != FAIL)
	{
		dbcanquery(dbproc);
		dbret = dbresults(dbproc);
		continue;
	}
	
	// Number of rows in the result set is now 0
	_sybaseNumRows = 0;
	_effectCount = 0;
	_sybaseCurrentRow = 0;

	// Clear any previous field information.
	_freeCollection(FIELD_INFORMATION);

	// Free the current field values
	_freeCollection(FIELD_VALUES);

	// IIF there is transaction support enabled then
	//   always make sure there is a transaction that the query is occuring in
	//Todo

	// Resolve the bind parameters
	string resolvedSqlStatement = _sqlStatement; //xzq rem _sybaseParseBindParameters(_sqlStatement);

	// Ping the connection to make sure it is still valid.
	
	g_dbclog.print("[%04d] ping start\n",this->_index);
	_parentConnection->_sybasePing(_index);
	// After reconnect dpproc maybe changed 
	dbproc = _parentConnection->_handles[_index]->dbproc;
	dbsetopt(dbproc, DBBUFFER, "10000", -1);

	// Execute the query

	g_dbclog.print("[%04d] Execute SQL Begin\n",this->_index);
	_parentConnection->_sybaseQuery(_index, resolvedSqlStatement);

	// Determine if we have any data or not and get the field structures if we do.
	// Using store result. This should change to be configurable from the config file.

	g_dbclog.print("[%04d]  Execute SQL End 0x%X\n",this->_index, dbproc); 
	RETCODE       return_code;
	while( (return_code = dbresults(dbproc)) != NO_MORE_RESULTS )
	{
		g_dbclog.print("[%04d] dbresult has results\n",
			this->_index);
		if(FAIL == return_code ||  DBROWS(dbproc) == SUCCEED)
			break;
	}
	if (return_code == NO_MORE_RESULTS || 
		DBROWS(dbproc) != SUCCEED)
	{
		g_dbclog.print("[%04d] no result set return  \n",this->_index);
		_fieldCount = 0;
		_eof = true;
		_effectCount = DBCOUNT(dbproc);
	}
	else
	{
		g_dbclog.print("[%04d] result set return\n",this->_index);
		// We have data
		_fieldCount = dbnumcols(dbproc);
		if (_fieldCount <= 0)
		{
			_eof = true;
			return;
		}
		// Get the field information for this query.
		_sybaseGetFieldsInformation();
		g_dbclog.print("[%04d] Get Fields OK ,_fieldCount =%d \n",this->_index,_fieldCount);
		
		// Store the number of rows in the result set.
		RETCODE row_code;
		while ((row_code = dbnextrow(dbproc)) != NO_MORE_ROWS)
		{
			/* If DB-Library's row buffer is full, throw
			* away the oldest row, to allow the newest
			* row to be read in.
			*/
			if (row_code == BUF_FULL)
			{
				// next command will be blocked if we didn't disconnect 
				_parentConnection->_sybaseDisconnect(this->_index);
				string err = "execute(): buffer is full";
				g_dbclog.print( "[%04d] buffer full,_sybaseNumRows =%d\n",this->_index,_sybaseNumRows);
				throw ResultSetError(err);
			}
			else if(row_code == FAIL )
			{
				string err = "execute(): fail";
				throw ResultSetError(err);
			}
			_sybaseNumRows++;
		}
		_effectCount = _sybaseNumRows;

		g_dbclog.print("[%04d]   return num=%d \n",this->_index,_sybaseNumRows);
		if(_sybaseNumRows < 0)
			_sybaseNumRows = 0;

		if (0 == _sybaseNumRows)
			_eof = true;     // No data in result set
		else
			_eof = false;

	}  
}  // SybaseQuery::execute


//------------------------------------------------------------------------------
// SybaseQuery::next
//------------------------------------------------------------------------------
void
SybaseQuery::fetchNext()
{        
	// Do nothing if the we have reached the end of file.
	if (_eof)
		return;

	DBPROCESS*	dbproc = _parentConnection->_handles[_index]->dbproc;

	_sybaseCurrentRow++;
	if (dbgetrow(dbproc, _sybaseCurrentRow) == NO_MORE_ROWS)
	{
		_eof = true;
		return;
	}

	// Get the data.  
	_sybaseGetResultSetRow();


	// Check if we are at the end of the result set.
	if (_sybaseCurrentRow >= _sybaseNumRows)
		_eof = true;
}  // SybaseQuery::next



//------------------------------------------------------------------------------
// SybaseQuery::transBegin
//------------------------------------------------------------------------------
void
SybaseQuery::transBegin()
{
	// Only allow transactions if they have been enabled
	g_dbclog.print("[%04d] transBegin()\n", this->_index );
	if (1==0)
	{
		// Transaction support enabled
		// Make sure a transaction is not already active
		if (_isTransaction)
			throw TransactionError("transBegin(): "
			"A transaction is already active. Commit or rollback the transaction before creating a new transaction.");

		// Ping the connection to make sure it is still valid.
		_parentConnection->_sybasePing(_index);

		string query = "BEGIN";
		dbcmd(_parentConnection->_handles[_index]->dbproc, (char *)query.c_str());
		if (FAIL == dbsqlexec(_parentConnection->_handles[_index]->dbproc))
		{
			// Something went wrong while trying to execute the sql statement, so throw an exception.
			string err = "transBegin(): Unable to start transaction";
			throw TransactionError(err);
		}      

		_isTransaction = true;      
	}
	else
	{
		// Transaction support not enabled
		throw NotImplemented("transBegin(): Transaction support is not enabled. Please enable in .ini file with 'sybase_opt_transaction = yes' entry.");
	}
}  // SybaseQuery::transBegin




//------------------------------------------------------------------------------
// SybaseQuery::commit
//------------------------------------------------------------------------------
void
SybaseQuery::commit()
{
	// Only allow transactions if they have been enabled
	g_dbclog.print("[%04d] commit()\n", this->_index );
	if (1==0)
	{
		// Transaction support enabled
		// Make sure a transaction is active
		if (!_isTransaction)
			throw TransactionError("commit(): "
			"A transaction is not active. Create a transaction before calling commit.");

		// Ping the connection to make sure it is still valid.
		_parentConnection->_sybasePing(_index);

		string query = "COMMIT";
		dbcmd(_parentConnection->_handles[_index]->dbproc, (char *)query.c_str());
		if (FAIL == dbsqlexec(_parentConnection->_handles[_index]->dbproc))
		{
			// Something went wrong while trying to execute the sql statement, so throw an exception.
			string err = "commit(): Unable to commit the transaction";
			throw TransactionError(err);
		}      

		_isTransaction = false;
	}
	else
	{
		// Transaction support not enabled
		throw NotImplemented("commit(): Transaction support is not enabled. Please enable in .ini file with 'sybase_opt_transaction = yes' entry.");
	}
}  // SybaseQuery::commit



//------------------------------------------------------------------------------
// SybaseQuery::rollback
//------------------------------------------------------------------------------
void
SybaseQuery::rollback()
{
	// Only allow transactions if they have been enabled
	g_dbclog.print("[%04d] rollback()\n", this->_index );
	if (1==0)
	{
		// Transaction support enabled
		// Make sure a transaction is active
		if (!_isTransaction)
			throw TransactionError("rollback(): "
			"A transaction is not active. Create a transaction before calling commit.");

		// Ping the connection to make sure it is still valid.
		_parentConnection->_sybasePing(_index);

		string query = "ROLLBACK";
		dbcmd(_parentConnection->_handles[_index]->dbproc, (char *)query.c_str());
		if (FAIL == dbsqlexec(_parentConnection->_handles[_index]->dbproc))
		{
			// Something went wrong while trying to execute the sql statement, so throw an exception.
			string err = "commit(): Unable to rollback the transaction";
			throw TransactionError(err);
		}

		_isTransaction = false;      
	}
	else
	{
		// Transaction support not enabled
		throw NotImplemented("rollback(): Transaction support is not enabled. Please enable in .ini file with 'sybase_opt_transaction = yes' entry.");
	}   
}  // SybaseQuery::rollback



//------------------------------------------------------------------------------
// SybaseQuery::getFieldInfoByColumn
//------------------------------------------------------------------------------
BaseFieldDescription*
SybaseQuery::getFieldInfoByColumn(
								  int index)
{
	// Make sure the index is in range.
	if (index < 0 || index >= (int) _numFieldInformation)
		throw IndexOutOfRange("getFieldInfoByColumn(): The field index is out of range for the current result set");

	return _fieldInformation[index];
}  // SybaseQuery::getFieldInfoByColumn


//------------------------------------------------------------------------------
// SybaseQuery::getFieldInfoByName
//------------------------------------------------------------------------------
BaseFieldDescription*
SybaseQuery::getFieldInfoByName(
								const string& fieldName)
{
	// Try and find the field name
	for (unsigned int i=0; i<_numFieldInformation; i++)
		if (strcasecmp(_fieldInformation[i]->name().c_str(), fieldName.c_str()) == 0)
			return _fieldInformation[i];

	throw NameNotFound("getFieldInfoByName(): The field name was not found for the current result set.");
}  // SybaseQuery::getFieldInfoByName


//------------------------------------------------------------------------------
// SybaseQuery::getFieldByColumn
//------------------------------------------------------------------------------
BaseValue*
SybaseQuery::getFieldByColumn(
							  int index)
{
	// Make sure the index is in range.
	if (index < 0 || index >= (int) _numRecordValues)
	{
		throw IndexOutOfRange("getFieldByColumn(): The field index is out of range for the current result set");
	}

	return _recordValues[index];
}  // SybaseQuery::getFieldByColumn


//------------------------------------------------------------------------------
// SybaseQuery::getFieldByName
//------------------------------------------------------------------------------
BaseValue*
SybaseQuery::getFieldByName(
							const string& fieldName)
{
	// Try and find the field name
	for (unsigned int i=0; i<_numRecordValues; i++)
		if (strcasecmp(_recordValues[i]->name().c_str(), fieldName.c_str()) == 0)
			return _recordValues[i];

	throw NameNotFound("getFieldByName(): The field name was not found for the current result set.");   
}  // SybaseQuery::getFieldByName
#endif