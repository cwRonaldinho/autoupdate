/*
**	Sybase Open Client/Server 
**	Confidential Property of Sybase, Inc.
**	Copyright  Sybase, Inc. 1992, 1996
**	All rights reserved
*/

#ifndef __CSTYPES_H__

#define __CSTYPES_H__

/*
** This file defines basic error codes and data types used in all Open
** Client/Server System 10 Products.
*/

/*
** Include the configuration header file. This file contains the defines and
** type definitions that are platform/compiler specific.
*/
#include	<csconfig.h>

/*****************************************************************************
**
** Return code defines used in client/server applications.
**
*****************************************************************************/

/*
** See the csconfig.h header file for the definition of CS_RETCODE.
*/

/*
** Define the core return codes.
*/
#define CS_SUCCEED		(CS_RETCODE)1
#define CS_FAIL			(CS_RETCODE)0
#define CS_MEM_ERROR		(CS_RETCODE)-1
#define	CS_PENDING		(CS_RETCODE)-2
#define	CS_QUIET		(CS_RETCODE)-3
#define	CS_BUSY			(CS_RETCODE)-4
#define	CS_INTERRUPT		(CS_RETCODE)-5
#define CS_BLK_HAS_TEXT		(CS_RETCODE)-6
#define CS_CONTINUE		(CS_RETCODE)-7
#define CS_FATAL		(CS_RETCODE)-8

/*
** Define error offsets. All other CS_RETCODE error codes should be
** based off of these values to avoid define conflicts.
*/
#define CS_CONV_ERR		(CS_RETCODE)(-100)
#define CS_EXTERNAL_ERR		(CS_RETCODE)(-200)
#define CS_INTERNAL_ERR		(CS_RETCODE)(-300)

/* common library errors */
#define CS_COMN_ERR		(CS_RETCODE)(-400)

/*
** Return code for a routine which was cancelled via ct_cancel().
*/
#define CS_CANCELED		(CS_RETCODE)(CS_EXTERNAL_ERR - 2)

/*
** Special returns for ct_fetch().
*/
#define CS_ROW_FAIL		(CS_RETCODE)(CS_EXTERNAL_ERR - 3)
#define CS_END_DATA		(CS_RETCODE)(CS_EXTERNAL_ERR - 4)

/*
** Special return for ct_results().
*/
#define CS_END_RESULTS		(CS_RETCODE)(CS_EXTERNAL_ERR - 5)

/*
** Special return for ct_get_data().
*/
#define CS_END_ITEM		(CS_RETCODE)(CS_EXTERNAL_ERR - 6)

/*
** Special return for ct_diag().
*/
#define CS_NOMSG		(CS_RETCODE)(CS_EXTERNAL_ERR - 7)

/*
** Special return for ct_poll().
*/
#define CS_TIMED_OUT		(CS_RETCODE)(CS_EXTERNAL_ERR - 8)

/*
** Special returns for passthru().
*/
#define CS_PASSTHRU_EOM		(CS_RETCODE)(CS_EXTERNAL_ERR - 10)
#define CS_PASSTHRU_MORE	(CS_RETCODE)(CS_EXTERNAL_ERR - 11)

/*
** Special return for ct_cancel().
*/
#define	CS_TRYING		(CS_RETCODE)(CS_EXTERNAL_ERR - 13)

/*
** Errors caused by external events (i.e. bad params supplied by user).
*/
#define CS_EBADPARAM		(CS_RETCODE)(CS_EXTERNAL_ERR - 14)
#define CS_EBADLEN		(CS_RETCODE)(CS_EXTERNAL_ERR - 15)
#define CS_ENOCNVRT		(CS_RETCODE)(CS_EXTERNAL_ERR - 16)

/*
** Return codes for conversion errors. These should be used in any user-
** defined conversion routines that are install via cs_set_convert().
*/
#define CS_EOVERFLOW		(CS_RETCODE)(CS_CONV_ERR - 1)
#define CS_EUNDERFLOW		(CS_RETCODE)(CS_CONV_ERR - 2)
#define CS_EPRECISION		(CS_RETCODE)(CS_CONV_ERR - 3)
#define CS_ESCALE		(CS_RETCODE)(CS_CONV_ERR - 4)
#define CS_ESYNTAX		(CS_RETCODE)(CS_CONV_ERR - 5)
#define CS_EFORMAT		(CS_RETCODE)(CS_CONV_ERR - 6)
#define CS_EDOMAIN		(CS_RETCODE)(CS_CONV_ERR - 7)
#define CS_EDIVZERO		(CS_RETCODE)(CS_CONV_ERR - 8)
#define CS_ERESOURCE		(CS_RETCODE)(CS_CONV_ERR - 9)
#define CS_ENULLNOIND		(CS_RETCODE)(CS_CONV_ERR - 10)
#define CS_ETRUNCNOIND		(CS_RETCODE)(CS_CONV_ERR - 11)
#define CS_ENOBIND		(CS_RETCODE)(CS_CONV_ERR - 12)
#define CS_TRUNCATED		(CS_RETCODE)(CS_CONV_ERR - 13)
#define CS_ESTYLE		(CS_RETCODE)(CS_CONV_ERR - 14)
#define CS_EBADXLT      	(CS_RETCODE)(CS_CONV_ERR - 15)
#define CS_ENOXLT       	(CS_RETCODE)(CS_CONV_ERR - 16)
#define CS_USEREP		(CS_RETCODE)(CS_CONV_ERR - 17)

/*
** Error Severities. 
*/
#define CS_SV_INFORM		(CS_INT)0
#define CS_SV_API_FAIL		(CS_INT)1
#define CS_SV_RETRY_FAIL	(CS_INT)2
#define CS_SV_RESOURCE_FAIL	(CS_INT)3
#define CS_SV_CONFIG_FAIL	(CS_INT)4
#define CS_SV_COMM_FAIL		(CS_INT)5
#define CS_SV_INTERNAL_FAIL	(CS_INT)6
#define CS_SV_FATAL		(CS_INT)7


/*****************************************************************************
**
** Error numbers, and macros for extracting information from a 
** Client-Library error number.
**
** Error numbers are broken down into four components:
**
**	Layer		- Represents which layer is reporting the error.
**	Origin		- Indicates where the error manifested itself. 
**	Severity	- How bad is the error?
**	Number		- The actual layer specific error number being
**			  reported.
**
*****************************************************************************/

/*
** The error message number.
*/
#if (_MIPS_SZLONG == 64) || defined(__LP64__)
typedef unsigned int  CS_MSGNUM;
#else
typedef long CS_MSGNUM;
#endif

/*
** The following macros are used to extract the components from a composite
** error number.
*/
#define CS_LAYER(L)	(CS_MSGNUM) (((L) >> 24) & 0xff)
#define CS_ORIGIN(O)	(CS_MSGNUM) (((O) >> 16) & 0xff)
#define CS_SEVERITY(S)	(CS_MSGNUM) (((S) >> 8) & 0xff)
#define CS_NUMBER(N)	(CS_MSGNUM) ((N) & 0xff)

/*
** The following are the possible values for cs_status field of
** CHAR_ATTRIB structure defined in intl_nls.h
*/

# define        CS_STAT_DEFAULT           0x0000
# define        CS_STAT_MULTIBYTE         0x0001
# define        CS_STAT_SPACELAST         0x0002
# define        CS_STAT_NONASCIISP        0x0004

/*****************************************************************************
**
** Defines used in client/server applications.
**
*****************************************************************************/

/*
** The maximum localization name allowed. A four byte value is added (instead
** of 1 byte) to provide space for null termination and remain on a modulo 4
** byte boundary.
*/
#define	CS_MAX_LOCALE		(CS_INT)(64 + 4)

/*
** The maximum column name allowed.
*/
#define	CS_MAX_NAME		(CS_INT)(128 + 4)

/*
** The maximum password length allowed.
*/
#define CS_MAX_PASS		(CS_INT)(30)

/*
** The maximum number of characters in arrays. Please note that this define
** does not correspond to any server definition of lengths (particularly
** the length of the character data type, which is 255 bytes for the Sybase
** server).
*/
#define CS_MAX_CHAR		(CS_INT)256

/*
** Maximum string in Directory Services
*/
#define	CS_MAX_DS_STRING	(CS_INT)512

/*
** Maximum data length of numeric/decimal datatypes.
*/
#define	CS_MAX_NUMLEN		(CS_INT)33

/*
** To be true or not true.
*/
#define CS_TRUE			(CS_BOOL)1
#define CS_FALSE		(CS_BOOL)0

/*
** Define basic default types.
*/
#define CS_NULLTERM		(CS_INT)(-9)
#define CS_WILDCARD		(CS_INT)(-99)
#define CS_NO_LIMIT		(CS_INT)(-9999)
#define CS_UNUSED		(CS_INT)(-99999)

/*
** timeout values
**
**	CS_NO_LIMIT		Resource will never expire.
**	CS_UNEXPIRED		Resource did not expire.
*/
#define CS_UNEXPIRED		(CS_INT)(-999999)

/*
** Enumerate what datatypes we support.
*/
#define CS_ILLEGAL_TYPE		(CS_INT)(-1)
#define CS_CHAR_TYPE		(CS_INT)0
#define CS_BINARY_TYPE		(CS_INT)1
#define CS_LONGCHAR_TYPE	(CS_INT)2
#define CS_LONGBINARY_TYPE	(CS_INT)3
#define CS_TEXT_TYPE		(CS_INT)4
#define CS_IMAGE_TYPE		(CS_INT)5
#define CS_TINYINT_TYPE		(CS_INT)6
#define CS_SMALLINT_TYPE	(CS_INT)7
#define CS_INT_TYPE		(CS_INT)8
#define CS_REAL_TYPE		(CS_INT)9
#define CS_FLOAT_TYPE		(CS_INT)10
#define CS_BIT_TYPE		(CS_INT)11
#define CS_DATETIME_TYPE	(CS_INT)12
#define CS_DATETIME4_TYPE	(CS_INT)13
#define CS_MONEY_TYPE		(CS_INT)14
#define CS_MONEY4_TYPE		(CS_INT)15
#define CS_NUMERIC_TYPE		(CS_INT)16
#define CS_DECIMAL_TYPE		(CS_INT)17
#define CS_VARCHAR_TYPE		(CS_INT)18
#define CS_VARBINARY_TYPE	(CS_INT)19
#define CS_LONG_TYPE		(CS_INT)20
#define CS_SENSITIVITY_TYPE	(CS_INT)21
#define CS_BOUNDARY_TYPE	(CS_INT)22
#define CS_VOID_TYPE		(CS_INT)23
#define CS_USHORT_TYPE		(CS_INT)24

/*
** Define the minimum and maximum datatype values.
*/
#define CS_MIN_SYBTYPE		CS_CHAR_TYPE
#define CS_MAX_SYBTYPE		CS_VOID_TYPE

/*
** Define the number of datatypes that are supported by Sybase.
*/
#define CS_MAXSYB_TYPE		(CS_MAX_SYBTYPE + 1)

/*
** The minumum user-defined data type.
*/
#define CS_USER_TYPE		(CS_INT)100

/*
** Define the bit values used in the format element of the CS_DATAFMT
** structure. The CS_FMT_JUSTIFY_RT flag exists for future use only.
*/
#define CS_FMT_UNUSED		(CS_INT)0x0
#define CS_FMT_NULLTERM		(CS_INT)0x1
#define CS_FMT_PADNULL		(CS_INT)0x2
#define CS_FMT_PADBLANK		(CS_INT)0x4
#define CS_FMT_JUSTIFY_RT	(CS_INT)0x8
#define CS_FMT_STRIPBLANKS	(CS_INT)0x10
#define CS_FMT_SAFESTR		(CS_INT)0x20

/*
** The following are bit values for the status field in the CS_DATAFMT
** structure.
** 
** CS_HIDDEN		Set if this column would normally be hidden
**			from the user.
**
** CS_KEY		Set if the column is a key column.
**
** CS_VERSION_KEY	Set if the column is part of the version key for
**			the row.
**
** CS_NODATA		Not currently used in Open Client/Server.
**
** CS_UPDATABLE		Set if the column is an updatable cursor column.
**
** CS_CANBENULL		Set if the column can contain NULL values.
**
** CS_DESCIN		
** CS_DESCOUT		Open Server-specific values for dynamic SQL.
**
** CS_INPUTVALUE	Set if the parameter is an input parameter value
**			for a Client-Library command.
**
** CS_UPDATECOL		Set if the parameter is the name of a column in
**			the update clause of a cursor declare command.
**
** CS_RETURN		Set if the parameter is a return parameter to
**			an RPC command.
**
** CS_TIMESTAMP		Set if the column is a timestamp column.
**
** CS_NODEFAULT		Open Server-specific status values for RPC parameters.
**
** CS_IDENTITY		Set if the column is an identity column.
**
*/
#define CS_HIDDEN		(CS_INT)0x1
#define CS_KEY			(CS_INT)0x2
#define CS_VERSION_KEY		(CS_INT)0x4
#define CS_NODATA		(CS_INT)0x8
#define CS_UPDATABLE		(CS_INT)0x10
#define CS_CANBENULL		(CS_INT)0x20
#define CS_DESCIN		(CS_INT)0x40
#define CS_DESCOUT		(CS_INT)0x80
#define CS_INPUTVALUE		(CS_INT)0x100
#define CS_UPDATECOL		(CS_INT)0x200
#define CS_RETURN 		(CS_INT)0x400
#define CS_TIMESTAMP		(CS_INT)0x2000
#define CS_NODEFAULT		(CS_INT)0x4000
#define CS_IDENTITY		(CS_INT)0x8000

/*
** The following are bit values for the status field in the CS_BROWSEDESC
** structure.
** 
** CS_EXPRESSION	Set when the column is the result of an expression.
**
** CS_RENAMED		Set when that the column's heading is not the 
**			original name of the column. 
*/
#define CS_EXPRESSION		(CS_INT)0x800
#define CS_RENAMED		(CS_INT)0x1000

/*
** Define special precision/scale value for using the precision/scale of the
** source element when doing a conversion from a numeric/decimal datatype to a
** numeric/decimal datatype.
*/
#define CS_SRC_VALUE		(CS_INT)-2562

/*
** Minimum/maximum/default precision and scale values for numeric/decimal
** datatypes.
*/
#define CS_MIN_PREC		(CS_INT)1
#define CS_MAX_PREC		(CS_INT)77
#define CS_DEF_PREC		(CS_INT)18

#define CS_MIN_SCALE		(CS_INT)0
#define CS_MAX_SCALE		(CS_INT)77
#define CS_DEF_SCALE		(CS_INT)0

/*
** The datetime format for converting datetime to char.
*/
#define CS_DATES_SHORT		(CS_INT)0	/* default	*/
#define CS_DATES_MDY1		(CS_INT)1	/* mm/dd/yy	*/
#define CS_DATES_YMD1		(CS_INT)2	/* yy.mm.dd	*/
#define CS_DATES_DMY1		(CS_INT)3	/* dd/mm/yy	*/
#define CS_DATES_DMY2		(CS_INT)4	/* dd.mm.yy	*/
#define CS_DATES_DMY3		(CS_INT)5	/* dd-mm-yy	*/
#define CS_DATES_DMY4		(CS_INT)6	/* dd mon yy	*/
#define CS_DATES_MDY2		(CS_INT)7	/* mon dd, yy	*/
#define CS_DATES_HMS		(CS_INT)8	/* hh:mm:ss	*/
#define CS_DATES_LONG		(CS_INT)9	/* default with micro secs */
#define CS_DATES_MDY3		(CS_INT)10	/* mm-dd-yy	*/
#define CS_DATES_YMD2		(CS_INT)11	/* yy/mm/dd	*/
#define CS_DATES_YMD3		(CS_INT)12	/* yymmdd	*/
#define CS_DATES_YDM1		(CS_INT)13	/* yy/dd/mm	*/
#define CS_DATES_MYD1		(CS_INT)14	/* mm/yy/dd	*/
#define CS_DATES_DYM1		(CS_INT)15	/* dd/yy/mm	*/
#define CS_DATES_SHORT_ALT	(CS_INT)100	/* default	*/
#define CS_DATES_MDY1_YYYY	(CS_INT)101	/* mm/dd/yyyy	*/
#define CS_DATES_YMD1_YYYY	(CS_INT)102	/* yyyy.mm.dd	*/
#define CS_DATES_DMY1_YYYY	(CS_INT)103	/* dd/mm/yyyy	*/
#define CS_DATES_DMY2_YYYY	(CS_INT)104	/* dd.mm.yyyy	*/
#define CS_DATES_DMY3_YYYY	(CS_INT)105	/* dd-mm-yyyy	*/
#define CS_DATES_DMY4_YYYY	(CS_INT)106	/* dd mon yyyy	*/
#define CS_DATES_MDY2_YYYY	(CS_INT)107	/* mon dd, yyyy	*/
#define CS_DATES_HMS_ALT	(CS_INT)108	/* hh:mm:ss	*/
#define CS_DATES_LONG_ALT	(CS_INT)109	/* default with micro secs */
#define CS_DATES_MDY3_YYYY	(CS_INT)110	/* mm-dd-yyyy	*/
#define CS_DATES_YMD2_YYYY	(CS_INT)111	/* yyyy/mm/dd	*/
#define CS_DATES_YMD3_YYYY	(CS_INT)112	/* yyyymmdd	*/


/*
** sizeof macro which forces a CS_INT cast for portability.
*/
#define CS_SIZEOF		(CS_INT)sizeof

/*
** Locale type information. CS_LC_MONETARY and CS_LC_NUMERIC are defined
** for future use.
*/
#define CS_LC_COLLATE           (CS_INT)1
#define CS_LC_CTYPE             (CS_INT)2
#define CS_LC_MESSAGE           (CS_INT)3
#define CS_LC_MONETARY          (CS_INT)4
#define CS_LC_NUMERIC           (CS_INT)5
#define CS_LC_TIME              (CS_INT)6
#define CS_LC_ALL               (CS_INT)7
#define CS_SYB_LANG		(CS_INT)8
#define CS_SYB_CHARSET		(CS_INT)9
#define CS_SYB_SORTORDER	(CS_INT)10
#define CS_SYB_COLLATE		CS_SYB_SORTORDER
#define CS_SYB_LANG_CHARSET	(CS_INT)11
#define CS_SYB_TIME		(CS_INT)12
#define CS_SYB_MONETARY		(CS_INT)13
#define CS_SYB_NUMERIC		(CS_INT)14

/*
** Object type information for the cs_objects() API.
*/
#define CS_CONNECTNAME		(CS_INT)1
#define CS_CURSORNAME		(CS_INT)2
#define CS_STATEMENTNAME	(CS_INT)3
#define CS_CURRENT_CONNECTION	(CS_INT)4
#define CS_MIN_USERDATA		(CS_INT)100

/*
** Info type information for the ct_ds_objinfo() API.
*/
#define CS_DS_CLASSOID		(CS_INT)1
#define CS_DS_DIST_NAME		(CS_INT)2
#define CS_DS_NUMATTR		(CS_INT)3
#define CS_DS_ATTRIBUTE		(CS_INT)4
#define CS_DS_ATTRVALS		(CS_INT)5

/*****************************************************************************
**
** Common datatype typedefs and structures used in client/server applications.
**
*****************************************************************************/

/*
** Define client/server datatypes.
** 
** CS_FLOAT is defined in csconfig.h
*/
typedef unsigned char   CS_TINYINT;	/* 1 byte integer */
typedef short           CS_SMALLINT;	/* 2 byte integer */
typedef char		CS_CHAR;	/* char type */
typedef unsigned char   CS_BINARY;	/* binary type */
typedef unsigned char   CS_BIT;		/* bit type */
typedef float		CS_REAL;	/* 4 byte float type */
typedef unsigned char	CS_BYTE;	/* 1 byte byte */
typedef unsigned char	CS_TEXT;	/* text data */
typedef unsigned char	CS_IMAGE;	/* image data */
typedef unsigned char	CS_LONGCHAR;	/* long char type */
typedef unsigned char	CS_LONGBINARY;	/* long binary type */
typedef long 		CS_LONG;	/* long integer type */
typedef CS_INT		CS_VOIDDATA;	/* void data */

typedef   struct  cs_ctx_globs	CS_CTX_GLOBS;
typedef   struct  cs_ctx_locglobs	CS_CTX_LOCGLOBS;

/*
** The following typedefs are platform specific.
**
**	CS_VOID 	(compiler void differences)
**	CS_THRDRES	(compiler void differences)
**	CS_INT		(need to force it to 4 bytes)
**
** Please see the csconfig.h file for the actual definitions.
*/

/*
** Unsigned types (CS_UINT can be found in csconfig.h).
*/
typedef unsigned short	CS_USHORT;

/*
** The datetime structures.
*/
typedef struct _cs_datetime
{
	CS_INT		dtdays;		/* number of days since 1/1/1900 */
	CS_INT		dttime;		/* number 300th second since mid */
} CS_DATETIME;

typedef struct _cs_datetime4
{
	CS_USHORT	days;		/* number of days since 1/1/1900 */
	CS_USHORT	minutes;	/* number of minutes since midnight */
} CS_DATETIME4;

/*
** The money structures.
*/
typedef struct _cs_money
{
	CS_INT		mnyhigh;
	CS_UINT		mnylow;
} CS_MONEY;

typedef struct _cs_money4
{
	CS_INT		mny4;
} CS_MONEY4;

/*
** The numeric structures.
*/
typedef struct _cs_numeric
{
	CS_BYTE		precision;
	CS_BYTE		scale;
	CS_BYTE		array[CS_MAX_NUMLEN];
} CS_NUMERIC;

typedef	CS_NUMERIC	CS_DECIMAL;

/*
** The var (pascal like) structures. Please don't confuse these datatypes
** with the Sybase server "varchar" column type.
*/
typedef struct _cs_varychar
{
	CS_SMALLINT	len;            	/* length of the string */
	CS_CHAR		str[CS_MAX_CHAR];	/* string, no NULL terminator */
} CS_VARCHAR;

typedef struct _cs_varybin
{
	CS_SMALLINT	len;			/* length of the binary array */
	CS_BYTE		array[CS_MAX_CHAR];	/* the array itself. */
} CS_VARBINARY;

/*
** Datetime value information.
*/
typedef struct _cs_daterec
{
	CS_INT		dateyear;	/* 1900 to the future */
	CS_INT		datemonth;	/* 0 - 11 */
	CS_INT		datedmonth;	/* 1 - 31 */
	CS_INT		datedyear;	/* 1 - 366 */
	CS_INT		datedweek;	/* 0 - 6 (Mon. - Sun.) */
	CS_INT		datehour;	/* 0 - 23 */
	CS_INT		dateminute;	/* 0 - 59 */
	CS_INT		datesecond;	/* 0 - 59 */
	CS_INT		datemsecond;	/* 0 - 997 */
	CS_INT		datetzone;	/* 0 - 127 */
} CS_DATEREC;

/*****************************************************************************
**
** Hidden information structures.
**
*****************************************************************************/

#ifndef CS__INTERNAL_STRUCTS

/*
** If passing code through lint, define the hidden structures as void.
*/
#ifdef lint

#ifndef CS_CONTEXT
#define CS_CONTEXT	CS_VOID
#endif /* CS_CONTEXT */

#ifndef CS_LOCALE
#define CS_LOCALE	CS_VOID
#endif /* CS_LOCALE */

#ifndef CS_CONNECTION
#define CS_CONNECTION	CS_VOID
#endif /* CS_CONNECTION */

#ifndef CS_COMMAND
#define CS_COMMAND	CS_VOID
#endif /* CS_COMMAND */
 
#ifndef CS_DS_OBJECT
#define CS_DS_OBJECT	CS_VOID
#endif /* CS_DS_OBJECT */
 
#ifndef CS_DS_RESULT
#define CS_DS_RESULT	CS_VOID
#endif /* CS_DS_RESULT */
 
#else /* lint */

/*
** Use anonymous structure tags to define the hidden structures.
*/
typedef struct _cscontext	CS_CONTEXT;
typedef struct _cslocale  	CS_LOCALE;
typedef struct _csconnection	CS_CONNECTION;
typedef struct _cscommand	CS_COMMAND;
typedef struct _csdsobject	CS_DS_OBJECT;
typedef struct _csdsresult	CS_DS_RESULT;

#endif /* lint */

#endif /* CS__INTERNAL_STRUCTS */

/*****************************************************************************
**
** User-accessible information structures.
**
*****************************************************************************/

/*
** The data format structure used by Open Client/Server.
**
** name[CS_MAX_NAME]	The name of the data.
**
** namelen		The actual length of the name.
**
** datatype 		The datatype field indicates what Sybase
**			or user defined datatype is being represented.
**
** format		The format field tells whether or not data
**			should be padded to the full length of the
**			variable. This will only be used if the type
**			of the variable is character or binary. The 
** 			format field also tells whether the data
**			should be null-terminated (for char
**			variables, only). This is a bit field in which
**			the format values are or'd together.
**	
** maxlength		The max length the data might be.
**
** scale		This is used if dataytype needs it (e.g.
**			CS_NUMERIC)
**
** precision		This is used if dataytype needs it.
**
** status		Additional data status values. This is a bit
**			field in which the status values are or'd
**			together.
**
** count		If binding data, the count field tells how
**			many rows should be bound.
**
** usertype		User-defined datatype code passed back from
**			the Server.
**
** *locale		Pointer to the locale for this data.
*/
typedef struct _cs_datafmt
{
	CS_CHAR		name[CS_MAX_NAME];
	CS_INT		namelen;
	CS_INT		datatype;
	CS_INT		format;
	CS_INT		maxlength;
	CS_INT		scale;
	CS_INT		precision;
	CS_INT		status;
	CS_INT		count;
	CS_INT		usertype;
	CS_LOCALE	*locale;
} CS_DATAFMT;

/*
** The object name structure used by Client-Library cs_object() API.	
**
** thinkexists 		indicates whether the application expects 
**			this object to exist. 
**
** object_type		The type of the object. This field is the 
**			first part of a 5-part key.
**
** last_name		The `last name' associated with the object of 
**			interest, if any. This field is the second 
**			part of a 5-part key.
**
** lnlen		The length, in bytes, of last_name.
**
** first_name		The `first name' associated with the object of 
**			interest, if any. This field is the third part 
**			of a 5-part key.
**
** fnlen		The length, in bytes, of first_name.
**
** scope		Data that describes the scope of the object. 
**			This field is the fourth part of a 5-part key.
**
** scopelen		The length, in bytes, of scope. 
**
** thread		Platform-specific data that is used to distinguish 
**			threads in a multi-threaded execution environment.
**			This field is the fifth part of a 5-part key.
**
** threadlen		The length, in bytes, of thread. 
*/
typedef struct _cs_objname
{
	CS_BOOL		thinkexists;
	CS_INT		object_type;
	CS_CHAR		last_name[CS_MAX_NAME];
	CS_INT		lnlen;
	CS_CHAR		first_name[CS_MAX_NAME];
	CS_INT		fnlen;
	CS_VOID		*scope;
	CS_INT		scopelen;
	CS_VOID		*thread;
	CS_INT		threadlen;
} CS_OBJNAME;

/*
** The object data structure used by Client-Library cs_object() API.	
**
** actuallyexists	Indicates whether this object actually exists. 
**			cs_objects sets actuallyexists to CS_TRUE 
**			if it finds a matching object.  
**			cs_objects sets actuallyexists to CS_FALSE 
**			if it does not find a matching object.
**
** connection		A pointer to the CS_CONNECTION structure 
**			representing the connection in which the object exists.
**
** command		A pointer to the CS_COMMAND structure representing the 
**			command space with which the object is associated, 
**			if any.
**
** buffer		A pointer to data space. An application can 
**			use buffer to associate data with a saved object.
**
** buflen		The length, in bytes, of *buffer. 
** 
*/
typedef struct _cs_objdata
{
	CS_BOOL		actuallyexists;
	CS_CONNECTION	*connection;
	CS_COMMAND	*command;
	CS_VOID		*buffer;
	CS_INT		buflen;
} CS_OBJDATA;

/*
** Definition of a pointer to a function for all conversion routines.
*/

CS_START_EXTERN_C

typedef CS_RETCODE	(CS_PUBLIC * CS_CONV_FUNC)PROTOTYPE((
						CS_CONTEXT *context,
						CS_DATAFMT *srcfmt,
						CS_VOID *src,
						CS_DATAFMT *destfmt,
						CS_VOID *dest,
						CS_INT *destlen
						));
/*
** Pointers to the thread primitive functions used in Open Client.
*/
typedef CS_RETCODE	(CS_PUBLIC * CS_THRDM_FUNC)PROTOTYPE((
						CS_THRDRES *resource
						));
typedef CS_RETCODE	(CS_PUBLIC * CS_THRDE_FUNC)PROTOTYPE((
						CS_THRDRES *resource
						));
typedef CS_RETCODE	(CS_PUBLIC * CS_THRDC_FUNC)PROTOTYPE((
						CS_THRDRES **resource
						));
typedef CS_RETCODE	(CS_PUBLIC * CS_THRDW_FUNC)PROTOTYPE((
						CS_THRDRES *resource,
						CS_INT millisecs
						));
typedef CS_RETCODE	(CS_PUBLIC * CS_THRDID_FUNC)PROTOTYPE((
						CS_VOID *buffer,
						CS_INT buflen,
						CS_INT *outlen
						));
CS_END_EXTERN_C

/*
** Define the thread primitive structure. This structure is used by
** application programs to pass thread primitives into Client-Library.
** These primitives are used internally in Client-Library to run in a
** threaded environment.
**
** create_mutex_fn	Create a recursive mutex.
**
** delete_mutex_fn	Delete a mutex.
**
** take_mutex_fn	Lock a mutex.
**
** release_mutex_fn	Release a mutex.
**
** create_event_fn	Create an event variable.
**
** delete_event_fn	Delete an event variable.
**
** signal_event_fn	Signal event variable.
**
** reset_event_fn	Reset event variable.
**
** waitfor_event_fn	Wait for event to be signaled.
**
** thread_id_fn		Return id of currently executing thread.
*/
typedef struct _cs_thread
{
	CS_THRDC_FUNC	create_mutex_fn;
	CS_THRDM_FUNC	delete_mutex_fn;
	CS_THRDM_FUNC	take_mutex_fn;
	CS_THRDM_FUNC	release_mutex_fn;
	CS_THRDC_FUNC	create_event_fn;
	CS_THRDE_FUNC	delete_event_fn;
	CS_THRDE_FUNC	signal_event_fn;
	CS_THRDE_FUNC	reset_event_fn;
	CS_THRDW_FUNC	waitfor_event_fn;
	CS_THRDID_FUNC	thread_id_fn;
} CS_THREAD;


/*
** Directory Service definitions
*/

/*
** Token name for predefined OID strings.
*/
#define CS_OID_SYBASE		"1.3.6.1.4.1.897"
#define CS_OID_DIRECTORY	"1.3.6.1.4.1.897.4"	
#define CS_OID_OBJCLASS		"1.3.6.1.4.1.897.4.1"	
#define CS_OID_ATTRTYPE		"1.3.6.1.4.1.897.4.2"	
#define CS_OID_ATTRSYNTAX	"1.3.6.1.4.1.897.4.3"	
#define CS_OID_OBJSERVER	"1.3.6.1.4.1.897.4.1.1"
#define CS_OID_ATTRVERSION	"1.3.6.1.4.1.897.4.2.1"	
#define CS_OID_ATTRSERVNAME	"1.3.6.1.4.1.897.4.2.2"	
#define CS_OID_ATTRSERVICE	"1.3.6.1.4.1.897.4.2.3"	
#define CS_OID_ATTRSTATUS	"1.3.6.1.4.1.897.4.2.4"	
#define CS_OID_ATTRADDRESS	"1.3.6.1.4.1.897.4.2.5"	
#define CS_OID_ATTRSECMECH	"1.3.6.1.4.1.897.4.2.6"	
#define CS_OID_ATTRRETRYCOUNT	"1.3.6.1.4.1.897.4.2.7"	
#define CS_OID_ATTRLOOPDELAY	"1.3.6.1.4.1.897.4.2.8"	

/*
** Current status of server object.
*/
#define CS_STATUS_ACTIVE	(CS_INT)1
#define CS_STATUS_STOPPED	(CS_INT)2
#define CS_STATUS_FAILED	(CS_INT)3
#define CS_STATUS_UNKNOWN	(CS_INT)4


/*
** Server object access type
*/
#define CS_ACCESS_CLIENT	(CS_INT)1
#define CS_ACCESS_ADMIN		(CS_INT)2
#define CS_ACCESS_MGMTAGENT	(CS_INT)3

/*
** String Attribute Value
**
**	This structure is used to describe a string attribute
**	value.
**
**	str_length	Length of string.
**	str_buffer	String data.
*/
typedef struct _cs_string
{
	CS_INT	str_length;
	CS_CHAR	str_buffer[CS_MAX_DS_STRING];
} CS_STRING;

/*
** Transport Address attribute value
**
**	This structure is used to describe a server address attribute
**	value. 
**
**	addr_accesstype		Access type provided on transport
**				address.
**	addr_trantype		Transport address type
**	addr_tranaddress	The address string.
*/
typedef struct _cs_tranaddr
{
	CS_INT		addr_accesstype;
	CS_STRING	addr_trantype;
	CS_STRING	addr_tranaddress;
} CS_TRANADDR;

/*
** Object Identifier
**	
**	This structure is used to represent an Object 
**		Identifier.
**
**	oid_length	Length of Object Identifier.
**	oid_buffer	Buffer containing object identifier.
*/
typedef struct _cs_oid
{
	CS_INT	oid_length;
	CS_CHAR	oid_buffer[CS_MAX_DS_STRING];
} CS_OID;

/*
** Attribute Value
**	
**	This union is used to represent an attribute value.
**
*/
typedef union _cs_attrvalue
{
	CS_STRING	value_string;
	CS_BOOL		value_boolean;
	CS_INT		value_enumeration;
	CS_INT		value_integer;
	CS_OID		value_oid;
	CS_TRANADDR	value_tranaddr;
} CS_ATTRVALUE;


/*
** Attribute 
**	
**	This structure describes an attribute. 
**
*/
typedef struct _cs_attribute
{
	CS_OID		attr_type;
	CS_INT		attr_syntax;
	CS_INT		attr_numvals;
} CS_ATTRIBUTE;

/*
** Syntax identifier tokens for the CS_ATTRIBUTE union.
*/
#define CS_ATTR_SYNTAX_NOOP		(CS_INT) 0
#define CS_ATTR_SYNTAX_STRING		(CS_INT) 1
#define CS_ATTR_SYNTAX_INTEGER		(CS_INT) 2
#define CS_ATTR_SYNTAX_BOOLEAN		(CS_INT) 3
#define CS_ATTR_SYNTAX_ENUMERATION	(CS_INT) 4
#define CS_ATTR_SYNTAX_TRANADDR		(CS_INT) 5
#define CS_ATTR_SYNTAX_OID		(CS_INT) 6


/* 
**	Structure for defining directory lookup criteria when using
**	ct_ds_lookup api.
*/
typedef struct _cs_ds_lookup_info
{
	CS_OID 		*objclass;
	CS_CHAR 	*path;
	CS_INT		pathlen;
	CS_DS_OBJECT	*attrfilter;
	CS_DS_OBJECT	*attrselect;
} CS_DS_LOOKUP_INFO;

/*
** Predefined signal handlers for client and server signal handler
** libraries.
*/

#define CS_SIGNAL_IGNORE	-1
#define CS_SIGNAL_DEFAULT	-2
#define CS_ASYNC_RESTORE	-3
#define CS_SIGNAL_BLOCK		-4
#define CS_SIGNAL_UNBLOCK	-5

#endif /* __CSTYPES_H__ */
