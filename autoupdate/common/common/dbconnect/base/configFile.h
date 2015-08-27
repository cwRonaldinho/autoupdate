/*
 * ConfigFile and ConfigSections Objects allows the reading and writing of config files
 * and name value pairs within sections of the config files.
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


#ifndef __CONFIGFILE_H__
#define __CONFIGFILE_H__

#ifdef WIN32
   // Windows
   // DLL Exports
   class  __declspec( dllexport ) ConfigFile;
   class  __declspec( dllexport ) ConfigSection;
   #pragma warning( disable : 4290 )

#endif   


#include <fstream>
#include <string>
#include <vector>
#include <map>


#include "baseException.h"
#include "simpleThreads.h"

using namespace std;


class ConfigFile;


class ConfigSection
{
protected:
   string  sectionName;


public:
   enum ValueStatus
   {
      UNKNOWN,
      CURRENT,    // The value is the current value read from the config file.
      CHANGED,    // The value has been changed and not commited.
      CREATED,    // The value has been created and not commited.
      DELETED     // The name value pair has been deleted and not commited.
   };
   
   struct Value
   {
      string       value;
      ValueStatus  status;
   };


   // Key value pairs for the given section.
   typedef pair< string, Value > ValuesPair;
   typedef map < string, Value > Values;
   typedef Values::iterator ValuesIterator;
   Values values;


   enum ErrorCodes 
   {
      NAME_KEY_NOT_FOUND
   };


   /* Exceptions Defined
    *
    */
   class NameKeyNotFoundError : public BaseException {
      public: NameKeyNotFoundError(const string description) 
              : BaseException(NAME_KEY_NOT_FOUND, "NameKeyNotFoundError", description) {} };


   /* Constructor
    *
    * @param sectionName     Name of the section.
    */
   ConfigSection(const string& sectionName);


   /* Overload the subscript operator to return a given name value.
    *
    */
   const string& 
   operator[](
          const string&) 
      throw(
         NameKeyNotFoundError);
};



class ConfigFile
{
private:
   string configFile;

   // Name value pair containing all the config sections
   typedef pair< string, ConfigSection > ConfigSectionsPair;
   typedef map< string, ConfigSection > ConfigSections;
   typedef ConfigSections::iterator ConfigSectionsIterator;
   ConfigSections configSections;

   // Thread mutexes
   SimpleThread_Mutex classMutex;     


public:
   // Global Error Message Constants 
   enum ErrorCodes 
   {
      OPEN_ERROR,
      DUPLICATE_NAME,
      SECTION_NOT_FOUND
   };


   /* Exceptions Defined
    *
    */
   class OpenError : public BaseException {
      public: OpenError(const string description) 
              : BaseException(OPEN_ERROR, "ConfigFileOpenError", description) {} };

   class DuplicateNameError : public BaseException {
      public: DuplicateNameError(const string description) 
              : BaseException(DUPLICATE_NAME, "DuplicateNameError", description) {} };

   class SectionNotFoundError : public BaseException {
      public: SectionNotFoundError(const string description) 
              : BaseException(SECTION_NOT_FOUND, "SectionNotFoundError", description) {} };


   /* Constructor
    *
    * @param configFile     Path of the config file to work with.
    */
   ConfigFile(const string& aConfigFile);

   /* Destructor
    *
    */
   ~ConfigFile();   
   
   
   /* Overload the subscript operator to return a given section object.
    *
    */
   ConfigSection& 
   operator[](
         const string&)
      throw(
         SectionNotFoundError);      
   
    
   /* Read the specified config files values.
    *
    */
   void 
   read();
      

   /* Write any changes or additions to the config file.
    *
    */
   void 
   write();
   

   /* Determine if a section name exists.
    *
    * @param section   Section name to look for.
    *
    * @return           Returns true if the section exists or false.
    */
   bool 
   hasSection(
         const string& section);
   

   /* Determine if a value exists in a given section.
    *
    * @param section   Section name to search in.
    * @param name      Key name within in the section.
    *
    * @return          Returns true if the section exists or false.
    */
   bool 
   hasKeyName(
         const string& section, 
         const string& name);
         

   /* Returns a string vector of all section names.
    *
    * @return          String vector containing all the section names.
    */
   vector<string>
   getSectionNames();


   /* Returns a string vector of all key names within a section.
    *
    * @param section   Section name.
    *
    * @return          String vector containing all the section key names.
    */
   vector<string>
   getSectionKeyNames(
         const string& section);
};

#endif
