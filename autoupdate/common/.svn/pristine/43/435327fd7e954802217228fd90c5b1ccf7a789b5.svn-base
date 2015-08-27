/* ORIGINAL_SCCSID:  Sccsid @(#) syblogin.h 8.1 8/10/93  */
/* Sccsid %Z% %M% %I% %G% */


/*
**	Sybase DB-LIBRARY Version 5.0
**	Confidential Property of Sybase, Inc.
**	(c) Copyright Sybase, Inc. 1988, 1992
**	All rights reserved
**
**
** Use, duplication, or disclosure by the Government
** is subject to restrictions as set forth in subparagraph (c) (1) (ii)
** of the Rights in Technical Data and Computer Software clause
** at DFARS 52.227-7013. Sybase, Inc. 6475 Christie Avenue, Emeryville,
** CA 94608.
**
** History
**
** 001  12JAN94   Added prototypes for strict ANSI compilation		sureshk
*/

#ifndef __syblogin__
#define __syblogin__

/*
** This structure was used by applications connecting with 1.0 Secure Server.
** It is now here only, so that such applications do not break.
*/
typedef struct seclab
{
        short   slhier;
        BYTE    slcomp[8];
        short   slstatus;
} SECLAB;

#define DB_MAX_LABELLEN	255

/*
** Openserver include file srv.h needs this define
*/
#define DBPROGNLEN	10

typedef struct Securepair
{
        struct Securepair	*next;
        char		*labelname;
        char		*labelvalue;
} SECLABELS;

/*
** Function Prototype for ANSI compilation
*/
#if ((COMPILE_STYLE == ANSI_C_COMPILE) || (COMPILE_STYLE == CPP_COMPILE) || defined(FORCE_PROTOTYPES))

CS_START_EXTERN_C

typedef RETCODE	(CS_PUBLIC *DB_LRESPONSE_FUNC)PROTOTYPE((
				CS_VOID *dbproc,
				BYTE *challenge,
				CS_INT challenge_len,
				CS_VOID *data,
				BYTE *response_len));
CS_END_EXTERN_C

#else

typedef RETCODE	(CS_PUBLIC *DB_LRESPONSE_FUNC)();

#endif

typedef struct loginrec
{
	DBVOIDPTR	ltds_loginrec;
	SECLABELS	*labels;
	DB_LRESPONSE_FUNC	lresponse_func;	/* login response function */
} LOGINREC;

#endif /* __syblogin__ */
