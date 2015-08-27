/*
 * BaseFieldDescription Object Defining the attributes of a field in a query.
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


#ifndef __BASE_FIELDDESC_H__
#define __BASE_FIELDDESC_H__

#include <string>
#include <vector>
#include <time.h>

#include "dbconnectTypes.h"

#ifdef WIN32
   // Windows
   // DLL Exports
   class  __declspec( dllexport ) BaseFieldDescription;
#endif   


using namespace std;

class BaseFieldDescription
{

protected:  
   string          _name;
   long int        _position;
   FieldType       _type;

   bool            _isIncrement;       // Auto increment field like MySQL and DB2 supports.
	bool            _isPriKey;
	bool            _isUnique;
	bool            _isNotNull;
	long int        _precision;
	long int        _scale;

public:

   /* Constructors.
    *
    */
   BaseFieldDescription();

   BaseFieldDescription(
      string&   name,
      long int  position,
      FieldType type,
      bool      isIncrement = false,
      bool      isPriKey    = false,
      bool      isUnique    = false,
      bool      isNotNull   = false,
      long int  precision   = 0,
      long int  scale       = 0);


   /* Destructor.
    *
    */
   virtual 
   ~BaseFieldDescription();
   

   /* Get the field name.
    *
    * @return        Returns the field name.
    */
   const string&
   name();


   /* Get the field position.
    *
    * @return        Returns the field position.
    */
   long int
   position();


   /* Get the field type.
    *
    * @return        Returns the field type.
    */
   FieldType
   type();


   /* Determine if the field is an incremented field.
    *
    * @return        Returns true/false.
    */
   bool
   isIncrement();

  
   /* Determine if the field is part of the primary key.
    *
    * @return        Returns true/false.
    */
   bool
   isPriKey();


   /* Determine if the field is part of a unique contraint.
    *
    * @return        Returns true/false.
    */
   bool
   isUnique();


   /* Determine if the field has a not null constraint.
    *
    * @return        Returns true/false.
    */
   bool
   isNotNull();


   /* Get the field precision.
    *
    * @return        Returns the field precision.
    */
   long int
   precision();


   /* Get the field scale.
    *
    * @return        Returns the field scale.
    */
   long int
   scale();
};


#endif


