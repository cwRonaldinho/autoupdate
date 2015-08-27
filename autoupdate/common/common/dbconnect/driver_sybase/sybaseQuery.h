#ifdef _DB_SYBASE_
#ifndef __SYBASE_QUERY_H__
#define __SYBASE_QUERY_H__

#include <string>

#include "dbconnect/base/baseQuery.h"

#include "dbconnect/driver_sybase/SybaseFieldDescription.h"
#include "dbconnect/driver_sybase/sybaseConnection.h"
#include "dbconnect/driver_sybase/sybaseBindParam.h"
#include "dbconnect/driver_sybase/sybaseValue.h"

#include "dbconnect/driver_sybase/sybase_inc/sybdb.h"


using namespace std;

class SybaseBindParam;
class SybaseValue;

class SybaseQuery : public BaseQuery
{

private:  
   // Collection types that can be freed within this object
   enum CollectionType
   {
      FIELD_INFORMATION,
      FIELD_VALUES,
      BIND_PARAMETERS
   };
      
   // Sybase Handles
   DBULONG          _sybaseNumRows;
   DBULONG          _sybaseCurrentRow;

   // Parameter collection.
   DBULONG          _numParameters;
   SybaseBindParam **_parameters;

   // Field Information collection
   DBULONG                _numFieldInformation;
   SybaseFieldDescription **_fieldInformation;

   // Field Value collection
   DBULONG      _numRecordValues;
   SybaseValue **_recordValues;

  
   SybaseConnection* _parentConnection;   
   int              _index;            // Index of the handle object
   bool             _isTransaction;    // Is the object in a state of a transaction running.

   /* Internal method to obtain the information 
    *   about the fields returned in a result set.
    */
   void
   _sybaseGetFieldsInformation();


   /* Internal method to obtain the data in the result set row
    */
   void
   _sybaseGetResultSetRow();


   /* Internal method to return the dbConnect type for the sybase field
    *
    * @param type    The Sybase field type to resolve to a dbConnect field type
    *
    * @return   Returns the dbConnect field type for the given sybase field type
    */
   FieldType
   _sybaseResolveFieldType(
      BYTE type);
 

   /* Internal method to parse the parameters that need to be bound in the SQL query
    *
    * @param originalSqlStatement  The sql statement with the bind parameters.
    *
    * @return   Returns the parsed SQL that can be executed by sybase
    */
   string
   _sybaseParseBindParameters(
         const string& originalSqlStatement);
   

   /* Internal method to free internally allocated memory
    *
    * @param type    The type representing which value to deallocate.
    *
    */
   void _freeCollection(
      CollectionType type);

public:

   /* Constructor.
    *
    */
   SybaseQuery(
      SybaseConnection* parentConnection,
      int              index);


   /* Destructor.
    *
    */
   virtual 
   ~SybaseQuery();
   
   
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
    *
    */
   void
   transBegin();
   
   
   /* Commit a transaction.
    *
    */
   void
   commit();
   
   
   /* Rollback a transaction.
    *
    */
   void
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
      
};

#endif
#endif