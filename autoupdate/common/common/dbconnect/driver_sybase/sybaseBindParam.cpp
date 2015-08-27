#ifdef _DB_SYBASE_
#include "stdafx.h"


#include "dbconnect/driver_sybase/sybaseBindParam.h"
#include "dbconnect/base/dbconnectTypes.h"


// -----------------------------------------------------------------------------
// SybaseBindParam::paramToSybaseString
// -----------------------------------------------------------------------------
string 
SybaseBindParam::paramToSybaseString()
{
   // Check if the value is null and return the appropriate null string
   if (!valuePtr.ptr)
      return "NULL";
  
  
   string res;
   char* to;
   
   switch(type)
   {
      case BP_STRING:
         // TODO: Find the correct way to escape the string and implement it
         res = "'";
         res += valuePtr.charPtr;       
         res += "'";
         break;   

      case BP_BINARY:                   
         res = "'";
         res += valuePtr.charPtr;       
         res += "'";
         break;      
     
      case BP_LONG:
         to = (char*)malloc(512);
         sprintf(to, DBLONG_SPRINTF, *valuePtr.longPtr);
         res = to;       
         free(to);         
         break;      

      case BP_ULONG:
         to = (char*)malloc(512);
         sprintf(to, DBULONG_SPRINTF, *valuePtr.unsignedLongPtr);
         res = to;       
         free(to);         
         break;      

      case BP_DOUBLE:
         to = (char*)malloc(512);
         sprintf(to, "%.25E", *valuePtr.doublePtr);    // Use a precision of 25 for the conversion
         res = "'";
         res += to;       
         res += "'";
         free(to);         
         break;      
      
      case BP_DATETIME:        
         // 'yyyy-mm-dd hh:mm:ss' string format
         res = "'";
         res += valuePtr.JDatePtr->asString("%Y-%m-%d %H:%M:%S");
         res += "'";
         break;      

      case BP_DATE:
         // 'yyyy-mm-dd' string format
         res = "'";
         res += valuePtr.JDatePtr->asString("%Y-%m-%d");
         res += "'";
         break;      

      case BP_TIME:
         // 'hh:mm:ss' string format
         res = "'";
         res += valuePtr.JDatePtr->asString("%H:%M:%S");
         res += "'";
         break;      

      case BP_BOOLEAN:
         // Convert a boolean to an integer value.
         if (*valuePtr.boolPtr)
            res = "1";
         else
            res = "0";
         break;
   }

   return res;
}  // SybaseBindParam::paramToSybaseString


#endif