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


#include "dbconnectExceptions.h"


Error::Error(
      const string description)
   :  BaseException(DBERROR, "Error", description) 
{};


DriverError::DriverError(
      const string description)
   :  BaseException(DRIVER_ERROR, "DriverError", description) 
{};


NotImplemented::NotImplemented(
      const string description)
   :  BaseException(NOT_IMPLEMENTED, "NotImplemented", description) 
{};


UnknownException::UnknownException(
      const string description)
   :  BaseException(UNKNOWN_EXCEPTION, "UnknownException", description) 
{};


NotConnected::NotConnected(
      const string description)
   :  BaseException(NOT_CONNECTED, "NotConnected", description) 
{};


AlreadyConnected::AlreadyConnected(
      const string description)
   :  BaseException(ALREADY_CONNECTED, "AlreadyConnected", description) 
{};


ErrorConnecting::ErrorConnecting(
      const string description)
   :  BaseException(ERROR_CONNECTING, "ErrorConnecting", description) 
{};


QueryConnectionTimeout::QueryConnectionTimeout(
      const string description)
   :  BaseException(QUERY_CONNECTION_TIMEOUT, "QueryConnectionTimeout", description) 
{};


ErrorPingingConnection::ErrorPingingConnection(
      const string description)
   :  BaseException(ERROR_PINGING_CONNECTION, "ErrorPingingConnection", description) 
{};


ErrorQuerying::ErrorQuerying(
      const string description)
   :  BaseException(ERROR_QUERYING, "ErrorQuerying", description)
{ };

ErrorQuerying::ErrorQuerying(
      const string description,
	  const string state)
   :  BaseException(ERROR_QUERYING, "ErrorQuerying", description)
{ this->state	= state;};


IndexOutOfRange::IndexOutOfRange(
      const string description)
   :  BaseException(INDEX_OUT_OF_RANGE, "IndexOutOfRange", description)
{ };


NameNotFound::NameNotFound(
      const string description)
   :  BaseException(NAME_NOT_FOUND, "NameNotFound", description)
{ };


EndOfResultSet::EndOfResultSet(
      const string description)
   :  BaseException(END_OF_RESULT_SET, "EndOfResultSet", description)
{ };


BindParameterError::BindParameterError(
      const string description)
   :  BaseException(BIND_PARAMETER_ERROR, "BindParameterError", description)
{ };


BindParameterNotPresent::BindParameterNotPresent(
      const string description)
   :  BaseException(BIND_PARAMETER_NOT_PRESENT, "BindParameterNotPresent", description)
{ };


BindParameterNotSet::BindParameterNotSet(
      const string description)
   :  BaseException(BIND_PARAMETER_NOT_SET, "BindParameterNotSet", description)
{ };


TransactionError::TransactionError(
      const string description)
   :  BaseException(TRANSACTION_ERROR, "TransactionError", description)
{ };


CommandError::CommandError(
      const string description)
   :  BaseException(COMMAND_ERROR, "CommandError", description)
{ };


ResultSetError::ResultSetError(
      const string description)
   :  BaseException(RESULTSET_ERROR, "ResultSetError", description)
{ };
