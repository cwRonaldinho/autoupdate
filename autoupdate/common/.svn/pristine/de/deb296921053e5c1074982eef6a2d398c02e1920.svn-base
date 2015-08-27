/*
 * ODBC Driver Implementation
 * Exports the public methods used to create and instance of the driver.
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

#include <string.h>
#include <string>

#include "baseConnection.h"
#include "odbcconnection.h"
#include <iostream>

using namespace std;

#define DBCONNECTVER "0.3.5"

//Stop the C++ name mangling
extern "C"
{

#ifdef WIN32
   // Windows
   // DLL Exports
   __declspec( dllexport ) void* createDriverInstance(int argc, const char** argv);
   __declspec( dllexport ) void* destroyDriverInstance(void* driver);
   __declspec( dllexport ) const char* getAuthor();
   __declspec( dllexport ) const char* getVendor();
   __declspec( dllexport ) const char* getDriverName();
   __declspec( dllexport ) const char* getDriverDesc();
   __declspec( dllexport ) const char* getDbConnectVersion();
   __declspec( dllexport ) const char* getCopyright();
   __declspec( dllexport ) const char* getDriverType();
#endif



/* Create an instance of the driver.
 
 * @param  argc     Number of parameters and values.
 * @param  argv     Parameters that can be used to instantiate the driver with.
 *
 * @return          Return the instance or NULL if an error occured.
 */
void*
createDriverInstance(int argc, const char** argv)
{   
   try
   {
      ODBCConnection *odbcConnection = NULL;
      odbcConnection = new ODBCConnection(argc, argv);
      return (void*)odbcConnection;
   }
   catch(...)
   {
      return NULL;
   }
}


/* Destroy an instance of the driver.
 *
 * @return        Return NULL for success or the the pointer to the instance for failure.
 *
 * @param  driver The pointer to the driver instance.
 */
void*
destroyDriverInstance(
   void* driver)
{
   ODBCConnection *odbcConnection = (ODBCConnection*)driver;
   
   if (odbcConnection)
   {
      // Make sure we have the correct driver.     
      if (strcmp( odbcConnection->driverName.c_str(), ODBC_DRIVERNAME) != 0)
         return driver;

      // Free the driver.
      delete odbcConnection;
   }
   return NULL;
}

   
/* Get the author of the driver.
 *
 * @return          Return the author.
 */
const char*
getAuthor()
{
   return "R&D@boentel";
}


/* Get the vendor information of the driver.
 *
 * @return          Return the vendor.
 */
const char*
getVendor()
{
   return "Boentel Technology";
}  


/* Get the copyright information of the driver.
 *
 * @return          Return the copyright information.
 */
const char*
getCopyright()
{
   return "Copyright (C) 2006 Boentel Technology";
}  


/* Get the driver type
 *
 * @return          Return the driver type.
 */
const char*
getDriverType()
{
   return "RDMS";
}  


/* Get the name of the driver.
 *
 * @return          Return the driver name.
 */
const char*
getDriverName()
{  
   return ODBC_DRIVERNAME;
}


/* Get the description of the driver.
 *
 * @return          Return the driver description.
 */
const char*
getDriverDesc()
{  
   return "ODBC Driver";
}


/* Returns the version of DbConnect this driver was compiled with.
 *
 * @return          Return the DbConnect version.
 */
const char*
getDbConnectVersion()
{
   return DBCONNECTVER;
}

} // extern "C"
