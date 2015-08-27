/*
 * ODBCQuery object defines the needed query functions for the dbConnect ODBC driver
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


#ifndef __ODBC_QUERY_H__
#define __ODBC_QUERY_H__

#include <vector>

#include "baseQuery.h"

#include "odbcconnection.h"
#include "odbcbindparam.h"
#include "odbcvalue.h"
#include "odbcfielddescription.h"


using namespace std;


#define FETCH_BLOCK_SIZE 10

class ODBCQuery : public BaseQuery
{

private:  
   // Collection types that can be freed within this object
   enum CollectionType
   {
      FIELD_INFORMATION,
      FIELD_VALUES,
      BIND_PARAMETERS
   };

   // ODBC Handles
   SQLHANDLE        _hstmt;
   DBULONG          _odbcCurrentRow;

   // Parameter collection.
   DBULONG        _numParameters;
   ODBCBindParam **_parameters;
   vector<string> _queryParameterNames;    // Holds all the parameter names that are in a query from left to right to allow binding correctly

   // Field Information collection
   DBULONG                _numFieldInformation;
   ODBCFieldDescription  **_fieldInformation;

   // Field Value collection
   DBULONG     _numRecordValues;
   ODBCValue  **_recordValues;

   
   ODBCConnection* _parentConnection;
   int            _index;            // Index of the handle object
   bool           _isTransaction;    // Is the object in a state of a transaction running.


   /* Internal method to obtain the information 
    *   about the fields returned in a result set.
    */
   void
   _odbcGetFieldsInformation();


   /* Internal method to obtain the data
    *   for a row in a result set
    */
   void
   _odbcGetResultSetRow();


   /* Internal method to return the dbConnect type for the ODBC field
    *
    * @param type    The ODBC field type to resolve to a dbConnect field type
    *
    * @return   Returns the dbConnect field type for the given ODBC field type
    */
   FieldType
   _odbcResolveFieldType(
      SQLSMALLINT type);


   /* Internal method to replace the parameters with ?
    *
    * @param originalSqlStatement  The sql statement with the bind parameters.
    * @param bindParameters        The vector to hold the pointers to the bind parameters (left to right for the ?'s)
    *
    * @return   Returns the SQL that has bad its parameters replace with ?
    */
   string
   _odbcParseBindParameters(
         const string& originalSqlStatement,
         vector<ODBCBindParam*> &bindParameters);


   /* Internal method to bind parameters with ? with the actual given parameter values
    *
    * @param resolvedSqlStatement  The sql statement with the ?'s?
    * @param bindParameters        The vector with pointers to the bind parameters (left to right for the ?'s)
    *
    */
   void
   _odbcBindParameters(
         const string& resolvedSqlStatement,
         vector<ODBCBindParam*> &bindParameters);


   /* Internal method to free internally allocated memory
    *
    * @param type    The type representing which value to deallocate.
    *
    */
   void _freeCollection(
      CollectionType type);

   void _handleOdbcErr( SQLHANDLE& stmt, SQLRETURN& cliRC, const char* action );
public:

   /* Constructor.
    *
    */
   ODBCQuery(
      ODBCConnection* parentConnection,
      int            index);


   /* Destructor.
    *
    */
   virtual 
   ~ODBCQuery();
   

   /* Clears any bind parameters
    *
    */
   void 
   clearBindParams();


   /* Set the SQL / DDL command that will be used for bind 
    *  paramaters and the execute
    *
    * @param sqlStatement  Statement wanting to execute
    *
    */
   void
   command(
      const string& sqlStatement);


   /* Set the value for a parameter giving the parameter name.
    *
    * @param paramName      Name of the parameter.
    *
    * @return               Returns the parameter object for the position.
    */
   BaseValue*
   bindParam(
      const string& paramName); 


   /* Execute the sql statement set by command
    *
    */
   void
   execute();


   /* Obtain the next row of data in the record set.
    *
    */
   void
   fetchNext();


   /* Begin a transaction.
    * Make virtual so a new driver can inherit and implement
    *
    */
   virtual void
   transBegin();
   
   
   /* Commit a transaction.
    * Make virtual so a new driver can inherit and implement
    *
    */
   virtual void
   commit();
   
   
   /* Rollback a transaction.
    * Make virtual so a new driver can inherit and implement
    *
    */
   virtual void
   rollback();   


   /* Returns the information about a field.
    *
    * @param    index     Index of the field.
    *
    * @return             Returns a pointer to the field information structure.
    */
   BaseFieldDescription*
   getFieldInfoByColumn(
      int index);


   /* Returns the information about a field.
    *
    * @param    fieldName Name of the field.
    *
    * @return             Returns a pointer to the field information structure.
    */
   BaseFieldDescription*
   getFieldInfoByName(
      const string& fieldName);
      
   /* Returns the value of a field.
    *
    * @param    index     Index of the field.
    *
    * @return   Returns a pointer to the value structure.
    */
   BaseValue*
   getFieldByColumn(
      int index);


   /* Returns the value of a field.
    *
    * @param    fieldName Name of the field.
    *
    * @return   Returns a pointer to the value structure.
    */
   BaseValue*
   getFieldByName(
      const string& fieldName);

   ODBCConnection*	getParentConnection(){ return _parentConnection; };
   int				getIndex(){ return _index; };
};


#endif
