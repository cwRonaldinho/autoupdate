/*
 * DBConnection Object defining the wrapper class for all the database drivers.
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


#include <dbconnect/dbconnect.h>
#include <dbconnect/base/configFile.h>

#ifdef WIN32
   #define DIRCHAR '\\'
   #define LOCALCONFIG "dbconnect.conf"
   #define SYSTEMCONFIG "\\win32\\system\\dbconnect.conf"
#else
   #define DIRCHAR '/'
   #define LOCALCONFIG "./dbconnect.conf"
   #define SYSTEMCONFIG "/etc/dbconn/dbconnect.cfg"
#endif

string DBCONNECTVER="0.3.5";


static char _copyright[] = {
"/*"
" * DBConnect API"
" * Copyright (C) 2003 Johnathan Ingram, jingram@rogueware.org"
" *"
" * This library is free software; you can redistribute it and/or"
" *   modify it under the terms of the GNU Lesser General Public"
" *   License as published by the Free Software Foundation; either"
" *   version 2.1 of the License, or (at your option) any later version."
" *"
" *   This library is distributed in the hope that it will be useful,"
" *   but WITHOUT ANY WARRANTY; without even the implied warranty of"
" *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU"
" *   Lesser General Public License for more details."
" *"
" *   You should have received a copy of the GNU Lesser General Public"
" *   License along with this library; if not, write to the Free Software"
" *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US"
" *"
" */" };


// Driver file name defaults
#ifdef WIN32
// Windows
const char* DbConnection::driverPaths[] = {  
      "",
      "driver_mysql.dll",
      "driver_msql.dll",
      "driver_postgresql.dll",
      "driver_ibmDB2.dll",
      "driver_oracle8.dll",
      "driver_odbc.dll",
	  "driver_sybase.dll",
	  "driver_oracle10.dll"
   };
#else
// Unices
const char* DbConnection::driverPaths[] = {  
      "",
      "dbconndrv_mysql.so",
      "dbconndrv_msql.so",
      "dbconndrv_postgresql.so",
      "dbconndrv_ibmDB2.so",
      "dbconndrv_oracle8.so",
      "dbconndrv_odbc.so",
	  "driver_sybase.so",
	  "driver_oracle10.so"
   };
#endif

const char* DbConnection::driverNames[] = {  
      "NONE",
      "MYSQL",
      "MSQL",
      "POSTGRESQL",
      "DB2",
      "ORACLE8",
      "ODBC",
	  "SYBASE",
	  "ORACLE10"
   };



//------------------------------------------------------------------------------
// PRIVATE
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// DbConnection::_buildCompatibilityTable
//------------------------------------------------------------------------------
void 
DbConnection::_buildCompatibilityTable()
{
   // Hard coded lookup table.
   vector<string> versions;
     
   // Populate version 0.3.5
   versions.push_back("0.3.4");
   versions.push_back("0.3.5");
   compatibilityTable["0.3.5"] = versions;
}  // DbConnection::_buildCompatibilityTable

   
   
//------------------------------------------------------------------------------
// PUBLIC
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// DbConnection::DbConnection
//------------------------------------------------------------------------------
DbConnection::DbConnection(
         Driver driver, 
         const string &cfgfile)
:
   ptr_getAuthor(NULL),
   ptr_getVendor(NULL),
   ptr_getCopyright(NULL),
   ptr_getDriverType(NULL),
   ptr_getDriverName(NULL),
   ptr_getDriverDesc(NULL),
   ptr_getDbConnectVersion(NULL),
   ptr_createDriverInstance(NULL),
   ptr_destroyDriverInstance(NULL),
   drvRef(NULL)
{
   	g_dbclog.open();
   // Build the compatibility table.
   _buildCompatibilityTable();


   /* Steps to finding the config file in order to determine the path and config
    *  parameters for the drivers.
    * 1. Use the file iif given in configFile param.
    * 2. Look in the current directory for the config file. (./dbconnect.cfg on unix).
    * 3. Look in the systems config dir for the file.       (eg /etc/dbconnect.cfg on unix).
    * 4. If still no library path, then use defaults.
    */
   size_t i;
   string driverPath = "";
   vector<string> args;    


   // 1. Use the file iif given in configFile param.
   if (cfgfile.length() > 0)
   {
      // Try and read the information from the config file.
      ConfigFile configFile(cfgfile.c_str());
      configFile.read();

      if (configFile.hasKeyName("dbconnect", "driverPath"))
      {
         driverPath = configFile["dbconnect"]["driverPath"];
         // Make sure we have a proper path
         if (driverPath[driverPath.length()-1] != DIRCHAR)
            driverPath += DIRCHAR;
      }

      // Check if we have a driver file name or need to use the default.
      if (configFile.hasKeyName(driverNames[driver], "driverFile"))
         driverPath += configFile[driverNames[driver]]["driverFile"];
      else
         driverPath += driverPaths[driver];

      // Check if we have a driver section and try and obtain needed values.
      if (configFile.hasSection(driverNames[driver]))
      {           
         //Get all the driver arguments and values
         vector<string> keyNames = configFile.getSectionKeyNames(driverNames[driver]);
         for (i=0; i<keyNames.size(); i++)
         {
            args.push_back(keyNames[i].c_str());                                   // Name
            args.push_back(configFile[driverNames[driver]][keyNames[i].c_str()]);  // Value
         }
      }
   }
   else  // Try and find a config file.
   {
      try
      {
         // 2. Look in the current directory for the config file. (./dbconnect.cfg on unix).
         ConfigFile configLocalFile(LOCALCONFIG);
         configLocalFile.read();
         if (configLocalFile.hasKeyName("dbconnect", "driverPath"))
         {
            driverPath = configLocalFile["dbconnect"]["driverPath"];
            // Make sure we have a proper path
            if (driverPath[driverPath.length()-1] != DIRCHAR)
               driverPath += DIRCHAR;
         }
         // Check if we have a driver file name or need to use the default.
         if (configLocalFile.hasKeyName(driverNames[driver], "driverFile"))
            driverPath += configLocalFile[driverNames[driver]]["driverFile"];
         else
            driverPath += driverPaths[driver];

         // Check if we have a driver section and try and obtain needed values.
         if (configLocalFile.hasSection(driverNames[driver]))
         {           
            //Get all the driver arguments and values
            vector<string> keyNames = configLocalFile.getSectionKeyNames(driverNames[driver]);
            for (i=0; i<keyNames.size(); i++)
            {
               args.push_back(keyNames[i].c_str());                                        // Name
               args.push_back(configLocalFile[driverNames[driver]][keyNames[i].c_str()]);  // Value
            }
         }
      }
      catch(...)
      {
         try
         {
            // 3. Look in the systems config dir for the file. (eg /etc/dbconnect.cfg on unix).
            ConfigFile configSystemFile(SYSTEMCONFIG);
            configSystemFile.read();
            
            if (configSystemFile.hasKeyName("dbconnect", "driverPath"))
            {
               driverPath = configSystemFile["dbconnect"]["driverPath"];
               // Make sure we have a proper path
               if (driverPath[driverPath.length()-1] != DIRCHAR)
                  driverPath += DIRCHAR;
            }
      
            // Check if we have a driver file name or need to use the default.
            if (configSystemFile.hasKeyName(driverNames[driver], "driverFile"))
               driverPath += configSystemFile[driverNames[driver]]["driverFile"];
            else
               driverPath += driverPaths[driver];

            // Check if we have a driver section and try and obtain needed values.
            if (configSystemFile.hasSection(driverNames[driver]))
            {           
               //Get all the driver arguments and values
               vector<string> keyNames = configSystemFile.getSectionKeyNames(driverNames[driver]);
               for (i=0; i<keyNames.size(); i++)
               {
                  args.push_back(keyNames[i].c_str());                                         // Name
                  args.push_back(configSystemFile[driverNames[driver]][keyNames[i].c_str()]);  // Value
               }
            }
         }
         catch(...)
         {}
      }
   }


   // 4. If still no library path, then use default driver name and hope its
   //    in the systems library path.
   if (driverPath.length() == 0)
      driverPath = driverPaths[driver];

   

 #ifndef	STATIC_DBC
   // Load the driver and resolve the symbols to pointers.
   try
   {
	  g_dbclog.print("driver: %s\n", driverPath.c_str());
      dlLoader.loadLibrary(driverPath);
      
      //Resolve the symbols required from the library
      ptr_createDriverInstance  = (void* (*)(int , const char**))dlLoader.resolveSymbol("createDriverInstance");
      ptr_destroyDriverInstance = (void* (*)(void*)) dlLoader.resolveSymbol("destroyDriverInstance");
      ptr_getAuthor             = (const char* (*)())dlLoader.resolveSymbol("getAuthor");
      ptr_getVendor             = (const char* (*)())dlLoader.resolveSymbol("getVendor");
      ptr_getCopyright          = (const char* (*)())dlLoader.resolveSymbol("getCopyright");
      ptr_getDriverType         = (const char* (*)())dlLoader.resolveSymbol("getDriverType");
      ptr_getDriverName         = (const char* (*)())dlLoader.resolveSymbol("getDriverName");
      ptr_getDriverDesc         = (const char* (*)())dlLoader.resolveSymbol("getDriverDesc");
      ptr_getDbConnectVersion   = (const char* (*)())dlLoader.resolveSymbol("getDbConnectVersion"); 
   }
   catch(BaseException &ex)
   {
      throw DriverError(ex.description); 
   }
#else
      ptr_createDriverInstance  = createDriverInstance;
      ptr_destroyDriverInstance = destroyDriverInstance;
      ptr_getAuthor             = getAuthor;
      ptr_getVendor             = getVendor;
      ptr_getCopyright          = getCopyright;
      ptr_getDriverType         = getDriverType;
      ptr_getDriverName         = getDriverName;
      ptr_getDriverDesc         = getDriverDesc;
      ptr_getDbConnectVersion   = getDbConnectVersion; 
#endif

   // Get all the driver information.
   driverInfo.author = ptr_getAuthor();
   driverInfo.vendor = ptr_getVendor();
   driverInfo.copyright = ptr_getCopyright();
   driverInfo.driverType = ptr_getDriverType();
   driverInfo.driverName = ptr_getDriverName();
   driverInfo.driverDescription = ptr_getDriverDesc();
   driverInfo.dbConnectVersion = ptr_getDbConnectVersion();
   g_dbclog.print("driverName: %s,driverType: %s\n", driverInfo.driverName.c_str(),driverInfo.driverType.c_str());

   // Get the version from the driver and make sure it is compatible.
   if (!checkCompatibility(driverInfo.dbConnectVersion.c_str()) )
   {
      string err = "The so library ";
      err += driverPath;
      err += " with version ";
      err += driverInfo.dbConnectVersion.c_str();
      err += " is not compatibible with version ";
      err += DBCONNECTVER;
      err += " of the DBConnect API";
      throw DriverError(err);
   }


   // Obtain a pointer reference to a driver instance passing the arguments.
   const char** argsref = NULL;
   argsref = (const char**)malloc(args.size()*sizeof(const char*));
   for (i=0; i<args.size(); i++)
      argsref[i] = args[i].c_str();

   try
   {
      drvRef = (BaseConnection*)ptr_createDriverInstance((int)args.size(), argsref);
   }
   catch(...)
   {
      drvRef = NULL;
   }

   free(argsref);

   // If we do not have a driver reference, throw an error
   if (!drvRef)
      throw DriverError("Unable to obtain a reference to the driver object");
}  // DbConnection::DbConnection

//------------------------------------------------------------------------------
// DbConnection::~DbConnection
//------------------------------------------------------------------------------
DbConnection::~DbConnection()
{

   // Free the driver instance. (Returns NULL if freed, or the reference if not freed)
   drvRef = (BaseConnection*)ptr_destroyDriverInstance((void*)drvRef);
   // Unload the driver.
   try
   {
      ptr_createDriverInstance = NULL;
      ptr_destroyDriverInstance = NULL;
      ptr_getAuthor = NULL;
      ptr_getVendor = NULL;
      ptr_getDriverName = NULL;
      ptr_getDriverDesc = NULL;
      ptr_getDbConnectVersion = NULL;  
#ifndef STATIC_DBC
      dlLoader.unloadLibrary();
#endif
   }
   catch(BaseException &ex)

   {
      throw Error(ex.description);     
   }  
   if (drvRef)
      throw Error("The memory for the driver was not released.");
}  // DbConnection::~DbConnection


//------------------------------------------------------------------------------
// DbConnection::connect
//------------------------------------------------------------------------------
void 
DbConnection::connect(
   const string &username, 
   const string &password, 
   const string &databaseName, 
   const string &host, 
   int   maxConnections,
   int   minConnections,
   const string &optParam1,
   const string &optParam2)
{
   // The driver will propogate any exceptions.
   drvRef->connect(username, password, databaseName, host,
                   maxConnections, minConnections,
                   optParam1,
                   optParam2);
} // DbConnection::connect


//------------------------------------------------------------------------------
// DbConnection::disconnect
//------------------------------------------------------------------------------
void 
DbConnection::disconnect(
   time_t timeout)
{
   // The driver will propogate any exceptions.

   drvRef->disconnect(timeout);   
}  // DbConnection::disconnect
      

//------------------------------------------------------------------------------
// DbConnection::setPingInterval
//------------------------------------------------------------------------------
void 
DbConnection::setPingInterval(
   time_t pingInterval)
{
   // The driver will propogate any exceptions.
   drvRef->setPingInterval(pingInterval);
}  // DbConnection::setPingInterval


//------------------------------------------------------------------------------
// DbConnection::requestQueryConnection
//------------------------------------------------------------------------------
BaseQuery*
DbConnection::requestQueryConnection()
{
   // The driver will propogate any exceptions.
   return (BaseQuery*)drvRef->requestQueryConnection();
}  // DbConnection::requestQueryConnection

void 
DbConnection::releaseQueryConnection(void* queryObject)
{
	return drvRef->releaseQueryConnection(queryObject);
}
//------------------------------------------------------------------------------
// DbConnection::checkCompatibility
//------------------------------------------------------------------------------
bool
DbConnection::checkCompatibility(
   const string &ver)
{
   for (size_t i=0; i<compatibilityTable[DBCONNECTVER].size(); i++)
      if (strcmp(compatibilityTable[DBCONNECTVER][i].c_str(), ver.c_str()) == 0)
         return true;
         
   return false;  
}  // DbConnection::checkCompatibility


//------------------------------------------------------------------------------
// DbConnection::checkCompatibility
//------------------------------------------------------------------------------
DbConnectionDriverInfo* 
DbConnection::getDriverInformation()
{
   return &driverInfo;
}
