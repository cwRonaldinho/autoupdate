#ifndef INCLUDED_STDAFX_H
#define INCLUDED_STDAFX_H

#pragma once

#if defined(WIN32)

// for MSVC on WIN32 platform
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif 


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// ?? Windows ????????????????
#endif

#ifndef WINVER				
#define WINVER 0x0500			// Windows 2K and above
#endif

#ifndef _WIN32_WINNT		
#define _WIN32_WINNT 0x0500		// Windows 2K and above
#endif						

#ifndef _WIN32_WINDOWS		
#define _WIN32_WINDOWS 0x0500	//Windows 2K and above
#endif

#ifndef _WIN32_IE			
#define _WIN32_IE 0x0400	// IE4.0 and above
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <winsock2.h>

#else
// non-win32

#endif // #if defined(_MSC_VER)

#ifdef PRECOMPILE_HEADER
// std include
#include "std_inc.h"

#endif
#include "evtmsg.h"
#include "reqmsg.h"
#include "resmsg.h"
#include "sxcode.h"
#include "tag.h"
#include "cacti/message/TransferMessage.h"
#endif



