/*
 * JDate date manipulation wrapper class using the Julian Day concept.
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
 * 
 * Notes:
 *  1)  The internal format of date time is stored in JD (Julian Day Number) format.
 *      The JD of 0.0 designates the 24 hours from noon 24 November 4713BC (Gregorian) to noon 25 November 4713BC (Gregorian)
 *      This is not to be confused with the Julian calander.
 *
 *  2)  This class uses years as astronomers years
 *      1 A.D. = 1 C.E. =  year 1
 *      1 B.C. = 1 B.C.E. = year 0
 *      2 B.C. = 2 B.C.E. = year -1
 *
 *  3)  All unix timestamps are treated as Coordinated Universal Time (UTC)
 *
 *  4)  Example JD's that can be used to test with
 *        -4713-11-25 18:00:00       JD=1.25
 *         1858-11-16 06:00:00       JD=2,399,999.75
 *         1858-11-16 00:00:00       JD=2,400,000.5
 *      
 *  5)  Understanding of the Julian Day concept will greatly enhance the using 
 *      of this class.
 *
 *  6)  The day of the week is in the range 0-6 with 0 = SUNDAY
 *
 *  7)  The ISO date inputes try to conform to ISO-8601
 *
 *  8)  JDate supports the concept of virtual time. I.e 1 second in real time can = x seconds in virtual time.
 *      The concept is used when constructing a JDate with the current time UTC. The current time is offsetted.
 *      A timeoffset file is required to enable virtual time. 
 *      The file structure is as follows:
 *         Time to start applying virtual time as Unix Time stamp UTC\n
 *         # of virtual seconds 1 real secons is equal to
 *      Use the JDATE_OFFSETTIME_FILE environment variable or
 *      use the setTimeOffsetFile function to set the timeoffset filename in order to enable virtual time.
 *
 * 
 * Date Time Format constants for converting a date to a string
 * Formating directives (Uses most of the formating elements as described in "man date")
 *    DIRECTIVE   MEANING    
 *    %%          A literal %
 *    %a          Abbreviated weekday name (Sun..Sat)
 *    %A          Full weekday name 
 *    %b          Abbreviated month name (Jan..Dec)
 *    %B          Full month name
 *    %d          Day of the month as a decimal number 01-31.
 *    %H          Hour (24-hour clock) as a decimal number 00-23.
 *    %m          Month as a decimal number 01-12.
 *    %M          Minute as a decimal number 00-59.
 *    %n          A newline
 *    %S          Second as a decimal number 00-61. (Leap seconds are also accounted for)
 *    %t          A horizontal tab
 *    %w          Day of week (0..6);  0 represents Sunday
 *    %y          Year without century as a decimal number 00-99.
 *    %Y          Year with century as a decimal number.    
 *    %z          Timezone of the time. In format of -08 etc   (Future Support)
 *
 * Date Time Format constants for converting a string to a date
 * Formating directives 
 *    DIRECTIVE   MEANING    
 *    yyyy        Year with century as a decimal number.
 *    yy          Year without century as a decimal number 00-99.
 *    mm          Month as a decimal number 01-12.
 *    dd          Day of the month as a decimal number 01-31.
 *    hh          Hour (24-hour clock) as a decimal number 00-23.
 *    nn          Minute as a decimal number 00-59.
 *    ss          Second as a decimal number 00-61. (Leap seconds are also accounted for)
 *    zzz         The optional timezone in the format -08 etc. If not present time is interpreted as -00 UTC
 *
 *
 */


#ifndef __JDATE_H__
#define __JDATE_H__

#ifdef WIN32
   // Windows
   // DLL Exports
   class  __declspec( dllexport ) JDate;
#endif   


#include <time.h>
#include <string>
#include <map>

#include "baseException.h"


using namespace std;


class JDate
{

private:
   map <long, double> meanTropicalYear;    // Stores the average tropical year for each 1000 year period
   map <int, int> maxDaysInMonths;         // Stores the maximum days per month in a normal gregorian year.
   map <int, int> maxDaysInMonthsLeap;     // Stores the maximum days per month in a leap gregorian year.


   // Static definitions
   static const string tropicalYearDef;
   static const char* shortDayOfWeek[7];
   static const char* dayOfWeek[7];
   static const char* shortNameOfMonth[12];
   static const char* nameOfMonth[12];

   // Actual date storage variables
   long _JD;    // Julian Day
   long _SN;    // Seconds since noon from the current JD. (Replaces decimal hour part of the JD number)
   long _TZ;    // Timezone offset in hours. FUTURE TIMEZONE SUPPORT
   long _DST;   // Daylight savings time offset in seconds. FUTURE DST SUPPORT
 
   // Used for the location of the virtual time config file
   static string timeOffsetFile;
   
   /* Initialize class constants etc
    *
    */
   void
   _initialize();



   /* Get the configuration parameters for virtual time if enabled
    *
    * @param startTimeStampUTC   Populated with the timestamp to start offset if function returns true
    * @param secondInterval      Populated with the interval to apply if function returns true
    *
    * @returns     Returns true if virtual time is enabled, otherwise false
    */
   static bool
   _getTimeOffsetParameters(
         unsigned long &startTimeStampUTC,
         unsigned long &secondInterval);



   /* Make an internal Julian Date Number from a gregorian date
    *
    * @param year  
    * @param month 
    * @param day
    * @param hour
    * @param minute
    * @param second
    *
    */
   void
   _makeJDFromGregorian(
         register int year,
         register int month,
         register int day,
         register int hour = 0,
         register int minute = 0,
         register int second = 0);


   /* Make a Gregorian date from the internal Julian Date Number
    *
    * @param year    Output parameter
    * @param month   Output parameter
    * @param day     Output parameter
    * @param hour    Output parameter
    * @param minute  Output parameter
    * @param second  Output parameter
    *
    */
   void
   _makeGregorianFromJD(
         int& year,
         int& month,
         int& day,
         int& hour,
         int& minute,
         int& second);



   /* Make an internal Julian Date Number from a Julian date
    *
    * @param year  
    * @param month 
    * @param day
    * @param hour
    * @param minute
    * @param second
    *
    */
   void
   _makeJDFromJulian(
         register int year,
         register int month,
         register int day,
         register int hour = 0,
         register int minute = 0,
         register int second = 0);


   /* Return the day of the week the objects date falls on
    *
    * @returns     Day of the week. 0 = Sun
    */
   int
   _makeDayOfWeek(
         register int year,
         register int month,
         register int day);


   /* Return the week number of the year
    *
    * @returns     Week of year.
    */
   int
   _makeWeekOfYear();


public:
   // Formatting constants
   static const char* DT_FMT_DATETIME_ISO;
   static const char* DT_FMT_DATE_ISO;
   static const char* DT_FMT_TIME_ISO;
   static const char* DT_FMT_SERIAL_ISO;
   static const char* SET_FMT_DATETIME_ISO;
   static const char* SET_FMT_DATETIME_ISO_TZ;
   static const char* SET_FMT_DATE_ISO;
   static const char* SET_FMT_TIME_ISO;
   static const char* SET_FMT_TIME_ISO_TZ;
   static const char* SET_FMT_SERIAL_ISO;
   static const char* SET_FMT_SERIAL_ISO_TZ;

   // Second constants
   static const long SECS_ONE_MINUTE;
   static const long SECS_ONE_HOUR;
   static const long SECS_ONE_DAY;

   struct JulianDayNumber
   {
      long julianDay;
      long secondsSinceMidday;
      
      // Constructors
      JulianDayNumber()
         :
            julianDay(0),
            secondsSinceMidday(0)
      { };

      JulianDayNumber(
            long _julianDay,
            long _secondsSinceMidday)
         :
            julianDay(_julianDay),
            secondsSinceMidday(_secondsSinceMidday)
      { };
         
   };

   enum Units
   {
      SECONDS,
      MINUTES,
      HOURS,
      DAYS,
      MONTHS,
      YEARS      
   };
   

   // Global Error Message Constants 
   enum ErrorCodes 
   {
      CONVERSION_ERROR,
      OUT_OF_RANGE
   };


   /* Exceptions Defined
    *
    */
   class ConversionError : public BaseException {
      public: ConversionError(const string description) 
              : BaseException(CONVERSION_ERROR, "ConversionError", description) {} };


   class OutOfRangeError : public BaseException {
      public: OutOfRangeError(const string description) 
              : BaseException(OUT_OF_RANGE, "OutOfRangeError", description) {} };


   /* Constructor
    *
    * Initialize the date to the current date time UTC
    */
   JDate();


   /* Constructor
    *
    * Initialize the date from the string specified
    */
   JDate(
         const string &dateStr,
         const string &fmt = SET_FMT_DATETIME_ISO);


   /* Constructor
    *
    * Initialize the date from the unix timestamp
    * Timezone is +ve or -ve integer defaulting to UTC
    */
   JDate(
         const time_t unixTime,
         const int    timeZone=0);


   /* Constructor
    *
    * Initialize the date as the JD specified
    */
   JDate(
         const double julianDayNumber);


   /* Constructor
    *
    * Initialize the date as the specified components
    * Timezone is +ve or -ve integer defaulting to UTC
    */
   JDate(
      int year,
      int month,
      int day,
      int hour = 0,
      int minute = 0,
      int second = 0,
      int timeZone = 0);


   /* Constructor
    *
    * Initialize the date as the specified JulianDayNumber structure
    */
   JDate(
      const JulianDayNumber& julianDayNumber);


   /* Destructor
    *
    */
   ~JDate();


   /* Sets the file to use to offset the time for virtual time
    *
    * @param file    The timeoffset file to use
    *
    */
   static void
   setTimeOffsetFile(
         const string &file);


   /* Returns the real seconds for the number of virtual seconds is equal to
    * This uses the time offset functionality.
    * If the time offset is disabled, then 1s virtual = 1s real
    *
    * @param virtualSecs  The number of virtual seconds to convert to real seconds
    *
    * @returns            The number of real seconds the virtual seconds are equal to
    */
   static unsigned long
   getRealSecondsForVirtualSeconds(
         const long virtualSecs);


   /* Determine if a year is a leap year according to the julian calander
    *
    * @param year  Astronomers year to test
    *
    * @returns     Returns true if the given year is a gregorian leap year
    */
   static bool
   isJulianLeapYear(
         long astronomersYear);


   /* Determine if a year is a leap year according to the gregorian calander
    *
    * @param year  Astronomers year to test
    *
    * @returns     Returns true if the given year is a gregorian leap year
    */
   static bool
   isGregorianLeapYear(
         long astronomersYear);


   /* Return a string representation of the internal date
    *
    * @param   fmt  Optional format parameter. Defaults to FMT_DATE_ISO
    *
    * @returns     Formated string representation of the internal JD.
    */
   string
   asString(
         const string &fmt = DT_FMT_DATETIME_ISO);


   /* Return a double representing the julian day number
    *
    * @returns     Julian Day Number. (JD)
    */
   double
   asJulianDayNumber();


   /* Return the unix timestamp if within range
    *
    * @returns     UnixTimeStamp
    */
   time_t
   asUnixTimeStamp();


   /* Return the date in its individual components
    *
    */
   void
   asComponents(
         int& year,
         int& month,
         int& day,
         int& hour,
         int& minute,
         int& second);


   /* Return the part of the date requested (ie. SECONDS, MINS, etc.)
    *
    * @returns     The date part unit as requested
    */
   int
   asComponent(
         Units unit);


   /* Return a JulianDayNumber struct
    *
    * @returns     JulianDayNumber struct
    */
   JulianDayNumber
   asJulianDayNumberStruct();


   /* Sets the date as a string
    *
    * @param dateStr     String representing the date in the format as in fmt
    * @param fmt         Optional format parameter. Defaults to FMT_DATE_ISO
    *
    */
   void
   setDate(
         const string &dateStr,
         const string &fmt = SET_FMT_DATETIME_ISO);


   /* Sets the date from a unix timestamp
    *
    * @unixTime   Unix timestamp to set the time to.
    * @timeZone   Optional timezone the date elements represent
    *
    */
   void
   setDate(
         const time_t unixTime,
         int timeZone = 0);


   /* Sets the date from a Julian Day Number with time fraction
    *
    * @julianDayNumber   JD to set the date to
    *
    */
   void
   setDate(
         const double julianDayNumber);


   /* Sets the date from individual date parameters.
    *
    * @param year    
    * @param month
    * @param day
    * @param hour
    * @param minute
    * @param second
    * @timeZone   Optional timezone the date elements represent
    *
    */
   void
   setDate(
         int year,
         int month,
         int day,
         int hour = 0,
         int minute = 0,
         int second = 0,
         int timeZone = 0);


   /* Sets the date from another jDate object
    *
    * @param dateObj     jDate Object to set the date from
    *
    */
   void
   setDate(
         const JDate &dateObj);


   /* Sets the date from a JulianDayNumber structure
    *
    * @param JulianDayNumber   The strucutre holding the julian day number and seconds 
    *
    */
   void
   setDate(
         const JulianDayNumber& julianDayNumber);


   /* Increases the stored date by the given number of unit types
    *
    * @param unit      Unit type to increase the date by
    * @param numUnits  Units to increase the date by
    *
    */
   void 
   modifyDate(
         Units unit,
         const long numUnits);


   /* Assignment operator to assign a string
    *
    * @param dateStr    date in the format of DT_FMT_DATETIME_ISO
    *
    */
   void
   operator=(
         const char* dateStr);


   /* Assignment operator to assign a string 
    *
    * @param dateStr    date in the format of DT_FMT_DATETIME_ISO
    *
    */
   void
   operator=(
         const string &dateStr);


   /* Assignment operator to assign a unix time stamp and convert it to a JD 
    * This converts using local time.
    *
    * @param time_t      unix time stamp to assign
    *
    */
   void
   operator=(
         time_t unixTime);


   /* Assignment operator to assign a date from a JD
    *
    * @param julianDayNumber    Julian Day number to assign
    *
    */
   void
   operator=(
         const double julianDayNumber);


   /* Assignment operator to assign a date from a JulianDayNumber structure
    *
    * @param julianDayNumber    Julian Day number strucuture to assign
    *
    */
   void
   operator=(
         const JulianDayNumber julianDayNumber);


   /* Arithmetic Addition Operator
    *
    * @returns     A new JDate object with the two dates added together.
    *
    */
   JDate
   operator+(
         const JDate &x);

   /* Arithmetic Subtraction Operator
    *
    * @returns     A new JDate object with the two dates subtracted from each together.
    *
    */
   JDate
   operator-(
         const JDate &x);


   /* Arithmetic Addition Assignment Operator
    *
    * @returns     A new JDate object with the two dates added together.
    *
    */
   JDate
   operator+=(
         const JDate &x);


   /* Arithmetic Subtraction Assignment Operator
    *
    * @returns     A new JDate object with the two dates added together.
    *
    */
   JDate
   operator-=(
         const JDate &x);


   /* Comparison Greater than Operator
    *
    * @returns    0 for false and 1 for true
    *
    */
   int
   operator>(
         const JDate &x);
         
         
   /* Comparison Greater than Equal Operator
    *
    * @returns    0 for false and 1 for true
    *
    */
   int
   operator>=(
         const JDate &x);


   /* Comparison Less than Operator
    *
    * @returns    0 for false and 1 for true
    *
    */
   int
   operator<(
         const JDate &x);


   /* Comparison Len than Equal Operator
    *
    * @returns    0 for false and 1 for true
    *
    */
   int
   operator<=(
         const JDate &x);


   /* Comparison Equal Operator
    *
    * @returns    0 for false and 1 for true
    *
    */
   int
   operator==(
         const JDate &x);


   /* Comparison Not Equal Operator
    *
    * @returns    0 for false and 1 for true
    *
    */
   int
   operator!=(
         const JDate &x);

};


#endif
