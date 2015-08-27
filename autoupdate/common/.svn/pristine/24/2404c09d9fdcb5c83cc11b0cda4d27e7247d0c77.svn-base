#pragma once

#ifndef INCLUDED_XNUX_DAEMON_H
#define INCLUDED_XNUX_DAEMON_H

#define		PIDFILENAME "./__this__.pid"

#ifndef WIN32
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <grp.h>

int   createMessageQueue(key_t msgKey);
bool  signalParent(int sem);
pid_t getRunningPID();
void  stopDaemon(pid_t PID);
void  makeDaemon(void);
void  sigBlock(void); 
void  sigUnblock(void); 
void  sigWait(void); 
void  onSignalProc(int signum);

#endif

#endif
