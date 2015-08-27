/*
 * OracleFieldDescription object defines the needed attributes of ODBC fields
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
#ifndef __ORACLE_FIELDDESC_H__
#define __ORACLE_FIELDDESC_H__


#ifdef WIN32
#include <windows.h>
#endif

#include <string>

#include "dbconnect/base/baseFieldDescription.h"
#include "dbconnect/driver_oracle/oracle_inc/oci.h"

using namespace std;


class OracleFieldDescription : public BaseFieldDescription
{

public:
   /* Constructors.
    *
    */
   OracleFieldDescription(
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
      int		  dbcolSize);

	~OracleFieldDescription();
   /* Get the ODBC SQL field type.
    *
    * @return   Returns the field type.
    */
   int
   oracleFieldType();


   /* Get the ODBC SQL field size.
    *
    * @return   Returns the field size.
    */
   int
   oracleFieldSize();

   void *
   getFetchBuf();

   void 
	clearFetchBuf();

   OCIDefine *_definePtr;
   ub2 indicator;
   ub2 data_len;
private:
   int  _oracleFieldType;
   int  _dbcolSize;
   void *_fetchBuf;


};

#endif
#endif