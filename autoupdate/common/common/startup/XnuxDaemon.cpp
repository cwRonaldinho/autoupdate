#include "stdafx.h"

#define new DEBUG_NEW

#ifndef WIN32

#include "XnuxDaemon.h"

#include <signal.h>
#include <grp.h>
#include <errno.h>
#include <unistd.h>

#define MAX_PATH 266

#include <iostream>

int createMessageQueue(key_t msgKey)
{
	int flag = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
	int sem  = msgget(msgKey, flag);

	if (sem == -1)
	{
		if(errno == EEXIST)
		{
			flag &= ~IPC_EXCL;
			sem = msgget(msgKey, flag);
			if (msgctl(sem, IPC_RMID, NULL) != 0)
				return -1;

			sem = msgget(msgKey, flag);
			if (sem == -1)
				return -1;
		}
		else
			return -1;
	}
	return sem;
}

bool signalParent(int sem)
{
#ifdef __sun
     struct  mymsg {
             long  mtype;     /* message type */
             char  mtext[1];  /* message text */
     } buff;
#else
	struct msgbuf buff;
#endif
	buff.mtype = 123;
	if(msgsnd(sem, &buff, 1, 0) < 0)
		return false;

	return true;
}

static sigset_t g_blockmask;
static sigset_t g_emptymask;
static int	g_blocked = 0;

void  makePIDFileName(char PIDFileName[MAX_PATH+2])
{
	getcwd(PIDFileName, MAX_PATH);
	size_t len = strlen(PIDFileName);
	if(len > 0 && (PIDFileName[len-1] != '/') )
		strcat(PIDFileName,"/");
	
	strcat(PIDFileName, PIDFILENAME);
}

pid_t getRunningPID()
{
	char PIDFileName[MAX_PATH+2];
	makePIDFileName(PIDFileName);

	pid_t	pid = -1;
	FILE *	fp;
	if ((fp = fopen(PIDFileName, "r")) != NULL
		&&fscanf(fp, "%d", &pid) == 1 && pid > 0) 
	{
		if(kill(pid, 0) < 0 || errno == ESRCH) 
		{
			pid = -1;
			unlink(PIDFileName);
		}
	}
	if (fp != NULL) 
	{
		fclose(fp);
	}
	return pid;
}

void stopDaemon(pid_t PID)
{
	if(PID > 0 && kill(PID, SIGTERM) >= 0)
	{
		alarm(0);
		do 
		{
			sleep(1);
		}while (kill(PID, 0) >= 0);
	}

	char PIDFileName[MAX_PATH+2];
	makePIDFileName(PIDFileName);

	unlink(PIDFileName);
}

void makeDaemon(void)
{
	pid_t pid;
	struct sigaction sa;
	FILE *fp;

	pid = fork();
	if(pid < 0)
	{
		std::cout << "Can't create a daemon" << std::endl;
		return;
	}
	if(pid > 0)
	{
		exit(0);
	}

	char PIDFileName[MAX_PATH+2];
	makePIDFileName(PIDFileName);

	fp = fopen(PIDFileName, "w");
	if (fp != NULL) 
	{
		fprintf(fp, "%d\n", getpid());
		fclose(fp);
	}

	if (setsid() < 0) 
	{
		std::cout << "setsid() failure." << std::endl;
	}

	sigemptyset(&g_emptymask);
	sigemptyset(&g_blockmask);
	sigaddset(&g_blockmask, SIGHUP);
	sigaddset(&g_blockmask, SIGALRM);

	memset(&sa, 0, sizeof(sa));
	sa.sa_mask = g_blockmask;
	sa.sa_handler = onSignalProc;
	sigaction(SIGALRM, &sa, NULL);
	sa.sa_handler = onSignalProc;
	sigaction(SIGHUP, &sa, NULL);
	sa.sa_handler = onSignalProc;
	sigaction(SIGTERM, &sa, NULL);
	sa.sa_handler = onSignalProc;
	sigaction(SIGINT, &sa,  NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sa, NULL);
}

void onSignalProc(int signum)
{
	if(signum == SIGTERM) {
		char PIDFileName[MAX_PATH+2];
		makePIDFileName(PIDFileName);
		unlink(PIDFileName);
		std::cout << "catch SIGTERM signal, please wait while exiting..." << std::endl;
		sigUnblock();
	}
	else
	{
		std::cout << "catch signal(" << signum << ")." << std::endl;
	}	
}

void sigBlock(void) 
{
	sigprocmask(SIG_BLOCK, &g_blockmask, NULL);
	if(g_blocked) 
	{
		std::cout << "signals already blocked\n";
	}
	g_blocked = 1;
}

void sigUnblock(void) 
{
	sigprocmask(SIG_SETMASK, &g_emptymask, NULL);
	g_blocked = 0;
}

void sigWait(void) 
{
	sigsuspend(&g_emptymask);
}
#endif
