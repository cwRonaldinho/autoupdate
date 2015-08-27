/*
 * OracleFieldDescription object defines the needed attributes of Oracle fields
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
#include "oraclefielddescription.h"

#define new DEBUG_NEW
//------------------------------------------------------------------------------
// OracleFieldDescription::OracleFieldDescription
//------------------------------------------------------------------------------
OracleFieldDescription::OracleFieldDescription(
	string&     name,
	long int    position,
	FieldType   type,
	bool        isIncrement,
	bool        isPriKey,
	bool        isUnique,
	bool        isNotNull,
	long int    precision,
	long int    scale,
	int   	  oraFieldType,
	int		  dbcolSize)

:  BaseFieldDescription(name, position, type, isIncrement, isPriKey,
                        isUnique, isNotNull, precision, scale),
   _oracleFieldType(oraFieldType),
   _dbcolSize(dbcolSize)
{
	_fetchBuf = (void *)new char[_dbcolSize+1];
	memset(_fetchBuf,0,dbcolSize+1);
	_definePtr = NULL;
	indicator = 0;
	data_len = 0;


}  // OracleFieldDescription::::OracleFieldDescription::          


//------------------------------------------------------------------------------
// OracleFieldDescription::odbcFieldType
//------------------------------------------------------------------------------
int
OracleFieldDescription::oracleFieldType()
{
   return _oracleFieldType;   
}  // OracleFieldDescription::odbcFieldType          


//------------------------------------------------------------------------------
// OracleFieldDescription::odbcFieldSize
//------------------------------------------------------------------------------
int
OracleFieldDescription::oracleFieldSize()
{
   return _dbcolSize;   
}  // OracleFieldDescription::odbcFieldSize


void* OracleFieldDescription::getFetchBuf()
{
	return (void*)_fetchBuf;
}

void OracleFieldDescription::clearFetchBuf()
{
	memset(_fetchBuf,0,_dbcolSize+1);
	indicator = 0;
	data_len = 0;
}


OracleFieldDescription::~OracleFieldDescription()
{
	delete[] _fetchBuf;
}

#endif