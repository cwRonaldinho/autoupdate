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

#include "configFile.h"

#include "string.h"
#include "ctype.h"


//------------------------------------------------------------------------------
// ConfigSection::ConfigSection
//------------------------------------------------------------------------------
ConfigSection::ConfigSection(const string& sectionName)
   : sectionName(sectionName)
{
   
} // ConfigSection::ConfigSection


//------------------------------------------------------------------------------
// ConfigSection::operator[]
//------------------------------------------------------------------------------
const string& 
ConfigSection::operator[]
      (const string& name) 
   throw 
      (NameKeyNotFoundError)
{
   // Find the section or throw an exception.   
   // Find the section as lower case or throw an exception.   
   string nameLower = name;
   for (unsigned int i=0; i<nameLower.length(); i++)
      nameLower[i] = tolower(nameLower[i]);

   ValuesIterator it = values.find(nameLower);
   
   if (it == values.end())
   {
      string errMsg = "The name key ";
      errMsg        += name.c_str();
      errMsg        += " does not exist in the config section ";
      errMsg        += sectionName.c_str();
      throw NameKeyNotFoundError(errMsg);      
   }
  
   return (const string&)it->second;
} // ConfigSection::operator[]



//------------------------------------------------------------------------------
// ConfigFile::ConfigFile
//------------------------------------------------------------------------------
ConfigFile::ConfigFile(const string& aConfigFile)
: configFile(aConfigFile)
{
 
}  // ConfigFile::ConfigFile


//------------------------------------------------------------------------------
// ConfigFile::~ConfigFile
//------------------------------------------------------------------------------
ConfigFile::~ConfigFile()
{
}  // ConfigFile::~ConfigFile



//------------------------------------------------------------------------------
// ConfigFile::operator[]
//------------------------------------------------------------------------------
ConfigSection& 
ConfigFile::operator[](
      const string& section)
   throw(
      SectionNotFoundError)      
{
   // Find the section as lower case or throw an exception.   
   string sectLower = section;
   for (unsigned int i=0; i<sectLower.length(); i++)
      sectLower[i] = tolower(sectLower[i]);
      
   ConfigSectionsIterator it = configSections.find(sectLower);
   
   if (it == configSections.end())
   {
      string errMsg = "The section ";
      errMsg        += section.c_str();
      errMsg        += " does not exist in the config file.";
      throw SectionNotFoundError(errMsg);      
   }
  
   return it->second;
} // ConfigFile::operator[]


//------------------------------------------------------------------------------
// ConfigFile::read
//------------------------------------------------------------------------------
void 
ConfigFile::read()
{
   // Synchronize the function.
   SimpleThread_Synchronize sync(classMutex);

   string currentSection;
   
   char maxLine[2048];       // Line can be a maximum of 2048 characters big.
   char *ptr, *ptr2, *ptr3;
   unsigned int  i;

   ifstream in;
   in.open(configFile.c_str());

   // Make sure we opened the file.
   if (in.fail())
   {
      string errMsg = "Unable to open the configuration file: ";
      errMsg       += configFile.c_str();
      throw OpenError(errMsg.c_str());
   }

   // Start with the global config section.
   currentSection = "";
   configSections.insert(ConfigSectionsPair(currentSection.c_str(), ConfigSection(currentSection)) );     
   

   // Read and process all the lines that are read in.
   while(in)
   {
      memset(maxLine, 0, sizeof(maxLine));
      in.getline(maxLine, sizeof(maxLine)-1);
      
      // Strip any '#' comments and ignore.
      ptr = strchr(maxLine, '#');
      if (ptr)
         ptr[0] = '\0';      

      // Make sure we have no \n or \r still in the line.
      ptr = strchr(maxLine, '\n');
      if (ptr)
         ptr[0] = '\0';      
      ptr = strchr(maxLine, '\r');
      if (ptr)
         ptr[0] = '\0';      
      
      
      // Don't waste time processing empty or too short lines.
      if (strlen(maxLine) > 3)
      {
         // Check if we have a new section and add it as the current section.
         if ((ptr = strchr(maxLine, '[')) && (ptr2 = strchr(maxLine, ']')) )
         {
            // Get the section name.
            ptr += 1;
            ptr2[0] = '\0';
            
            //Convert the section name to lower case
            for (unsigned i=0; i<strlen(ptr); i++)
               ptr[i] = tolower(ptr[i]);

            currentSection = ptr;
            configSections.insert(ConfigSectionsPair(currentSection.c_str(), ConfigSection(currentSection)) );
         }
         // else do we have a name value pair.
         else if ((ptr = strchr(maxLine, '=')) )
         {
            ptr3 = ptr + 1;   // Point to first char after = sign for start of value.           
            ptr[0] = '\0';

            // Lets get the name and strip any leading and trailing spaces. ptr=name
            //Convert the name to lower case
            for ( i=0; i<strlen(maxLine); i++)
               maxLine[i] = tolower(maxLine[i]);
                             
            // Leading
            ptr = maxLine;
            for ( i=0; i<strlen(maxLine); i++)
               if (maxLine[i] != ' ')
               {
                  ptr = maxLine + i;
                  break;
               }
               
            // Trailing
            for ( i =( unsigned int) strlen(maxLine)-1; i>=0; i--)
               if (maxLine[i] == ' ')
                  maxLine[i] = '\0';
               else
                  break;            
            
            // Lets get the value and strip any trailing and leading spaces. ptr2=value
            // Leading
            ptr2 = ptr3;
            for ( i=0; i<strlen(ptr3); i++)
               if (ptr3[i] != ' ')
               {
                  ptr2 = ptr3 + i;
                  break;
               }
               
            // Trailing
            for ( i = (unsigned int) strlen(ptr3)-1; i>=0; i--)
               if (ptr3[i] == ' ')
                  ptr3[i] = '\0';
               else
                  break;


            // Now add the name value pair to the section if it is unique.
            ConfigSectionsIterator it = configSections.find(currentSection);
            
            ConfigSection::ValuesIterator itValue = it->second.values.find(string(ptr));
            if (itValue != it->second.values.end())
            {
               string errMsg = "The section ";
               errMsg        += currentSection.c_str();
               errMsg        += " contains a duplicate key name: ";
               errMsg        += ptr;
               throw DuplicateNameError(errMsg);
            }

            ConfigSection::Value val;
            val.value = ptr2;
            val.status = ConfigSection::CURRENT;
            it->second.values.insert( ConfigSection::ValuesPair(ptr, val) );            
         }
         
         //If none of the above then ignore the line.  
      }           
   }

   in.close();   
}  // ConfigFile::read



//------------------------------------------------------------------------------
// ConfigFile::write
//------------------------------------------------------------------------------
void 
ConfigFile::write()
{
   // Synchronize the function.
   SimpleThread_Synchronize sync(classMutex);

   // TODO 
   // (Only try write values that have been flagged as new or changed)   

} // ConfigFile::write



//------------------------------------------------------------------------------
// ConfigFile::hasSection
//------------------------------------------------------------------------------
bool 
ConfigFile::hasSection(
      const string& section)
{
   try
   {   
      // Try and get the config section object for the section name
      // using the subscript operator.
      (*this)[section];
      return true;
   }
   catch(...)
   {
      return false;      
   }      
} // ConfigFile::hasSection      
   


//------------------------------------------------------------------------------
// ConfigFile::hasKeyName
//------------------------------------------------------------------------------
bool 
ConfigFile::hasKeyName(
      const string& section, 
      const string& name)
{
   try
   {   
      // Try and get the key name value using the subscript operators.
      (*this)[section][name];
      return true;
   }
   catch(...)
   {
      return false;      
   }      
} // ConfigFile::hasKeyName



//------------------------------------------------------------------------------
// ConfigFile::getSectionNames
//------------------------------------------------------------------------------
vector<string>
ConfigFile::getSectionNames()
{
   vector<string> sectionNames;
   
   ConfigSectionsIterator i;
   for (i = configSections.begin(); i != configSections.end(); i++)
      sectionNames.push_back(i->first.c_str());
      
   return sectionNames;   
} // ConfigFile::getSectionNames



//------------------------------------------------------------------------------
// ConfigFile::getSectionKeyNames
//------------------------------------------------------------------------------
vector<string>
ConfigFile::getSectionKeyNames(
      const string& section)
{
   // Find the section as lower case or throw an exception.   
   string sectLower = section;
   for (unsigned int i=0; i<sectLower.length(); i++)
      sectLower[i] = tolower(sectLower[i]);
      
   ConfigSectionsIterator it = configSections.find(sectLower);
   
   if (it == configSections.end())
   {
      string errMsg = "The section ";
      errMsg        += section.c_str();
      errMsg        += " does not exist in the config file.";
      throw SectionNotFoundError(errMsg);      
   }
   
   // Build the list of keyname;
   vector<string> keyNames;
   ConfigSection::ValuesIterator p;

   for (p = it->second.values.begin(); p != it->second.values.end(); p++)
      keyNames.push_back(p->first.c_str());

   return keyNames;   
}
