/*
 * ODBCBindParam object defines the needed conversion of bind parameters
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

#ifndef __ODBC_BINDPARAM_H__
#define __ODBC_BINDPARAM_H__

#include <windows.h>
#include <sql.h>
#include <sqlext.h>

#include <string>


#include "baseValue.h"



using namespace std;


class ODBCBindParam : public BaseValue
{
private: 
   // Internal values used to store translated Base Values for ODBC bind parameters
   SQLINTEGER        _strlenIndPtr;

   long int          _odbcLongInt;
   unsigned long int _odbcULongInt;
   unsigned char     _odbcBool;
   TIMESTAMP_STRUCT  _odbcTimeStamp;
   DATE_STRUCT       _odbcDate;   
   TIME_STRUCT       _odbcTime;

public:
 
   /* Constructor.
    *
    */
   ODBCBindParam(
         const string& bindName)
         : BaseValue(bindName) 
   { };

   
   /* Returns the ODBC "C" bind paramter type based on the base value type stored
    *
    * @return   Returns the ODBC "C" type required for the value stored
    *
    */
   SQLSMALLINT
   getODBCCType();
   

   /* Returns a ODBC compliant pointer for the bind parameter
    *
    * @return   Returns the a pointer to the data stored by the parameter
    *
    */
   SQLPOINTER
   getODBCDataPointer();
   
   
   /* Returns the length of the ODBC compliant data 
    *
    * @return   Returns the length of the data.
    *
    */
   SQLINTEGER
   getODBCDataPointerLength();


   /* Returns the required StrLenIndPtr value
    *
    * @return   Returns the StrLenIndPtr value.
    *
    */
   SQLINTEGER*
   getODBCStrLenIndPtr();

};


#endif
