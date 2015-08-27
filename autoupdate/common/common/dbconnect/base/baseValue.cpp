/*
 * BaseValue defines a generic value storage and conversion class
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
 * Note: The class makes a deep copy of the values
 *
 */

#include "baseValue.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "hexDigest.h"

//------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// _strtoll
//------------------------------------------------------------------------------
DBLONG
_strtoll(
      const char *nptr, 
      int base)
{
   /* Conversion Range: -9223372036854775808 to 9223372036854775807 */
   DBLONG res = 0 ;
   const  char * ptr = nptr;
   if (!base) 
      base = 10;
   if(*ptr == '-' || *ptr == '+')
	   ptr++;
   for( ;*ptr; ptr++)
   {
	   if(!isdigit(*ptr))
		   break;
	   res = res*base + (*ptr - '0');
   }
   // Check if we have a - sign & negate.
   if (nptr[0] == '-')
      res *= -1;
   return res;
}  // _strtoll


//Private function to convert a string to a unsigned long long integer
DBULONG 
_strtoull(
      const char *nptr, 
      int base)
{
   /* Conversion Range: 0 to 18446744073709551615 */
   DBULONG res = 0;
   if (!base) 
      base = 10;
   const  char * ptr = nptr;
   for( ;*ptr; ptr++)
   {
	   if(!isdigit(*ptr))
		   break;
	   res = res*base + (*ptr - '0');
   }
   return res;
}  // _strtoull          


//------------------------------------------------------------------------------
// BaseValue::_releasePtr
//------------------------------------------------------------------------------
void
BaseValue::_releasePtr()
{
   if (valuePtr.ptr)
   {
      switch (type)
      {
         case BP_STRING:
            free((void*)valuePtr.charPtr);
            break;         

         case BP_LONG:
            delete valuePtr.longPtr;
            break;

         case BP_ULONG:
            delete valuePtr.unsignedLongPtr;
            break;

         case BP_DOUBLE:
            delete valuePtr.doublePtr;
            break;

         case BP_DATETIME:
         case BP_DATE:
         case BP_TIME:
            delete valuePtr.JDatePtr;
            break;

         case BP_BOOLEAN:
            delete valuePtr.boolPtr;
            break;

         case BP_BINARY:
            free(valuePtr.ptr);
            break;
         
         // Don't free BP_NULL as a deep copy is not made
      }

      valuePtr.ptr = NULL;
      valueSize = 0;
      type = BP_UNKNOWN;
   }

   // Free any internal buffers
   if (_strBuffer)
   {
      free(_strBuffer);
      _strBuffer = NULL;
   }
}  // BaseValue::_releasePtr          


//------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// BaseValue::BaseValue
//------------------------------------------------------------------------------
BaseValue::BaseValue(
      const string& fieldName)   
   :
      _strBuffer(NULL)
{
   valuePtr.ptr = NULL;
   valueSize = 0;
   type = BP_UNKNOWN;
   
   // Convert the parameter name to all lowercase
   BaseValue::bindName = fieldName.c_str();
   for (unsigned int i=0; i<BaseValue::bindName.length(); i++)
      BaseValue::bindName[i] = tolower(bindName[i]);
}  // BaseValue::BaseValue          


//------------------------------------------------------------------------------
// BaseValue::~BaseValue
//------------------------------------------------------------------------------
BaseValue::~BaseValue()
{   
   // Release the bind parameter and free any deep copies made
   _releasePtr();
}  // BaseValue::~BaseValue


//------------------------------------------------------------------------------
// BaseValue::name
//------------------------------------------------------------------------------
string
BaseValue::name()
{
   return bindName;
}  // BaseValue::name


//------------------------------------------------------------------------------
// BaseValue::setNULL
//------------------------------------------------------------------------------
void 
BaseValue::setNULL()
{
   _releasePtr();

   type = BP_NULL;
   valuePtr.ptr = NULL;

   valueSize = 0;
}  // BaseValue::setNULL


//------------------------------------------------------------------------------
// BaseValue::setString
//------------------------------------------------------------------------------
void 
BaseValue::setString(
   const char* bindVar)
{
   _releasePtr();

   type = BP_STRING;
   valuePtr.charPtr = strdup(bindVar);
   
   valueSize =(DBULONG) strlen(valuePtr.charPtr) + 1;
}  // BaseValue::setString


//------------------------------------------------------------------------------
// BaseValue::setString
//------------------------------------------------------------------------------
void 
BaseValue::setString(
   const string &bindVar)
{
   _releasePtr();

   type = BP_STRING;
   valuePtr.charPtr = strdup(bindVar.c_str());
   
   valueSize =(DBULONG) strlen(valuePtr.charPtr);
}  // BaseValue::setString


//------------------------------------------------------------------------------
// BaseValue::setLong
//------------------------------------------------------------------------------
void 
BaseValue::setLong(
   DBLONG bindVar) 
{
   _releasePtr();

   type = BP_LONG;
   valuePtr.longPtr = new DBLONG;

   *(valuePtr.longPtr) = bindVar;
   valueSize = sizeof(DBLONG);
}  // BaseValue::setLong


//------------------------------------------------------------------------------
// BaseValue::setUnsignedLong
//------------------------------------------------------------------------------
void 
BaseValue::setUnsignedLong(
   DBULONG bindVar)
{
   _releasePtr();

   type = BP_ULONG;
   valuePtr.unsignedLongPtr = new DBULONG;

   *(valuePtr.unsignedLongPtr) = bindVar;
   valueSize = sizeof(DBULONG);
}  // BaseValue::setUnsignedLong


//------------------------------------------------------------------------------
// BaseValue::setFloat
//------------------------------------------------------------------------------
void 
BaseValue::setFloat(
   double bindVar)
{
   _releasePtr();

   type = BP_DOUBLE;
   valuePtr.doublePtr = new double;

   *(valuePtr.doublePtr) = bindVar;
   valueSize = sizeof(double);
}  // BaseValue::setFloat


//------------------------------------------------------------------------------
// BaseValue::setDateTime
//------------------------------------------------------------------------------
void 
BaseValue::setDateTime(
   JDate &bindVar) 
{
   _releasePtr();

   type = BP_DATETIME;
   valuePtr.JDatePtr = new JDate();

   valuePtr.JDatePtr->setDate(bindVar);
   valueSize = sizeof(JDate);
}  // BaseValue::setDateTime


//------------------------------------------------------------------------------
// BaseValue::setDate
//------------------------------------------------------------------------------
void 
BaseValue::setDate(
   JDate &bindVar) 
{
   _releasePtr();

   type = BP_DATE;
   valuePtr.JDatePtr = new JDate();

   valuePtr.JDatePtr->setDate(bindVar);
   valueSize = sizeof(JDate);
}  // BaseValue::setDate


//------------------------------------------------------------------------------
// BaseValue::setTime
//------------------------------------------------------------------------------
void 
BaseValue::setTime(
   JDate &bindVar) 
{
   _releasePtr();

   type = BP_TIME;
   valuePtr.JDatePtr = new JDate();

   valuePtr.JDatePtr->setDate(bindVar);
   valueSize = sizeof(JDate);
}  // BaseValue::setTime


//------------------------------------------------------------------------------
// BaseValue::setBoolean
//------------------------------------------------------------------------------
void 
BaseValue::setBoolean(
   bool bindVar)
{
   _releasePtr();

   type = BP_BOOLEAN;
   valuePtr.boolPtr = new bool;

   *(valuePtr.boolPtr) = bindVar;
   valueSize = sizeof(bool);
}  // BaseValue::setBoolean


//------------------------------------------------------------------------------
// BaseValue::setBinary
//------------------------------------------------------------------------------
void 
BaseValue::setBinary(
   void *bindVar, 
   unsigned long bindVarSize)
{
   _releasePtr();

   type = BP_BINARY;
   valuePtr.ptr = malloc(bindVarSize);
   
   memcpy(valuePtr.ptr, bindVar, bindVarSize);
   valueSize = bindVarSize;
}  // BaseValue::setBinary


//------------------------------------------------------------------------------
// BaseValue::getSize
//------------------------------------------------------------------------------
DBULONG
BaseValue::getSize()
{
   return valueSize;
}  // BaseValue::getSize


//------------------------------------------------------------------------------
// BaseValue::isNULL
//------------------------------------------------------------------------------
bool
BaseValue::isNULL()
{
   if (type == BP_NULL)
      return true;
   else
      return false;
}  // BaseValue::isNULL


//------------------------------------------------------------------------------
// BaseValue::asString
//------------------------------------------------------------------------------
const char*
BaseValue::asString()
{
   string res;

   // Check if the field is NULL
   if (!valuePtr.ptr || type == BP_NULL)
      return "NULL";

   

   char *bufPtr = NULL;
   char buf[256] = {0};
   
   // Convert 
   switch (type)
   {
      case BP_STRING:
         res = valuePtr.charPtr;
         break;         

      case BP_LONG:
         snprintf(buf, 255, DBLONG_SPRINTF, *(valuePtr.longPtr));
         res = buf;
         break;

      case BP_ULONG:
         snprintf(buf, 255, DBULONG_SPRINTF, *(valuePtr.unsignedLongPtr));
         res = buf;
         break;

      case BP_DOUBLE:
          {
			  int len = snprintf(buf, 255,"%#f", *(valuePtr.doublePtr));
			  for( int i = len-1 ; i>0 ; i--)
			  {
				  if( '0' == buf[i] )
					  continue;
				  if( '.' == buf[i]) 
				    buf[i+2] = '\0';
				  else
					buf[i+1] = '\0';
				  break;
			  }
			  res = buf;
		  }
		  break;

      case BP_DATETIME:
         res = valuePtr.JDatePtr->asString("%Y-%m-%d %H:%M:%S").c_str();
         break;

      case BP_DATE:
         res = valuePtr.JDatePtr->asString("%Y-%m-%d").c_str();
         break;

      case BP_TIME:
         res = valuePtr.JDatePtr->asString("%H:%M:%S").c_str();
         break;

      case BP_BOOLEAN:
         if (*(valuePtr.boolPtr))
            res = "True";
         else
            res = "False";
         break;

      case BP_BINARY:
         // Return the result as a hex encoded string.
         char *hexRes = (char*)malloc((2*valueSize)+1);
         HexDigest::encode((unsigned char*)valuePtr.ptr, valueSize, hexRes);               
         res = hexRes;         
         free(hexRes);
         break;
   }
   
   // Make a deep copy of the generated result
   if (_strBuffer)
   {
      free(_strBuffer);
      _strBuffer = NULL;
   }
   _strBuffer = strdup(res.c_str());      
   return (const char*)_strBuffer;
}  // BaseValue::asString


//------------------------------------------------------------------------------
// BaseValue::asLong
//------------------------------------------------------------------------------
DBLONG
BaseValue::asLong()
{   
   if (!valuePtr.ptr || type == BP_NULL)
      return 0;   

   DBLONG res = 0;

   // Convert 
   switch (type)
   {
      case BP_STRING:
         res = _strtoll(valuePtr.charPtr, 10);
         break;         

      case BP_LONG:
         res = *(valuePtr.longPtr);
         break;

      case BP_ULONG:
         res = (DBLONG)*(valuePtr.unsignedLongPtr);
         break;

      case BP_DOUBLE:
         res = (DBLONG)*(valuePtr.doublePtr);
         break;

      case BP_DATETIME:
      case BP_DATE:
      case BP_TIME:
         // Try and return the unix time stamp otherwise 0
         try
         {
            res = (DBULONG) valuePtr.JDatePtr->asUnixTimeStamp();
         }
         catch(...)
         {
            // Date is out of range for a unix timestamp
            res = 0;
         }
         break;

      case BP_BOOLEAN:
         if (*(valuePtr.boolPtr))
            res = 1;
         else
            res = 0;
         break;

      case BP_BINARY:
         // Always return zero as this is undefined :/
         res = 0;
         break;
   }

   return res;
}  // BaseValue::asLong



//------------------------------------------------------------------------------
// BaseValue::asUnsignedLong
//------------------------------------------------------------------------------
DBULONG
BaseValue::asUnsignedLong()
{
   if (!valuePtr.ptr || type == BP_NULL)
      return 0;   

   DBULONG res = 0;

   // Convert 
   switch (type)
   {
      case BP_STRING:
         res = _strtoull(valuePtr.charPtr, 10);
         break;         

      case BP_LONG:
         res = (DBULONG)*(valuePtr.longPtr);
         break;

      case BP_ULONG:
         res = *(valuePtr.unsignedLongPtr);
         break;

      case BP_DOUBLE:
         res = (DBULONG)*(valuePtr.doublePtr);
         break;

      case BP_DATETIME:
      case BP_DATE:
      case BP_TIME:
         // Try and return the unix time stamp otherwise 0
         try
         {
            res = (DBULONG)valuePtr.JDatePtr->asUnixTimeStamp();
         }
         catch(...)
         {
            // Date is out of range for a unix timestamp
            res = 0;
         }
         break;

      case BP_BOOLEAN:
         if (*(valuePtr.boolPtr))
            res = 1;
         else
            res = 0;
         break;

      case BP_BINARY:
         // Always return zero as this is undefined :/
         res = 0;
         break;
   }

   return res;   
}  // BaseValue::asUnsignedLong



//------------------------------------------------------------------------------
// BaseValue::asFloat
//------------------------------------------------------------------------------
double
BaseValue::asFloat()
{
   if (!valuePtr.ptr || type == BP_NULL)
      return 0.0;   

   double res = 0.0;

   // Convert 
   switch (type)
   {
      case BP_STRING:
         res = strtod(valuePtr.charPtr, NULL);
         break;         

      case BP_LONG:
         res = (double)*(valuePtr.longPtr);
         break;

      case BP_ULONG:
         res = (double)*(valuePtr.unsignedLongPtr);
         break;

      case BP_DOUBLE:
         res = *(valuePtr.doublePtr);
         break;

      case BP_DATETIME:
      case BP_DATE:
      case BP_TIME:
         // Return the number as the Julian Day Number.
         // See the JDate class for more details
         res = valuePtr.JDatePtr->asJulianDayNumber();
         break;

      case BP_BOOLEAN:
         if (*(valuePtr.boolPtr))
            res = 1.0;
         else
            res = 0.0;
         break;

      case BP_BINARY:
         // Always return zero as this is undefined :/
         res = 0.0;
         break;
   }

   return res;
}  // BaseValue::asFloat


//------------------------------------------------------------------------------
// BaseValue::asDateTime
//------------------------------------------------------------------------------
JDate
BaseValue::asDateTime()
{
   if (!valuePtr.ptr || type == BP_NULL)
      return JDate(0.0);   

   JDate res(0.0);  // Set to Julian Day Epoch
   
   const char *fmt[8] = {JDate::SET_FMT_DATETIME_ISO, JDate::SET_FMT_DATETIME_ISO_TZ, 
                         JDate::SET_FMT_DATE_ISO, JDate::SET_FMT_TIME_ISO, 
                         JDate::SET_FMT_TIME_ISO_TZ, JDate::SET_FMT_SERIAL_ISO, 
                         JDate::SET_FMT_SERIAL_ISO_TZ, "yyyy"};
   int i;
      
   // Convert 
   switch (type)
   {
      case BP_STRING:
         // Try and convert the date using all possible formats starting with the most common
         // Can be in one of the following formats
         //   - yyyy-mm-dd hh:nn:ss
         //   - yyyy-mm-dd hh:nn:sszzz
         //   - yyyy-mm-dd
         //   - hh:nn:ss
         //   - hh:nn:sszzz
         //   - yyyymmddhhnnss
         //   - yyyymmddhhnnsszzz
         //   - yyyy
         for(i=0; i<8; i++)
         { 
            try
            {
               res.setDate(valuePtr.charPtr, fmt[i]);
               break;  // We have managed to convert
            }
            catch(...)
            { }  // Date not in the correct format yet.
         }
         
         break;         

      case BP_LONG:
         try
         {
            res.setDate((time_t)*valuePtr.longPtr);
         }
         catch(...)
         { }
         break;

      case BP_ULONG:
         try
         {
            res.setDate((time_t)*(valuePtr.unsignedLongPtr));
         }
         catch(...)
         { }
         break;

      case BP_DOUBLE:
         try
         {
            res.setDate(*valuePtr.doublePtr);  // Sets in Julian Day Number
         }
         catch(...)
         { }              
         break;

      case BP_DATETIME:
      case BP_DATE:
      case BP_TIME:
         try
         {
            res.setDate(*valuePtr.JDatePtr);
         }
         catch(...)
         { }
         break;

      case BP_BOOLEAN:
         // Always return epoch as this is undefined :/
         res.setDate(0.0);
         break;

      case BP_BINARY:
         // Always return epoch as this is undefined :/
         res.setDate(0.0);
         break;
   }
    
   return res;   
}  // BaseValue::asDateTime


//------------------------------------------------------------------------------
// BaseValue::asBoolean
//------------------------------------------------------------------------------
bool
BaseValue::asBoolean()
{
   if (!valuePtr.ptr || type == BP_NULL)
      return false;   

   bool res = false;  
   DBLONG check = 0;
   

   // Convert 
   switch (type)
   {
      case BP_STRING:
         // Check if the text of the string has a boolean representation
         if (strcasecmp("true", valuePtr.charPtr) == 0)
            res = true;
         
         if (strcasecmp("t", valuePtr.charPtr) == 0)
            res = true;         
         
         // Finally check if an integer value is present in the string
         if (!res)
         {
            check = _strtoll(valuePtr.charPtr, 10);
            res = check > 0;
         }
         break;

      case BP_LONG:
         res = *(valuePtr.longPtr) > 0L;
         break;

      case BP_ULONG:
         res = *(valuePtr.unsignedLongPtr) > 0UL;
         break;

      case BP_DOUBLE:
         res = *(valuePtr.doublePtr) > 0.0;
         break;

      case BP_DATETIME:
      case BP_DATE:
      case BP_TIME:
         res = false;
         break;

      case BP_BOOLEAN:
         res = *(valuePtr.boolPtr);
         break;

      case BP_BINARY:
         // Always return false as this is undefined :/
         res = false;
         break;
   }
     
   return res;   
}  // BaseValue::asBoolean


//------------------------------------------------------------------------------
// BaseValue::asBinary
//------------------------------------------------------------------------------
void*
BaseValue::asBinary()
{
   return valuePtr.ptr;     // So what did you expect, fancy mmx assembler optimized code ? :)
}  // BaseValue::asBinary
