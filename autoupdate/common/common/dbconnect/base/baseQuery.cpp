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
 

#include "baseQuery.h"
#include "dbconnectTypes.h"
#include "baseConnection.h"

// -----------------------------------------------------------------------------
// PROTECTED:
// -----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// BaseQuery::_isBindParameterPresent
//------------------------------------------------------------------------------
bool            
BaseQuery::_isBindParameterPresent(
      const string& paramName)
{
   if (_sqlStatement.find(paramName) == string::npos)
      return false;
   else 
      return true;
}  // BaseQuery::_isBindParameterPresent          


// -----------------------------------------------------------------------------
// PUBLIC:
// -----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// BaseQuery::BaseQuery
//------------------------------------------------------------------------------
BaseQuery::BaseQuery()
   : _eof(true)
   , _fieldCount(0)
   , _outputCount(0)
   , _effectCount(0)
   , _isFunctionQuery(false)
{
}  // BaseQuery::BaseQuery          



//------------------------------------------------------------------------------
// BaseQuery::~BaseQuery
//------------------------------------------------------------------------------
BaseQuery::~BaseQuery()
{   
     
}  // BaseQuery::~BaseQuery


//------------------------------------------------------------------------------
// BaseQuery::command
//------------------------------------------------------------------------------
void
BaseQuery::command(
     const string& sqlStatement)
{
   // Keep a copy of the sql statement.
   _sqlStatement = sqlStatement.c_str(); 
}  // BaseQuery::command



//------------------------------------------------------------------------------
// BaseQuery::execute
//------------------------------------------------------------------------------
void
BaseQuery::execute()
{
   // Set the default flags.
   _eof = true;   
   _fieldCount = 0;
}  // BaseQuery::execute



//------------------------------------------------------------------------------
// BaseQuery::eof
//------------------------------------------------------------------------------
bool
BaseQuery::eof()
{
   return _eof;   
}  // BaseQuery::eof



//------------------------------------------------------------------------------
// BaseQuery::fieldCount
//------------------------------------------------------------------------------
unsigned int
BaseQuery::fieldCount()
{
   return _fieldCount;
}  // BaseQuery::fieldCount

unsigned int
BaseQuery::outputCount()
{
	return _outputCount;
}
//------------------------------------------------------------------------------
// BaseQuery::effectCount
//------------------------------------------------------------------------------
unsigned int BaseQuery::effectCount()
{
	return _effectCount;
}