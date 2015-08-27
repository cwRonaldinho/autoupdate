/*
 * ODBCValue object defines the needed conversion result values
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

#ifndef __ODBC_RESVAL_H__
#define __ODBC_RESVAL_H__

#include <string>

#include "baseValue.h"


using namespace std;

class ODBCValue : public BaseValue
{
private: 

public:
 
   /* Constructors.
    *
    */
   ODBCValue(
         const string& bindName);


   /* Destructor.
    *
    */
   virtual
   ~ODBCValue();
      
};


#endif
