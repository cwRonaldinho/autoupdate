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

#include "baseFieldDescription.h"


//------------------------------------------------------------------------------
// BaseFieldDescription::BaseFieldDescription
//------------------------------------------------------------------------------
BaseFieldDescription::BaseFieldDescription()
   : _name(""),
     _position(-1)
{

}  // BaseFieldDescription::BaseFieldDescription          


BaseFieldDescription::BaseFieldDescription(
   string&   name,
   long int  position,
   FieldType type,
   bool      isIncrement,
   bool      isPriKey,
   bool      isUnique,
   bool      isNotNull,
   long int  precision,
   long int  scale )

:  _name(name),
   _position(position),
   _type(type),
   _isIncrement(isIncrement),
   _isPriKey(isPriKey),
   _isUnique(isUnique),
   _isNotNull(isNotNull),
   _precision(precision),
   _scale(scale)
{

}  // BaseFieldDescription::BaseFieldDescription          


//------------------------------------------------------------------------------
// BaseFieldDescription::~BaseFieldDescription
//------------------------------------------------------------------------------
BaseFieldDescription::~BaseFieldDescription()
{   
     
}  // BaseFieldDescription::~BaseFieldDescription


//------------------------------------------------------------------------------
// BaseFieldDescription::name
//------------------------------------------------------------------------------
const string&
BaseFieldDescription::name()
{   
   return _name;
}  // BaseFieldDescription::name


//------------------------------------------------------------------------------
// BaseFieldDescription::position
//------------------------------------------------------------------------------
long int
BaseFieldDescription::position()
{
   return _position;
}  // BaseFieldDescription::position


//------------------------------------------------------------------------------
// BaseFieldDescription::type
//------------------------------------------------------------------------------
FieldType
BaseFieldDescription::type()
{
   return _type;
}  // BaseFieldDescription::type


//------------------------------------------------------------------------------
// BaseFieldDescription::isIncrement
//------------------------------------------------------------------------------
bool
BaseFieldDescription::isIncrement()
{
   return _isIncrement;
}  // BaseFieldDescription::isIncrement


//------------------------------------------------------------------------------
// BaseFieldDescription::isPriKey
//------------------------------------------------------------------------------
bool
BaseFieldDescription::isPriKey()
{
   return _isPriKey;
}  // BaseFieldDescription::isPriKey


//------------------------------------------------------------------------------
// BaseFieldDescription::isUnique
//------------------------------------------------------------------------------
bool
BaseFieldDescription::isUnique()
{
   return _isUnique;
}  // BaseFieldDescription::isUnique


//------------------------------------------------------------------------------
// BaseFieldDescription::isNotNull
//------------------------------------------------------------------------------
bool
BaseFieldDescription::isNotNull()
{
   return _isNotNull;
}  // BaseFieldDescription::isNotNull


//------------------------------------------------------------------------------
// BaseFieldDescription::precision
//------------------------------------------------------------------------------
long int
BaseFieldDescription::precision()
{
   return _precision;
}  // BaseFieldDescription::precision


//------------------------------------------------------------------------------
// BaseFieldDescription::scale
//------------------------------------------------------------------------------
long int
BaseFieldDescription::scale()
{
   return _scale;
}  // BaseFieldDescription::scale



