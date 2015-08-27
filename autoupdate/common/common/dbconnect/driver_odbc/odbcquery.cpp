/*
* ODBCQuery object defines the needed query functions for the dbConnect ODBC driver
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

#include "odbcquery.h"

// -----------------------------------------------------------------------------
// PRIVATE:
// -----------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ODBCQuery::_odbcGetFieldsInformation
//------------------------------------------------------------------------------
void 
ODBCQuery::_odbcGetFieldsInformation()
{
	// Clear any previouse field information just incase.
	_freeCollection(FIELD_INFORMATION);

	// Internal method used to build the field information.

	// Allocate the information pointers
	_numFieldInformation = _fieldCount;
	_fieldInformation = (ODBCFieldDescription**)malloc(_numFieldInformation * sizeof(ODBCFieldDescription*));

	// Assign the data to the corresponding fields.
	for (unsigned int i=0; i<_numFieldInformation; i++)
	{
		// Get the field information
		SQLRETURN cliRC;
		SQLCHAR colName[256];
		SQLSMALLINT colNameLen;
		SQLSMALLINT colType;
		SQLUINTEGER colSize;
		SQLSMALLINT colScale;
		SQLSMALLINT colPrecision	= 0;
		SQLSMALLINT colNullable;
		SQLSMALLINT colAutoUnique	= SQL_FALSE;

		// Obtain the required information
		cliRC = SQLDescribeCol(_hstmt, (SQLSMALLINT)i+1,
			colName, sizeof(colName)-1, &colNameLen,
			&colType, &colSize, &colScale, &colNullable);

		/*
		if (cliRC == SQL_SUCCESS)
		cliRC = SQLColAttribute(_hstmt, (SQLSMALLINT)i+1, SQL_DESC_PRECISION, NULL, 0, NULL, &colPrecision);

		if (cliRC == SQL_SUCCESS)
		cliRC = SQLColAttribute(_hstmt, (SQLSMALLINT)i+1, SQL_DESC_AUTO_UNIQUE_VALUE, NULL, 0, NULL, &colAutoUnique);
		*/
		// Finally bail on an error      
		if (cliRC != SQL_SUCCESS && cliRC != SQL_SUCCESS_WITH_INFO )
		{
			SQLCHAR			sqlState[16]	= { 0 };
			SQLINTEGER		sqlCode			= 0;
			SQLCHAR			message[SQL_MAX_MESSAGE_LENGTH]	= { 0 };
			SQLSMALLINT		length			= 0;

			SQLGetDiagRec(SQL_HANDLE_STMT, _hstmt, 1, sqlState, &sqlCode, message, SQL_MAX_MESSAGE_LENGTH - 1, &length);

			string err = "_odbcGetFieldsInformation(): ";      
			err += (const char*)message;
			
			throw ErrorQuerying(err);
		}

		// Work out the field type.
		FieldType _type = _odbcResolveFieldType(colType);

		// Field Properties.      
		string fName = (const char*)colName;

		long int  _scale = colScale;

		long int  _precision = 0;
		if (colType == SQL_DECIMAL || colType == SQL_NUMERIC || colType == SQL_DOUBLE ||
			colType == SQL_FLOAT || colType == SQL_INTEGER || colType == SQL_REAL || colType == SQL_SMALLINT)
			_precision = colPrecision;

		bool     _isIncrement = (colAutoUnique == SQL_FALSE ? false : true);  // Will always return false. Future versions of CLI will support this.
		bool     _isPriKey    = false;    // Not supported 
		bool     _isUnique    = false;    // Not supported 
		bool     _isNotNull   = (colNullable == SQL_NULLABLE ? false : true);

		// Add the field properties to the vector.
		_fieldInformation[i] = new ODBCFieldDescription(fName, i, _type, _isIncrement, _isPriKey, _isUnique, 
			_isNotNull, _precision, _scale, colType, colSize);

	}
}  // ODBCQuery::_odbcGetFieldsInformation


//------------------------------------------------------------------------------
// ODBCQuery::_odbcGetResultSetRow
//------------------------------------------------------------------------------
void
ODBCQuery::_odbcGetResultSetRow()
{
	// NOTE: This function always assumes fetchNext has been called before the start   
	// Clear any previouse field information just incase.
	_freeCollection(FIELD_VALUES);


	// Allocate the value pointers
	_numRecordValues = _fieldCount;
	_recordValues = (ODBCValue**)malloc(_numRecordValues * sizeof(ODBCValue*));


	// Get the data :)  
	SQLRETURN cliRC; 
	for (unsigned int i=0; i<_fieldCount; i++)
	{
		// Allocate the field
		_recordValues[i] = new ODBCValue(_fieldInformation[i]->name().c_str());

		// Get the field value according to the field type
		SQLINTEGER strlen_indptr, tmp_strlen_indptr;
		void *buffer = NULL;
		long  bufferSize = 0;
		void *bufferOffset = NULL;

		JDate dVal(0.0);   // Epoch

		switch(_fieldInformation[i]->odbcFieldType())
		{         
			// Obtain the data as an 64bit integer value            
		case SQL_BIGINT:
		case SQL_TINYINT:
		case SQL_INTEGER:
		case SQL_SMALLINT:
			//bufferSize = sizeof(DBLONG);
			bufferSize	= 8;
			buffer = malloc(bufferSize);

			cliRC = SQLGetData(_hstmt, i+1, SQL_C_SBIGINT, buffer, bufferSize, &strlen_indptr); 
			if (cliRC == SQL_SUCCESS)
			{
				if (strlen_indptr == SQL_NULL_DATA)
					_recordValues[i]->setNULL();
				else               
					_recordValues[i]->setLong(*(DBLONG*)buffer);
			}
			break;       


			// Obtain the data as an double floating point value            
		case SQL_DECIMAL:
		case SQL_DOUBLE:
		case SQL_FLOAT:
		case SQL_NUMERIC:
		case SQL_REAL:
			bufferSize = sizeof(double) * 2;
			buffer = malloc(bufferSize);

			cliRC = SQLGetData(_hstmt, i+1, SQL_C_DOUBLE, buffer, bufferSize, &strlen_indptr); 
			if (cliRC == SQL_SUCCESS)
			{
				if (strlen_indptr == SQL_NULL_DATA)
					_recordValues[i]->setNULL();
				else               
					_recordValues[i]->setFloat(*(double*)buffer);
			}
			break;       


			// Obtain the data as character data (Allocate enough for field size)
		case SQL_CHAR:
			//case SQL_CLOB:
		case SQL_VARCHAR:
		case SQL_UNICODE:
		case SQL_UNICODE_VARCHAR:
		case SQL_UNICODE_LONGVARCHAR:
			//case SQL_GRAPHIC:
			//case SQL_VARGRAPHIC:
			bufferSize = _fieldInformation[i]->odbcFieldSize() + 64;  // +1 for NULL terminating char
			buffer = malloc(bufferSize);
			memset(buffer, 0, bufferSize);

			cliRC = SQLGetData(_hstmt, i+1, SQL_C_CHAR, buffer, bufferSize, &strlen_indptr); 
			if (cliRC == SQL_SUCCESS)
			{
				if (strlen_indptr == SQL_NULL_DATA)
					_recordValues[i]->setNULL();
				else               
					_recordValues[i]->setString((char*)buffer);
			}
			break;                


			// Obtain the data as timstamp value
		case SQL_TIME:
		case SQL_TIMESTAMP:
		case SQL_TYPE_DATE:
		case SQL_TYPE_TIME:
		case SQL_TYPE_TIMESTAMP:
			bufferSize = sizeof(TIMESTAMP_STRUCT);
			buffer = malloc(bufferSize);

			cliRC = SQLGetData(_hstmt, i+1, SQL_TYPE_TIMESTAMP, buffer, bufferSize, &strlen_indptr); 
			if (cliRC == SQL_SUCCESS)
			{
				if (strlen_indptr == SQL_NULL_DATA)
					_recordValues[i]->setNULL();
				else               
				{
					// Build a JDate from the components
					dVal.setDate(((TIMESTAMP_STRUCT*)buffer)->year,
						((TIMESTAMP_STRUCT*)buffer)->month,
						((TIMESTAMP_STRUCT*)buffer)->day,
						((TIMESTAMP_STRUCT*)buffer)->hour,
						((TIMESTAMP_STRUCT*)buffer)->minute,
						((TIMESTAMP_STRUCT*)buffer)->second );
					_recordValues[i]->setDateTime(dVal);
				}
			}
			break;


			// Obtain the data as binary value (Obtain the data block by block)
			//case SQL_BLOB:
		case SQL_BINARY:
			bufferSize = FETCH_BLOCK_SIZE;
			buffer = malloc(bufferSize);

			cliRC = SQLGetData(_hstmt, i+1, SQL_C_BINARY, buffer, bufferSize, &strlen_indptr); 
			if (cliRC == SQL_SUCCESS || cliRC == SQL_SUCCESS_WITH_INFO)
			{
				if (strlen_indptr == SQL_NULL_DATA)
					_recordValues[i]->setNULL();
				else
				{
					// Get any left over data in a single query. Result is set to SQL_SUCCESS_WITH_INFO 
					//   if there is still data left to be read
					if (cliRC == SQL_SUCCESS_WITH_INFO)
					{
						// strlen_indptr now has the total size of the data in the column
						tmp_strlen_indptr = strlen_indptr;

						buffer = realloc(buffer, tmp_strlen_indptr);
						bufferOffset = (void*)((unsigned int)buffer + bufferSize);

						cliRC = SQLGetData(_hstmt, i+1, SQL_C_BINARY, bufferOffset, tmp_strlen_indptr - bufferSize, &strlen_indptr); 
						if (cliRC != SQL_SUCCESS)
							break;

						// Now finally set the buffer size
						bufferSize = tmp_strlen_indptr;
					}
					else
					{
						// The buffer size is the number of bytes read
						bufferSize = strlen_indptr;
					}
					if (cliRC == SQL_SUCCESS)
						_recordValues[i]->setBinary(buffer, bufferSize);
				}
			}
			break;       


		default:
			char buf[20];
			memset(buf, 0, sizeof(buf));
			snprintf(buf, sizeof(buf)-1, "%d", _fieldInformation[i]->odbcFieldType());

			string err = "_odbcGetResultSetRow(): Unknown ODBC SQL field type ";         
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

		// Make sure fetching the field data was ok.      
		if (cliRC != SQL_SUCCESS)
		{
			SQLCHAR			sqlState[16]	= {0};
			SQLINTEGER		sqlCode			= 0;
			SQLCHAR			message[SQL_MAX_MESSAGE_LENGTH] = { 0 };
			SQLSMALLINT		length			= 0;

			SQLGetDiagRec(SQL_HANDLE_STMT, _hstmt, 1, sqlState, &sqlCode, message, SQL_MAX_MESSAGE_LENGTH - 1, &length);

			string err = "_odbcGetResultSetRow(): Unable to obtain field data, ";
			err += (const char*)message;

			throw ResultSetError(err);
		}
	}


	// Pre-fetch the next row
	cliRC = SQLFetch(_hstmt);

	// Check if the previouse fetchNext fetched the last row   
	if (cliRC == SQL_NO_DATA_FOUND)
	{
		_eof = true;
	}
	else if (cliRC != SQL_SUCCESS)
	{
		SQLCHAR		sqlState[16]	= { 0 };
		SQLINTEGER	sqlCode			= 0;
		SQLCHAR		message[SQL_MAX_MESSAGE_LENGTH]	= { 0 };
		SQLSMALLINT length			= 0;

		SQLGetDiagRec(SQL_HANDLE_STMT, _hstmt, 1, sqlState, &sqlCode, message, SQL_MAX_MESSAGE_LENGTH - 1, &length);

		string err = "_odbcGetResultSetRow(): ";      
		err += (const char*)message;

		throw ResultSetError(err);
	}   
}  // ODBCQuery::_odbcGetResultSetRow


//------------------------------------------------------------------------------
// ODBCQuery::_odbcResolveFieldType
//------------------------------------------------------------------------------
FieldType
ODBCQuery::_odbcResolveFieldType(
								 SQLSMALLINT type)
{
	FieldType res;

	// LONG types are not supported as they should be replaced by LOB types according to the ODBC documentation

	// TODO: Research and support the following types
	//       SQL_BIT, SQL_DATALINK
	//       SQL_DB_CLOB, SQL_DB_LOCATOR, SQL_WCHAR
	// TODO: Support VARBINARY
	//case SQL_VARBINARY:
	//   res = FT_BLOB;
	//   break;      

	switch(type)
	{      
	case SQL_BIGINT:
		res = FT_LONG;
		break;
		/*
		case SQL_BLOB:
		res = FT_BLOB;
		break;
		*/
	case SQL_CHAR:
		res = FT_STRING;
		break;
	case SQL_TINYINT:
		res = FT_LONG;
		break;
	case SQL_BINARY:
		res = FT_BLOB;
		break;
		/*
		case SQL_CLOB:
		res = FT_STRING;
		break;
		*/
	case SQL_TYPE_DATE:
		res = FT_DATETIME;
		break;
	case SQL_DECIMAL:
		res = FT_DOUBLE;
		break;
	case SQL_DOUBLE:
		res = FT_DOUBLE;
		break;
	case SQL_FLOAT:
		res = FT_DOUBLE;
		break;
	case SQL_INTEGER:
		res = FT_LONG;
		break;
	case SQL_NUMERIC:
		res = FT_DOUBLE;
		break;
	case SQL_REAL:
		res = FT_DOUBLE;
		break;
	case SQL_SMALLINT:
		res = FT_SHORT;
		break;
	case SQL_TYPE_TIME:
		res = FT_DATETIME;
		break;
	case SQL_TYPE_TIMESTAMP:
		res = FT_DATETIME;
		break;
	case SQL_VARCHAR:
		res = FT_STRING;
		break;
		/*
		case SQL_GRAPHIC:
		res = FT_STRING;
		break;      
		case SQL_VARGRAPHIC:
		res = FT_STRING;
		break;      
		*/
	default:
		res = FT_UNKNOWN;
		break;      
	}

	return res;
}  // ODBCQuery::_odbcResolveFieldType


//------------------------------------------------------------------------------
// ODBCQuery::_odbcParseBindParameters
//------------------------------------------------------------------------------
string
ODBCQuery::_odbcParseBindParameters(
									const string& originalSqlStatement,
									vector<ODBCBindParam*> &bindParameters)
{
	// Replace the ":name" parameters with a ? from left to right building the bindParameter vector to point to the correct values
	// Try and substitute the parameters checking to make sure they exist.
	string res = originalSqlStatement;   

	//add by xzq for the sql has : such as time format
	//should modify according to sybase version
	return res;


	size_t pos, endPos;
	char paramName[1024];

	pos = res.find(":");
	while (pos != string::npos)
	{
		memset(paramName, 0, sizeof(paramName));

		// Get the position at the end of the parameter name and determine the parameter name
		endPos = pos +1;
		char tstChar = res.c_str()[endPos];
		while ((tstChar >= '0' && tstChar <= '9') ||    // 0 - 9
			(tstChar >= 'A' && tstChar <= 'Z') ||    // A - Z
			(tstChar >= 'a' && tstChar <= 'z')||     // a - z
			(tstChar == '_') )                       // _              
		{
			endPos++;         
			if (endPos > res.length())
				throw BindParameterError("_odbcParseBindParameters(): Unable to properly parse the bind parameters");

			tstChar = res.c_str()[endPos];
		}

		char *paramNameStart = (char*)((int)res.c_str() + pos+1);   // Jump the : in the param name

		strncpy(paramName, paramNameStart, endPos - pos-1);
		// Replace the parameter with a ?      
		res.replace(pos, endPos-pos, "?");

		// Make sure the parameter name exists and add the value to the parameter vector
		bool found = false;
		for (unsigned int i =0; i<_numParameters; i++)
		{
			if (strcasecmp(_parameters[i]->name().c_str(), paramName) == 0)
			{
				found = true;
				bindParameters.push_back(_parameters[i]);
				break;
			}
		}

		if (!found)
		{
			string err = "_odbcParseBindParameters(): The bind parameter, ";
			err += paramName;
			err += ", in the SQL statement: ";
			err += originalSqlStatement;
			err += " has not been set.";
			throw BindParameterNotSet(err);
		}

		// Find the next parameter and continue
		pos = res.find(":");
	}

	return res;      
}  // ODBCQuery::_odbcParseBindParameters


//------------------------------------------------------------------------------
// ODBCQuery::_odbcBindParameters
//------------------------------------------------------------------------------
void
ODBCQuery::_odbcBindParameters(
							   const string& resolvedSqlStatement,
							   vector<ODBCBindParam*> &bindParameters)
{
	SQLRETURN cliRC = SQL_SUCCESS;

	// Iterate through the pointer list and bind the parameter values according to type from left to right
	for (unsigned int i=0; i<bindParameters.size(); i++)
	{
		// Get the required parameter information      
		SQLSMALLINT  paramSQLType;
		SQLUINTEGER  columnSize;
		SQLSMALLINT  decDigits;
		SQLSMALLINT  nullableInfo;

		cliRC = SQLDescribeParam(_hstmt, i+1, &paramSQLType, &columnSize, &decDigits, &nullableInfo);
		if (cliRC != SQL_SUCCESS)
		{
			SQLCHAR		sqlState[16]	= { 0 };
			SQLINTEGER	sqlCode			= 0;
			SQLCHAR		message[SQL_MAX_MESSAGE_LENGTH]	= { 0 };
			SQLSMALLINT length			= 0;

			SQLGetDiagRec(SQL_HANDLE_STMT, _hstmt, 1, sqlState, &sqlCode, message, SQL_MAX_MESSAGE_LENGTH - 1, &length);

			string err = "_odbcBindParameters(): ";
			err += (const char*)message;              
			throw BindParameterError(err);
		}


		cliRC = SQLBindParameter(_hstmt, i+1, SQL_PARAM_INPUT, bindParameters[i]->getODBCCType(),
			paramSQLType, columnSize, decDigits, bindParameters[i]->getODBCDataPointer(),
			bindParameters[i]->getODBCDataPointerLength(), bindParameters[i]->getODBCStrLenIndPtr() );        
		if (cliRC != SQL_SUCCESS)
		{
			SQLCHAR			sqlState[16]	= { 0 };
			SQLINTEGER		sqlCode			= 0;
			SQLCHAR			message[SQL_MAX_MESSAGE_LENGTH]	= { 0 };
			SQLSMALLINT		length			= 0;

			SQLGetDiagRec(SQL_HANDLE_STMT, _hstmt, 1, sqlState, &sqlCode, message, SQL_MAX_MESSAGE_LENGTH - 1, &length);

			string err = "_odbcBindParameters(): ";
			err += (const char*)message;              
			throw BindParameterError(err);
		}
	}
}  // ODBCQuery::_odbcBindParameters


//------------------------------------------------------------------------------
// ODBCQuery::_freeCollection
//------------------------------------------------------------------------------
void 
ODBCQuery::_freeCollection(
						   CollectionType type)
{
	unsigned int i;
	switch (type)
	{
	case FIELD_INFORMATION:
		if (_fieldInformation)
		{
			for (i=0; i<_numFieldInformation; i++)
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
			for (i=0; i<_numRecordValues; i++)
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
			for (i=0; i<_numParameters; i++)
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
}  // ODBCQuery::_freeCollection


// -----------------------------------------------------------------------------
// PUBLIC:
// -----------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ODBCQuery::ODBCQuery
//------------------------------------------------------------------------------
ODBCQuery::ODBCQuery(
					 ODBCConnection* parentConnection,
					 int                   index)
					 : 
_parentConnection(parentConnection),
_index(index),
_isTransaction(false),
_hstmt(SQL_NULL_HSTMT),
_odbcCurrentRow(0),
_recordValues(NULL),
_numRecordValues(0),
_fieldInformation(NULL),
_numFieldInformation(0),
_parameters(NULL),
_numParameters(0)
{

}  // ODBCQuery::ODBCQuery


//------------------------------------------------------------------------------
// ODBCQuery::~ODBCQuery
//------------------------------------------------------------------------------
ODBCQuery::~ODBCQuery()
{   
	// If there is still a transaction, rollback
	if (_isTransaction)
		rollback();

	//Make sure all ODBC resources are released
	if (_hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
		_hstmt = SQL_NULL_HSTMT;
	}

	// Free any parameters
	_freeCollection(BIND_PARAMETERS);

	// Free the current field values
	_freeCollection(FIELD_VALUES);

	// Clear any previouse field information.
	_freeCollection(FIELD_INFORMATION);

	// Release this connection from the parent connection object.
	_parentConnection->releaseQueryConnection(this);
}  // ODBCQuery::~ODBCQuery


//------------------------------------------------------------------------------
// ODBCQuery::clearBindParams
//------------------------------------------------------------------------------
void 
ODBCQuery::clearBindParams()
{
	// Free all the bind parameters.
	_freeCollection(BIND_PARAMETERS);
}  // ODBCQuery::clearBindParams


//------------------------------------------------------------------------------
// ODBCQuery::command
//------------------------------------------------------------------------------
void
ODBCQuery::command(
				   const string& sqlStatement)
{
	g_dbclog.print("[%04d] command(%s)\n", this->_index, sqlStatement.c_str() );
	// Clear any bind parameters as we now have a new query
	clearBindParams();

	// Check if this is a function been called and build the correct sql if it is
	//string newSqlStatement = _pgsqlParseFunctionCall(sqlStatement);
	string newSqlStatement = sqlStatement;
	// TODO: Add procedure calling "CALL XYZ(?,?)"

	// Call the base query to store a copy of the query.
	BaseQuery::command(newSqlStatement);
} // ODBCQuery::command


//------------------------------------------------------------------------------
// ODBCQuery::bindParam
//------------------------------------------------------------------------------
BaseValue*
ODBCQuery::bindParam(
					 const string& paramName)
{
	// Make sure the name has not already been added. If it has, return the instance to it
	for (unsigned int i=0; i<_numParameters; i++)
		if (strcasecmp(_parameters[i]->bindName.c_str(), paramName.c_str()) == 0)
			return _parameters[i];

	// Make sure the parameter is present in the quey
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
	_parameters = (ODBCBindParam**)realloc((void*)_parameters, _numParameters * sizeof(ODBCBindParam*));
	_parameters[_numParameters-1] = new ODBCBindParam(paramName);
	return _parameters[_numParameters-1];
}  // ODBCQuery::bindParam

void 
ODBCQuery::_handleOdbcErr( SQLHANDLE& stmt, SQLRETURN& cliRC, const char* action )
{
		SQLCHAR			sqlState[16]	= { 0 };
		SQLINTEGER		sqlCode			= 0;
		SQLCHAR			message[ SQL_MAX_MESSAGE_LENGTH ]	= {0};
		SQLSMALLINT		length			= 0;

		SQLGetDiagRec(SQL_HANDLE_STMT, stmt, 1, sqlState, &sqlCode, message, SQL_MAX_MESSAGE_LENGTH - 1, &length);
		char		info[SQL_MAX_MESSAGE_LENGTH * 2];
		sprintf( info, "execute(); SQLSTATE:%s, SQLCODE:0x%08x, MSG:%s",
			sqlState, sqlCode, message );
		string err = info;
		string statestr	= (const char*)sqlState;
		g_dbclog.print("[%04d] %s %s\n", _index, action, message );
		if ( cliRC != SQL_ERROR ){
			printf("[%04d] %s %s\n", _index,action, info );
		}
		else{
			if (stmt != SQL_NULL_HSTMT)
			{
				SQLFreeHandle(SQL_HANDLE_STMT, stmt);
				stmt = SQL_NULL_HSTMT;
			}
			throw ErrorQuerying(err, statestr);
		};
}

//------------------------------------------------------------------------------
// ODBCQuery::execute
//------------------------------------------------------------------------------
void
ODBCQuery::execute()
{
	g_dbclog.print("[%04d] execute\n", _index );
	// Holds pointers for the bind values (left to right alinged to the ?'s that will be substituted)
	vector<ODBCBindParam*> bindParameters;   
	
	SQLRETURN cliRC = SQL_SUCCESS;

	//Make sure all ODBC resources are released
	if (_hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, _hstmt);
		_hstmt = SQL_NULL_HSTMT;
	}
	_queryParameterNames.clear();

	// Number of rows in the result set is now 0
	_odbcCurrentRow = 0;

	// Clear any previouse field information.
	_freeCollection(FIELD_INFORMATION);

	// Free the current field values
	_freeCollection(FIELD_VALUES);


	// Replace all the bind parameters in the SQL with '?' characters and build name list for correct binding
	string resolvedSqlStatement = _odbcParseBindParameters(_sqlStatement, bindParameters);

	// Ping the connection to make sure it is still valid.
	//xzq _parentConnection->_odbcPing(_index);

	// Allocate the statement handle
	cliRC	= SQLAllocHandle(SQL_HANDLE_STMT, _parentConnection->_handles[_index]->_hdbc, &_hstmt);
	if (cliRC != SQL_SUCCESS){
		if ( cliRC == SQL_INVALID_HANDLE ){
			_parentConnection->_odbcReconnect( _index );
			cliRC	= SQLAllocHandle(SQL_HANDLE_STMT, _parentConnection->_handles[_index]->_hdbc, &_hstmt);
		}
		if ( cliRC != SQL_SUCCESS ){
			throw ErrorQuerying("execute(): Unable to allocate the statement handle.");
		};
	}

	// Prepare the statement 
	cliRC	= SQLPrepare(_hstmt, (SQLCHAR*)resolvedSqlStatement.c_str(), SQL_NTS);
	if (cliRC != SQL_SUCCESS )
	{
		_handleOdbcErr( _hstmt, cliRC, "SQLPrepare");
	}

	// Always make sure there is a transaction that the query is occuring in
	// Call this after the SQLPrepare as SQLPrepare will create a transaction

	/*	may cause SQL Server locked
	 *
	if (!_isTransaction)
		transBegin();
	*/

	// Bind the parameters
	_odbcBindParameters(resolvedSqlStatement, bindParameters);

	// Execute the query
	cliRC = SQLExecute(_hstmt);

	if ( cliRC == SQL_NO_DATA ){
		_fieldCount = 0;
		_eof		= true;
		return;
	};

	// Make sure it executed correctly
	if (cliRC != SQL_SUCCESS )
	{
		_handleOdbcErr( _hstmt, cliRC, "SQLExecute");
	}

	// Check if there is a result set
	SQLSMALLINT nResultCols	= 0;
	cliRC = SQLNumResultCols(_hstmt, &nResultCols);
	
	if (cliRC != SQL_SUCCESS )
	{
		_handleOdbcErr( _hstmt, cliRC, "SQLNumResultCols");
	}

	while ( nResultCols == 0 ){	//	SP ?
		cliRC	= SQLMoreResults( _hstmt );

		if(cliRC == SQL_NO_DATA)
			break;

		switch( cliRC ){
		case	SQL_NO_DATA:	break;
		case	SQL_SUCCESS:
			{
				cliRC = SQLNumResultCols( _hstmt, &nResultCols );
				if ( cliRC != SQL_SUCCESS ){
					_handleOdbcErr( _hstmt, cliRC, "SQLNumResultCols" );
				}
				break;
			}
		default:
			_handleOdbcErr( _hstmt, cliRC, "SQLMoreResults");
			break;
		}
	};

	// Handle the result set comming back
	if (nResultCols)
	{
		// We have a result set with data      
		_fieldCount = nResultCols;

		// Get the field information for this query.
		_odbcGetFieldsInformation();

		_eof = false;

		// Pre-fetch the first row making sure there is data in the result set
		cliRC = SQLFetch(_hstmt);
		if (cliRC == SQL_NO_DATA_FOUND)
		{
			_eof = true;
		}
		else if (cliRC != SQL_SUCCESS)
		{
			_handleOdbcErr( _hstmt, cliRC, "SQLFetch");
		}            
	}
	else
	{
		// We have no records, could be an update or something
		_fieldCount = 0;
		_eof = true;            
	}
}  // ODBCQuery::execute


//------------------------------------------------------------------------------
// ODBCQuery::next
//------------------------------------------------------------------------------
void
ODBCQuery::fetchNext()
{
	// Do nothing if the we have reached the end of the record set.
	if (_eof)
		return;

	// Get the data. _odbcGetResultSetRow will set the _eof parameter 
	_odbcGetResultSetRow();
	_odbcCurrentRow++;   
}  // ODBCQuery::next


//------------------------------------------------------------------------------
// ODBCQuery::transBegin
//------------------------------------------------------------------------------
void
ODBCQuery::transBegin()
{
	// Make sure a transaction is not already active
	if (_isTransaction)
		throw TransactionError("transBegin(): "
		"A transaction is already active. Commit or rollback the transaction before creating a new transaction.");

	_isTransaction = true;

	/*
	* A ODBC transaction is automatically started when a SQLPrepare, SQLExecDirect or SQLGetTypeInfo call is made.
	* Hence a transaction is implied, and does not need to be explicilty created.      
	* This function should remain as a token function with basic error checking, Johnathan Ingram
	*
	*/      
}  // ODBCQuery::transBegin


//------------------------------------------------------------------------------
// ODBCQuery::commit
//------------------------------------------------------------------------------
void
ODBCQuery::commit()
{
	// Make sure a transaction is active
	if (!_isTransaction)
		throw TransactionError("commit(): "
		"A transaction is not active. Create a transaction before calling commit.");

	// Commit the transaction
	SQLRETURN cliRC = SQL_SUCCESS;
	cliRC = SQLEndTran(SQL_HANDLE_DBC, _parentConnection->_handles[_index]->_hdbc, SQL_COMMIT);

	// Check if an error occured while commiting
	if (cliRC != SQL_SUCCESS)
	{
		SQLCHAR		sqlState[16]	= { 0 };
		SQLINTEGER	sqlCode			= 0;
		SQLCHAR		message[SQL_MAX_MESSAGE_LENGTH]	= { 0 };
		SQLSMALLINT length			= 0;

		SQLGetDiagRec(SQL_HANDLE_DBC, _parentConnection->_handles[_index]->_hdbc, 1, sqlState, &sqlCode, message, SQL_MAX_MESSAGE_LENGTH - 1, &length);

		string err = "commit(): ";      
		err += (const char*)message;

		throw TransactionError(err);
	}

	_isTransaction = false;   
}  // ODBCQuery::commit


//------------------------------------------------------------------------------
// ODBCQuery::rollback
//------------------------------------------------------------------------------
void
ODBCQuery::rollback()
{
	// Make sure a transaction is active
	if (!_isTransaction)
		throw TransactionError("rollback(): "
		"A transaction is not active. Create a transaction before calling rollback.");

	// Roolback the transaction
	SQLRETURN cliRC;
	cliRC = SQLEndTran(SQL_HANDLE_DBC, _parentConnection->_handles[_index]->_hdbc, SQL_ROLLBACK);

	// Check if an error occured while rolling back
	if (cliRC != SQL_SUCCESS)
	{
		SQLCHAR		sqlState[16]	= { 0 };
		SQLINTEGER	sqlCode			= 0;
		SQLCHAR		message[SQL_MAX_MESSAGE_LENGTH]	= { 0 };
		SQLSMALLINT length			= 0;

		SQLGetDiagRec(SQL_HANDLE_DBC, _parentConnection->_handles[_index]->_hdbc, 1, sqlState, &sqlCode, message, SQL_MAX_MESSAGE_LENGTH - 1, &length);

		string err = "rollback(): ";      
		err += (const char*)message;

		throw TransactionError(err);
	}

	_isTransaction = false;      
}  // ODBCQuery::rollback


//------------------------------------------------------------------------------
// ODBCQuery::getFieldInfoByColumn
//------------------------------------------------------------------------------
BaseFieldDescription*
ODBCQuery::getFieldInfoByColumn(
								int index)
{
	// Make sure the index is in range.
	if (index < 0 || index >= (int)_numFieldInformation)
		throw IndexOutOfRange("getFieldInfoByColumn(): The field index is out of range for the current result set");

	return _fieldInformation[index];      
}  // ODBCQuery::getFieldInfoByColumn


//------------------------------------------------------------------------------
// ODBCQuery::getFieldInfoByName
//------------------------------------------------------------------------------
BaseFieldDescription*
ODBCQuery::getFieldInfoByName(
							  const string& fieldName)
{
	// Try and find the field name
	for (unsigned int i=0; i<_numFieldInformation; i++)
		if (strcasecmp(_fieldInformation[i]->name().c_str(), fieldName.c_str()) == 0)
			return _fieldInformation[i];

	throw NameNotFound("getFieldInfoByName(): The field name was not found for the current result set.");   
}  // ODBCQuery::getFieldInfoByName


//------------------------------------------------------------------------------
// ODBCQuery::getFieldByColumn
//------------------------------------------------------------------------------
BaseValue*
ODBCQuery::getFieldByColumn(
							int index)
{
	// Make sure the index is in range.
	if (index < 0 || index >= (int)_numRecordValues)
	{
		throw IndexOutOfRange("getFieldByColumn(): The field index is out of range for the current result set");
	}

	return _recordValues[index];      
}  // ODBCQuery::getFieldByColumn


//------------------------------------------------------------------------------
// ODBCQuery::getFieldByName
//------------------------------------------------------------------------------
BaseValue*
ODBCQuery::getFieldByName(
						  const string& fieldName)
{
	// Try and find the field name
	for (unsigned int i=0; i<_numRecordValues; i++)
		if (strcasecmp(_recordValues[i]->name().c_str(), fieldName.c_str()) == 0)
			return _recordValues[i];

	throw NameNotFound("getFieldByName(): The field name was not found for the current result set.");   
}  // ODBCQuery::getFieldByName
