#ifdef _DB_SYBASE_
#include "stdafx.h"

#include "dbconnect/driver_sybase/SybaseFieldDescription.h"

//------------------------------------------------------------------------------
// SybaseFieldDescription::SybaseFieldDescription
//------------------------------------------------------------------------------
SybaseFieldDescription::SybaseFieldDescription(
	string&     name,
	long int    position,
	FieldType   type,
	bool        isIncrement,
	bool        isPriKey,
	bool        isUnique,
	bool        isNotNull,
	long int    precision,
	long int    scale,
	BYTE		  sybaseFieldType,
	DBINT       dbcolSize)  
:  BaseFieldDescription(name, position, type, isIncrement, isPriKey,
                        isUnique, isNotNull, precision, scale),
   _sybaseSQLFieldType(sybaseFieldType),
   _dbcolSize(dbcolSize)
{

}  // DB2FieldDescription::::DB2FieldDescription::          


//------------------------------------------------------------------------------
// DB2FieldDescription::db2FieldType
//------------------------------------------------------------------------------
BYTE
SybaseFieldDescription::sybaseFieldType()
{
   return _sybaseSQLFieldType;   
}  // DB2FieldDescription::db2FieldType          


//------------------------------------------------------------------------------
// DB2FieldDescription::db2FieldSize
//------------------------------------------------------------------------------
DBINT
SybaseFieldDescription::sybaseFieldSize()
{
   return _dbcolSize;   
}  // SybaseFieldDescription::SybaseFieldDescription
#endif