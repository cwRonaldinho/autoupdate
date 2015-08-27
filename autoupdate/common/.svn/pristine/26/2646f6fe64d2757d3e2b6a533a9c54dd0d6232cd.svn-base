/*
 * BaseException defines the object that all other exceptions will inherit from
 * Copyright (C) 2002 Johnathan Ingram, jingram@rogue-order.net
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


#ifndef __BASEEXCEPTION_H__
#define __BASEEXCEPTION_H__

#ifdef WIN32
   // Windows
   // DLL Exports
   #pragma warning(disable: 4251)
   class  __declspec( dllexport ) BaseException;
#endif   

#include <time.h>
#include <string>

using namespace std;

class BaseException
{

private:
   string  longFormat;

public:
   int     code;                              // Error Code.
   string  state;							  // Error State.
   string  name;                              // Error Name
   string  description;                       // Error Description.  
   time_t  when;                              // When the exception was thrown.

   /* Constructor
    *
    * @param code             Error Code.
    * @param name             Name of the error causing the exception.
    * @param description      Description of the error.
    * @param when             Timestamp of when the exception occured. Default now.
    */
   BaseException(
      const int _code=-1, 
      const string &_name="Unknown", 
      const string &_description="", 
      const time_t _when=time(NULL));


   /* Destructor
    *
    */
   ~BaseException();
   

   /* char* Converstion Operator
    *
    * Treat BaseException object as a char* and output a long error description.
    */      
   operator char*();                          

   /* const char* Converstion Operator
    *
    * Treat BaseException object as a const char* and output a long error description.
    */      
   operator const char*();                    
   
   /* string Converstion Operator
    *
    * Treat BaseException object as a string and output a long error description.
    */      
   operator string();
};

#endif
