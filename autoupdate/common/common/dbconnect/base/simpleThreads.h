/*
 * SimpleThread Object wrapping thread implementations of different platforms.
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
 *
 */


#ifndef __SIMPLE_THREADS_H__
#define __SIMPLE_THREADS_H__

#ifdef WIN32
   // Windows
   // DLL Exports
   class  __declspec( dllexport ) SimpleThread;
   class  __declspec( dllexport ) SimpleThread_Mutex;
   class  __declspec( dllexport ) SimpleThread_Synchronize;
#endif   


#include <time.h>
#include <string>
#include <stdio.h>

#ifdef WIN32
   //Windows
   #include <windows.h>
   #include <winbase.h>
#else
   // Unices
   #include <pthread.h>
   #include <unistd.h>
   #include <sys/time.h>
   #include <sys/types.h>
#endif

#include "baseException.h"

using namespace std;

class SimpleThread;
class SimpleThread_Mutex;
class SimpleThread_Synchronize;


class SimpleThread
{

private:

public:
   /* Constructor
    *
    */
   SimpleThread();

   /* Destructor
    *
    */
   ~SimpleThread();   
   
   /* Cause the calling thread to sleep the specified milli seconds
    *
    */
   static void 
   sleep(
         unsigned int milliSeconds);
};



class SimpleThread_Mutex
{

private:
#ifdef WIN32
   //Windows   
   HANDLE          hmutex;
#else
   // Unices
   pthread_mutex_t hmutex;
#endif
   bool            isMutexOwned;
   time_t          lockTime;     


public:
   // Global Error Message Constants 
   enum ErrorCodes 
   {
      MUTEX_ERROR
   };

   /* Exceptions Defined
    *
    */
   class MutexError : public BaseException {
      public: MutexError(const string description) 
              : BaseException(MUTEX_ERROR, "MutexError", description) {} };

   /* Constructor
    *
    */
   SimpleThread_Mutex();

   /* Destructor
    *
    */
   ~SimpleThread_Mutex();
   
   /* Determine if the mutex is locked.
    *
    * @return  Returns true if the mutex is currently locked.
    */
   bool 
   isLocked();

   /* Obtain the timestamp of when the mutex was locked.
    *
    * @return  Returns timestamp of when locked, or 0 if not locked.
    */
   time_t 
   getLockTime();
   
   /* Block until the mutex is owned. (Locked)
    *
    */
   void 
   lock();

   /* Release ownership of the mutex. (UnLocked)
    *
    */
   void 
   unlock();
};



class SimpleThread_Synchronize
{

private:
   SimpleThread_Mutex *mutex;   //Pointer to the mutex that is been synchronized.

public:  
   /* Constructor
    *
    */
   SimpleThread_Synchronize(
         SimpleThread_Mutex &amutex);
   

   /* Destructor
    *
    */
   ~SimpleThread_Synchronize();  
};


#endif


