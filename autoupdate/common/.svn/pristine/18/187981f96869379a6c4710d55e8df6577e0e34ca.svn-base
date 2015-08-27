/*
 * BaseQuery object defining commmon functionality that all drivers inherit from
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
 */
 
#ifndef __BASE_QUERY_H__
#define __BASE_QUERY_H__

#pragma warning(disable:4786)
#pragma warning(disable:4251)

#ifdef WIN32
   // Windows
   // DLL Exports
   class  __declspec( dllexport ) BaseQuery;
#endif   


#include <string>
#include <time.h>

#include "baseValue.h"
#include "baseFieldDescription.h"


using namespace std;

class BaseQuery
{   
protected:  
   string          _sqlStatement;
   bool            _eof;
   unsigned int    _fieldCount;
   unsigned int    _outputCount;
   unsigned int    _effectCount;
   bool            _isFunctionQuery;


   /* Checks if a bind paramter is present in the stored sql statement
    *
    * @param sqlStatement  Statement wanting to execute
    *
    * @return              Returns true if the paramter was found, otherwise false
    *
    */
   bool            
   _isBindParameterPresent(
         const string& paramName);

public:


   /* Constructors.
    *
    */
   BaseQuery();


   /* Destructor.
    *
    */
   virtual 
   ~BaseQuery();
   
   
   /* Clears any bind parameters
    *
    */
   virtual void 
   clearBindParams() = 0;  


   /* Set the SQL / DDL command that will be used for bind 
    *  paramaters and the execute
    *
    * @param sqlStatement  Statement wanting to execute
    *
    */
   virtual void
   command(
         const string& sqlStatement);


   /* Set the value for a parameter giving the parameter name.
    *
    * @param paramName      Name of the parameter.
    *
    * @return               Returns the parameter object for the parameter name.
    */
   virtual BaseValue*
   bindParam(
         const string& paramName) = 0; 


   /* Execute the sql statement set by command
    *
    */
   virtual void
   execute();


   /* Returns if the end of the dataset for the query has been reached.
    *
    * @return    Returns true if end of dataset, else false.
    */
   virtual bool
   eof();


   /* Obtain the next row of data in the record set.
    *
    */
   virtual void
   fetchNext() = 0;


   /* Begin a transaction.
    *
    */
   virtual void
   transBegin() = 0;
   
   
   /* Commit a transaction.
    *
    */
   virtual void
   commit() = 0;
   
   
   /* Rollback a transaction.
    *
    */
   virtual void
   rollback() = 0;

   
   /* Returns the number of fields in the result set.
    *
    * @return               Returns true if end of dataset, else false.
    */
   virtual unsigned int
   fieldCount();

   virtual unsigned int
   outputCount();

   /* Returns the number of records was effect by SQL
   *  such as DELETED, UPDATED, INSERTED effect count
   *
   * @return               Returns true if end of dataset, else false.
   */
	virtual unsigned int 
		effectCount();

   /* Returns the information about a field.
    *
    * @param    index     Index of the field.
    *
    * @return   Returns a pointer to the field information structure.
    */
   virtual BaseFieldDescription*
   getFieldInfoByColumn(
         int index) = 0;


   /* Returns the information about a field.
    *
    * @param    fieldName Name of the field.
    *
    * @return   Returns a pointer to the field information structure.
    */
   virtual BaseFieldDescription*
   getFieldInfoByName(
         const string& fieldName) = 0;   
      

   /* Returns the value of a field.
    *
    * @param    index     Index of the field.
    *
    * @return   Returns a pointer to the value structure.
    */
   virtual BaseValue*
   getFieldByColumn(
         int index) = 0;


   /* Returns the value of a field.
    *
    * @param    fieldName Name of the field.
    *
    * @return   Returns a pointer to the value structure.
    */
   virtual BaseValue*
   getFieldByName(
         const string& fieldName) = 0;

   virtual void 
   registerOutParam(
		int index, int type) {};

   virtual BaseValue*
	   getOutParamByIndex(
	   int index) { return NULL;};

   virtual BaseValue* bindParam(int index, int type){return NULL;};

};


#endif
