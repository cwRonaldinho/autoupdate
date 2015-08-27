#ifndef  _ALARMＭSGLIST_H_
#define _ALARMＭSGLIST_H_

////////////////////////////////////////////////////////////////////////////
//                             关于msgid的约定                            //
//msgID = 程序告警编号*1000000 + 程序内模块编号 * 1000 + 模块内告警编号    //
//程序告警编号为前3位																									//
//	程序内模块编号为中间3位																						  //
//	模块内告警编号为最后3位																						//
///////////////////////////////////////////////////////////////////////////
#define	ALARM_BASE		1000000

//程序告警编号
#define	ALARM_SLEE						1				//slee
#define	ALARM_HTTPGW				2				//httpgw
#define	ALARM_LOGMONITOR		3				//logmonitor


//程序内模块编号(SLEE)
#define	ALARM_SLEE_MODULE			1000		//

#define	MAKE_SLEE_VACMODULE_ALARMID(INDEX)					(ALARM_SLEE * ALARM_BASE + ALARM_SLEE_MODULE + INDEX)
#define	MAKE_SLEE_NOCALLMODULE_ALARMID(INDEX)			(ALARM_SLEE * ALARM_BASE + ALARM_SLEE_MODULE + INDEX)
#define	MAKE_SLEE_HTTPGWMODULE_ALARMID(INDEX)			(ALARM_SLEE * ALARM_BASE + ALARM_SLEE_MODULE + INDEX)
#define MAKE_SLEE_DAGW_ALARMID(INDEX)							(ALARM_SLEE * ALARM_BASE + ALARM_SLEE_MODULE + INDEX)
//程序内模块编号(HTTPGW)
#define  ALARM_HTTPGW_MODULE		1000    

#define	MAKE_HTTPGW_EVENTMODULE_ALARMID(INDEX)		(ALARM_HTTPGW * ALARM_BASE + ALARM_HTTPGW_MODULE + INDEX)


//slee模块内告警编号
#define	ALARM_SLEE_HTTPGW_OR_GW_FAIL								MAKE_SLEE_HTTPGWMODULE_ALARMID(1)//httpgw模块没有启动，或者消息没有发送到httpgw
#define	ALARM_SLEE_VAC_TIMEOUT_OR_FAIL							MAKE_SLEE_VACMODULE_ALARMID(2)//vac鉴权、计费、查询超时或者失败
#define	ALARM_SLEE_NOCALL_LONG_TIME								MAKE_SLEE_NOCALLMODULE_ALARMID(3)//一段时间内无呼叫
#define ALARM_SLEE_DAGW_FAIL												MAKE_SLEE_DAGW_ALARMID(4)
//httpgw模块内告警编号
#define	ALARM_HTTPGW_MODULE_TIMEOUT							MAKE_HTTPGW_EVENTMODULE_ALARMID(1)//httpgw超时
#endif