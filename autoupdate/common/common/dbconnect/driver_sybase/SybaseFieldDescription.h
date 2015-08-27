#ifdef _DB_SYBASE_
#ifndef __SYBASE_FIELDDESC_H__
#define __SYBASE_FIELDDESC_H__

#include <string>

#include "dbconnect/base/baseFieldDescription.h"

#include "dbconnect/driver_sybase/sybase_inc/sybfront.h"


using namespace std;


class SybaseFieldDescription : public BaseFieldDescription
{

public:
   /* Constructors.
    *
    */
   SybaseFieldDescription(
      string&     name,
      long int    position,
      FieldType   type,
      bool        isIncrement,
      bool        isPriKey,
      bool        isUnique,
      bool        isNotNull,
      long int    precision,
      long int    scale,
      BYTE		  sybaseFieldType,
      DBINT       dbcolSize);


   /* Get the Sybase SQL field type.
    *
    * @return   Returns the field type.
    */
   BYTE
   sybaseFieldType();


   /* Get the Sybase SQL field size.
    *
    * @return   Returns the field size.
    */
   DBINT
   sybaseFieldSize();


private:
   BYTE		_sybaseSQLFieldType;
   DBINT	_dbcolSize;

};

#endif
#endif