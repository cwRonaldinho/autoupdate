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

#include "simpleThreads.h"

//------------------------------------------------------------------------------
// Class: SimpleThread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// SimpleThread::SimpleThread
//------------------------------------------------------------------------------
SimpleThread::SimpleThread()
{

} // SimpleThread::SimpleThread


//------------------------------------------------------------------------------
// SimpleThread::~SimpleThread
//------------------------------------------------------------------------------
SimpleThread::~SimpleThread()
{

} // SimpleThread::~SimpleThread


//------------------------------------------------------------------------------
// SimpleThread::sleep
//------------------------------------------------------------------------------
void 
SimpleThread::sleep(
         unsigned int milliSeconds)
{
#ifdef WIN32
   // Windows
   Sleep(milliSeconds);
#else
   // Unices:
   // Use the unix select to get milliseconds when sleeping.
   struct timeval tv;   
   tv.tv_sec = milliSeconds / 1000;
   tv.tv_usec = milliSeconds - (milliSeconds / 1000 * 1000);
   select(0, NULL, NULL, NULL, &tv);   
#endif      
}

//------------------------------------------------------------------------------
// Class: SimpleThread_Mutex
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// SimpleThread_Mutex::SimpleThread_Mutex
//------------------------------------------------------------------------------
SimpleThread_Mutex::SimpleThread_Mutex()
:
   isMutexOwned(false),
   lockTime(0)
{


#ifdef WIN32
   // Windows
   // Create the mutex as not inheritable, initially not owned and unamed
   hmutex = CreateMutex(NULL, FALSE, NULL);
   if (!hmutex)
   {
      char lpMsgBuf[10240];
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                     NULL,
                     GetLastError(),
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                     (LPTSTR) &lpMsgBuf,
                     sizeof(lpMsgBuf)-1,
                     NULL);         
      string err = "Error creating windows mutex. Error: ";
      err += lpMsgBuf;
      throw MutexError(err);
   }
#else
   // Unices:
   // Init the mutex attribute
int ret;
   pthread_mutexattr_t hmattr;
   ret = pthread_mutexattr_init(&hmattr);
   if (ret != 0) 
      throw MutexError("Error creating pthread mutex attribute.");

   // Create the mutex
   ret = pthread_mutex_init(&hmutex, &hmattr);
   if (ret != 0) 
      throw MutexError("Error creating pthread mutex.");

   // Destroy the mutex attribute
   ret = pthread_mutexattr_destroy(&hmattr);
   if (ret != 0) 
      throw MutexError("Error destroying pthread mutex attribute.");
#endif   

}  // SimpleThread_Mutex::SimpleThread_Mutex



//------------------------------------------------------------------------------
// SimpleThread_Mutex::~SimpleThread_Mutex
//------------------------------------------------------------------------------
SimpleThread_Mutex::~SimpleThread_Mutex() 
{
   // If the mutex is in use, wait up to 3 seconds to become unblocked.
   if (isMutexOwned) SimpleThread::sleep(1000);
   if (isMutexOwned) SimpleThread::sleep(1000);
   if (isMutexOwned) SimpleThread::sleep(1000);

#ifdef WIN32
   // Windows
   // Close the mutex handle allowing it to be destroyed
   if (CloseHandle(hmutex) == 0)
   {
      char lpMsgBuf[10240];
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                     NULL,
                     GetLastError(),
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                     (LPTSTR) &lpMsgBuf,
                     sizeof(lpMsgBuf)-1,
                     NULL);         
      string err = "Error orccured while destroying the windows mutex. Error: ";
      err += lpMsgBuf;
      throw MutexError(err);
   }
#else
   // Unices:
   int ret;

   // Destroy the mutex.
   ret = pthread_mutex_destroy(&hmutex);
   if (ret != 0) 
      throw MutexError("Error orccured while destroying the pthread mutex.");
#endif      

   isMutexOwned = false;
   lockTime = 0;
}  // SimpleThread_Mutex::~SimpleThread_Mutex



//------------------------------------------------------------------------------
// SimpleThread_Mutex::isLocked
//------------------------------------------------------------------------------
bool 
SimpleThread_Mutex::isLocked()
{
   return isMutexOwned;
}  // SimpleThread_Mutex::isLocked



//------------------------------------------------------------------------------
// SimpleThread_Mutex::getLockTime
//------------------------------------------------------------------------------
time_t 
SimpleThread_Mutex::getLockTime()
{
   return lockTime;
}  // SimpleThread_Mutex::getLockTime



//------------------------------------------------------------------------------
// SimpleThread_Mutex::lock
//------------------------------------------------------------------------------
void 
SimpleThread_Mutex::lock()
{
   
#ifdef WIN32
   // Windows
   DWORD ret;
   
   ret = WaitForSingleObject(hmutex, INFINITE);
   if (ret == WAIT_FAILED)
   {
      char lpMsgBuf[10240];
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                     NULL,
                     GetLastError(),
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                     (LPTSTR) &lpMsgBuf,
                     sizeof(lpMsgBuf)-1,
                     NULL);         
      string err = "Error orccured while trying to obtain a lock on the windows mutex. Error: ";
      err += lpMsgBuf;
      throw MutexError(err);
   }
#else
   // Unices:
   int ret;

   ret = pthread_mutex_lock(&hmutex);
   if (ret != 0) 
      throw MutexError("Error orccured while trying to obtain a lock on the pthread mutex.");   
#endif   

   isMutexOwned =  true;
   lockTime = time(NULL);   

}  // SimpleThread_Mutex::lock



//------------------------------------------------------------------------------
// SimpleThread_Mutex::unlock
//------------------------------------------------------------------------------
void 
SimpleThread_Mutex::unlock()
{
   time_t saveT = lockTime;

   isMutexOwned = false;
   lockTime = 0;  
   
#ifdef WIN32
   // Windows
   if (ReleaseMutex(hmutex) == 0)
   {
      // Rollback
      isMutexOwned = true;    
      lockTime = saveT;

      char lpMsgBuf[10240];
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                     NULL,
                     GetLastError(),
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                     (LPTSTR) &lpMsgBuf,
                     sizeof(lpMsgBuf)-1,
                     NULL);         
      string err = "Error orccured while trying to release a lock on the windows mutex. Error: ";
      err += lpMsgBuf;
      throw MutexError(err);
   }
#else
   // Unices:
   int ret;

   ret = pthread_mutex_unlock(&hmutex);
   if (ret != 0) 
   {
      // Rollback
      isMutexOwned = true;    
      lockTime = saveT;
      throw MutexError("Error orccured while trying to release a lock on the pthread mutex.");
   }
#endif   
      
}  // SimpleThread_Mutex::unlock



//------------------------------------------------------------------------------
// Class: SimpleThread_Synchronize
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// SimpleThread_Synchronize::SimpleThread_Synchronize
//------------------------------------------------------------------------------
SimpleThread_Synchronize::SimpleThread_Synchronize(
      SimpleThread_Mutex &amutex)
   : 
      mutex(&amutex)
{
   // Class is in scope, lock the mutex or block until able to lock.   
   mutex->lock();   
}  // SimpleThread_Synchronize::SimpleThread_Synchronize


//------------------------------------------------------------------------------
// SimpleThread_Synchronize::~SimpleThread_Synchronize
//------------------------------------------------------------------------------
SimpleThread_Synchronize::~SimpleThread_Synchronize()
{
   // Class has gone out of scope, free the mutex.   
   mutex->unlock();   
   mutex = NULL;
}  // SimpleThread_Synchronize::~SimpleThread_Synchronize

