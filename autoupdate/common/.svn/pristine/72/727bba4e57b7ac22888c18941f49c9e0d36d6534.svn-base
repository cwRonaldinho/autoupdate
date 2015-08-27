#pragma once
enum RequestMessage
{
	//------------------------
	//MTL req message
	//0x10000001 - 0x10010000
	_ReqTraceBegin					=	0x10000001,
	_ReqTraceEnd	 				=	0x10000002,
	_ReqWhoAmI						=	0x10000003,
	_ReqAddModule					=	0x10000004,
	_ReqDelModule					=	0x10000005,
	_ReqAssociateModule				=	0x10000006,
	_ReqDeassociateModule			=	0x10000007,
	_ReqQueryModule					=	0x10000008,
	_ReqQueryAssociate				=	0x10000009,
	//------------------------
	//config req message 
	//0x10010001 - 0x10020000
	_ReqGetBoardCfg 				=	0x10010001,
	_ReqGetTrunkCfg 				=	0x10010002,
	_ReqGetPhoneCfg					=	0x10010003,
	
	
	_ReqAddCalleeCfg				=	0x10010010,	
	_ReqDelCalleeCfg				=	0x10010011,
	_ReqQueryCalleeCfg				=	0x10010012,
	
	_ReqAddCalleeRoute				=	0x10010013,
	_ReqDelCalleeRoute				=	0x10010014,
	_ReqQueryCalleeRoute			=	0x10010015,

	_ReqAddCallerCfg				=	0x10010016,	
	_ReqDelCallerCfg				=	0x10010017,
	_ReqQueryCallerCfg				=	0x10010018,

	_ReqAddCallerRoute				=	0x10010019,
	_ReqDelCallerRoute				=	0x1001001a,
	_ReqQueryCallerRoute			=	0x1001001b,
	
	_ReqAddInRouteCfg				=	0x1001001c,
	_ReqDelInRouteCfg				=	0x1001001d,
	_ReqQueryInRouteCfg				=	0x1001001e,

	_ReqAddInRouteRoute				=	0x1001001f,
	_ReqDelInRouteRoute				=	0x10010020,
	_ReqQueryInRouteRoute			=	0x10010021,
	
	_ReqAddRouteCfg					=	0x10010022,
	_ReqDelRouteCfg					=	0x10010023,
	_ReqQueryRouteCfg				=	0x10010024,

	_ReqAddCCSService				=	0x10010025,
	_ReqDelCCSService				=	0x10010026,
	_ReqQueryCCSService				=	0x10010027,

	_ReqAddCCSGroup					=	0x10010028,
	_ReqDelCCSGroup					=	0x10010029,
	_ReqQueryCCSGroup				=	0x1001002a,

	_ReqAddCCSAgent					=	0x1001002b,
	_ReqDelCCSAgent					=	0x1001002c,
	_ReqQueryCCSAgent				=	0x1001002d,

	_ReqAddCCSSGA					=	0x1001002e,
	_ReqDelCCSSGA					=	0x1001002f,
	_ReqQueryCCSSGA					=	0x10010030,

	_ReqAddCCSGAA					=	0x10010031,
	_ReqDelCCSGAA					=	0x10010032,
	_ReqQueryCCSGAA					=	0x10010033,

	_ReqAddCCSAdjService			=	0x10010034,
	_ReqDelCCSAdjService			=	0x10010035,
	_ReqQueryCCSAdjService			=	0x10010036,

	_ReqGetSS7DeviceCfg				=	0x10010037,
	_ReqGetSS7LinkSetCfg			=	0x10010038,
	_ReqGetSS7LinkCfg				=	0x10010039,
	_ReqGetSS7RouteCfg				=	0x1001003a,
	_ReqGetSS7CICCfg				=	0x1001003b,
	_ReqGetSS7TrunkConnectCfg		= 	0x1001003c,
	_ReqGetSS7NumberCfg				=	0x1001003d,
	
	_ReqQueryConfig					=	0x1001003e,

	_ReqLoadFlow					=	0x1001003f,
	_ReqUnLoadFlow					=	0x10010040,
	_ReqQueryFlow					=	0x10010041,

	_ReqQueryChannelInfo			=	0x10010051,
	_ReqQueryResourceInfo			=	0x10010052,
	_ReqQuerySpanInfo				=	0x10010053,
 
	_ReqConsole						=	0x10010060,


	_ReqLoadOutRouteSpan			=	0x10010071,	
	_ReqUnloadOutRouteSpan			=	0x10010072,
	_ReqQueryOutRouteSpan			=	0x10010073,

	_ReqLoadInRouteSpan				=	0x10010081,	
	_ReqUnloadInRouteSpan			=	0x10010082,
	_ReqQueryInRouteSpan			=	0x10010083,

	_ReqHeartBeat					=	0x10010090,
	//------------------------
	//call req message
	//0x10020001 - 0x10030000
	_ReqCallInfoRequest				=	0x10020010,
	_ReqBlockChannel				=	0x10020011,
	_ReqUnblockChannel				=	0x10020012,
	_ReqResetChannel				=	0x10020013,

	_ReqAllocResource				=	0x10020014,
	_ReqAllocChannel				=	0x10020015,
	_ReqCreateNewCall				=	0x10020016,
	_ReqEstablishCall				=	0x10020017,
	_ReqStartRecordDevice			=	0x10020018,
	_ReqStopRecordDevice			=	0x10020019,
	_ReqStartMonitorDevice			=	0x1002001a,
	_ReqStopMonitorDevice			=	0x1002001b,

	_ReqSS7TrunkAssociate			=	0x1002001e,
	_ReqSS7TrunkDeassociate			=	0x1002001f,
	_ReqStartFlow					=	0x10020020,
	_ReqStopFlow					=	0x10020021,
	_ReqStartTraceFlow				=	0x10020022,
	_ReqStopTraceFlow				=	0x10020023,
	_ReqStartIRSFlow				=	0x10020024,
	_ReqStopIRSFlow					=	0x10020025,
	_ReqAgentService				=	0x10020026,
	_ReqStartFlowEx				=	0x10020027,
	_ReqCallStartFlow				=	0x10020028,
	
	//------------------------
	//media req message
	//0x10030001 - 0x10040000
	_ReqAllocMediaRsc				=	0x10030001,
	_ReqPlayFile					=	0x10030002,
	_ReqRecordFile					=	0x10030003,
	_ReqCollectDigits				=	0x10030004,
	_ReqFlushDigitQueue				=	0x10030005,
	_ReqPlayList					=	0x10030006,
	_ReqText2Speech					=	0x10030007,
	_ReqRecognize					=	0x10030008,
	_ReqReleaseMediaRsc				=	0x10030009,
	_ReqSetVolume					=	0x10030010,
	_ReqSetSpeed					=	0x10030011,
	_ReqStopMediaOperation			=	0x10030012,
	_ReqPlayText					=	0x10030013,
	_ReqAllocASRResource			=	0x10030014,
	_ReqReleaseASRResource			=	0x10030015,
	_ReqDetachVoice					=	0x10030016,
	_ReqPlayStream					=	0x10030017,
	_ReqJoinRadioChannel			=	0x10030018,
	_ReqLeaveRadioChannel			=	0x10030019,
		
	_ReqLoadPackedVox				=	0x10030020,
	_ReqUnloadPackedVox				=	0x10030021,

	_ReqCreateVoiceMail				=	0x10030031, //创建语音邮箱
	_ReqDestroyVoiceMail			=	0x10030032, //删除语音邮箱
	_ReqOpenVoiceMail				=	0x10030033, //打开语音邮箱
	_ReqCloseVoiceMail				=	0x10030034, //关闭语音邮箱
	_ReqInsertVoiceMail				=	0x10030035, //插入语音邮件 //在尾部 
	_ReqRemoveVoiceMail				=	0x10030036, //删除指定位置的语音邮件
	_ReqLockVoiceMail				=	0x10030037, //锁定语音邮箱。 邮件
	_ReqUnlockVoiceMail				=	0x10030038, //带index的key表示lock单个mail， 否则表示lock文件
	_ReqReplaceVoiceMail			=	0x10030039, //替换指定位置的语音邮件
	_ReqZipVoiceMail				=	0x10030040, //压缩语音邮箱，去掉空的index
	_ReqGetVoiceMailInfo			=	0x10030041,	//更新邮箱信息
	_ReqWriteVoiceMailText			=	0x10030042, //修改邮件文本
	_ReqReadVoiceMailText			=	0x10030043,	//读取邮件文本
	_ReqGetVoiceMailFileName		=	0x10030044,	//读取邮件语音文本

	

	_ReqRemoveVEGrammarEntry		=	0x10031045,
//for dmu
	_ReqSendFax						=	0x10031050,
	_ReqRecvFax						=	0x10031051,
	
	_ReqNetMediaInfo			=	0x10031100,
	_ReqNetMediaLink			=	0x10031101,
	_ReqNetMediaUnlink			=	0x10031102,
	//------------------------
	//connect req message
	//0x10040001 - 0x10050000
	_ReqLinkChannel 				=	0x10040001,
	_ReqUnlinkChannel				=	0x10040002,
	_ReqLinkMultiChassisChannel 	=	0x10040003,
	_ReqUnlinkMultiChassisChannel	=	0x10040004,

	_ReqE1LinkMultiChassisChannel	=	0x10040005,
	_ReqE1UnlinkMultiChassisChannel =	0x10040006,
	
	_ReqSetupConf					=	0x10040010,
	_ReqCloseConf					=	0x10040011,
	_ReqJoinConf					=	0x10040012,
	_ReqLeaveConf 					=	0x10040013,
	_ReqJoinMultiChassisConf		=	0x10040014,
	_ReqLeaveMultiChassisConf 		=	0x10040015,
	_ReqGetConfInfo					=	0x10040016,
	_ReqConfResourceReport			=	0x10040017,
	_ReqJoinConfByName			=	0x10040018,
	_ReqLeaveConfByName		=	0x10040019,
	_ReqChangeConfMode				=	0x1004001a,

	
	//------------------------
	//bill req message
	//0x10050001 - 0x10060000
	_ReqLocalAccessRequest			=	0x10050001,
	_ReqCheckSession				=	0x10050002,
	_ReqSynServiceInfo				=	0x10050003,
	_ReqSynProductInfo				=	0x10050004,
	_ReqSynFeeInfo					=	0x10050005,
	_ReqSynNumCityInfo				=	0x10050006,
	_ReqDeleteInfo					=	0x10050007,
	_ReqGetServiceInfo				=	0x10050008,
	_ReqGetProductInfo				=	0x10050009,
	_ReqGetFeeInfo					=	0x1005000a,
	_ReqBillRequest					=	0x1005000b,
	_ReqSynDSInfo					=	0x1005000c,
	_ReqGetDSInfo					=	0x1005000d,
	_ReqBillKRequest			    =   0x1005000e, 
	_ReqLimitAccessRequest			=	0x1005000f,
	
	
	//------------------------
	//cfgserver internal req message
	//0x10100001 - 0x10110000
	
	//------------------------
	//sag internal req message
	//0x10110001 - 0x10120000
	
	
	//------------------------
	//dmu internal req message
	//0x10120001 - 0x10130000
	_ReqMixVoice				= 0x10120001,
	

	//------------------------
	//isr internal req message
	//0x10130001 - 0x10140000
	_ReqPredictiveCall				=	0x10130001,
	_ReqTestCall					=	0x10130002,
	_ReqConnectCall					=	0x10130003,
	_ReqDisconnectCall				=	0x10130004,
	_ReqJoinCall					=	0x10130005,
	_ReqUnJoinCall					=	0x10130006,
	_ReqRecordChannel				=	0x10130007,
	_ReqUnRecordChannel				=	0x10130008,
	_ReqAddRecordTask				=	0x10130009,
	_ReqDelRecordTask				=	0x1013000a,
	//------------------------
	//slee internal req message
	//0x10140001 - 0x10150000
	_ReqExecSQL						=	0x10140001,
	_ReqSetShareObject				=	0x10140002,
	_ReqWaitShareObject				=	0x10140003,
		
	
	//------------------------
	//bcm internal req message
	//0x10150001 - 0x10160000
	_ReqNewTask						=	0x10150001,
	_ReqQueryTask					=	0x10150002,
	_ReqQueryCall					=	0x10150003,
	_ReqEndCall						=	0x10150004,
	
	//------------------------
	//tas internal req message
	//0x10160001 - 0x10170000
	_ReqAccessRequest				=  0x10160001,
	_ReqAccountingStart				=  0x10160002,
	_ReqAccountingStop				=  0x10160003,
	_ReqAccountingOnce				=  0x10160004,
	_ReqForwardBill					=  0x10160005,
	_ReqAccountingSet				=  0x10160006,
	_ReqCalloutAuthen				=  0x10160007,
	_ReqPrmAccountingStart          =  0x10160009,//added by hanfei
	_ReqPrmAccountingStop           =  0x1016000a,//added by hanfei
	_ReqLocalAccountingOnce         =  0x1016000b,//added by hanfei
	_ReqPrmAccountingSet            =  0x1016000c,//added by hanfei
	_ReqAccountingStartOnce         =  0x1016000d,//added by hanfei
    _ReqAccountingStopOnce          =  0x1016000e,//added by hanfei
	_ReqLocalCompare                 =  0x1016000f,//added by hanfei
	//------------------------
	//ccs internal req message
	//0x10170001 - 0x10180000	
	_ReqLogon			=	0x10170001,
	_ReqLogoff			=	0x10170002,
	_ReqUnReady			=	0x10170003,
	_ReqReady			=	0x10170004,
	_ReqRecvCall		=	0x10170005,
	_ReqUnRecvCall		=	0x10170006,
	_ReqBlock           =	0x10170007,
	_ReqUnblock			=	0x10170008,
	_ReqConsultationCall	=	0x10170009,
	_ReqHoldCall			=	0x1017000a,
	_ReqRetrieveCall		=	0x1017000b,
	_ReqAlternateCall		=	0x1017000c,
	_ReqMakeCall			=	0x1017000d,
	_ReqTransferCall		=	0x1017000e,
	_ReqSingleStepTransferCall	=	0x1017000f,
	_ReqWorkNotReady			=	0x10170010,
	_ReqWorkReady				=	0x10170011,
	_ReqDivertCall				=	0x10170012,
	_ReqSendAgentMsg			=	0x10170013,
	_ReqSendGroupMsg			=	0x10170014,
	_ReqSendServiceMsg			=	0x10170015,
	_ReqStartCoachCall			=	0x10170016,
	_ReqStopCoachCall			=	0x10170017,
	_ReqInterceptCall			=	0x10170018,
	_ReqKickOutAgent			=	0x10170019,
	_ReqClearAgentCall			=	0x1017001a,
	_ReqStartListenCall			=	0x1017001b,
	_ReqStopListenCall			=	0x1017001c,
	_ReqStartInterventCall		=	0x1017001d,
	_ReqStopInterventCall		=	0x1017001e,
	_ReqStartRecordCall			=	0x1017001f,
	_ReqStopRecordCall			=	0x10170020,
	_ReqConfCall				=	0x10170021,
	_ReqSendAllMsg				=	0x10170022,
	_ReqChangePassword			=	0x10170023,
	_ReqGetQueueLen				=	0x10170024,

	_ReqAllocClient				=	0x10170100,
	_ReqPingClient				=	0x10170101,
	_ReqPingService				=	0x10170102,

	_ReqGetServiceRunInfo		=	0x10170202,
	_ReqGetGroupRunInfo			=	0x10170203,
	_ReqGetAgentRunInfo			=	0x10170204,
	_ReqGetServiceStatInfo		=	0x10170205,
	_ReqGetGroupStatInfo		=	0x10170206,
	_ReqGetAgentStatInfo		=	0x10170207,
	_ReqGetServiceAgentInfo		=	0x10170208,
	_ReqGetGroupAgentInfo		=	0x10170209,

	_ReqAgentRegister				=	0x10170301,
	_ReqAgentUnRegister			=	0x10170302,
	_ReqAgentNotReady				=	0x10170303,
	_ReqAgentReady					=	0x10170304,
	_ReqAgentCallComing			=	0x10170305,
	_ReqAgentDivertCall				=	0x10170306,
	_ReqAgentPreMakeCall			=	0x10170307,
	_ReqAgentBusy						=	0x10170308,
	_ReqAgentCreateNewCall		=	0x10170309,
	_ReqAgentConsultationCall	=	0x1017030a,
	_ReqAgentAlternateCall		=	0x1017030b,

	//------------------------
	//asr internal req message
	//0x10180001 - 0x10190000
	_ReqAttachAsrRes			=	0x10180001,	//绑定ASR资源
	_ReqDetachAsrRes			=	0x10180002,	//解除绑定
	_ReqSetParameter			=	0x10180003,	//设置参数
	_ReqQueryParameter			=	0x10180004,	//查询参数
	_ReqAddGrammar				=   0x10180005,	//添加识别语法
	_ReqStartRecognize			=   0x10180006,	//启动识别
	_ReqStopRecognize			=   0x10180007,	//关闭识别
	
	//------------------------
	//gateway internal req message
	//0x10190001 - 0x101a0000
	_ReqSendSMS					=	0x10190001,
	_ReqGWService				=   0x10190002,
	_ReqRegisterService			=	0x10190003,
	_ReqSendWapPush				=	0x10190005,
	_ReqQuerySendWindowSize		=	0x10190006,//ken ,2010/11/5
	_ReqSMSReport				=	0x10190007,//ken,2010/11/9
	_ReqSMSDeliver				=	0x10190008,//ken ,2010/11/23
	_ReqSMSSendBatch			=   0x10190009,//ken,2010/11/23
	//telephone Q request message
	_ReqTQPreAllocID			=	0x101A0001,	// 预申请TQ号码
	_ReqTQLogin					=	0x101A0002,	// 登录
	_ReqTQModifyInfo			=   0x101A0003,	// 修改个人信息
	_ReqTQRememberFriend		=	0x101A0004,	// 登记好友号码
	_ReqTQFindNumber			=	0x101A0005,	// 按照条件查找号码
	_ReqTQInviteNumber			=	0x101A0006,	// 聊天邀请
	_ReqTQAcceptInvite			=	0x101A0007,	// 接受（或拒绝）邀请
	_ReqTQFinishTalk			=	0x101A0008,	// 结束聊天
	_ReqTQWaitForInvite			=   0x101A0009,	// 等待邀请
	_ReqTQAllocIDConfirm		=   0x101A000A,	// 确认申请过的号码

	_ReqTQEnterRoom				=   0x101A000B,	// 进入聊天室
	_ReqTQLeaveRoom				=   0x101A000C,	// 离开聊天室
	_ReqTQRoomBroadcast			=   0x101A000D,	// 聊天室广播
	_ReqTQQueryRoomInfo			=   0x101A000E,	// 查询聊天室信息
	_ReqTQQueryActiveRoom       =   0x101A000F,	// 查询当前活动的聊天室
	_ReqTQQueryRoomDetails		=   0x101A0010,	// 查询某个活动的聊天室的详细内容
	_ReqTQQueryChats			=   0x101A0011, // 查询聊天室对对碰的情况
	_ReqTQUpdateRoomInfo        =   0x101A0012, // 更新聊天室信息
	_ReqTQMonitor				=   0x101A0013, // 监听聊天室特定成员
	_ReqTQChangeToTalk			=   0x101A0014, // 把当前用户改变成可说模式
	_ReqTQChangeToListenOnly	=   0x101A0015, // 把当前用户改变成只听模式
	_ReqTQListNumber			=   0x101A0016, // 取得好友列表
	_ReqTQCreateRoom			=   0x101A0017, // 自建聊天室
	_ReqTQQueryNumberInfo		=   0x101A0018, // 查询指定号码的详细信息

	// alarm system request message
	_ReqCheckServiceStatus		=   0x101B0001, // 检查服务的状态
	_ReqDeployService			=   0x101B0002, // 部署服务
	_ReqGetDiskUsage			=   0x101B0003, // 取得服务器的磁盘使用情况
	_ReqGetCPUUsage				=   0x101B0004, // 取得服务器的CPU使用情况
	_ReqGetDBUsage				=   0x101B0005, // 取得数据库服务器的使用情况
	_ReqStartService			=   0x101B0006, // 启动制定的服务
	_ReqStopService				=   0x101B0007, // 停止制定的服务
	_ReqHookAlarm				=   0x101B0008, // 注册等级接收监控信息
	_ReqUnhookAlarm				=   0x101B0009, // 取消注册等级监控信息
	_ReqCommand					=   0x101B000A, // 命令行消息



	// SMC request message
	_ReqConfigRBUser			=   0x101C0001, // 配置红黑名单用户
	_ReqPutFile					=   0x101C0002, // 上传文件
	_ReqPutFileData				=   0x101C0003,	// 上传文件数据
	_ReqCheckFile				=   0x101C0004,	// 检查文件是否需要更新
	_ReqFinalCheck				=   0x101C0005, // 上传文件完毕，校验文件
	_ReqUpdateSQL				=	0x101C0006,
	_ReqQuerySQL				=	0x101C0007,
	_ReqCallSQL					=	0x101C0008,
	_ReqConfigService			=	0x101C0009,
	_ReqConfigFlow				=   0x101C000a, // 上传流程配置
	_ReqDeleteRBUser            =	0x101C000b,
	_ReqConfigRBType			= 	0x101C000c,
	_ReqDelteRBType				=	0x101C000d,
	_ReqGetFile					=	0x101C000e,
	_ReqUpdateML				=	0x101C000f,
	_ReqDeleteML				=	0x101C0010,
	_ReqNewML					=	0x101C0011,
	_ReqConfigML				=	0x101C0012,
	_ReqUpdateService			=	0x101C0013,
	_ReqDelService				=	0x101C0014,
	_ReqDelFlow					=	0x101C0015,
	_ReqQueryLog				=	0x101C0016,
	_ReqNewGW					=	0x101C0017,
	_ReqUpdateGW				=	0x101C0018,
	_ReqDelGW					=	0x101C0019,
	_ReqGetGWInfo				=	0x101C001a,
	_ReqGetBillStatus			=	0x101C001b,
	
	_ReqSetupTConf				=	0x101D0001,
	_ReqJoinTConf				=	0x101D0002,
	_ReqLeaveTConf				=	0x101D0003,
	_ReqCloseTConf				=	0x101D0004,
	_ReqAwayFromTConf			=	0x101D0005,
	_ReqBackToTConf				=	0x101D0006,
	_ReqTConfVote				=   0x101D0007,
	_ReqTConfAttendance			=   0x101D0008,

	_ReqExportBill				=   0x101D0009,	// 请求导出话单

	//net manager
	_ReqGetNodeInfo				=	0x101E0001,
	_ReqGetServerInfo			=	0x101E0002,
	_ReqGetDiskInfo				=	0x101E0003,
	_ReqGetCPUInfo				=	0x101E0004,
	_ReqGetNCInfo				=	0x101E0005,
	_ReqGetServerStatus			=	0x101E0006,
	_ReqGetDiskStatus			=	0x101E0007,
	_ReqGetNCStatus				=	0x101E0008,
	_ReqGetCPUStatus			=	0x101E0009,
	_ReqGetMemoryStatus			=	0x101E000a,
	_ReqGetModuleInfo			=	0x101E000b,
	_ReqGetModuleStatus			=	0x101E000c,
	_ReqGetTrunkStatus          =	0x101E000d,
	_ReqGetLinkStatus			=	0x101E000e,
	_ReqGetBoardInfo			=	0x101E000f,
	_ReqGetTrunkInfo			=	0x101E0010,

	_ReqTConfCreate				=	0x101F0001,
	_ReqTConfDelete				=	0x101F0002,
	_ReqTConfJoin				=	0x101F0003,
	_ReqTConfAway				=	0x101F0004,
	_ReqTConfBack				=	0x101F0005,
	_ReqTConfLeave				=	0x101F0006,
	_ReqTConfChangeMode			=	0x101F0007,
	_ReqTConfMoveParty			=	0x101F0008,
	_ReqTConfLink				=	0x101F0009,
	_ReqTConfFind				=	0x101F000a,
	//------------------------
	//vac internal req message
	//0x10200001 - 0x10210000
	_ReqCheckPrice				=	0x10200001,
	_ReqCheckPriceConfirm		=	0x10200002,
	_ReqTrafficPrice			=	0x10200003, 
	_ReqBind					=	0x10200004,
	//-----------------
	//checkfile moudle
	_ReqPing					=	0x10300001,
	_ReqCalFile					=	0x10300002,
	// alarm agent
	_ReqAlarmRequestConfig		=	0x10310001,
	_ReqPlayTextToVoice			=	0x10310002,
	_ReqCallEarlyMedia			=	0x10310003,
	_ReqCallUpdateMedia			=	0x10310004,
	_ReqSipReInvite				=	0x10310005,
	_ReqSmsStart				=	0x10310006,
	_ReqGetRadioInfo			=	0x10310007,

	_ReqSendDigits				=	0x10310008,
};

