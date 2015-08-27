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

#include "stdafx.h"

#include "odbcbindparam.h"
#include "dbconnectTypes.h"

// -----------------------------------------------------------------------------
// PRIVATE:
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// PUBLIC:
// -----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ODBCBindParam::getODBCCType
//------------------------------------------------------------------------------
SQLSMALLINT
ODBCBindParam::getODBCCType()
{
   SQLSMALLINT res;

   switch(type)
   {
      case BP_NULL:
         res = SQL_C_DEFAULT;
         break;
      
   	case BP_STRING:
         res = SQL_C_CHAR;
         break;

   	case BP_LONG:
         res = SQL_C_LONG;
         break;
   	
      case BP_ULONG:
         res = SQL_C_ULONG;
         break;

   	case BP_DOUBLE:
         res = SQL_C_DOUBLE;
         break;

   	case BP_BOOLEAN:
         res = SQL_C_BIT;
         break;

   	case BP_DATETIME:
         res = SQL_C_TIMESTAMP;
         break;
            
      case BP_DATE:
         res = SQL_C_DATE;
         break;
      
      case BP_TIME:
         res = SQL_C_TIME;
         break;

   	case BP_BINARY:
         res = SQL_C_BINARY;
         break;

      case BP_UNKNOWN:
      default:
         throw BindParameterError("getODBCCType(): Unable to determine ODBC \"C\" bind parameter type for paramter value stored");
         break;
   }
   
   return res;
}  // ODBCBindParam::getODBCCType


//------------------------------------------------------------------------------
// ODBCBindParam::getODBCDataPointer
//------------------------------------------------------------------------------
SQLPOINTER
ODBCBindParam::getODBCDataPointer()
{
   SQLPOINTER res = NULL;
   
   // This will use the baseValue internals
   switch(type)
   {
      case BP_NULL:
         res = NULL;
         break;
      
   	case BP_STRING:
         res = valuePtr.charPtr;
         break;
      
   	case BP_LONG:
         // Make a local copy of the value as types are different.
         _odbcLongInt = (long int)*(valuePtr.longPtr);
         res = &_odbcLongInt;
         break;

      case BP_ULONG:
         // Make a local copy of the value as types are different.
         _odbcULongInt = (unsigned long int)*(valuePtr.unsignedLongPtr);
         res = &_odbcULongInt;
         break;
      
   	case BP_DOUBLE:
         res = valuePtr.doublePtr;
         break;

   	case BP_BOOLEAN:
         _odbcBool = (unsigned char)asBoolean();
         res = &_odbcBool;
         break;

   	case BP_DATETIME:
         // Make a local copy of the value as types are different.
         _odbcTimeStamp.year = asDateTime().asComponent(JDate::YEARS);
         _odbcTimeStamp.month = asDateTime().asComponent(JDate::MONTHS);
         _odbcTimeStamp.day = asDateTime().asComponent(JDate::DAYS);
         _odbcTimeStamp.hour = asDateTime().asComponent(JDate::HOURS);
         _odbcTimeStamp.minute = asDateTime().asComponent(JDate::MINUTES);
         _odbcTimeStamp.second = asDateTime().asComponent(JDate::SECONDS);
         _odbcTimeStamp.fraction = 0;    // TODO: Get JDate to support fraction of a second         
         res = &_odbcTimeStamp;
         break;

      case BP_DATE:
         // Make a local copy of the value as types are different.
         _odbcDate.year = asDateTime().asComponent(JDate::YEARS);
         _odbcDate.month = asDateTime().asComponent(JDate::MONTHS);
         _odbcDate.day = asDateTime().asComponent(JDate::DAYS);
         res = &_odbcDate;         
         break;

      case BP_TIME:
         // Make a local copy of the value as types are different.
         _odbcTime.hour = asDateTime().asComponent(JDate::HOURS);
         _odbcTime.minute = asDateTime().asComponent(JDate::MINUTES);
         _odbcTime.second = asDateTime().asComponent(JDate::SECONDS);
         res = &_odbcTime;
         break;

   	case BP_BINARY:
         res = (unsigned char*)valuePtr.ptr;
         break;
         
      case BP_UNKNOWN:
      default:
         throw BindParameterError("getODBCDataPointer(): Unable to create ODBC bind parameter data pointer");
         break;
   }
   
   return res;
}  // ODBCBindParam::getODBCDataPointer

//------------------------------------------------------------------------------
// ODBCBindParam::getODBCDataPointerLength
//------------------------------------------------------------------------------
SQLINTEGER
ODBCBindParam::getODBCDataPointerLength()
{
   // TODO: For OUTPUT Params this function will be required to change 
   SQLINTEGER res = 0;
   
   switch(type)
   {
   	case BP_STRING:
   	case BP_BINARY:
         res = valueSize;
         break;
      
      case BP_UNKNOWN:
         throw BindParameterError("getODBCDataPointerLength(): Unable to determine ODBC bind parameter data length");
         break;

      default:
         res = 0;
         break;
   }

   return res;   
}  // ODBCBindParam::getODBCDataPointerLength


//------------------------------------------------------------------------------
// ODBCBindParam::getODBCStrLenIndPtr
//------------------------------------------------------------------------------
SQLINTEGER*
ODBCBindParam::getODBCStrLenIndPtr()
{
   _strlenIndPtr = 0;
   
   switch(type)
   {
      case BP_NULL:
         _strlenIndPtr = SQL_NULL_DATA;
         break;

   	case BP_STRING:
         _strlenIndPtr = SQL_NTS;
         break;
      
   	case BP_BINARY:
         _strlenIndPtr = valueSize;
         break;

      case BP_UNKNOWN:
         throw BindParameterError("getODBCStrLenIndPtr(): Unable to determine ODBC bind parameter Str Len/Ind Ptr");
         break;
      
      default:
         _strlenIndPtr = 0;
         break;
   }
   
   if (_strlenIndPtr == 0)
      return NULL;
   else
      return &_strlenIndPtr;      
}  // ODBCBindParam::getODBCStrLenIndPtr
