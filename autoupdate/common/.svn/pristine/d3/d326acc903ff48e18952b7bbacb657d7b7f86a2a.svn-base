/*
 * dlLoader Object defining routines to dynamically load libraries and retrieve pointers to symbols
 *  within the library. (DL: dynamic library)
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
 
 
#include "dlLoader.h"


//------------------------------------------------------------------------------
// DLLoader::DLLoader
//------------------------------------------------------------------------------
DlLoader::DlLoader()
{

   libname = "";
   handle = NULL;
}  // DLLoader::DLLoader



//------------------------------------------------------------------------------
// DLLoader::~DLLoader
//------------------------------------------------------------------------------
DlLoader::~DlLoader() 
{
   //Close the library if it has not been closed.
   if (handle)
      unloadLibrary();
}  // DlLoader::~DlLoader


//------------------------------------------------------------------------------
// DLLoader::loadLibrary
//------------------------------------------------------------------------------
void 
DlLoader::loadLibrary(
      const string &libname)
{
   if (libname.size() < 5)
      throw OpenError("Invalid file name");
   
   //Try and open the library
#ifdef WIN32
   // Windows
   handle = LoadLibraryA(libname.c_str());
   if (!handle)
   {
      // Get the windows error
      char lpMsgBuf[10240];
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                     NULL,
                     GetLastError(),
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                     (LPTSTR) &lpMsgBuf,
                     sizeof(lpMsgBuf)-1,
                     NULL);         
      throw OpenError(lpMsgBuf);
   }
#else
   // Unices:
   handle = dlopen(libname.c_str(), RTLD_NOW);

   if (!handle)
      throw OpenError(dlerror());   
#endif   
   
   DlLoader::libname = libname;
}  // DLLoader::loadLibrary



//------------------------------------------------------------------------------
// DLLoader::resolveSymbol
//------------------------------------------------------------------------------
PROCADDRESS
DlLoader::resolveSymbol(
      const string &symbol)
{
   PROCADDRESS sympos = NULL;

   if (!handle)
      throw SymbolLoadError("No valid handle to an open library.");

#ifdef WIN32
   //Windows
   sympos = GetProcAddress(handle, symbol.c_str());
   if (!sympos)
   {
      // Get the windows error
      char lpMsgBuf[10240];
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                     NULL,
                     GetLastError(),
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                     (LPTSTR) &lpMsgBuf,
                     sizeof(lpMsgBuf)-1,
                     NULL);         
      throw SymbolLoadError(lpMsgBuf);
   }
#else
   // Unices     
   sympos = dlsym(handle, symbol.c_str());
   if (!sympos)
      throw SymbolLoadError(dlerror());
#endif

   return sympos; 
}  // DLLoader::resolveSymbol



//------------------------------------------------------------------------------
// DLLoader::unloadLibrary
//------------------------------------------------------------------------------
void 
DlLoader::unloadLibrary()
{
#ifdef WIN32
   //Windows   
   if (handle)
   {
      if (!FreeLibrary(handle))
      {
         // Get the windows error
         char lpMsgBuf[10240];
         FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                        NULL,
                        GetLastError(),
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                        (LPTSTR) &lpMsgBuf,
                        sizeof(lpMsgBuf)-1,
                        NULL);         
         throw CloseError(lpMsgBuf);
      }
   }
   else
      throw CloseError("No valid handle to an open library.");
#else
   // Unices
   if (handle)
   {
      if (dlclose(handle) != 0)
         throw CloseError(dlerror());
   }
   else
      throw CloseError("No valid handle to an open library.");
   
#endif   
   
   libname = "";
   handle = NULL;
} // DLLoader::unloadLibrary
