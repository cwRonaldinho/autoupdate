/*
 * DBConnect global type definitions commonly used
 * Copyright (C) 2002 Johnathan Ingram, jingram@rogueware.org
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


#ifndef __DBCONN_TYPES_H__
#define __DBCONN_TYPES_H__

#include <string>

#include "dbconnectExceptions.h"

using namespace std;


#ifdef WIN32
   #include <windows.h>

   // Types
   // TODO: Look at using LONG64 & ULONG64 with %lld & %llu. Does not work with cout :(
   #define DBLONG          long
   #define DBULONG         unsigned long
   #define DBLONG_SPRINTF  "%ld"
   #define DBULONG_SPRINTF "%lu"

   // Functions
   #define strcasecmp      _stricmp
   #define snprintf        _snprintf
   #define strdup          _strdup
#else
   // Types
   #define DBLONG           long long
   #define DBULONG          unsigned long long
   #define DBLONG_SPRINTF  "%lld"
   #define DBULONG_SPRINTF "%llu"

   // Functions
   #define strcasecmp      strcasecmp
   #define snprintf        snprintf
#endif


// Define the internal field types
enum FieldType 
{
   FT_UNKNOWN,
   FT_NULL,
   FT_STRING,
   FT_WSTRING,            //Wide String for unicode.
   FT_BLOB,
   FT_CLOB,
   FT_DATETIME,
   FT_DOUBLE,
   FT_BIT,
   FT_SHORT,
   FT_LONG,
   FT_UNSIGNED_SHORT,
   FT_UNSIGNED_LONG,
   FT_BOOLEAN,
   FT_RESULTSET
};


// Define the internal field categories
enum FieldTypeCategory 
{
   FTC_NORMAL,
   FTC_ARRAY   
};


           
#endif
