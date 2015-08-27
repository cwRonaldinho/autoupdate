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


#ifndef __DLLOADER_H__
#define __DLLOADER_H__

#include <string>

#ifdef WIN32
   // Windows
   #include <windows.h>
   #define PROCADDRESS FARPROC

   // DLL Exports
   class  __declspec( dllexport ) DlLoader;
#else
   // Unices
   #include <dlfcn.h>
   #define PROCADDRESS void*
#endif

#include "baseException.h"

using namespace std;




class DlLoader
{
private:


#ifdef WIN32
   // Windows  
   HMODULE handle;
#else
   // Unices
   void   *handle;
#endif
   string libname;

public:
   // Global Error Message Constants 
   enum ErrorCodes 
   {
      OPEN_ERROR,
      CLOSE_ERROR,
      RESOLVE_SYMBOL_ERROR
   };


   /* Exceptions Defined
    *
    */
   class OpenError : public BaseException {
      public: OpenError(const string description) 
              : BaseException(OPEN_ERROR, "dlOpenError", description) {} };

   class CloseError : public BaseException {
      public: CloseError(const string description) 
              : BaseException(CLOSE_ERROR, "dlCloseError", description) {} };

   class SymbolLoadError : public BaseException {
      public: SymbolLoadError(const string description) 
              : BaseException(RESOLVE_SYMBOL_ERROR, "dlResolveSymbolError", description) {} };



   /* Constructor
    *
    */
   DlLoader();

   /* Destructor
    *
    */
   ~DlLoader();
   
   /* Load the dynamic library.
    *
    * @param  libname    Full Path of library or only library name if wanting to search for library.
    */
   void 
   loadLibrary(
         const string &libname);


   /* Obtail a pointer to a symbol (function) within the loaded library.
    *
    * @param  symbol    Name of the exported symbol (function) in the library.
    */
   PROCADDRESS
   resolveSymbol(
         const string &symbol);


   /* Unload the dynamic library.
    *
    */
   void 
   unloadLibrary();
};

#endif