#ifdef _DB_SYBASE_
#ifndef __SYBASE_BINDPARAM_H__
#define __SYBASE_BINDPARAM_H__

#include <string>

#include "dbconnect/base/baseValue.h"

using namespace std;


class SybaseBindParam : public BaseValue
{
private: 

public:
 
   SybaseBindParam(
      const string& bindName)
      : BaseValue(bindName) 
   { };


   /* Converts the parameter to a sybase query string representation.
    *
    * @return   String that can be used in a sybase query for the parameter type.
    */
   string 
   paramToSybaseString();
};


#endif

#endif