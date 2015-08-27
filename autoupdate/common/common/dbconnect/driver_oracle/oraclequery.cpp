
#include "stdafx.h"
#ifdef _DB_ORACLE_
#include "OracleQuery.h"

#define new DEBUG_NEW

void
OracleQuery::_oracleGetOutParam()
{
	map<int,OracleBindParam*>::iterator i;
	for( i=_outParam.begin(); i!=_outParam.end(); i++ )
	{
		OracleBindParam* bindParam =  (*i).second;
		switch( bindParam->getRetType())
		{
		case FT_SHORT:
		case FT_LONG:
			{
				long l = 0;
				sword ret1 =  OCINumberToInt (_parentConnection->_handles[_index]->_errHandle,
					reinterpret_cast <OCINumber *> (bindParam->valuePtr.ptr),
					sizeof(long),
					OCI_NUMBER_SIGNED,&l);
				bindParam->setLong(l);

			}
			break;
		case FT_UNSIGNED_SHORT:
		case FT_UNSIGNED_LONG:
			{
				unsigned long l = 0;
				sword ret1 =  OCINumberToInt (_parentConnection->_handles[_index]->_errHandle,
					reinterpret_cast <OCINumber *> (bindParam->valuePtr.ptr),
					sizeof(long),
					OCI_NUMBER_UNSIGNED,&l);
				bindParam->setUnsignedLong(l);

			}
			break;

		case FT_DOUBLE:
			{
				double d = 0;
				sword ret2 =  OCINumberToReal (_parentConnection->_handles[_index]->_errHandle,
					reinterpret_cast <OCINumber *> (bindParam->valuePtr.ptr),
					sizeof (double),&d);
				bindParam->setFloat(d);
			}
			break;

		case FT_DATETIME:
			{
				JDate dVal(0.0);
				OCIDate *pOCIDate = (OCIDate *) bindParam->valuePtr.ptr;
				ub1 c =  ((ub1*)bindParam->valuePtr.ptr)[0];
				ub1 y =  ((ub1*)bindParam->valuePtr.ptr)[1];

				dVal.setDate((c-100)*100+y-100,				
					pOCIDate->OCIDateMM,
					pOCIDate->OCIDateDD, 
					pOCIDate->OCIDateTime.OCITimeHH+1,
					pOCIDate->OCIDateTime.OCITimeMI,
					pOCIDate->OCIDateTime.OCITimeSS );
				bindParam->setDateTime(dVal);
			}
			break;
		case FT_RESULTSET:
			{
				sword ret = OCIHandleFree(_parentConnection->_handles[_index]->_stmtHandle,OCI_HTYPE_STMT);
				g_dbclog.print("[%04d] release handle %X,ret=%d\n", this->_index, _parentConnection->_handles[_index]->_stmtHandle, ret);
				_parentConnection->_handles[_index]->_stmtHandle = *(OCIStmt**)bindParam->asBinary();
				return;
			}
			break;

		default:
			{
				char  *s  = reinterpret_cast <char *> (bindParam->valuePtr.ptr);
				size_t len = strlen(s);
				char *d = new char[len+1];
				strncpy(d,s,len);
				d[len] = '\0';
				bindParam->setString(d);
				delete d;
			}
			break;
		}
	}
}
//------------------------------------------------------------------------------
// OracleQuery::_oracleGetFieldsInformation
//------------------------------------------------------------------------------
void 
OracleQuery::_oracleGetFieldsInformation()
{
	// Clear any previous field information just in case.
	_freeCollection(FIELD_INFORMATION);

	ub4 paramcnt = 0;
	OciFunctionCheck(_index,"Get Param Count", _parentConnection->_handles[_index]->_errHandle, 
		OCIAttrGet((dvoid *)_parentConnection->_handles[_index]->_stmtHandle, OCI_HTYPE_STMT, 
					(dvoid *)&paramcnt,(ub4 *)0, OCI_ATTR_PARAM_COUNT, _parentConnection->_handles[_index]->_errHandle));

	_fieldCount = paramcnt;
	// Allocate the information pointers
	_numFieldInformation = _fieldCount;
	_fieldInformation = (OracleFieldDescription**)malloc(_numFieldInformation * sizeof(OracleFieldDescription*));

	g_dbclog.print("[%04d] column number is %d\n",_index,_numFieldInformation);
	// Assign the data to the corresponding fields.
	for (int i=0; i<(int)_numFieldInformation; i++)
	{
		// Get the field information
		ub4 colType = 0,colSize =0 ;
		ub4 _scale =0 ,_precision = 0, isnull = 0;
		char *colName = NULL;
		OCIParam *colhd = NULL;
		OCIParamGet((dvoid *)_parentConnection->_handles[_index]->_stmtHandle, OCI_HTYPE_STMT,_parentConnection->_handles[_index]->_errHandle, 
			(dvoid **)&colhd, i+1);

		OCIAttrGet((dvoid *)colhd, OCI_DTYPE_PARAM,(dvoid *)&colName, (ub4 *)0, OCI_ATTR_NAME,_parentConnection->_handles[_index]->_errHandle);
		OCIAttrGet((dvoid *)colhd, OCI_DTYPE_PARAM,(dvoid *)&colType, (ub4 *)0, OCI_ATTR_DATA_TYPE,_parentConnection->_handles[_index]->_errHandle);
		OCIAttrGet((dvoid *)colhd, OCI_DTYPE_PARAM,(dvoid *)&colSize, (ub4 *)0, OCI_ATTR_DATA_SIZE,_parentConnection->_handles[_index]->_errHandle);
		OCIAttrGet((dvoid *)colhd, OCI_DTYPE_PARAM,(dvoid *)&isnull, (ub4 *)0, OCI_ATTR_IS_NULL,_parentConnection->_handles[_index]->_errHandle);
		OCIAttrGet((dvoid *)colhd, OCI_DTYPE_PARAM,(dvoid *)&_precision, (ub4 *)0, OCI_ATTR_PRECISION,_parentConnection->_handles[_index]->_errHandle);
		OCIAttrGet((dvoid *)colhd, OCI_DTYPE_PARAM,(dvoid *)&_scale, (ub4 *)0, OCI_ATTR_SCALE,_parentConnection->_handles[_index]->_errHandle);

		if( SQLT_NUM == colType  || SQLT_INT == colType)
				colType = SQLT_VNU;
		else if(SQLT_TIMESTAMP == colType )
			colType = SQLT_DAT;

		// Work out the field type.
		FieldType _type = _oracleResolveFieldType(colType);

		// Field Properties.      
		bool     _isIncrement = false;
		bool     _isPriKey    = false;
		bool     _isUnique    = false;
		bool     _isNotNull   = !isnull;

		_fieldInformation[i] = new OracleFieldDescription(string(colName), i, _type, _isIncrement, _isPriKey, _isUnique, 
			_isNotNull, _precision, _scale, colType, colSize);
		OciFunctionCheck(_index,"DefineByPos",_parentConnection->_handles[_index]->_errHandle,
			OCIDefineByPos(_parentConnection->_handles[_index]->_stmtHandle,&_fieldInformation[i]->_definePtr,
			_parentConnection->_handles[_index]->_errHandle,i+1,_fieldInformation[i]->getFetchBuf(),
			colSize,colType,&_fieldInformation[i]->indicator,&_fieldInformation[i]->data_len,NULL,OCI_DEFAULT));
	}
}  // OracleQuery::_oracleGetFieldsInformation


//------------------------------------------------------------------------------
// OracleQuery::_oracleGetResultSetRow

// VARCHAR2		1 char[n]
// NUMBER		2 char[n](n 22)
// INTEGER		3 int
// FLOAT		4 float
// STRING		5 Float char[n+1]
// VARNUM		6 char[n](n 22)
// DECIMAL		7 float
// LONG			8 char[n]
// VARCHAR		9 char[n+2]
// ROWID		11 char[n]
// DATE			12 char[n]
// VARRAW		15 char[n]
// RAW			23 unsigned Char[n]
// LONG RAW		24 unsigned Char[n]
// UNSIGNED		68 unsigned int
// DISPLAY		91 char[n]
// LONG VARCHAR 94 char[n+4]
// LONG VARRAW	95 unsigned char[n+4]
// CHARTER(VHAR) 96 char[n]
// MISLABLE		106 char[n]
//------------------------------------------------------------------------------
void
OracleQuery::_oracleGetResultSetRow()
{
	// NOTE: This function always assumes fetchNext has been called before the start   
	// Clear any previous field information just in case.
	_freeCollection(FIELD_VALUES);


	// Allocate the value pointers
	_numRecordValues = _fieldCount;
	_recordValues = (OracleValue**)malloc(_numRecordValues * sizeof(OracleValue*));

	// Get the data :)  
	for (int i=0; i<(int)_fieldCount; i++)
	{
		// Allocate the field
		_recordValues[i] = new OracleValue(_fieldInformation[i]->name().c_str());

		// Get the field value according to the field type
		void *buffer = NULL;
		long  bufferSize = 0;
		void *bufferOffset = NULL;

		switch(_fieldInformation[i]->oracleFieldType())
		{

		case SQLT_CHR :	//"VARCHAR2";
		case SQLT_STR :	//"STRING";
		case SQLT_VCS :	//"VARCHAR";
		case SQLT_AFC :	//"CHAR";
		case SQLT_AVC :	//"CHARZ";
		case SQLT_LVC :	//"LONG VARCHAR";
		case SQLT_RID :	//"ROWID";
		case SQLT_RDD :	//"ROWID";
		case SQLT_REF :	//"REF";
			{
				char* pvalue = (char*)_fieldInformation[i]->getFetchBuf();
				_recordValues[i]->setString(pvalue);
			}
			break;

		case SQLT_VNU :
			{
				double fValue = 0;
				void* temp = _fieldInformation[i]->getFetchBuf();
				OCINumber* number = (OCINumber*)temp;
				OCINumberToReal (_parentConnection->_handles[_index]->_errHandle, 
					number, 
					sizeof (double),
					&fValue);
				_recordValues[i]->setFloat(fValue);
			}
			break;
		case SQLT_TIMESTAMP:
		case SQLT_DAT :	//"DATE";
			{
				JDate dVal(0.0);
				OCIDate *pOCIDate = (OCIDate *) _fieldInformation[i]->getFetchBuf();
				ub1 c =  ((ub1*)_fieldInformation[i]->getFetchBuf())[0];
				ub1 y =  ((ub1*)_fieldInformation[i]->getFetchBuf())[1];

				dVal.setDate((c-100)*100+y-100,				
					pOCIDate->OCIDateMM,
					pOCIDate->OCIDateDD, 
					pOCIDate->OCIDateTime.OCITimeHH-1,
					pOCIDate->OCIDateTime.OCITimeMI,
					pOCIDate->OCIDateTime.OCITimeSS );
				_recordValues[i]->setDateTime(dVal);
			}
			break;
		default:
			{
				char buf[20];
				memset(buf, 0, sizeof(buf));
				snprintf(buf, sizeof(buf)-1, "%d", _fieldInformation[i]->oracleFieldType());
				string err = "_oracleGetResultSetRow(): Unknown Oracle SQL field type ";         
				err += buf;
				throw ResultSetError(err);
			}
			break;
		}
		_fieldInformation[i]->clearFetchBuf();
		// Free the buffer
		if (buffer)
		{
			free(buffer);
			buffer = NULL;         
		}
	}

}  // OracleQuery::_oracleGetResultSetRow

//------------------------------------------------------------------------------
// OracleQuery::_oracleResolveFieldType
//------------------------------------------------------------------------------
FieldType
OracleQuery::_oracleResolveFieldType(
									 int type)
{
	FieldType res;

	switch(type)
	{
	case SQLT_CHR :	//"VARCHAR2";
		res = FT_STRING;
		break;

	case SQLT_STR :	//"STRING";
		res = FT_STRING;
		break;

	case SQLT_VCS :	//"VARCHAR";
		res = FT_STRING;
		break;

	case SQLT_AFC :	//"CHAR";
		res = FT_STRING;
		break;

	case SQLT_AVC :	//"CHARZ";
		res = FT_STRING;
		break;

	case SQLT_LVC :	//"LONG VARCHAR";
		res = FT_STRING;
		break;

	case SQLT_RID :	//"ROWID";
		res = FT_STRING;
		break;

	case SQLT_RDD :	//"ROWID";
		res = FT_STRING;
		break;

	case SQLT_VNU :	//"VARNUM";
		res = FT_STRING;
		break;

	case SQLT_REF :	//"REF";
		res = FT_STRING;
		break;

	case SQLT_NUM :	//"NUMBER";
		res = FT_DOUBLE;
		break;

	case SQLT_INT :			//"INTEGER";
		res = FT_LONG;
		break;

	case SQLT_UIN :	//"UNSIGNED INT";
		res = FT_UNSIGNED_LONG;
		break;

	case SQLT_FLT :		//"FLOAT";
		res = FT_DOUBLE;
		break;

	case SQLT_LNG :	//"LONG";
		res = FT_DOUBLE;
		break;

	case SQLT_DAT :	//"DATE";
	case SQLT_TIMESTAMP:
		res = FT_DATETIME;
		break;

	case SQLT_VBI :	//"VARRAW";
	case SQLT_BIN :	//"RAW";
	case SQLT_LBI :	//"LONG RAW";
	case SQLT_LVB :	//"LONG VARRAW";
	case SQLT_NTY :	//"NAMED DATA TYPE";
		res = FT_BLOB;
		break;

	case SQLT_CLOB:	//"CLOB";
		res = FT_CLOB;
		break;

	case SQLT_BLOB:	//"BLOB";
		res = FT_BLOB;
		break;

	case SQLT_FILE:	//"BFILE";
		res = FT_BLOB;
		break;

	default:
		res = FT_UNKNOWN;
		break;
	}

	return res;
}  // OracleQuery::_oracleResolveFieldType


int 
OracleQuery::_oracleResolveBindType( int type)
{
	int res;

	switch(type)
	{

	case FT_STRING:
		res = SQLT_STR;
		break;

	case FT_SHORT:
	case FT_LONG:
		res = SQLT_INT;
		break;

	case FT_UNSIGNED_LONG :
		res = SQLT_UIN;
		break;

	case FT_DOUBLE:	
		res = SQLT_FLT;
		break;

	case FT_DATETIME  :	//"DATE";
		res = SQLT_DAT;
		break;

	default:
		res = FT_UNKNOWN;
		break;
	}
	return res;
}  // OracleQuery::_oracleResolveFieldType

//------------------------------------------------------------------------------
// OracleQuery::_oracleParseSql
//------------------------------------------------------------------------------
string
OracleQuery::_oracleParseSql( const string& originalSqlStatement)
							
{
	_outputCount = 0;
	string res = originalSqlStatement;   
	size_t 	pos = res.find_first_of(":?");
	while (pos != string::npos)
	{
		size_t ind = pos;
		bool isTag = false;
		while( ind > 0 ) {
			ind--;
			if ( res[ind] == ',' || res[ind]=='(' ) {
				isTag = true;			
				break;
			}
			if( res[ind]== '\t' || res[ind] ==' ')
				continue;
			else
				break;
		}
		if( isTag ) {
			_outputCount++;
			if(res[pos] == '?') {
				char buf[128];
				sprintf(buf,":%d",_outputCount);
				res = res.substr(0,pos)+buf+res.substr(pos+1);
			}
		}
		// Find the next parameter and continue
		pos = res.find_first_of("?:",pos+1);
	}
	return res;      
}  // OracleQuery::_oracleParseSql

//------------------------------------------------------------------------------
// OracleQuery::_oracleBindParameters
//------------------------------------------------------------------------------
void
OracleQuery::_oracleBindParameters(	)
{
	for( int i= 0; i < (int)_numParameters; i ++ )
	{
		OracleBindParam *bindParam = _parameters[i];
		if( !bindParam->valuePtr.ptr)
			continue;
		static ub2 alen = 0; 
		if( bindParam->getBindType() == OracleBindParam::BIND_BY_NAME )
			OciFunctionCheck (_index, "Bind Parameters" ,_parentConnection->_handles[_index]->_errHandle,
			OCIBindByName(_parentConnection->_handles[_index]->_stmtHandle,&bindParam->bindObjPtr,
			_parentConnection->_handles[_index]->_errHandle,
			(text*)bindParam->name().c_str(),
			-1,
			bindParam->valuePtr.ptr,
			bindParam->valueSize,
			bindParam->getOciType(),&bindParam->indicator,
			&alen, NULL,0,NULL,OCI_DEFAULT));
		else{
			OciFunctionCheck ( _index,"Bind Parameters" ,_parentConnection->_handles[_index]->_errHandle,
			OCIBindByPos(_parentConnection->_handles[_index]->_stmtHandle,&bindParam->bindObjPtr,
			_parentConnection->_handles[_index]->_errHandle,
			bindParam->getBindIndex(),
			bindParam->valuePtr.ptr,
			bindParam->valueSize,
			bindParam->getOciType(), bindParam->isOutParam()?&bindParam->indicator:NULL,
			bindParam->isOutParam()?&alen:NULL,NULL,0,NULL,OCI_DEFAULT));
		}
	}
}  // OracleQuery::_oracleBindParameters


//------------------------------------------------------------------------------
// OracleQuery::_freeCollection
//------------------------------------------------------------------------------
void 
OracleQuery::_freeCollection(
							 CollectionType type)
{
	int i;
	switch (type)
	{
	case FIELD_INFORMATION:
		if (_fieldInformation)
		{
			for (i=0; i<(int)_numFieldInformation; i++)
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
			for (i=0; i<(int)_numRecordValues; i++)
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
			for (i=0; i<(int)_numParameters; i++)
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
		_outParam.clear();
		break;
	}
}  // OracleQuery::_freeCollection


// -----------------------------------------------------------------------------
// PUBLIC:
// -----------------------------------------------------------------------------


//------------------------------------------------------------------------------
// OracleQuery::OracleQuery
//------------------------------------------------------------------------------
OracleQuery::OracleQuery(
						 OracleConnection* parentConnection,
						 int                   index)
						 : 
_parentConnection(parentConnection),
_index(index),
_isTransaction(false),
_oracleCurrentRow(0),
_oracleNumRows(0),
_recordValues(NULL),
_numRecordValues(0),
_fieldInformation(NULL),
_numFieldInformation(0),
_parameters(NULL),
_numParameters(0)
{

}  // OracleQuery::OracleQuery


//------------------------------------------------------------------------------
// OracleQuery::~OracleQuery
//------------------------------------------------------------------------------
OracleQuery::~OracleQuery()
{   
	// If there is still a transaction, rollback
	if (_isTransaction)
		rollback();

	// Free any parameters
	_freeCollection(BIND_PARAMETERS);

	// Free the current field values
	_freeCollection(FIELD_VALUES);

	// Clear any previous field information.
	_freeCollection(FIELD_INFORMATION);

}  // OracleQuery::~OracleQuery


//------------------------------------------------------------------------------
// OracleQuery::clearBindParams
//------------------------------------------------------------------------------
void 
OracleQuery::clearBindParams()
{
/*	for(int i = 0; i < (int)_numParameters; i++)
	{
		OracleBindParam* param = _parameters[i];
		if(param->getRetType() == FT_RESULTSET)
		{
			void** temp = (void**)param->asBinary();
			g_dbclog.print("[%04d] release param[%d] cursor %X\n", this->_index, i, *temp);
			OCIHandleFree(*temp, OCI_HTYPE_STMT);
		}
	}*/
	// Free all the bind parameters.
	_freeCollection(BIND_PARAMETERS);

	_outParam.clear();
}  // OracleQuery::clearBindParams


//------------------------------------------------------------------------------
// OracleQuery::command
//------------------------------------------------------------------------------
void
OracleQuery::command(
					 const string& sqlStatement)
{
	g_dbclog.print("[%04d] command(%s)\n", this->_index, sqlStatement.c_str() );
	// Clear any bind parameters as we now have a new query
	clearBindParams();
	if (_parentConnection->_handles[_index]->_stmtHandle != NULL)
	{
		sword ret = OCIHandleFree(_parentConnection->_handles[_index]->_stmtHandle,OCI_HTYPE_STMT);
		g_dbclog.print("[%04d] release handle %X,ret=%d\n", this->_index, _parentConnection->_handles[_index]->_stmtHandle, ret);
		_parentConnection->_handles[_index]->_stmtHandle = NULL;
	}
	string newSqlStatement = _oracleParseSql(sqlStatement);

	OciFunctionCheck(_index, "Alloc Statement", _parentConnection->_handles[_index]->_errHandle,
		OCIHandleAlloc(_parentConnection->_handles[_index]->_evtHandle,(dvoid **)&_parentConnection->_handles[_index]->_stmtHandle,
		OCI_HTYPE_STMT, 0, (dvoid **) 0));
	g_dbclog.print("[%04d] alloc handle for stmt ,value=%X,env=%X\n", _index, _parentConnection->_handles[_index]->_stmtHandle,
		_parentConnection->_handles[_index]->_evtHandle);

	OciFunctionCheck(_index,"Prepare Statement", _parentConnection->_handles[_index]->_errHandle,
		OCIStmtPrepare(_parentConnection->_handles[_index]->_stmtHandle,_parentConnection->_handles[_index]->_errHandle,(text*)newSqlStatement.c_str(),
		(ub4)newSqlStatement.length()+1,OCI_NTV_SYNTAX,OCI_DEFAULT));///sqlÓï¾äÒª²»Òª+1ÄØ£¿

	BaseQuery::command(newSqlStatement);
} // OracleQuery::command


//------------------------------------------------------------------------------
// OracleQuery::bindParam
//------------------------------------------------------------------------------
BaseValue*
OracleQuery::bindParam(
					   const string& paramName)
{
	// Make sure the name has not already been added. If it has, return the instance to it
	for (int i=0; i<(int)_numParameters; i++)
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
	_parameters = (OracleBindParam**)realloc((void*)_parameters, _numParameters * sizeof(OracleBindParam*));
	_parameters[_numParameters-1] = new OracleBindParam(paramName,false);
	return _parameters[_numParameters-1];
}  // OracleQuery::bindParam

BaseValue* OracleQuery::bindParam(int index, int type)
{
	_numParameters++;
	_parameters = (OracleBindParam**)realloc((void*)_parameters, _numParameters * sizeof(OracleBindParam*));
	_parameters[_numParameters-1] = new OracleBindParam(index,false,type);
	return _parameters[_numParameters-1];
}


void OracleQuery::registerOutParam(int index, int type)
{
	map<int,OracleBindParam*>::iterator reslut =  _outParam.find(index);

	if (reslut != _outParam.end())
		throw BaseException(0," Aleady register Outparam");


	_numParameters++;
	_parameters = (OracleBindParam**)realloc((void*)_parameters, _numParameters * sizeof(OracleBindParam*));
	_parameters[_numParameters-1] = new OracleBindParam(index,true,type);
	_outParam[index] = _parameters[_numParameters-1];

	if( FT_RESULTSET == type) 
	{
		OciFunctionCheck(_index, "registerOutParam", _parentConnection->_handles[_index]->_errHandle,
			OCIHandleAlloc(_parentConnection->_handles[_index]->_evtHandle,(dvoid **)(_parameters[_numParameters-1]->asBinary()),
			OCI_HTYPE_STMT, 0, (dvoid **) 0));	
		void** temp = (void**)_parameters[_numParameters-1]->asBinary();
		g_dbclog.print("[%04d] alloc handle for cursor param(%d),value=%X,env=%X\n", _index, _numParameters - 1, *temp,
				_parentConnection->_handles[_index]->_evtHandle);
	}
	return;	

}
//------------------------------------------------------------------------------
// OracleQuery::execute
//------------------------------------------------------------------------------
void
OracleQuery::execute()
{
	g_dbclog.print("[%04d] execute\n", _index );
	

	// Holds pointers for the bind values (left to right aligned to the ?'s that will be substituted)
	vector<OracleBindParam*> bindParameters;   
	_eof =true;

	// Number of rows in the result set is now 0
	_oracleCurrentRow = 0;
	// Clear any previous field information.
	_freeCollection(FIELD_INFORMATION);
	// Free the current field values
	_freeCollection(FIELD_VALUES);

	_oracleBindParameters();

	ub2	stmt_type = 1;
	OciFunctionCheck(_index,"Get SQL Type", _parentConnection->_handles[_index]->_errHandle,
		OCIAttrGet (_parentConnection->_handles[_index]->_stmtHandle, OCI_HTYPE_STMT,&stmt_type,	NULL,OCI_ATTR_STMT_TYPE,
		_parentConnection->_handles[_index]->_errHandle));

	ub4 iters = ( stmt_type == OCI_STMT_SELECT) ? 0 : 1;
	g_dbclog.print("[%04d] ExeSQL Begin SQL type is %d\n",_index,stmt_type);
	//zhanglq change second to ms
	unsigned long lastTick = GetTickCount();
	sword status = OCIStmtExecute(_parentConnection->_handles[_index]->_svcCtx,_parentConnection->_handles[_index]->_stmtHandle,
		_parentConnection->_handles[_index]->_errHandle,iters,0,NULL,NULL,OCI_COMMIT_ON_SUCCESS);

	g_dbclog.print("[%04d] ExeSQL End ret= %d  cost time %d ms \n",_index,status,GetTickCount() - lastTick);
	
	if(OCI_ERROR == status)
		_parentConnection->_oraclePing(_index);

	OciFunctionCheck(_index,"ExecSQL",_parentConnection->_handles[_index]->_errHandle,status);

	//get the output param first ,if find the first reference cursor ,replace the current statement
	_oracleGetOutParam();

	_oracleGetFieldsInformation();
	
	if( _fieldCount <= 0 ) 
	{
		g_dbclog.print("[%04d] ExeSQL No Select list return \n",_index);
		return ;
	}
	status =  OCIStmtFetch2(_parentConnection->_handles[_index]->_stmtHandle,_parentConnection->_handles[_index]->_errHandle,1,OCI_FETCH_NEXT,0,OCI_DEFAULT);	
	OciFunctionCheck( _index,"ExecSQL",_parentConnection->_handles[_index]->_errHandle,status);

	if( status == OCI_NO_DATA )  
	{
		g_dbclog.print("[%04d] ExeSQL no match record \n",_index);
		return ;
	}
	_eof = false;
	return;
}  // OracleQuery::execute

//------------------------------------------------------------------------------
// OracleQuery::next
//------------------------------------------------------------------------------
void
OracleQuery::fetchNext()
{
	if (_eof)
		return;

	_oracleCurrentRow++;   
	_oracleGetResultSetRow();
	sword  ret=  OCIStmtFetch2(_parentConnection->_handles[_index]->_stmtHandle,_parentConnection->_handles[_index]->_errHandle,1,
								OCI_FETCH_NEXT,0,OCI_DEFAULT);
	if( ret	== OCI_NO_DATA )  
	{
		_eof = true;
		g_dbclog.print("[%04d] total row number is %d\n",_index,_oracleCurrentRow);
	}
	else
	{
		OciFunctionCheck(_index,"FetchNext",_parentConnection->_handles[_index]->_errHandle,ret );
	}
}  // OracleQuery::next


//------------------------------------------------------------------------------
// OracleQuery::transBegin
//------------------------------------------------------------------------------
void
OracleQuery::transBegin()
{
	// Make sure a transaction is not already active
	if (_isTransaction)
		throw TransactionError("transBegin(): "
		"A transaction is already active. Commit or rollback the transaction before creating a new transaction.");

	_isTransaction = true;

}  // OracleQuery::transBegin


//------------------------------------------------------------------------------
// OracleQuery::commit
//------------------------------------------------------------------------------
void
OracleQuery::commit()
{
	// Make sure a transaction is active
	if (!_isTransaction)
		throw TransactionError("commit(): "
		"A transaction is not active. Create a transaction before calling commit.");

	_isTransaction = false;   
}  // OracleQuery::commit


//------------------------------------------------------------------------------
// OracleQuery::rollback
//------------------------------------------------------------------------------
void
OracleQuery::rollback()
{
	// Make sure a transaction is active
	if (!_isTransaction)
		throw TransactionError("rollback(): "
		"A transaction is not active. Create a transaction before calling rollback.");

	_isTransaction = false;      
}  // OracleQuery::rollback


//------------------------------------------------------------------------------
// OracleQuery::getFieldInfoByColumn
//------------------------------------------------------------------------------
BaseFieldDescription*
OracleQuery::getFieldInfoByColumn(
								  int index)
{
	// Make sure the index is in range.
	if (index < 0 || index >= (int)_numFieldInformation)
		throw IndexOutOfRange("getFieldInfoByColumn(): The field index is out of range for the current result set");

	return _fieldInformation[index];      
}  // OracleQuery::getFieldInfoByColumn


//------------------------------------------------------------------------------
// OracleQuery::getFieldInfoByName
//------------------------------------------------------------------------------
BaseFieldDescription*
OracleQuery::getFieldInfoByName(
								const string& fieldName)
{
	// Try and find the field name
	for (unsigned int i=0; i<_numFieldInformation; i++)
		if (strcasecmp(_fieldInformation[i]->name().c_str(), fieldName.c_str()) == 0)
			return _fieldInformation[i];

	throw NameNotFound("getFieldInfoByName(): The field name was not found for the current result set.");   
}  // OracleQuery::getFieldInfoByName


//------------------------------------------------------------------------------
// OracleQuery::getFieldByColumn
//------------------------------------------------------------------------------
BaseValue*
OracleQuery::getFieldByColumn(
							  int index)
{
	// Make sure the index is in range.
	if (index < 0 || index >= (int)_numRecordValues)
	{
		throw IndexOutOfRange("getFieldByColumn(): The field index is out of range for the current result set");
	}

	return _recordValues[index];      
}  // OracleQuery::getFieldByColumn

//------------------------------------------------------------------------------
// OracleQuery::getFieldByName
//------------------------------------------------------------------------------
BaseValue*
OracleQuery::getFieldByName(
							const string& fieldName)
{
	// Try and find the field name
	for (unsigned int i=0; i<_numRecordValues; i++)
		if (strcasecmp(_recordValues[i]->name().c_str(), fieldName.c_str()) == 0)
			return _recordValues[i];

	throw NameNotFound("getFieldByName(): The field name was not found for the current result set.");   
}  // OracleQuery::getFieldByName


BaseValue*
OracleQuery::getOutParamByIndex(
								int index)
{
	map<int,OracleBindParam*>::iterator i = _outParam.find( index);

	if(i == _outParam.end())
		throw IndexOutOfRange("getOutParamByIndex(): The field index is out of range for the output param");

	return (*i).second;      
}  // OracleQuery::getFieldByColumn

#endif