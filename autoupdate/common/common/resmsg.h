#pragma once
enum ResponseMessage
{
	//------------------------
	//MTL res message
	//0x20000001 - 0x20010000
	_ResTraceBegin					=	0x20000001,
	_ResTraceEnd					=	0x20000002,
	_ResWhoAmI						=	0x20000003,
	_ResAddModule					=	0x20000004,
	_ResDelModule					=	0x20000005,
	_ResAssociateModule				=	0x20000006,
	_ResDeassociateModule			=	0x20000007,
	_ResQueryModule					=	0x20000008,
	_ResQueryAssociate				=	0x20000009,
	//------------------------
	//config res message 
	//0x20010001 - 0x20020000
	_ResGetBoardCfg 				=	0x20010001,
	_ResGetTrunkCfg 				=	0x20010002,
	_ResGetPhoneCfg					=	0x20010003,
	
	
	_ResAddCalleeCfg				=	0x20010010,	
	_ResDelCalleeCfg				=	0x20010011,
	_ResQueryCalleeCfg				=	0x20010012,

	_ResAddCalleeRoute				=	0x20010013,
	_ResDelCalleeRoute				=	0x20010014,
	_ResQueryCalleeRoute			=	0x20010015,

	_ResAddCallerCfg				=	0x20010016,	
	_ResDelCallerCfg				=	0x20010017,
	_ResQueryCallerCfg				=	0x20010018,

	_ResAddCallerRoute				=	0x20010019,
	_ResDelCallerRoute				=	0x2001001a,
	_ResQueryCallerRoute			=	0x2001001b,

	_ResAddInRouteCfg				=	0x2001001c,
	_ResDelInRouteCfg				=	0x2001001d,
	_ResQueryInRouteCfg				=	0x2001001e,

	_ResAddInRouteRoute				=	0x2001001f,
	_ResDelInRouteRoute				=	0x20010020,
	_ResQueryInRouteRoute			=	0x20010021,

	_ResAddRouteCfg					=	0x20010022,
	_ResDelRouteCfg					=	0x20010023,
	_ResQueryRouteCfg				=	0x20010024,

	_ResAddCCSService				=	0x20010025,
	_ResDelCCSService				=	0x20010026,
	_ResQueryCCSService				=	0x20010027,

	_ResAddCCSGroup					=	0x20010028,
	_ResDelCCSGroup					=	0x20010029,
	_ResQueryCCSGroup				=	0x2001002a,

	_ResAddCCSAgent					=	0x2001002b,
	_ResDelCCSAgent					=	0x2001002c,
	_ResQueryCCSAgent				=	0x2001002d,

	_ResAddCCSSGA					=	0x2001002e,
	_ResDelCCSSGA					=	0x2001002f,
	_ResQueryCCSSGA					=	0x20010030,

	_ResAddCCSGAA					=	0x20010031,
	_ResDelCCSGAA					=	0x20010032,
	_ResQueryCCSGAA					=	0x20010033,

	_ResAddCCSAdjService			=	0x20010034,
	_ResDelCCSAdjService			=	0x20010035,
	_ResQueryCCSAdjService			=	0x20010036,

	_ResGetSS7DeviceCfg				=	0x20010037,
	_ResGetSS7LinkSetCfg			=	0x20010038,
	_ResGetSS7LinkCfg				=	0x20010039,
	_ResGetSS7RouteCfg				= 	0x2001003a,
	_ResGetSS7CICCfg				=	0x2001003b,
	_ResGetSS7TrunkConnectCfg		=	0x2001003c,
	_ResGetSS7NumberCfg				=	0x2001003d,

	_ResQueryConfig					=	0x2001003e,
	_ResLoadFlow					=	0x2001003f,
	_ResUnLoadFlow					=	0x20010040,
	_ResQueryFlow					=	0x20010041,

	_ResQueryChannelInfo			=	0x20010051,
	_ResQueryResourceInfo			=	0x20010052,
	_ResQuerySpanInfo				=	0x20010053,

	_ResConsole						=	0x20010060,


	_ResLoadOutRouteSpan			=	0x20010071,	
	_ResUnloadOutRouteSpan			=	0x20010072,
	_ResQueryOutRouteSpan			=	0x20010073,

	_ResLoadInRouteSpan				=	0x20010081,	
	_ResUnloadInRouteSpan			=	0x20010082,
	_ResQueryInRouteSpan			=	0x20010083,

	_ResHeartBeat					=	0x20010090,
	//------------------------
	//call res message
	//0x20020001 - 0x20030000
	_ResCallInfoRequest				=	0x20020010,
	_ResBlockChannel				=	0x20020011,
	_ResUnblockChannel				=	0x20020012,
	_ResResetChannel				=	0x20020013,

	_ResAllocResource				=	0x20020014,
	_ResAllocChannel				=	0x20020015,
	_ResCreateNewCall				=	0x20020016,
	_ResEstablishCall				=	0x20020017,
	_ResStartRecordDevice			=	0x20020018,
	_ResStopRecordDevice			=	0x20020019,
	_ResStartMonitorDevice			=	0x2002001a,
	_ResStopMonitorDevice			=	0x2002001b,

	_ResSS7TrunkAssociate			=	0x2002001e,
	_ResSS7TrunkDeassociate			=	0x2002001f,
	_ResStartFlow					=	0x20020020,
	_ResStopFlow					=	0x20020021,
	_ResStartTraceFlow				=	0x20020022,
	_ResStopTraceFlow				=	0x20020023,
	_ResStartIRSFlow				=	0x20020024,
	_ResStopIRSFlow					=	0x20020025,
	_ResAgentService				=	0x20020026,
	_ResStartFlowEx				=	0x20020027,
	_ResCallStartFlow				=	0x20020028,

	//------------------------
	//media res message
	//0x20030001 - 0x20040000
	_ResAllocMediaRsc				=	0x20030001,
	_ResPlayFile					=	0x20030002,
	_ResRecordFile					=	0x20030003,
	_ResCollectDigits				=	0x20030004,
	_ResFlushDigitQueue				=	0x20030005,
	_ResPlayList					=	0x20030006,
	_ResText2Speech					=	0x20030007,
	_ResRecognize					=	0x20030008,
	_ResReleaseMediaRsc				=	0x20030009,	
	_ResSetVolume					=	0x20030010,
	_ResSetSpeed					=	0x20030011,
	_ResStopMediaOperation			=	0x20030012,
	_ResPlayText					=	0x20030013,
	_ResAllocASRResource			=	0x20030014,
	_ResReleaseASRResource			=	0x20030015,
	_ResDetachVoice					=	0x20030016,

	_ResJoinRadioChannel			=	0x20030018,
	_ResLeaveRadioChannel			=	0x20030019,
	
	_ResLoadPackedVox				=	0x20030020,
	_ResUnloadPackedVox				=	0x20030021,

	_ResCreateVoiceMail				=	0x20030031,
	_ResDestroyVoiceMail			=	0x20030032,
	_ResOpenVoiceMail				=	0x20030033,
	_ResCloseVoiceMail				=	0x20030034,
	_ResInsertVoiceMail				=	0x20030035, //带key表示index， 否则在最后添加
	_ResRemoveVoiceMail				=	0x20030036,
	_ResLockVoiceMail				=	0x20030037,
	_ResUnlockVoiceMail				=	0x20030038,
	_ResReplaceVoiceMail			=	0x20030039,
	_ResZipVoiceMail				=	0x20030040,
	_ResGetVoiceMailInfo			=	0x20030041,
	_ResWriteVoiceMailText			=	0x20030042, 
	_ResReadVoiceMailText			=	0x20030043,	
	_ResGetVoiceMailFileName		=	0x20030044,	
		
	_ResRemoveVEGrammarEntry		=	0x20031045,
//for dmu
	_ResSendFax						=	0x20031050,
	_ResRecvFax						=	0x20031051,
	_ResDetachFax					=	0x20031052,

	_ResNetMediaInfo			=	0x20031100,
	_ResNetMediaLink			=	0x20031101,
	_ResNetMediaUnlink			=	0x20031102,

	//------------------------
	//connect res message
	//0x20040001 - 0x20050000
	_ResLinkChannel 				=	0x20040001,
	_ResUnlinkChannel				=	0x20040002,
	_ResLinkMultiChassisChannel 	=	0x20040003,
	_ResUnlinkMultiChassisChannel	=	0x20040004,

	_ResE1LinkMultiChassisChannel	=	0x20040005,
	_ResE1UnlinkMultiChassisChannel =	0x20040006,
	
	_ResSetupConf					=	0x20040010,
	_ResCloseConf					=	0x20040011,
	_ResJoinConf					=	0x20040012,
	_ResLeaveConf 					=	0x20040013,
	_ResJoinMultiChassisConf		=	0x20040014,
	_ResLeaveMultiChassisConf 		=	0x20040015,
	_ResGetConfInfo					=	0x20040016,
	_ResConfResourceReport			=	0x20040017,
	_ResJoinConfByName			=	0x20040018,
	_ResLeaveConfByName			=	0x20040019,
	_ResChangeConfMode					=	0x2004001a,




	//------------------------
	//bill res message
	//0x20050001 - 0x20060000
	_ResLocalAccessRequest			=	0x20050001,
	_ResCheckSession				=	0x20050002,
	_ResSynServiceInfo				=	0x20050003,
	_ResSynProductInfo				=	0x20050004,
	_ResSynFeeInfo					=	0x20050005,
	_ResSynNumCityInfo				=	0x20050006,
	_ResDeleteInfo					=	0x20050007,
	_ResGetServiceInfo				=	0x20050008,
	_ResGetProductInfo				=	0x20050009,
	_ResGetFeeInfo					=	0x2005000a,
	_ResBillRequest					=	0x2005000b,
	_ResSynDSInfo					=	0x2005000c,
	_ResGetDSInfo					=	0x2005000d,
	_ResBillKRequest			   =	0x2005000e,
	_ResLocalCompare             =   0x2005000f,	
	_ResLocalLimitAccessRequest			=	0x20050010,
	
	
	//------------------------
	//cfgserver internal res message
	//0x20100001 - 0x20110000
	
	//------------------------
	//sag internal res message
	//0x20110001 - 0x20120000
	
	
	//------------------------
	//dmu internal res message
	//0x20120001 - 0x20130000
	_ResMixVoice			=	0x20120001,
	

	//------------------------
	//isr internal res message
	//0x20130001 - 0x20140000
	_ResPredictiveCall			=	0x20130001,
	_ResTestCall				=	0x20130002,
	_ResConnectCall				=	0x20130003,
	_ResDisconnectCall			=	0x20130004,
	_ResJoinCall				=	0x20130005,
	_ResUnJoinCall				=	0x20130006,
	_ResRecordChannel			=	0x20130007,
	_ResUnRecordChannel			=	0x20130008,
	_ResAddRecordTask			=	0x20130009,
	_ResDelRecordTask			=	0x2013000a,


	//------------------------
	//slee internal res message
	//0x20140001 - 0x20150000
	_ResExecSQL					=	0x20140001,
	_ResSetShareObject			=	0x20140002,
	_ResWaitShareObject			=	0x20140003,
	
	
	//------------------------
	//confserver internal res message
	//0x20150001 - 0x20160000
	_ResNewTask						=	0x20150001,
	_ResQueryTask					=	0x20150002,
	_ResQueryCall					=	0x20150003,
	_ResEndCall						=	0x20150004,
	
	//------------------------
	//tas internal res message
	//0x20160001 - 0x20170000	
	_ResAccessRequest				=	0x20160001,
	_ResAccountingStart				=	0x20160002,
	_ResAccountingStop				=	0x20160003,
	_ResAccountingOnce				=	0x20160004,	
	_ResForwardBill					=   0x20160005,
	_ResAccountingSet				=   0x20160006,
	_ResCalloutAuthen				=	0x20160007,
	_ResLocalLocalAccessRequest     =   0x20160008,//added by hanfei
	_ResVacclientStatus             =   0x20160009,//added by hanfei
	_ResLocalAccountingSet          =   0x2016000a,//added by hanfei
   
	//------------------------
	//ccs internal res message
	//0x20170001 - 0x20180000	
	_ResLogon			=	0x20170001,
	_ResLogoff			=	0x20170002,
	_ResUnReady			=	0x20170003,
	_ResReady			=	0x20170004,
	_ResRecvCall		=	0x20170005,
	_ResUnRecvCall		=	0x20170006,
	_ResBlock           =	0x20170007,
	_ResUnblock			=	0x20170008,
	_ResConsultationCall	=	0x20170009,
	_ResHoldCall			=	0x2017000a,
	_ResRetrieveCall		=	0x2017000b,
	_ResAlternateCall		=	0x2017000c,
	_ResMakeCall			=	0x2017000d,
	_ResTransferCall		=	0x2017000e,
	_ResSingleStepTransferCall	=	0x2017000f,
	_ResWorkNotReady			=	0x20170010,
	_ResWorkReady				=	0x20170011,
	_ResDivertCall				=	0x20170012,
	_ResSendAgentMsg			=	0x20170013,
	_ResSendGroupMsg			=	0x20170014,
	_ResSendServiceMsg			=	0x20170015,
	_ResStartCoachCall			=	0x20170016,
	_ResStopCoachCall			=	0x20170017,
	_ResInterceptCall			=	0x20170018,
	_ResKickOutAgent			=	0x20170019,
	_ResClearAgentCall			=	0x2017001a,
	_ResStartListenCall			=	0x2017001b,
	_ResStopListenCall			=	0x2017001c,
	_ResStartInterventCall		=	0x2017001d,
	_ResStopInterventCall		=	0x2017001e,
	_ResStartRecordCall			=	0x2017001f,
	_ResStopRecordCall			=	0x20170020,
	_ResConfCall				=	0x20170021,
	_ResSendAllMsg				=	0x20170022,
	_ResChangePassword			=	0x20170023,
	_ResGetQueueLen				=	0x20170024,

	_ResAllocClient				=	0x20170100,
	_ResPingClient				=	0x20170101,
	_ResPingService				=	0x20170102,

	_ResGetServiceRunInfo		=	0x20170202,
	_ResGetGroupRunInfo			=	0x20170203,
	_ResGetAgentRunInfo			=	0x20170204,
	_ResGetServiceStatInfo		=	0x20170205,
	_ResGetGroupStatInfo		=	0x20170206,
	_ResGetAgentStatInfo		=	0x20170207,
	_ResGetServiceAgentInfo		=	0x20170208,
	_ResGetGroupAgentInfo		=	0x20170209,

	_ResAgentRegister				=	0x20170301,
	_ResAgentUnRegister			=	0x20170302,
	_ResAgentNotReady				=	0x20170303,
	_ResAgentReady					=	0x20170304,
	_ResAgentCallComing			=	0x20170305,
	_ResAgentDivertCall				=	0x20170306,
	_ResAgentPreMakeCall			=	0x20170307,
	_ResAgentBusy						=	0x20170308,
	_ResAgentCreateNewCall		=	0x20170309,
	_ResAgentConsultationCall	=	0x2017030a,
	_ResAgentAlternateCall			=	0x2017030b,







	//------------------------
	//asr internal res message
	//0x20180001 - 0x20190000
	_ResAttachAsrRes			=	0x20180001,	//绑定ASR资源
	_ResDetachAsrRes			=	0x20180002,	//解除绑定
	_ResSetParameter			=	0x20180003,	//设置参数
	_ResQueryParameter			=	0x20180004,	//查询参数
	_ResAddGrammar				=   0x20180005,	//添加识别语法
	_ResStartRecognize			=   0x20180006,	//启动识别
	_ResStopRecognize			=   0x20180007,	//关闭识别

	//------------------------
	//gateway internal res message
	//0x20190001 - 0x201a0000
	_ResSendSMS					=	0x20190001,
	_ResGWService				=   0x20190002,
	_ResRegisterService			=	0x20190003,
	_ResSendWapPush				=	0x20190005,
	_ResQuerySendWindowSize		=   0x20190006,//ken ,2010/11/5
	_ResSMSReport				=	0x20190007,//ken,2010/11/9
	_ResSMSDeliver				=	0x20190008,//ken ,2010/11/23
	_ResSMSSendBatch			=   0x20190009,//ken,2010/11/23

	//telephone Q response message
	_ResTQPreAllocID			=	0x201A0001,
	_ResTQLogin					=	0x201A0002,
	_ResTQModifyInfo			=   0x201A0003,
	_ResTQRememberFriend		=	0x201A0004,
	_ResTQFindNumber			=	0x201A0005,
	_ResTQInviteNumber			=	0x201A0006,
	_ResTQAcceptInvite			=	0x201A0007,
	_ResTQFinishTalk			=	0x201A0008,	
	_ResTQWaitForInvite			=	0x201A0009,
	_ResTQAllocIDConfirm		=   0x201A000A,

	_ResTQEnterRoom				=   0x201A000B,
	_ResTQLeaveRoom				=   0x201A000C,
	_ResTQRoomBroadcast			=   0x201A000D,
	_ResTQQueryRoomInfo			=   0x201A000E,
	_ResTQQueryActiveRoom		=   0x201A000F,
	_ResTQQueryRoomDetails		=   0x201A0010,
	_ResTQQueryChats			=   0x201A0011,
	_ResTQUpdateRoomInfo		=   0x201A0012,
	_ResTQMonitor				=   0x201A0013,
	_ResTQChangeToTalk			=   0x201A0014,
	_ResTQChangeToListenOnly	=   0x201A0015,
	_ResTQListNumber			=   0x201A0016,
	_ResTQCreateRoom			=   0x201A0017,
	_ResTQQueryNumberInfo		=   0x201A0018,
	_ResTQDeleteRoom			=   0x201A0019,

	// alarm system response message
	_ResCheckServiceStatus		=   0x201B0001,
	_ResDeployService			=   0x201B0002,
	_ResGetDiskUsage			=   0x201B0003,
	_ResGetCPUUsage				=   0x201B0004,
	_ResGetDBUsage				=   0x201B0005,
	_ResStartService			=   0x201B0006,
	_ResStopService				=   0x201B0007,

	_ResHookAlarm				=   0x201B0008,
	_ResUnhookAlarm				=   0x201B0009,
	_ResCommand					=   0x201B000A,

	// SMC response message
	_ResConfigRBUser			=	0x201C0001,
	_ResPutFile					=   0x201C0002,
	_ResPutFileData				=   0x201C0003,
	_ResCheckFile				=   0x201C0004,
	_ResFinalCheck				=   0x201C0005,
	_ResUpdateSQL				=	0x201C0006,
	_ResQuerySQL				=	0x201C0007,
	_ResCallSQL					=	0x201C0008,
	_ResConfigService			=	0x201C0009,
	_ResConfigFlow				=   0x201C000a, // 上传流程配置
	_ResDeleteRBUser            =	0x201C000b,
	_ResConfigRBType			= 	0x201C000c,
	_ResDelteRBType				=	0x201C000d,
	_ResGetFile					=	0x201C000e,
	_ResUpdateML				=	0x201C000f,
	_ResDeleteML				=	0x201C0010,
	_ResNewML					=	0x201C0011,
	_ResConfigML				=	0x201C0012,
	_ResUpdateService			=	0x201C0013,
	_ResDelService				=	0x201C0014,
	_ResDelFlow					=	0x201C0015,
	_ResQueryLog				=	0x201C0016,
	_ResNewGW					=	0x201C0017,
	_ResUpdateGW				=	0x201C0018,
	_ResDelGW					=	0x201C0019,
	_ResGetGWInfo				=	0x201C001a,
	_ResGetBillStatus			=	0x201C001b,

	// ConfUnit response message
	_ResSetupTConf				=	0x201D0001,
	_ResJoinTConf				=	0x201D0002,
	_ResLeaveTConf				=	0x201D0003,
	_ResCloseTConf				=	0x201D0004,
	_ResAwayFromTConf			=	0x201D0005,
	_ResBackToTConf				=	0x201D0006,
	_ResTConfVote				=   0x201D0007,
	_ResTConfAttendance			=   0x201D0008,

	_ResExportBill				=   0x201D0009,


	_ResGetNodeInfo				=	0x201E0001,
	_ResGetServerInfo			=	0x201E0002,
	_ResGetDiskInfo				=	0x201E0003,
	_ResGetCPUInfo				=	0x201E0004,
	_ResGetNCInfo				=	0x201E0005,
	_ResGetServerStatus			=	0x201E0006,
	_ResGetDiskStatus			=	0x201E0007,
	_ResGetNCStatus				=	0x201E0008,
	_ResGetCPUStatus			=	0x201E0009,
	_ResGetMemoryStatus			=	0x201E000a,
	_ResGetModuleInfo			=	0x201E000b,
	_ResGetModuleStatus			=	0x201E000c,
	_ResGetTrunkStatus			=	0x201E000d,
	_ResGetLinkStatus			=	0x201E000e,
	_ResGetBoardInfo			=	0x201E000f,
	_ResGetTrunkInfo			=	0x201E0010,

	_ResTConfCreate				=	0x201F0001,
	_ResTConfDelete				=	0x201F0002,
	_ResTConfJoin				=	0x201F0003,
	_ResTConfAway				=	0x201F0004,
	_ResTConfBack				=	0x201F0005,
	_ResTConfLeave				=	0x201F0006,
	_ResTConfChangeMode			=	0x201F0007,
	_ResTConfMoveParty			=	0x201F0008,
	_ResTConfLink				=	0x201F0009,
	_ResTConfFind				=	0x201F000a,
	// alarm agent
	_ResAlarmRequestConfig		=	0x20310001,
	_ResPlayTextToVoice			=	0x20310002,
	_ResCallEarlyMedia			=	0x20310003,
	_ResCallUpdateMedia			=	0x20310004,
	_ResSipReInvite				=	0x20310005,
	_ResSmsStart				=	0x20310006,
	_ResGetRadioInfo			=	0x20310007,

	_ResSendDigits				=	0x20310008,

};

