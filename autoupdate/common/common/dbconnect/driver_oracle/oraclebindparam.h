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

#ifdef _DB_ORACLE_
#ifndef __ORACLE_BINDPARAM_H__
#define __ORACLE_BINDPARAM_H__

#ifdef WIN32
#include <windows.h>
#endif

#include <string>

#include "dbconnect/driver_oracle/oracle_inc/oci.h"

#include "dbconnect/base/baseValue.h"


using namespace std;


class OracleBindParam : public BaseValue
{
private: 
	int		_bindType;
	int		_index;
	bool	_outparam;
	int		_ocitype;
	int		_rettype;
public:

	/* Constructor.
	*
	*/
	OracleBindParam(
		const string& bindName, bool outparam, int type = FT_UNKNOWN);

	OracleBindParam(
		int index, bool outparam, int type = FT_UNKNOWN);

	
	int getBindType () { return _bindType; }
	int getBindIndex() { return _index; }
	int getRetType()   { return _rettype;}
	int getOciType() ;
	void setOciType();
	bool isOutParam(){return _outparam;};
	enum  {
		BIND_BY_NAME,
		BIND_BY_INDEX
	};

	OCIBind *bindObjPtr;
	sb2		indicator; 

};


#endif
#endif