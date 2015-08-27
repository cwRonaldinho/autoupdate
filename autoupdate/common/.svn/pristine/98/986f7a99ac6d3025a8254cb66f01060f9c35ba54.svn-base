/*
* OracleBindParam object defines the needed conversion of bind parameters
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
#include "dbconnect/driver_oracle/oraclebindparam.h"
#include "dbconnect/base/dbconnectTypes.h"

#define new DEBUG_NEW
// -----------------------------------------------------------------------------
// PRIVATE:
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// PUBLIC:
// -----------------------------------------------------------------------------
OracleBindParam::OracleBindParam(const std::string &bindName,bool outparam , int type)
:BaseValue(bindName)
{
	_bindType = BIND_BY_NAME;
	bindObjPtr = NULL;
	_outparam = outparam;
	_rettype = type;
	indicator = OCI_IND_NULL;
	setOciType();
}

OracleBindParam::OracleBindParam(int index,bool outparam, int type)
:BaseValue("index")
{
	_bindType = BIND_BY_INDEX;
	bindObjPtr = NULL;
	_index = index;
	_outparam = outparam;
	_rettype = type;
	indicator = OCI_IND_NULL;
	setOciType();
}
//------------------------------------------------------------------------------
// OracleBindParam::
//------------------------------------------------------------------------------
void OracleBindParam::setOciType()
{
	int size = 1024;
	switch( _rettype )
	{
	case FT_STRING:
		_ocitype = SQLT_STR;
		break;

	case FT_SHORT:
	case FT_UNSIGNED_SHORT:
	case FT_UNSIGNED_LONG:
	case FT_LONG:
		if(_outparam)
			_ocitype = SQLT_VNU;
		else
			_ocitype = SQLT_INT;
		break;

	case FT_DOUBLE:
		_ocitype = SQLT_VNU;
		break;

	case FT_DATETIME:
		_ocitype = SQLT_DAT;
		break;
	case FT_RESULTSET:
		_ocitype = SQLT_RSET;
		break;
	default:
		_rettype = FT_STRING;
		_ocitype = SQLT_STR;
		break;
	}
	if(_outparam){
		char buffer[1024] = {0};
		memset( buffer,0,sizeof(buffer));
		setBinary(buffer,size);
	}
}


int OracleBindParam::getOciType()
{
	return _ocitype;
}
#endif