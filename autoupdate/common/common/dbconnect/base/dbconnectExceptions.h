/*
 * DBConnect exceptions 
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

#ifndef __DBCONN_EXCEPTIONS_H__
#define __DBCONN_EXCEPTIONS_H__

#ifdef WIN32
   // Windows
   // DLL Exports
   class  __declspec( dllexport ) Error;
   class  __declspec( dllexport ) ErrorQuerying;
   class  __declspec( dllexport ) DriverError;
   class  __declspec( dllexport ) NotImplemented;
   class  __declspec( dllexport ) UnknownException;
   class  __declspec( dllexport ) NotConnected;
   class  __declspec( dllexport ) AlreadyConnected;
   class  __declspec( dllexport ) ErrorConnecting;
   class  __declspec( dllexport ) QueryConnectionTimeout;
   class  __declspec( dllexport ) ErrorPingingConnection;
   class  __declspec( dllexport ) IndexOutOfRange;
   class  __declspec( dllexport ) NameNotFound;
   class  __declspec( dllexport ) EndOfResultSet;     
   class  __declspec( dllexport ) BindParameterError;
   class  __declspec( dllexport ) BindParameterNotPresent;
   class  __declspec( dllexport ) BindParameterNotSet;
   class  __declspec( dllexport ) TransactionError;
   class  __declspec( dllexport ) CommandError;
   class  __declspec( dllexport ) ResultSetError;
#endif   


#include "baseException.h"


// Global Error Message Constants 
enum DBConnectErrorCodes 
{
   DBERROR,
   DRIVER_ERROR,
   NOT_IMPLEMENTED,
   UNKNOWN_EXCEPTION,
   NOT_CONNECTED,
   ALREADY_CONNECTED,
   ERROR_CONNECTING,
   QUERY_CONNECTION_TIMEOUT,
   ERROR_PINGING_CONNECTION,
   ERROR_QUERYING,
   INDEX_OUT_OF_RANGE,
   NAME_NOT_FOUND,
   END_OF_RESULT_SET,
   BIND_PARAMETER_ERROR,
   BIND_PARAMETER_NOT_PRESENT,
   BIND_PARAMETER_NOT_SET,
   TRANSACTION_ERROR,
   COMMAND_ERROR,
   RESULTSET_ERROR
};


/* Exceptions Defined
 *
 */
class Error : public BaseException {
   public: Error(const string description); };

class DriverError : public BaseException {
   public: DriverError(const string description); };

class NotImplemented : public BaseException {
   public: NotImplemented(const string description); };
        
class UnknownException : public BaseException {
   public: UnknownException(const string description); };

class NotConnected : public BaseException {
   public: NotConnected(const string description); }; 

class AlreadyConnected : public BaseException {
   public: AlreadyConnected(const string description); };

class ErrorConnecting : public BaseException {
   public: ErrorConnecting(const string description); };

class QueryConnectionTimeout : public BaseException {
   public: QueryConnectionTimeout(const string description); };

class ErrorPingingConnection : public BaseException {
   public: ErrorPingingConnection(const string description); };
              
class ErrorQuerying : public BaseException {
   public: ErrorQuerying(const string description); ErrorQuerying(const string description, const string state);};

class IndexOutOfRange : public BaseException {
   public: IndexOutOfRange(const string description); };

class NameNotFound : public BaseException {
   public: NameNotFound(const string description); };

class EndOfResultSet : public BaseException {
   public: EndOfResultSet(const string description); };
              
class BindParameterError : public BaseException {
   public: BindParameterError(const string description); }; 

class BindParameterNotPresent : public BaseException {
   public: BindParameterNotPresent(const string description); }; 
           
class BindParameterNotSet : public BaseException {
   public: BindParameterNotSet(const string description); };

class TransactionError : public BaseException {
   public: TransactionError(const string description); };

class CommandError : public BaseException {
   public: CommandError(const string description); };

class ResultSetError : public BaseException {
   public: ResultSetError(const string description); };

   
#endif
