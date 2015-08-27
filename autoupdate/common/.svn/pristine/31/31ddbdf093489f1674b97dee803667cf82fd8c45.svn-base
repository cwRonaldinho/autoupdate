/*
 * JDate date manipulation wrapper class using the Julian Day concept.
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


#include "jDate.h"
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <fstream>


// Internal defines
#define INVALID_FUNCTION_RES      -31999
#define MIN_JD_FOR_UNIXTIMESTAMP  2440587.5      // 1970-01-01 00:00:00
#define MAX_JD_FOR_UNIXTIMESTAMP  2465442.5      // 2038-01-19 00:00:00
#define MAX_JD_FOR_DATE           5373484.499988 // 9999-12-31 23:59:59
#define JDATE_OFFSETTIME_FILE_ENV "JDATE_OFFSETTIME_FILE"


// Initialize the class static vars
const long JDate::SECS_ONE_MINUTE = 60;
const long JDate::SECS_ONE_HOUR = 3600;
const long JDate::SECS_ONE_DAY = 86400;


const string JDate::tropicalYearDef = "The time it takes the Sun to appear to travel around the sky from a given point of the tropical zodiac back to that same point in the tropical zodiac.";
const char* JDate::DT_FMT_DATETIME_ISO = "%Y-%m-%d %H:%M:%S";
const char* JDate::DT_FMT_DATE_ISO = "%Y-%m-%d";
const char* JDate::DT_FMT_TIME_ISO = "%H:%M:%S";
const char* JDate::DT_FMT_SERIAL_ISO = "%Y%m%d%H%M%S";
const char* JDate::SET_FMT_DATETIME_ISO = "yyyy-mm-dd hh:nn:ss";
const char* JDate::SET_FMT_DATETIME_ISO_TZ = "yyyy-mm-dd hh:nn:sszzz";
const char* JDate::SET_FMT_DATE_ISO = "yyyy-mm-dd";
const char* JDate::SET_FMT_TIME_ISO = "hh:nn:ss";
const char* JDate::SET_FMT_TIME_ISO_TZ = "hh:nn:sszzz";
const char* JDate::SET_FMT_SERIAL_ISO = "yyyymmddhhnnss";
const char* JDate::SET_FMT_SERIAL_ISO_TZ = "yyyymmddhhnnsszzz";

const char* JDate::shortDayOfWeek[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char* JDate::dayOfWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char* JDate::shortNameOfMonth[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                                           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char* JDate::nameOfMonth[12] = {"January", "Febuary", "March", "April", "May", "June", 
                                      "July", "August", "September", "October", "November", "December"};

string JDate::timeOffsetFile = "";


//------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// JDate::_initialize
//------------------------------------------------------------------------------
void
JDate::_initialize()
{
   // Populate the average tropical year
   meanTropicalYear[-5000] = 365.24253;
   meanTropicalYear[-4000] = 365.24250;
   meanTropicalYear[-3000] = 365.24246;
   meanTropicalYear[-2000] = 365.24242;
   meanTropicalYear[-1000] = 365.24237;
   meanTropicalYear[0]     = 365.24231;
   meanTropicalYear[1000]  = 365.24225;
   meanTropicalYear[2000]  = 365.24219;
   meanTropicalYear[3000]  = 365.24213;
   meanTropicalYear[4000]  = 365.24207;
   meanTropicalYear[5000]  = 365.24201;

   // Populate the max days per month in a normal gregorian year.
   maxDaysInMonths[1]  = 31;
   maxDaysInMonths[2]  = 28;
   maxDaysInMonths[3]  = 31;
   maxDaysInMonths[4]  = 30;
   maxDaysInMonths[5]  = 31;
   maxDaysInMonths[6]  = 30;
   maxDaysInMonths[7]  = 31;
   maxDaysInMonths[8]  = 31;
   maxDaysInMonths[9]  = 30;
   maxDaysInMonths[10] = 31;
   maxDaysInMonths[11] = 30;
   maxDaysInMonths[12] = 31;

   // Populate the max days per month in a leap gregorian year.
   maxDaysInMonthsLeap[1]  = 31;
   maxDaysInMonthsLeap[2]  = 29;
   maxDaysInMonthsLeap[3]  = 31;
   maxDaysInMonthsLeap[4]  = 30;
   maxDaysInMonthsLeap[5]  = 31;
   maxDaysInMonthsLeap[6]  = 30;
   maxDaysInMonthsLeap[7]  = 31;
   maxDaysInMonthsLeap[8]  = 31;
   maxDaysInMonthsLeap[9]  = 30;
   maxDaysInMonthsLeap[10] = 31;
   maxDaysInMonthsLeap[11] = 30;
   maxDaysInMonthsLeap[12] = 31;   
}  // JDate::_initialize


//------------------------------------------------------------------------------
// JDate::_getTimeOffsetParameters
//------------------------------------------------------------------------------
bool
JDate::_getTimeOffsetParameters(
      unsigned long &startTimeStampUTC,
      unsigned long &secondInterval)
{
   startTimeStampUTC = ULONG_MAX;
   secondInterval = 0;
                                                                                                                        
   // Check if we need to overide the time UTC using the JDate virtual time feature
   const char *file = NULL;
                                                                                                                        
   // RULE 1: Check the environment variable for the offset file to use
   // RULE 2: Check the manually configured offset time file and use that if the environment variable not set
   char *offsetFileEnv = getenv(JDATE_OFFSETTIME_FILE_ENV);
   if (offsetFileEnv && strlen(offsetFileEnv) > 0)
   {
      // RULE 1
      file = offsetFileEnv;
   }
   else
   {
      // RULE 2
      if (timeOffsetFile.length() > 0)
         file = timeOffsetFile.c_str();
   }
   
   // If we have an offset file, try and get the settings and apply them to the time
   if (file)
   {
      /*
       * The file is in the format
       *    xxx\n
       *    yyy
       * xxx: Unix time stamp UTC to start the virtual time from
       * yyy: The number of virtual seconds each real second is equal to
       *
       */
                                                                                                                        
      // Try and read the contents of the file
      char *endptr1, *endptr2;
      char buf[512];
                                                                                                                        
      ifstream in;
      in.open(file);
      if (!in.fail())
      {
         in.getline(buf, sizeof(buf));
         startTimeStampUTC = strtol(buf, &endptr1, 10);
                                                                                                                        
         in.getline(buf, sizeof(buf));
         secondInterval = strtol(buf, &endptr2, 10);
                                                                                                                        
         in.close();
      }
      else
         return false;  // Could not open the file, virtual time is disabled
                                                                                                                        
      // If there are no errors then the time is offet
      if (endptr1 && endptr2 && startTimeStampUTC > 0 && secondInterval > 0)
         return true;     
   }
   
   return false;
} // JDate::_getTimeOffsetParameters


//------------------------------------------------------------------------------
// JDate::_makeJDFromGregorian
//------------------------------------------------------------------------------
void
JDate::_makeJDFromGregorian(
      register int year,
      register int month,
      register int day,
      register int hour,
      register int minute,
      register int second)
{
   // TODO: Validate the date.

   register long a, y, m;
   
   // Calculate the Gregorian Julian Day Number. (Will be at noon)
   a = (14 - month) / 12;
   y = year + 4800 - a;
   m = month + (12 * a) -3;

   _JD = day + ( ((153 * m) + 2) / 5 ) + (365 * y) + (y / 4) - (y / 100) + (y / 400) - 32045;

   // Offset the JD with the given time.
   if (hour < 12)
   {
      // Falls on the previouse julian day.
      _JD -= 1;  
      _SN = ((hour + 12) * 3600) + (minute * 60) + second;
   }
   else
   {
      // Falls on the current Julian Day.
      _SN = ((hour - 12) * 3600) + (minute * 60) + second;
   }
}  // JDate::_makeJDFromGregorian


//------------------------------------------------------------------------------
// JDate::_makeGregorianFromJD
//------------------------------------------------------------------------------
void
JDate::_makeGregorianFromJD(
      int& year,
      int& month,
      int& day,
      int& hour,
      int& minute,
      int& second)
{
   register long a, b, c, d, e, m;
      
   long jd = _JD, sn = _SN;

   // Offset the current JD given the time.
   if (sn >= 43200)   // Seconds since miday. 43200 is midnight of next day.
   {
      jd++;
      sn -= 43200;
   }
   else
   {
      sn += 43200;
   }
   
   a = jd + 32044;
   b = ((4 * a) + 3) / 146097;
   c = a - ((146097*b) / 4);

   d = ((4*c) + 3) / 1461;
   e = c - ((1461 * d) / 4);
   m = ((5 * e) + 2) / 153;
      
   day = e - (((153 * m) + 2) / 5) + 1; 
   month = m + 3 - 12 * (m / 10);
   year = (100 * b) + d - 4800 + (m / 10);
   
   hour = sn / 3600;
   minute = (sn - hour * 3600) / 60;
   second = sn - ((hour * 3600) + (minute * 60));

   if (24 == hour)
      hour = 0;
}  // JDate::_makeGregorianFromJD


//------------------------------------------------------------------------------
// JDate::_makeJDFromJulian
//------------------------------------------------------------------------------
void
JDate::_makeJDFromJulian(
      register int year,
      register int month,
      register int day,
      register int hour,
      register int minute,
      register int second)
{
   // TODO: Validate the date.
   register long a, y, m;
   
   // Calculate the Gregorian Julian Day Number. (Will be at noon)
   a = (14 - month) / 12;
   y = year + 4800 - a;
   m = month + (12 * a) -3;
   _JD = day + ( ((153 * m) + 2) / 5 ) + (365 * y) + (y / 4) - 32083;

   
   // Offset the JD with the given time.
   if (hour < 12)
   {
      // Falls on the previouse julian day.
      _JD -= 1;  
      _SN = ((hour + 12) * 3600) + (minute * 60) + second;
   }
   else
   {
      // Falls on the current Julian Day.
      _SN = ((hour - 12) * 3600) + (minute * 60) + second;
   }
}  // JDate::_makeJDFromJulian


//------------------------------------------------------------------------------
// JDate::_makeDayOfWeek
//------------------------------------------------------------------------------
int
JDate::_makeDayOfWeek(
      register int year,
      register int month,
      register int day)
{
   // ISO-8601 states that monday is the first day of the week.
   // We use sunday as the first day of the week.

   register long a, d, m, y;

   a = (14 - month) / 12;
   y = year - a;   
   m = month + (12 * a) - 2;
   
   d = (day + y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12)) % 7;
  
   return d;   
}  // JDate::_makeDayOfWeek


//------------------------------------------------------------------------------
// JDate::_makeWeekOfYear
//------------------------------------------------------------------------------
int
JDate::_makeWeekOfYear()
{
   // Calculate using ISO-8601
   register long d1, L, d4;
   int w;
   
   d4 = (_JD + 31741 - (_JD % 7)) % 146097 % 36524 % 1461;
   L = d4 / 1460;
   d1 = ((d4 - L) % 365) + L;
   w = (d1 / 7) + 1;

   return w;
}  // JDate::_makeWeekOfYear


//------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// JDate::JDate
//------------------------------------------------------------------------------
JDate::JDate()
   :
      _JD(0),
      _SN(0),
      _TZ(0),
      _DST(0)
{
   // Initialize the class
   _initialize();


   // Assign the current date & time UTC using the operator=(time_t)
   time_t t;
   time(&t);
   
   // Check if we need to offset the virtual time
   unsigned long startTimeStampUTC, secondInterval;
   if (_getTimeOffsetParameters(startTimeStampUTC, secondInterval))
   {
      if (t >= startTimeStampUTC)
      {
         unsigned long numSecondIntervals = (unsigned long) t - startTimeStampUTC;
         t = startTimeStampUTC + (numSecondIntervals * secondInterval);
      }
   }

   // Assign the current date & time UTC using the setDate(time_t)
   setDate(t);
}  // JDate::JDate


//------------------------------------------------------------------------------
// JDate::JDate
//------------------------------------------------------------------------------
JDate::JDate(
      const string &dateStr,
      const string &fmt)
   :
      _JD(0),
      _SN(0),
      _TZ(0),
      _DST(0)
{
   // Initialize the class
   _initialize();

   setDate(dateStr, fmt);
}  // JDate::JDate


//------------------------------------------------------------------------------
// JDate::JDate
//------------------------------------------------------------------------------
JDate::JDate(
      const time_t unixTime,
      const int    timeZone)
   :
      _JD(0),
      _SN(0),
      _TZ(0),
      _DST(0)

{
   // Initialize the class
   _initialize();

   setDate(unixTime, timeZone);
}  // JDate::JDate


//------------------------------------------------------------------------------
// JDate::JDate
//------------------------------------------------------------------------------
JDate::JDate(
      const double julianDayNumber)
   :
      _JD(0),
      _SN(0),
      _TZ(0),
      _DST(0)
{
   // Initialize the class
   _initialize();

   setDate(julianDayNumber);
}  // JDate::JDate


//------------------------------------------------------------------------------
// JDate::JDate
//------------------------------------------------------------------------------
JDate::JDate(
   int year,
   int month,
   int day,
   int hour,
   int minute,
   int second,
   int timeZone)
   :
      _JD(0),
      _SN(0),
      _TZ(0),
      _DST(0)
{
   // Initialize the class
   _initialize();

   setDate(year, month, day, hour, minute, second, timeZone);
}  // JDate::JDate


//------------------------------------------------------------------------------
// JDate::JDate
//------------------------------------------------------------------------------
JDate::JDate(
   const JulianDayNumber& julianDayNumber)
{
   // Initialize the class
   _initialize();

   setDate(julianDayNumber);
}  // JDate::JDate


//------------------------------------------------------------------------------
// JDate::~JDate
//------------------------------------------------------------------------------
JDate::~JDate()
{

}  // JDate::~JDate


//------------------------------------------------------------------------------
// JDate::setTimeOffsetFile
//------------------------------------------------------------------------------
void
JDate::setTimeOffsetFile(
      const string &file)
{
   timeOffsetFile = file.c_str();
} // JDate::setTimeOffsetFile


//------------------------------------------------------------------------------
// JDate::getRealSecondsForVirtualSeconds
//------------------------------------------------------------------------------
unsigned long
JDate::getRealSecondsForVirtualSeconds(
      const long virtualSecs)
{
   unsigned long startTimeStampUTC, secondInterval;
   unsigned long res = virtualSecs;
                                                                                                                        
   if (_getTimeOffsetParameters(startTimeStampUTC, secondInterval))
   {
      res /= secondInterval;    // The number of real seconds the virtual seconds are equal to :)
   }
                                                                                                                        
   return res;
} // JDate::getRealSecondsForVirtualSeconds


//------------------------------------------------------------------------------
// JDate::isJulianLeapYear
//------------------------------------------------------------------------------
bool
JDate::isJulianLeapYear(
      long astronomersYear)
{
   // If the year is divisible by 4 it is a Julian Leap year unless,
   // there where no leap years between 9BC and 8AD (Not certain as some theories stat 4AD) or
   // between 45BC and 10BC every 3 years was a leap year.

   // -8 = 9BC, 8 = 8AD in astronomers standards
   if (astronomersYear >= -8 && astronomersYear <= 8)
      return false;

   // -44 = 45BC, -9 = 10BC in astronomers standards.   
   if (astronomersYear >= -44 && astronomersYear <= -9)
      return !(astronomersYear % 3);

   // Default rule.
   return !(astronomersYear % 4);
}  // JDate::isJulianLeapYear


//------------------------------------------------------------------------------
// JDate::isGregorianLeapYear
//------------------------------------------------------------------------------
bool
JDate::isGregorianLeapYear(
      long astronomersYear)
{
   // If the year is divisible by 4 it is a Gregorian Leap year unless,
   // the year is divisible by 100,
   // but finaly if the year is divisible by 400 it is a leap year.   
   // The first Gregorian calander was founded in 1582

   if (astronomersYear < 1582)
      return false;

   if (astronomersYear % 400 == 0)
      return true;
               
   if ((astronomersYear % 4 == 0) && !(astronomersYear % 100 == 0))
      return true;

   return false;
}  // JDate::isGregorianLeapYear
 
 
//------------------------------------------------------------------------------
// JDate::asString
//------------------------------------------------------------------------------
string
JDate::asString(
      const string &fmt)
{
   string res = fmt;   
   int y, m, d, hh, mm, ss;
   size_t pos;
   char tmp[16];

   // Get the gregorian date from the JD
   _makeGregorianFromJD(y, m, d, hh, mm, ss);
            
   // Substitute the longest directive to the smallest.

   // %%
   if ((pos = res.find("%%")) != string::npos)
   {
      res.replace(pos, 2, "%");
   }

   // %a
   if ((pos = res.find("%a")) != string::npos)
   {
      int dow = _makeDayOfWeek(y, m, d);    
      res.replace(pos, 2, shortDayOfWeek[dow]);
   }

   // %A
   if ((pos = res.find("%A")) != string::npos)
   {
      int dow = _makeDayOfWeek(y, m, d);    
      res.replace(pos, 2, dayOfWeek[dow]);
   }

   // %b
   if ((pos = res.find("%b")) != string::npos)
   {
      res.replace(pos, 2, shortNameOfMonth[m-1]);
   }

   // %B
   if ((pos = res.find("%B")) != string::npos)
   {
      res.replace(pos, 2, nameOfMonth[m-1]);
   }

   // %d
   if ((pos = res.find("%d")) != string::npos)
   {
      sprintf(tmp, "%02d", d);
      res.replace(pos, 2, tmp);                      
   }

   // %H
   if ((pos = res.find("%H")) != string::npos)
   {
      sprintf(tmp, "%02d", hh);
      res.replace(pos, 2, tmp);                                 
   }

   // %m
   if ((pos = res.find("%m")) != string::npos)
   {
      sprintf(tmp, "%02d", m);
      res.replace(pos, 2, tmp);                      
   }

   // %M
   if ((pos = res.find("%M")) != string::npos)
   {
      sprintf(tmp, "%02d", mm);
      res.replace(pos, 2, tmp);           
   }

   // %n
   if ((pos = res.find("%n")) != string::npos)
   {
      res.replace(pos, 2, (const char*)'\n');
   }

   // %S
   if ((pos = res.find("%S")) != string::npos)
   {
      sprintf(tmp, "%02d", ss);
      res.replace(pos, 2, tmp);                                                       
   }

   // %t
   if ((pos = res.find("%t")) != string::npos)
   {
      res.replace(pos, 2, (const char*)'\t');           
   }

   // %w
   if ((pos = res.find("%w")) != string::npos)
   {
      int dow = _makeDayOfWeek(y, m, d);
      sprintf(tmp, "%d", dow);
      res.replace(pos, 2, tmp);
   }

   // %y
   if ((pos = res.find("%y")) != string::npos)
   {
      sprintf(tmp, "%02d", y % 100);
      res.replace(pos, 2, tmp);           
   }

   // %Y
   if ((pos = res.find("%Y")) != string::npos)
   {
      sprintf(tmp, "%04d", y);
      res.replace(pos, 2, tmp);
   }
 
   return res;
}  // JDate::asString


//------------------------------------------------------------------------------
// JDate::asJulianDayNumber
//------------------------------------------------------------------------------
double
JDate::asJulianDayNumber()
{
   // Build the floating point representation of a JD
   double res = _SN;
   res /= (1.0 * SECS_ONE_DAY);
   res += _JD;
   return res;
}  // JDate::asJulianDayNumber


//------------------------------------------------------------------------------
// JDate::asComponents
//------------------------------------------------------------------------------
void
JDate::asComponents(
   int& year,
   int& month,
   int& day,
   int& hour,
   int& minute,
   int& second)
{
   _makeGregorianFromJD(year, month, day, hour, minute, second);
}  // JDate::asComponents
   

//------------------------------------------------------------------------------
// JDate::asComponent
//------------------------------------------------------------------------------
int
JDate::asComponent(
   Units unit)
{
   int year, month, day, hour, minute, second;
   asComponents(year, month, day, hour, minute, second);
                                                                                                                        
   switch(unit)
   {
      case YEARS:   return year; break;
      case MONTHS:  return month; break;
      case DAYS:    return day; break;
      case HOURS:   return hour; break;
      case MINUTES: return minute; break;
      case SECONDS: return second; break;
      default: return -1;
   }
}  // JDate::asComponent


//------------------------------------------------------------------------------
// JDate::asJulianDayNumberStruct
//------------------------------------------------------------------------------
JDate::JulianDayNumber
JDate::asJulianDayNumberStruct()
{
   // Build the floating point representation of a JD
   JulianDayNumber res;
   res.julianDay = _JD;
   res.secondsSinceMidday = _SN;
   return res;
}  // JDate::asJulianDayNumberStruct


//------------------------------------------------------------------------------
// JDate::asUnixTimeStamp
//------------------------------------------------------------------------------
time_t
JDate::asUnixTimeStamp()
{
   // Check if the timestamp is within range
   double jd = asJulianDayNumber();
   if (jd > MAX_JD_FOR_UNIXTIMESTAMP || jd < MIN_JD_FOR_UNIXTIMESTAMP)
      throw OutOfRangeError("The internal date is not within the range 1970-01-01 00:00:00 --> 2038-01-19 00:00:00"); 


   // Convert to a unix timestamp as UTC.
   time_t res = 0;
   long days = _JD - (long)MIN_JD_FOR_UNIXTIMESTAMP;


   // Create the value from the number of days and seconds.
   res = days * SECS_ONE_DAY;
   res = res + _SN - 43200;

   return res;   
}  // JDate::asUnixTimeStamp


//------------------------------------------------------------------------------
// JDate::setDate
//------------------------------------------------------------------------------
int convertDatePortion(
      const string &item,
      const string &dateStr,
      const string &fmt)
{
   size_t pos;
   int res = INVALID_FUNCTION_RES;

   if ( (pos = fmt.find(item)) != string::npos)
   {
      char tmp[16];
      char *offset, *endptr = NULL;
   
      memset(tmp, 0, sizeof(tmp));
      offset = (char*)dateStr.c_str()+(pos * sizeof(char));      
      strncpy(tmp, offset, item.length());
      res = strtol(tmp, &endptr, 10);

      // Check if a conversion error occured
      if (endptr[0] != '\0')
      {
         string err = "The ";
         err += item;
         err += " value is in the incorrect format";
         throw JDate::ConversionError(err);
      }
   }

   return res;   
}


//------------------------------------------------------------------------------
// JDate::setDate
//------------------------------------------------------------------------------
void
JDate::setDate(
      const string &dateStr,
      const string &fmt)
{
   bool yearNeg = false;
   string dateString = dateStr;
    
   // If the dateStr is 1 bigger than the formating string, make sure it is due to a -ve sign
   if (1 == dateString.length() - fmt.length())
   {
      if ('-' == dateString.c_str()[0])
      {
         yearNeg = true;         
         // Strip the -ve sign from the dateString
         dateString = (dateString.c_str() + (1 * sizeof(char)));
      }
      else
         throw ConversionError("The date elements do not match the formatting string elements");
   }   
   else
   {   
      // Make sure the strings are the same length 
      if (dateString.length() != fmt.length())
         throw ConversionError("The date elements do not match the formatting string elements");
   }


   // Try and obtain the elements as decribed in the format string      
   int tmpVal;
   int y = -4713, m = 1, d = 1, hh = 0, mm = 0, ss = 0, zzz = 0;

   // yyyy
   if (-4713 == y && (tmpVal = convertDatePortion("yyyy", dateString, fmt)) != INVALID_FUNCTION_RES)
      y = tmpVal;

   // yy
   if (-4713 == y && (tmpVal = convertDatePortion("yy", dateString, fmt)) != INVALID_FUNCTION_RES)
      y = tmpVal;

   // mm
   if ((tmpVal = convertDatePortion("mm", dateString, fmt)) != INVALID_FUNCTION_RES)
      m = tmpVal;

   // dd
   if ((tmpVal = convertDatePortion("dd", dateString, fmt)) != INVALID_FUNCTION_RES)
      d = tmpVal;

   // hh
   if ((tmpVal = convertDatePortion("hh", dateString, fmt)) != INVALID_FUNCTION_RES)
      hh = tmpVal;

   // nn
   if ((tmpVal = convertDatePortion("nn", dateString, fmt)) != INVALID_FUNCTION_RES)
      mm = tmpVal;

   // ss
   if ((tmpVal = convertDatePortion("ss", dateString, fmt)) != INVALID_FUNCTION_RES)
      ss = tmpVal;

   // zzz
   if ((tmpVal = convertDatePortion("zzz", dateString, fmt)) != INVALID_FUNCTION_RES)
      zzz = tmpVal;

   // Convert the year to negative if needed
   if (yearNeg)
      y *= -1;

   // Build the JD
   _makeJDFromGregorian(y, m, d, hh, mm, ss);
   
   // If we have a timezone specified, convert to UTC from the timezone
   if (zzz != 0)
   {
      int changeHour = zzz * -1;     // Eg for GMT +2 need to subtract 2 hours
      modifyDate(HOURS, changeHour); // Internal date is now UTC 
   }
   _TZ = zzz;
}  // JDate::setDate


//------------------------------------------------------------------------------
// JDate::setDate
//------------------------------------------------------------------------------
void
JDate::setDate(
      const time_t unixTime,
      const int    timeZone)
{
   // Assign using the time of the unix time stamp
   struct tm ut;

   ut = *gmtime(&unixTime);
   _makeJDFromGregorian(ut.tm_year+1900, ut.tm_mon+1, ut.tm_mday, ut.tm_hour, ut.tm_min, ut.tm_sec);
   
   // If we have a timezone specified, convert to UTC from the timezone
   if (timeZone != 0)
   {
      int changeHour = timeZone * -1;     // Eg for GMT +2 need to subtract 2 hours
      modifyDate(HOURS, changeHour);      // Internal date is now UTC 
   }
   _TZ = timeZone;
}  // JDate::setDate
         

//------------------------------------------------------------------------------
// JDate::setDate
//------------------------------------------------------------------------------
void
JDate::setDate(
      const double julianDayNumber)
{
   _JD = (long)fabs(julianDayNumber);
   _SN = (long)fabs((julianDayNumber - _JD) * SECS_ONE_DAY);
   _TZ = 0;
}  // JDate::setDate


//------------------------------------------------------------------------------
// JDate::setDate
//------------------------------------------------------------------------------
void
JDate::setDate(
   int year,
   int month,
   int day,
   int hour,
   int minute,
   int second,
   int timeZone)
{   
   _makeJDFromGregorian(year, month, day, hour, minute, second);
   
   // If we have a timezone specified, convert to UTC from the timezone
   if (timeZone != 0)
   {
      int changeHour = timeZone * -1;     // Eg for GMT +2 need to subtract 2 hours
      modifyDate(HOURS, changeHour);      // Internal date is now UTC 
   }   
   _TZ = timeZone;   
}  // JDate::setDate
   

//------------------------------------------------------------------------------
// JDate::setDate
//------------------------------------------------------------------------------
void
JDate::setDate(
      const JDate &dateObj)
{
   _JD = dateObj._JD;
   _SN = dateObj._SN;
   _TZ = dateObj._TZ;
}  // JDate::setDate


//------------------------------------------------------------------------------
// JDate::setDate
//------------------------------------------------------------------------------
void
JDate::setDate(
      const JulianDayNumber& julianDayNumber)
{
   _JD = julianDayNumber.julianDay;
   _SN = julianDayNumber.secondsSinceMidday;      
   _TZ = 0;
}


//------------------------------------------------------------------------------
// JDate::modifyDate
//------------------------------------------------------------------------------
void 
JDate::modifyDate(
      Units unit,
      const long numUnits)
{
   long years, months, days, secs;
   int y, m, d, hh, mm, ss;
   
   switch (unit)
   {
      case SECONDS:
         // Add given seconds to the date.
         days = numUnits / SECS_ONE_DAY;
         secs = numUnits % SECS_ONE_DAY;

         // Make sure we are not out of bounds
         if (days > 0 && (unsigned long)_JD + (unsigned long)days > (unsigned long)MAX_JD_FOR_DATE)
            throw OutOfRangeError("Unable to convert seconds. Value increases date larger than year 9999.");
         if (days < 0 && days + _JD < 0)
            throw OutOfRangeError("Unable to convert seconds. Value decreases date smaller than year -4713.");

         // Offset the days.
         _JD += days;
                  
         // Offset the seconds correctly.
         // > 1 day in seconds        
         if (secs > 0 && secs + _SN >= SECS_ONE_DAY)
         {
            _JD++;
            _SN = (secs + _SN) - SECS_ONE_DAY;
         }
         else
         {
            // < 1 day in seconds
            if (secs < 0 && secs + _SN < 0)
            {
               _JD--;   
               _SN = SECS_ONE_DAY + (secs + _SN);
            }
            else
            {         
               // Normal, just offset the number of seconds
               _SN += secs;            
            }
         }
         break;   

      case MINUTES:
         if (numUnits > 0 && numUnits > LONG_MAX / SECS_ONE_MINUTE)
            throw OutOfRangeError("Unable to convert minutes to seconds. Value too large.");
         if (numUnits < 0 && numUnits < LONG_MIN / SECS_ONE_MINUTE)
            throw OutOfRangeError("Unable to convert minutes to seconds. Value too large.");
         modifyDate(SECONDS, numUnits * SECS_ONE_MINUTE);
         break;   

      case HOURS:
         if (numUnits > 0 && numUnits > LONG_MAX / SECS_ONE_HOUR)
            throw OutOfRangeError("Unable to convert hours to seconds. Value too large.");
         if (numUnits < 0 && numUnits < LONG_MIN / SECS_ONE_HOUR)
            throw OutOfRangeError("Unable to convert hours to seconds. Value too large.");
         modifyDate(SECONDS, numUnits * SECS_ONE_HOUR);
         break;

      case DAYS:
         // Make sure the days does not exceed the range
         if (numUnits > 0 && (unsigned long)_JD + (unsigned long)numUnits > (unsigned long)MAX_JD_FOR_DATE)
            throw OutOfRangeError("Unable to convert days. Value increases date larger than year 9999.");
         if (numUnits < 0 && numUnits + _JD < 0)
            throw OutOfRangeError("Unable to convert days. Value decreases date smaller than year -4713.");
         _JD += numUnits;
         break;

      case MONTHS:
         // Convert to gregorian, change and convert back
         _makeGregorianFromJD(y, m, d, hh, mm, ss);
         years = numUnits / 12;
         months = numUnits % 12;
         // Offset the years correctly         
         y += years;         

         // Offset the months
         m += months;
      
         if (m > 12)
         {
            y++;
            m -= 12;            
         }
         
         if (m <= 0)
         {
            y--;
            m = 12 + m;            
         }
         

         if (y > 9999)
            throw OutOfRangeError("Unable to convert months. Value increases date larger than year 9999.");
         if (y < -4713)
            throw OutOfRangeError("Unable to convert months. Value decreases date smaller than year -4713.");
      
         // Check for a day overflow
         if (isGregorianLeapYear(y))
         {
           if (d > maxDaysInMonthsLeap[m])
               d = maxDaysInMonthsLeap[m];
         }
         else
         {
            if (d > maxDaysInMonths[m])
               d = maxDaysInMonths[m];
         }

         _makeJDFromGregorian(y, m, d, hh, mm, ss);
         break;

      case YEARS:
         // Convert to gregorian, change and convert back
         _makeGregorianFromJD(y, m, d, hh, mm, ss);

         if (numUnits > 0 && numUnits + y > 9999)
            throw OutOfRangeError("Unable to convert years. Value increases date larger than year 9999.");
         if (numUnits < 0 && numUnits + y < -4713)
            throw OutOfRangeError("Unable to convert years. Value decreases date smaller than year -4713.");
         y += numUnits;

         _makeJDFromGregorian(y, m, d, hh, mm, ss);
         break;
   }
}  // JDate::incDate


//------------------------------------------------------------------------------
// JDate::operator=
//------------------------------------------------------------------------------
void
JDate::operator=(
      const char* dateStr)
{
   setDate(dateStr);   
}  // JDate::operator=


//------------------------------------------------------------------------------
// JDate::operator=
//------------------------------------------------------------------------------
void
JDate::operator=(
      const string &dateStr)
{
   setDate(dateStr);   
}  // JDate::operator=

   
//------------------------------------------------------------------------------
// JDate::operator=
//------------------------------------------------------------------------------
void
JDate::operator=(
      time_t unixTime)
{
   setDate(unixTime);
}  // JDate::operator=

   
//------------------------------------------------------------------------------
// JDate::operator=
//------------------------------------------------------------------------------
void
JDate::operator=(
      const double julianDayNumber)
{
   setDate(julianDayNumber);
}  // JDate::operator=


//------------------------------------------------------------------------------
// JDate::operator=
//------------------------------------------------------------------------------
void
JDate::operator=(
      const JulianDayNumber julianDayNumber)
{
   setDate(julianDayNumber);
}


//------------------------------------------------------------------------------
// JDate::operator+  
//------------------------------------------------------------------------------
JDate
JDate::operator+(
      const JDate &x)
{
   // Initialize z with the value of our current date
   JDate z;
   z._JD = _JD;
   z._SN = _SN;

   // Add x to the z date.
   z.modifyDate(DAYS, x._JD);
   z.modifyDate(SECONDS, x._SN);
   
   return z;
}  // JDate::operator+


//------------------------------------------------------------------------------
// JDate::operator-
//------------------------------------------------------------------------------
JDate
JDate::operator-(
      const JDate &x)
{
   // Initialize z with the value of our current date
   JDate z;
   z._JD = _JD;
   z._SN = _SN;

   // Subtract x from the z date.
   z.modifyDate(DAYS, -1 * x._JD);
   z.modifyDate(SECONDS, -1 * x._SN);
   
   return z;
}  // JDate::operator-


//------------------------------------------------------------------------------
// JDate::operator+=
//------------------------------------------------------------------------------
JDate
JDate::operator+=(
      const JDate &x)
{
   // Add x to our date
   modifyDate(DAYS, x._JD);
   modifyDate(SECONDS, x._SN);
   
   return *this;
}  // JDate::operator+=


//------------------------------------------------------------------------------
// JDate::operator-=
//------------------------------------------------------------------------------
JDate
JDate::operator-=(
      const JDate &x)
{   
   // Subtract x from our date
   modifyDate(DAYS, -1 * x._JD);
   modifyDate(SECONDS, -1 * x._SN);
   
   return *this;
}  // JDate::operator-=


//------------------------------------------------------------------------------
// JDate::operator>
//------------------------------------------------------------------------------     
int
JDate::operator>(
      const JDate &x)
{
   if (_JD > x._JD)
      return 1;
   if (_JD == x._JD && _SN > x._SN)
      return 1;
   
   return 0;
}
   

//------------------------------------------------------------------------------
// JDate::operator>=
//------------------------------------------------------------------------------     
int
JDate::operator>=(
      const JDate &x)
{
   if (_JD > x._JD)
      return 1;
   if (_JD == x._JD && _SN >= x._SN)
      return 1;
   
   return 0;
}


//------------------------------------------------------------------------------
// JDate::operator<
//------------------------------------------------------------------------------     
int
JDate::operator<(
      const JDate &x)
{
   if (_JD < x._JD)
      return 1;
   if (_JD == x._JD && _SN < x._SN)
      return 1;
   
   return 0;
}


//------------------------------------------------------------------------------
// JDate::operator<=
//------------------------------------------------------------------------------     
int
JDate::operator<=(
      const JDate &x)
{
   if (_JD < x._JD)
      return 1;
   if (_JD == x._JD && _SN <= x._SN)
      return 1;
   
   return 0;
}


//------------------------------------------------------------------------------
// JDate::operator==
//------------------------------------------------------------------------------     
int
JDate::operator==(
      const JDate &x)
{
   return (_JD == x._JD && _SN == x._SN);
}


//------------------------------------------------------------------------------
// JDate::operator!=
//------------------------------------------------------------------------------     
int
JDate::operator!=(
      const JDate &x)
{
   return (!(_JD == x._JD && _SN == x._SN));
}
