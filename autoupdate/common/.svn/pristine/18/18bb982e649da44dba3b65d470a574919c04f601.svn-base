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
 *
 * Notes:
 * 1. A deep copy of the value is made for all types except binary types.
 * 2. Windows does not like returning a string type for the asString function :/
 *    Hence the workaround using the _strBuffer
 *
 */

#ifndef __BASE_VALUE_H__
#define __BASE_VALUE_H__

#ifdef WIN32
   // Windows
   // DLL Exports
   class  __declspec( dllexport ) BaseValue;
#endif   


#include <string>

#include "dbconnectTypes.h"
#include "jDate.h"


using namespace std;

class BaseValue
{
private:
   char *_strBuffer;


   /* Internal function to release any memory allocated
    *
    */
   void
   _releasePtr();


protected:  
   enum BaseValueType
   {
      BP_UNKNOWN,
      BP_NULL,
   	BP_STRING,
   	BP_LONG,
   	BP_ULONG,
   	BP_DOUBLE,
   	BP_DATETIME,
      BP_DATE,
      BP_TIME,
   	BP_BOOLEAN,
   	BP_BINARY
   };

   union BaseValuePtr
   {
      void       *ptr;
      char       *charPtr;
      DBLONG     *longPtr;      
      DBULONG    *unsignedLongPtr;
      double     *doublePtr;
      JDate      *JDatePtr;
      bool       *boolPtr;
   };


public:
   string          bindName;     // Name of the field to bind.
   BaseValuePtr    valuePtr;     // Pointer to the value,
   DBULONG         valueSize;    // Size of the variable.
   BaseValueType   type;         // Bind Parameter type.



   /* Constructors.
    *
    */
   BaseValue(
      const string& fieldName);


   /* Destructor.
    *
    */
   virtual 
   ~BaseValue();
   

   /* Return the name of the value
    *
    * @return  The name
    *
    */
   virtual string
   name();


   /* Set the value is special type NULL.
    *
    */
   virtual void 
   setNULL();


   /* Set the bind parameter as a string.
    *
    * @param bvar       String to bind.
    *
    */
   virtual void 
   setString(
      const char* bindVar);


   /* Set the bind parameter as a string.
    *
    * @param bvar       String to bind.
    *
    */
   virtual void 
   setString(
      const string &bindVar);



   /* Set the bind parameter as a long integer.
    *
    * @param bvar       Signed long value to bind
    *
    */
   virtual void 
   setLong(
      DBLONG bindVar);



   /* Set the bind parameter as an unsigned long integer.
    *
    * @param bvar       Unsigned long value to bind
    *
    */
   virtual void 
   setUnsignedLong(
      DBULONG bindVar);



   /* Set the bind parameter as a floating point number.
    *
    * @param bvar       Double value to bind
    *
    */
   virtual void 
   setFloat(
      double bindVar);



   /* Set the bind parameter as a JDate object
    *
    * @param bvar       JDate object representing the date and time to bind
    *
    */
   virtual void 
   setDateTime(
      JDate &bindVar);


   /* Set the bind parameter as a JDate object
    * The interpretation will only use the date part and ignore the time
    *
    * @param bvar       JDate object representing the date to bind
    *
    */
   virtual void 
   setDate(
      JDate &bindVar);


   /* Set the bind parameter as a JDate object
    * The interpretation will only use the time part and ignore the date
    *
    * @param bvar       JDate object representing the time to bind
    *
    */
   virtual void 
   setTime(
      JDate &bindVar);


   /* Set the bind parameter as a boolean.
    *
    * @param bvar       Bool value to bind
    *
    */
   virtual void 
   setBoolean(
      bool bindVar);



   /* Set the bind parameter as a raw binary data. This will not make a deep copy
    *
    * @param bvar       Pointer containing the raw binary data
    * @param bvarsize   Size of the data
    *
    */
   virtual void 
   setBinary(
      void *bindVar, 
      unsigned long bindVarSize);
   

   /* Get the size of the value stored
    *
    * @return  The size of the value
    *
    */
   virtual DBULONG
   getSize();


   /* Checks if the value is the special value NULL
    *
    * @return  True if value is special value NULL
    *
    */
   virtual bool
   isNULL();


   /* Return the stored value as a string converting if neccessary
    *
    * @return  The stored value as a string
    *
    */
   virtual const char*
   asString();


   /* Return the stored value as a long converting if neccessary
    *
    * @return  The stored value as a long
    *
    */
   virtual DBLONG
   asLong();


   /* Return the stored value as a unsinged long converting if neccessary
    *
    * @return  The stored value as a unsinged long
    *
    */
   virtual DBULONG
   asUnsignedLong();


   /* Return the stored value as a floating point value converting if neccessary
    *
    * @return  The stored value as a floating point value
    *
    */
   virtual double
   asFloat();


   /* Return the stored value as a JDate datetime Object
    *
    * @return  The stored value as a JDate datetime value
    *
    */
   virtual JDate
   asDateTime();


   /* Return the stored value as boolean 
    *
    * @return  The stored value as a boolean value
    *
    */
   virtual bool
   asBoolean();


   /* Return the stored value as a pointer
    *
    * @return  The stored value as a pointer
    *
    */
   virtual void*
   asBinary();


};


#endif
