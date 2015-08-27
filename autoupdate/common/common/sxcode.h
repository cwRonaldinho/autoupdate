/*
 *	@file		sxcode.h
	@comment
		the file contains all error code in the BA System.
		however, for other constants, please refer to file "sxconst.h",
		which contains the constants unrelevant to error/warning.
*/


#pragma once 
enum SxCode
{
	ERR_SUCCESS						=	0x00,
	//------------------------
	//public code
	//0x00000001 - 0x0000FFFF
	ERR_PUB_TEST					=	0x00000001,	
	ERR_PUB_TIMEOUT					=	0x00000002,
	
	//------------------------
	//mtl code
	//0x00010001 - 0x0001FFFF
	ERR_MTL_TEST					=	0x00010001,
	
	
	//------------------------
	//ss7 code
	//0x00020001 - 0x0002FFFF
	ERR_SS7_TEST					=	0x00020001,
	ERR_SS7_SIZE_INVALID			=	0x00020002,
	ERR_SS7_MFP_NUM					=	0x00020003,
	ERR_SS7_MVP_NUM					=	0x00020004,
	ERR_SS7_DATA_NOT_REGISTER		=	0x00020005,
	ERR_SS7_ENCODER_NOT_REGISTER	=	0x00020006,
	ERR_SS7_DECODER_NOT_REGISTER	=	0x00020007,
	ERR_SS7_POSTION_ERROR			=	0x00020008,
	ERR_SS7_MFP_SIZE				=	0x00020009,
	ERR_SS7_MTL_NOT_REGISTER		=	0x0002000a,
	ERR_SS7_OPC_NOT_FOUND			=	0x0002000c,
	ERR_SS7_DPC_NOT_FOUND			=	0x0002000d,
	ERR_SS7_CIC_NOT_FOUND			=	0x0002000e,
	ERR_SS7_ROUTE_NOT_FOUND			=   0x0002000f,
	ERR_SS7_ROUTE_NOT_ACTIVE		=	0x00020010,
	ERR_SS7_LINKSET_ERROR			=	0x00020011,
	ERR_SS7_LINK_ERROR				=	0x00020012,
	ERR_SS7_BOARD_ERROR				=	0x00020013,
	ERR_SS7_PROTOCOL_NOT_SUPPORT	=	0x00020014,
	ERR_SS7_DEVICE_NOT_FOUND		=	0x00020015,
	ERR_SS7_DEVICE_STATE_ERROR		=	0x00020016,
	ERR_SS7_CONFIG_TIME_OUT			=	0x00020017,
	//------------------------
	//trunk code
	//0x00030001 - 0x0003FFFF
	ERR_TRK_ChannelNotExist			=	0x00030001,		//通道不存在
	ERR_TRK_TrunkNotExist			=	0x00030002,		//中继不存在
	ERR_TRK_WrongChannelParam		=	0x00030003,		//通道ID错误
	ERR_TRK_NoFreeChannel			=	0x00030004,		//通道分配失败	
	ERR_TRK_WrongRouteID			=	0x00030005,		//路由ID太大
	ERR_TRK_WrongChannelType		=	0x00030006,		//信令错误
	ERR_TRK_SetTimerFail			=	0x00030007,		//定时器设置错误
	ERR_TRK_NoIsrFound				=	0x00030008,		
	ERR_TRK_IsrCantConnect			=	0x00030009,		
	ERR_TRK_WrongSignalling			=	0x00030010,		
	ERR_TRK_ChannelNotConnect		=	0x00030011,

	ERR_PHN_StartToneFail			=	0x00032001,		//拨放提示音失败
	ERR_PHN_StopToneFail			=	0x00032002,		//停止提示音失败
	ERR_PHN_StartRingFail			=	0x00032003,		//振铃失败
	ERR_PHN_StopRingFail			=	0x00032004,		//停止振铃失败
	ERR_PHN_StartDtmfDetectorFail	=	0x00032005,		//检测按键失败
	ERR_PHN_StopDtmfDetectorFail	=	0x00032006,		//停止检测按键失败
	ERR_PHN_EnableBatteryFail		=	0x00032007,		//启动电源失败
	ERR_PHN_DisableBatteryFail		=	0x00032008,		//关闭电源失败
	ERR_PHN_StartSignalDetectorFail	=	0x00032009,		//检测话机状态失败
	ERR_PHN_StopSignalDetectorFail	=	0x00032010,		//停止检测话机状态失败

	ERR_PHN_ChannelNotExist			=	0x00032011,		//通道不存在
	ERR_PHN_ConnectToDspFail		=	0x00032012,		//和DSP连接失败
	ERR_PHN_DisconnectFromDspFail	=	0x00032013,		//和DSP断开连接失败
	ERR_PHN_OnHook					=	0x00032014,		//用户挂机
	ERR_PHN_NoFreeChannel			=	0x00032015,		//分配通道失败
	ERR_PHN_WrongParam				=	0x00032016,		//参数错误
	ERR_PHN_ChannelInUse			=	0x00032017,		//通道已经被分配


	//------------------------
	//voice code
	//0x00040001 - 0x0004FFFF	
	ERR_VOC_TEST					=	0x00040001,

	ERR_PVX_OpenFail				=	0x00040101,		//pv文件打开失败
	ERR_PVX_WrongIndicator			=	0x00040102,		//pv校验信息错误
	ERR_PVX_IndexNoTooBig			=	0x00040103,		//pv文件消息过多
	ERR_PVX_UsedIndexNoTooBig		=	0x00040104,		//pv文件已使用的消息数过多
	ERR_PVX_TextLenTooBig			=	0x00040105,		//pv文件文本长度错误
	ERR_PVX_CloseFail				=	0x00040106,		//pv文件关闭失败
	ERR_PVX_MsgIdTooBig				=	0x00040107,		//pv文件消息ID过大
	ERR_PVX_WrongParam				=	0x00040108,		//pv参数错误
	ERR_PVX_MemoryFail				=	0x00040109,		//pv内存出错
	ERR_PVX_NotFound				=	0x0004010A,		//pv文件查找失败
	ERR_PVX_InsertFail				=	0x0004010B,		//pv文件插入失败
	ERR_PVX_MsgNotZero				= 	0x0004010C,		//非pv文件msg不为0
	
	ERR_WAV_CodeErr					=	0x00040110,		//wave文件编码错误
	ERR_WAV_UnSupportFormat			=	0x00040111,		//不支持的WAV文件

	
	ERR_VOC_WrongPlayPara			=	0x00040120,		//放音参数错误
	ERR_VOC_PlayAssignHandleFail	=	0x00040121,		//放音assign handle错误
	ERR_VOC_VcePlayFail				=	0x00040122,		//api fail
	ERR_VOC_WrongRecordPara			=	0x00040123,		//录音参数错误
	ERR_VOC_RecordAssignHandleFail	=	0x00040124,		//录音assign handle错误
	ERR_VOC_VceRecordFail			=	0x00040125,		//api fail
	ERR_VOC_WrongCollectPara		=	0x00040126,		//收键参数错误
	ERR_VOC_VceCollectFail			=	0x00040127,		//api fail
	ERR_VOC_VceFlushFail			=	0x00040128,		//api fail
	ERR_VOC_PlayListAssignHandleFail=	0x00040129,		//放List assign handle错误
	ERR_VOC_WrongPlayListPara		=	0x0004012A,		//放List 参数错误
	ERR_VOC_AdiStopCollectFail		=	0x0004012B,		//api fail
	ERR_VOC_VceStopFail				=	0x0004012C,		//api fail
	ERR_VOC_VceCloseFail			=	0x0004012D,		//api fail
	ERR_VOC_VceDefineFail			=	0x0004012E,		//api fail
	ERR_VOC_VceSetListFail			=	0x0004012F,		//api fail
	ERR_VOC_VcePlayListFail			=	0x00040130,		//api fail
	ERR_VOC_GetSysVcehdFail			=	0x00040131,		
	ERR_VOC_VceSetGainFail			=	0x00040132,		
	ERR_VOC_VceSetSpeedFail			=	0x00040133,		
	ERR_VOC_UnsupportFormat			=	0x00040134,		

	ERR_VOC_WrongMediaCapa			=	0x00040135,		//语音能力参数错误
	ERR_VOC_NoFreeResource			=	0x00040136,		//没有空闲的语音资源
	ERR_VOC_ChannelNotExist			=	0x00040137,		//设备不存在
	ERR_VOC_FileBeingUsed			=	0x00040138,		//文件被超过一个请求访问
	ERR_VOC_NotAuthorizedOper		=	0x00040139,		//对尚未分配的语音进行操作
	ERR_VOC_InsertAllocationFail	=	0x0004013a,		
	ERR_VOC_AllocationNotExist		=	0x0004013b,		
	ERR_VOC_VoiceNotAllocated		=	0x0004013c,		//对尚未分配的语音进行操作
	ERR_VOC_SetTimerFail			=	0x0004013d,		
	ERR_VOC_PlayListFail			=	0x0004013e,		

	ERR_VOC_GetParmFail				=	0x00040140,
	ERR_VOC_SetMessageError			=	0x00040141,
	ERR_VOC_GetMessageSizeError		=	0x00040141,
	ERR_VOC_SetPositionError		=	0x00040142,

	ERR_VOC_AdiStartRecordingFail	=	0x00040143,
	ERR_VOC_AdiStopRecordingFail	=	0x00040144,
	ERR_VOC_PostMessageFail			=	0x00040145,
	ERR_VOX_CreateDirFail			=	0x00040146,
	ERR_VOX_TdxError				=	0x00040147,

	ERR_VOX_FileNotExistOrSizeTooSmall		=	0x00040148,

	FSM_VOC_MediaOperFlag			=	0x00040200,


	ERR_VMB_WrongIndex				=	0x00040201,		//voice mail index 错误
	ERR_VMB_AlreadyLocked			=	0x00040202,		//voice mail已经被其他请求lock
	ERR_VMB_CreateFile				=	0x00040203,		//voice mail创建文件错误
	ERR_VMB_FileNotExist			=	0x00040204,		//voice mail文件不存在
	ERR_VMB_WrongLocker				=	0x00040205,		//不是此id lock的mail
	ERR_VMB_NotLocked				=	0x00040206,		//voice mail尚未被lock
	ERR_VMB_MailFileNotExist		=	0x00040207,		//要添加的mail文件不存在
	ERR_VMB_IndexFull				=	0x00040208,		//index已经使用完毕
	ERR_VMB_CopyMailFileFail		=	0x00040209,		//
	ERR_VMB_MailFileTooBig			=	0x00040210,		//
	ERR_VMB_FileAlreadyExist		=	0x00040211,		//已经存在此Voicemail



	
	//------------------------
	//fax code
	//0x00050001 - 0x0005FFFF	
	ERR_FAX_TEST					=	0x00050001,
	ERR_FAX_ChannelNotExist			=	0x00050002,
	ERR_FAX_InsertAllocationFail	=	0x00050003,
	ERR_FAX_AllocationNotExist		=	0x00050004,
	ERR_FAX_CreateDirFail			=	0x00050005,
	ERR_FAX_SetTimerFail			=	0x00050006,
	ERR_FAX_NoFreeResource			=	0x00050007,
	ERR_FAX_NotAuthorizedOper		=	0x00050008,
	ERR_FAX_FaxNotAllocated			=	0x00050009,
	ERR_FAX_FileNotExistOrSizeTooSmall	=	0x0005000a,
	
	//------------------------
	//asr code
	//0x00060001 - 0x0006FFFF	
	ERR_ASR_TEST					=	0x00060001,
	ERR_ASR_FAIL					=	0x00060002,
	ERR_ASR_NORESULT				=	0x00060003,
	ERR_ASR_TOOSOON					=	0x00060004,
	ERR_ASR_TOOSOFT					=	0x00060005,
	ERR_ASR_TOONOISY				=	0x00060006,
	ERR_ASR_TOOLONG					=	0x00060007,
	ERR_ASR_TIMEOUT					=	0x00060008,
	ERR_ASR_CANCELED				=	0x00060009,
	ERR_ASR_NEEDMORE				=	0x0006000a,
	ERR_ASR_VERECOGNIZE				=	0x0006000b,
	ERR_ASR_RECOGNIZE				=	0x0006000c,
	ERR_ASR_SETGRAMMAR				=	0x0006000d,
	ERR_ASR_STARTVE					=	0x0006000e,
	ERR_ASR_STARRECOGNIZE			=	0x0006000f,
	
	//------------------------
	//tts code
	//0x00070001 - 0x0007FFFF	
	ERR_TTS_TEST					=	0x00070001,
	ERR_TTS_NOTSTARTED				=	0x00070002,
	ERR_TTS_ENGINEFAILED			=   0x00070003,
	ERR_TTS_FILENOTEXIST			=   0x00070004,
	
	//------------------------
	//call code
	//0x00080001 - 0x0008FFFF	
	ERR_CAL_TEST					=	0x00080001,
	ERR_CAL_REL_NORMAL				=	0x00080002,	//	normal clear
	ERR_CAL_REL_UNKNOWN_REASON		=	0x00080003,	//	unknown reason
	ERR_CAL_REL_UNALLOCATED_NUMBER	=	0x00080004,	//	unallocated number
	ERR_CAL_REL_BUSY				=	0x00080005,	//	called is busy
	ERR_CAL_REL_NO_ANSWER			=	0x00080006,	//	called no answer
	ERR_CAL_REL_ADDR_INCOMPLETE		=	0x00080007,	//	address incomplete
	ERR_CAL_REL_ACCESS_BARRED		=	0x00080008,	//	access is barred
	ERR_CAL_REL_LINE_NO_SERVICE		=	0x00080009,	//	line out of service
	ERR_CAL_REL_CIRCUIT_CONGESTION	=	0x0008000a,	//	circuit/trunk congest
	ERR_CAL_REL_DEVICE_CONGESTION	=	0x0008000b,	//	device congestion
	ERR_CAL_REL_CALL_FAILURE		=	0x0008000c,	//	call service failure
	ERR_CAL_REL_NORMAL_NOSPEC		=	0x0008000d,	//	normal, not specified
	ERR_PFX_NOT_DIGIT				=   0x0008000e,
	ERR_PFX_LOAD_NULL				=   0x0008000f,
	ERR_PFX_LOAD_EXIST				=   0x00080010,
	ERR_PFX_FIND_NULL				=   0x00080011,
	ERR_PFX_FIND					=	0x00080012,
	ERR_PFX_NOT_FIND				=	0x00080013,
	ERR_PFX_FIND_NEED_MORE			=	0x00080014,
	ERR_PFX_SET_NULL				=	0x00080015,
	ERR_PFX_SET_NOT_FIND			=	0x00080016,
	ERR_PFX_SET_FATAL_ERROR			=	0x00080017,
	ERR_PFX_MODIFY_ORIG_TOO_LONG	=	0x00080018,
	ERR_PFX_MODIFY_SETDATA_LONG		=	0x00080019,
	ERR_PFX_MODIFY_NULL				=	0x0008001a,
	ERR_PFX_TOO_MANY_ROUTE			=	0x0008001b,
	ERR_PFX_TOO_LONG				=	0x0008001c,
	ERR_PFX_TOO_SHORT				=	0x0008001d,
	ERR_PFX_IS_NULL					=	0x0008001e,
	ERR_PFX_ID_CALLER				=	0x0008001f,
	ERR_PFX_EXIST_CALLER_ID			=	0x00080020,
	ERR_PFX_DATA_ERROR				=	0x00080021,
	ERR_BUR_ID_ERROR				=	0x00080022,
	ERR_BUR_ID_EXIST				=	0x00080023,
	ERR_VAL_CALLER					=	0x00080024,
	ERR_VAL_CALLEE					=	0x00080025,
	ERR_VAL_BUREAU					=	0x00080026,
	ERR_VAL_ERROR					=	0x00080027,
	ERR_PFX_EXIST_CALLEE_ID			=	0x00080028,
	ERR_RUT_ID_ERROR				=	0x00080029,
	ERR_CLE_LOAD_NULL				=	0x0008002a,
	ERR_CLR_LOAD_NULL				=	0x0008002b,
	ERR_PFX_CALLEE_NOT_FIND			=	0x0008002c,
	ERR_PFX_CALLER_NOT_FIND			=	0x0008002e,
	ERR_PFX_BUREAU_NOT_FIND			=	0x0008002f,
	ERR_PFX_CALLER_NULL				=	0x00080030,
	ERR_PFX_BUREAU_NULL				=	0x00080031,
	ERR_PFX_TOO_MANY_ANALYZE		=	0x00080032,
	ERR_PFX_SET_NOT_CHANGED			=	0x00080033,
	ERR_PFX_MAY_NEED_MORE			=	0x00080034,

	ERR_DVC_NOT_FIND				=	0x00080035,
	ERR_NOD_NOT_FIND				=	0x00080036,
	ERR_CAL_CREATE_FAIL				=	0x00080037,
	ERR_ADD_CALL_DEVICE				=	0x00080038,
	ERR_CAL_NOT_FIND				=	0x00080039,
	ERR_DVC_CALL_MANY				=	0x0008003a,
	ERR_NEED_CALL_INFO				=	0x0008003b,
	ERR_TSK_CREATE_FAIL				=	0x0008003c,
	ERR_DVC_ALREADY_RECORD			=	0x0008003d,
	ERR_DVC_STOP_LIMITED			=	0x0008003e,
	ERR_DVC_ALREADY_MONITOR			=	0x0008003f,
	ERR_CAL_DEVICE_ERROR			=	0x00080040,
	ERR_OBC_PARAM_ERROR				=	0x00080041,
	ERR_OBC_NO_SESSION				=	0x00080042,
	ERR_OBC_FIRSTNOTALERTING			=	0x00080043,
	ERR_OBC_FIRSTNOTANSWERED			=	0x00080044,
	ERR_OBC_SECONDNOTALERTING			=	0x00080045,
	ERR_OBC_SECONDNOTANSWERED			=	0x00080046,

	ERR_LAD_SIZE_ERROR				=	0x00080047,
	ERR_CON_TIMEOUT					=	0x00080048,

	ERR_REROUTE_REQUEST				=	0x00080049,

	ERR_INROUTE_COPYROUTE			=	0x0008004a,

	//------------------------
	//agent code
	//0x00090001 - 0x0009FFFF	
	ERR_AGT_TEST					=	0x00090001,
	ERR_AGT_NO_SERVICE				=	0x00090002,
	ERR_AGT_NO_AGENT				=	0x00090003,
	ERR_AGT_NOT_LOGON				=	0x00090004,
	ERR_AGT_BUSY					=	0x00090005,
	ERR_AGT_NOT_READY				=	0x00090006,
	ERR_AGT_BLOCKED					=	0x00090007,
	ERR_AGT_NO_QUEUE				=	0x00090008,
	ERR_AGT_QUEUE_FULL				=	0x00090009,
	ERR_AGT_CALL_MANY				=	0x0009000a,
	ERR_AGT_ALERDY_LOGON			=	0x0009000b,
	ERR_AGT_TELNUM_LOGON			=	0x0009000c,
	ERR_AGT_PASSWORD				=	0x0009000d,
	ERR_AGT_NO_CALL					=	0x0009000e,
	ERR_AGT_NO_SERVICEMODULE		=	0x0009000f,
	ERR_AGT_STARTFLOW				=	0x00090010,
	ERR_AGT_NOTSUPPORT			=	0x00090011,
	ERR_AGT_NO_GROUP				=	0x00090012,
	ERR_AGT_TIMEOUT					=	0x00090013,
	ERR_NO_AGT_LOGIN					=	0x00090014,
	ERR_AGT_PARAERROR				=	0x00090015,
	//------------------------
	//charge code
	//0x000A0001 - 0x000AFFFF	
	ERR_CHG_TEST					=	0x000A0001,
	
	//------------------------
	//service/flow code
	//0x000B0001 - 0x000BFFFF	
	ERR_SVC_TEST					=	0x000B0001,
	ERR_SLEE_PostMessageFail		=	0x000B0002,
	ERR_SLEE_WrongFileName			=	0x000B0003,
	ERR_SLEE_MailNotOpened			=	0x000B0004,
	ERR_SLEE_MailAlreadyOpened		=	0x000B0005,
	ERR_SLEE_PromptIsEmpty			=	0x000B0006,
	ERR_SLEE_WrongParam				=	0x000B0007,
	ERR_SLEE_ConfIdNotEmpty			=	0x000B0008,
	ERR_SLEE_ConfIdEmpty			=	0x000B0009,
	ERR_SLEE_NoCall					=	0x000B000a,
	ERR_SLEE_MaxCallReach			=	0x000B000b,
	ERR_SLEE_CallTimeout			=	0x000B000c,
	ERR_SLEE_InvalidSecondCall		=	0x000B000d,
	ERR_SLEE_FlowNotLoaded			=	0x000B000e,
	ERR_SLEE_RequestPending			=	0x000B000f,
	ERR_SLEE_Timeout				=	0x000B0010,
	ERR_SLEE_RepeatAccount			=	0x000B0011,
	ERR_SLEE_PostLinkFail			=	0x000B0012,
	

	//------------------------
	//database code
	//0x000C0001 - 0x000CFFFF	
	ERR_DBS_TEST					=	0x000C0001,
	ERR_DBS_ERROR					=	0x000C0002,
	
	//------------------------
	//bill code
	//0x000D0001 - 0x000DFFFF	
	ERR_BIL_TEST					=	0x000D0001,
	ERR_TAS_DATABASE				=	0x000D0002, //数据库出错
	ERR_TAS_ACCESSDENY				=	0x000D0003,	
	ERR_TAS_ADDBILLNODE				=	0x000D0004,
	ERR_TAS_NOBILLNODE				=	0x000D0005,
	ERR_TAS_WRONGRATETYPE			=	0x000D0006,
	ERR_TAS_WrongCommand			=	0x000D0007,
	ERR_TAS_WrongBillitem			=	0x000D0008,
	ERR_TAS_CalloutAuthenFail		=	0x000D0009,
	ERR_TAS_CalloutForbidden		=	0x000D000a,
	ERR_TAS_TimeOut					=	0x000D000b,
	ERR_TAS_VACDeactivate			=	0x000D000c,
	ERR_TAS_VACCommunicationFAIL	=	0x000D000d,
	ERR_TAS_NeedRemoteAction		=	0x000D000e,	
	ERR_TAS_NumberInvalid			=	0x000D000f,
	ERR_TAS_CityInvalid				=	0x000D0010,
	ERR_TAS_SynCompleted			=	0x000D0011,
	ERR_TAS_SynInternalError		=	0x000D0012,
	ERR_TAS_NoItemExists			=	0x000D0013,
	ERR_TAS_NoVACClient				=	0x000D0014,
	ERR_TAS_FEENotSupport			=	0x000D0015,
	ERR_TAS_VACRetError				=	0x000D0016,
	ERR_TAS_NoRouteFound				=	0x000D0017,
	ERR_TAS_NoRateTypeInMap				=	0x000D0018,
	ERR_TAS_VACMSGTimeOut				=	0x000D0019,

	//obs code
	//0x000E0001 - 0x000EFFFF
	ERR_OBS_TASKINVALID				=	0x000E0001,
	ERR_OBS_NOT_RES					=	0x000E0002,
	ERR_OBS_NO_TASK					=	0x000E0003,
	ERR_OBS_NO_CALL					=	0x000E0004,
	ERR_BCM_BUSY					=	0x000E0005,
	
	//cxm code
	//0x000F0001 - 0x000FFFFF
	ERR_CXM_NoFreeResource			=	0x000F0001,
	ERR_CXM_ChannelNotExist			=	0x000F0002,
	ERR_CXM_SetTimerFail			=	0x000F0003,
	ERR_CXM_InsertChannelFail		=	0x000F0004,
	ERR_CXM_CreateChannelFail		=	0x000F0005,
	ERR_CXM_AllocateSessionFail		=	0x000F0006,
	ERR_CXM_InsertConfFail			=	0x000F0007,
	ERR_CXM_ConfNotFound			=	0x000F0008,
	ERR_CXM_SendMessageFail			=	0x000F0009,
	ERR_CXM_ParamError				=	0x000F0010,
	ERR_CXM_ConfFull				=	0x000F0011,
	ERR_CXM_ConfPartyNotFound		=	0x000F0012,
	ERR_CXM_SessionNotExist			=	0x000F0013,
	ERR_CXM_ChannelAlreadyInConf	=	0x000F0014,
	ERR_CXM_AllocateVoiceFail		=	0x000F0015,
	ERR_CXM_CreateConfFail			=	0x000F0016,
	ERR_CXM_GeneralError			=	0x000F0017,
	ERR_CXM_ConfCapabilityWrong		=	0x000F0018,
	ERR_CXM_PostMessageFail			=	0x000F0019,
	ERR_CXM_InsertResourceFail		=	0x000F001A,
	ERR_CXM_AllocateSlotFail		=	0x000F0020,
	ERR_CXM_NoMoreResource			=	0x000F0021,
	ERR_CXM_MultiChassis			=	0x000F0022,
	ERR_CXM_RemoveConfFail			=	0x000F0023,
	ERR_CXM_ConfAlreadyExist		=	0x000F0024,
	ERR_CXM_TimeOut					=	0x000F0025,
	ERR_CXM_ConfResourceNotFound	=	0x000F0026,
	ERR_CXM_ConfResourceCreateFail	=	0x000F0027,
	ERR_CXM_ConfResourceInsertFail	=	0x000F0028,
	ERR_CXM_AllocateResourceFail	=	0x000F0029,
	ERR_CXM_NoRadioFind				=	0x000F0030,
	ERR_CXM_RadioTimeOut			=	0x000F0031,


	//telephone Q code
	//0x00100001 - 0x0010FFFF
	ERR_INVALID_PARAMETER			=	0x00100001,
	ERR_ALLOCNUMBER_FAILED			=	0x00100002,
	ERR_LOGIN_FAILED				=   0x00100003,
	ERR_DUPLICATE_LOGIN				=   0x00100004,
	ERR_NO_NUMBER_FOUND				=	0x00100005,
	ERR_ROOM_FULL					=   0x00100006,
	ERR_POST_MESSAGE_FAILED			=   0x00100007,
	ERR_TARGET_INVITING				=   0x00100008,
	ERR_ROOM_NOT_READY				=   0x00100009,
	ERR_DUPLICATE_ROOMID			=   0x0010000A,
	ERR_CANT_COMMIT_NUMBER			=   0x0010000B,		// 无法确认号码，（内部错误）
	ERR_NOT_FOUND_NUMBER_LIST		=   0x0010000C,		// 没有符合条件的号码列表
	ERR_INVALID_NUMBERID			=   0x0010000D,		// 此号码不存在或者未登录
	ERR_INVALID_ROOMID				=   0x0010000E,		// 此聊天室号码不存在或者没创建好
	ERR_ALREADY_IN_ROOM				=   0x0010000F,		// 此用户已经在聊天室里面了
	ERR_WAITE_TIMEOUT				=	0x00100010,

	//sublayer code
	//0x00110001 - 0x0011FFFF
	ERR_SUB_NoMoreSlot				=	0x00110001,
	ERR_SUB_WrongParamter			=	0x00110002,
	ERR_SUB_AddContextFail			=	0x00110003,
	ERR_SUB_DelContextFail			=	0x00110004,
	ERR_SUB_CreateChannelFail		=	0x00110005,
	ERR_SUB_ChannelNotFound			=	0x00110006,
	ERR_SUB_SwiMakeConnectionFail	=	0x00110007,
	ERR_SUB_XdsMc3ListenFail		=	0x00110008,
	ERR_SUB_XdsCtListenFail			=	0x00110009,
	ERR_SUB_SwiDisableOutputFail	=	0x0011000A,
	ERR_SUB_XdsCtRlsFail			=	0x0011000B,
	ERR_SUB_CreateConfFail			=	0x0011000C,
	ERR_SUB_CnfCreateConferenceFail	=	0x0011000D,
	ERR_SUB_NoMoreConfResource		=	0x0011000E,
	ERR_SUB_CnfCloseConference		=	0x0011000F,
	ERR_SUB_ConfNotFound			=	0x00110010,
	ERR_SUB_CreateConfMemberFail	=	0x00110011,
	ERR_SUB_CnfLeaveConferenceFail	=	0x00110012,
	ERR_SUB_CnfJoinConference		=	0x00110013,
	ERR_SUB_CnfGetMemberInfo		=	0x00110014,
	ERR_SUB_XdsMc3RlsFail			=	0x00110015,
	ERR_SUB_ServiceInitFail			=	0x00110016,
	ERR_SUB_ChannelNotListen		=	0x00110017,
	ERR_SUB_NoMoreMemory			=	0x00110018,
	ERR_SUB_WrongState				=	0x00110019,

	//------------------------
	//IP code
	//0x00120001 - 0x0012FFFF	
	ERR_IP_TEST						=	0x00120001,
	ERR_IP_ChannelNotExist			=	0x00120002,
	ERR_IP_SetTimerFail				=	0x00120003,
	ERR_IP_NoIsrFound				=	0x00120004,
	ERR_IP_IsrCantConnect			=	0x00120005,
	ERR_IP_NoFreeChannel			=	0x00120006,

	//Excel code
	ERR_EXL_NO_CPU_ACTIVE				=	0x00130001,
	ERR_EXL_DEVICE_CACHE_NOT_FOUND		=	0x00130002,
	ERR_EXL_ALLOC_CACHE_FAIL			=	0x00130003,
	ERR_EXL_CHANNEL_CACHED				=	0x00130004,
	ERR_EXL_SEQ_CACHED					=	0x00130005,
	ERR_EXL_NODE_INVALID				=	0x00130006,
	ERR_EXL_MSG_NOT_REGISTER			=	0x00130007,
	ERR_EXL_CPU_NOT_CONNECTED			=	0x00130008,
	ERR_EXL_SEND_FAILED					=	0x00130009,
	ERR_EXL_NO_SEQ_FOUND				=	0x0013000a,
	ERR_EXL_NO_CACHE_FOUND				=	0x0013000b,
	ERR_EXL_NO_RESPONSE					=	0x0013000c,
	ERR_EXL_NOT_HOST_MSG				=	0x0013000d,
	ERR_EXL_TAG_NOT_EXIST				=	0x0013000e,
	ERR_EXL_AIB_NOT_EXIST				=	0x0013000f,
	ERR_EXL_TAG_TYPE_ERROR				=	0x00130010,
	ERR_EXL_TAG_NUM_INVALID				=	0x00130011,
	ERR_EXL_AIB_LEN_INVALID				=	0x00130012,
	ERR_EXL_PACK_LEN_INVALID			=	0x00130013,
	ERR_EXL_AIB_INVALID					=	0x00130014,
	ERR_EXL_NOT_SUPPORTED				=	0x00130015,
	ERR_EXL_TLV_NOT_EXIST				=	0x00130016,
	ERR_EXL_TAG_VALUE_NUM_INVALID		=	0x00130017,
	ERR_EXL_TAG_TYPE_INVALID			=	0x00130018,
	ERR_EXL_CACHER_ID_INVALID			=	0x00130019,
	ERR_EXL_CACHER_OTHER_ERROR			=	0x0013001a,
	ERR_EXL_CACHED_MSG_GETTED			=	0x0013001b,

	ERR_EXL_ERROR_BASE					=	0x00131000,


	// Gateway Code
	ERR_GW_COMMANDINVALID			=   0x00140001,
	ERR_GW_CLASSERROR				=   0x00140002,
	ERR_GW_METHODERROR				=   0x00140003,
	ERR_GW_EXECUTEERROR				=   0x00140004,
	ERR_GW_LOGICALERROR				=   0x00140005,
	ERR_GW_INVALIDNAME				=	0x00140006,
	ERR_GW_QUEUEFULL				=	0x00140007,
	ERR_GW_NotExits					=   0x00140008,
	ERR_GW_PostFailed				=   0x00140009,
	ERR_GW_RemoteNotExts			=   0x0014000A,
	ERR_GW_RemotePostFailed			=	0x0014000B,
	ERR_GW_IDNameNotMatch			=	0x0014000C,
	ERR_GW_DuplicatedName			=   0x0014000D,
	ERR_GW_NotRegister				=   0x0014000E,
	ERR_GW_MaxReached				=	0x0014000F,
	ERR_GW_IPLimited				=   0x00140010,
	ERR_GW_NotResponse               =	0x00140011,


	// SMC
	ERR_PATH_NOTEXIST				=   0x00150002,
	ERR_CREATE_UNKNOWNFAIL			=   0x00150003,
	ERR_NO_SPACE					=   0x00150004,
	ERR_CREATE_PERMISSION			=   0x00150005,
	ERR_RE_SEND						=   0x00150006,
	ERR_CHECK_FAILED				=   0x00150007,
	ERR_DB_ERROR					=   0x00150008,
	ERR_LOAD_FAIL					=   0x00150009,
	ERR_SLEE_NOTRES					=   0x0015000A,

	// ConfUnit Code
	ERR_CU_WrongParam				=   0x00160001,
	ERR_CU_InsertPartyFail			=   0x00160002,
	ERR_CU_PartyNotExist			=   0x00160003,
	ERR_CU_InsertConfFail			=   0x00160004,
	ERR_CU_ConfNotExist				=   0x00160005,
	ERR_CU_GenerateIndexFail		=   0x00160006,
	ERR_CU_SetTimerFail				=   0x00160007,
	ERR_CU_TimeOut					=	0x00160008,
	ERR_CU_PartyExist				=	0x00160009,
	ERR_CU_MasterExist				=	0x0016000A,
	ERR_CU_WrongPassword			=	0x0016000B,
	ERR_CU_PutInQueue				=	0x0016000C,
	ERR_CU_PartyIsAway				=   0x0016000D,
	ERR_CU_PartyNotAway				=   0x0016000E,

	ERR_CNF_NoResource				=	0x00160001,
	ERR_CNF_NotExists				=	0x00160002,
	ERR_CNF_Busy					=	0x00160003,
	ERR_CNF_IsMaster				=	0x00160004,
	ERR_CNF_AlreadyIn				=	0x00160005,
	ERR_CNF_ParamError				=	0x00160006,
	ERR_CNF_ConfDeleted				=	0x00160007,
	ERR_CNF_Permission				=	0x00160008,
	ERR_CNF_InternalTimeOut			=	0x00160009,
	ERR_CNF_PartyRemoved			=	0x0016000a,
	ERR_CNF_AlreadyExists			=	0x0016000b,
	ERR_CNF_ReFind					=	0x0016000c,
	ERR_CNF_FlowNoResource			=	0x0016000d,
	ERR_CNF_FlowStartFail			=	0x0016000e,
	ERR_CNF_FlowStopped				=	0x0016000f,
	ERR_CNF_LoadPartyFAIL			=	0x00160010,
	ERR_CNF_NoSleeActive			=	0x00160011,
	ERR_CNF_Deleted					=	0x00160012,

	ERR_MIX_WrongParam				=	0x00170001,
	ERR_MIX_OrigOpenFail			=	0x00170002,
	ERR_MIX_DestOpenFail			=	0x00170003,
	ERR_MIX_TooBusy					=	0x00170004,
	ERR_MIX_NotMix					=	0x00170005,
	ERR_MIX_OneRecordFAIL			=	0x00170006,
	ERR_MIX_NoVxServer				=	0x00170007,


	//vacproxy 
	ERR_VACPRROXY_STATUS_TRUE		=	0x00180001,
	ERR_VACPRROXY_STATUS_FASLE		=	0x00180002,

	
	ERR_IPT_NotAllocation			=	0x00180003,
	ERR_IPT_ChannelNotExsit			=	0x00180004,
	ERR_IPT_SetIpmEnableEvtFail		=	0x00180005,
	ERR_IPT_SetIpmDefaultFail		=	0x00180006,
	ERR_IPT_StartMediaFail			=	0x00180007,
	ERR_IPT_StopMediaFail			=	0x00180008,
	ERR_IPT_NoResouce				=	0x00180009,
	ERR_IPT_SwichIdExist			=	0x0018000a,

	//tas for vac 
	SLEE_DISCONNECT                 =   0x00190002,
	OVER_LIMIT_FEE                  =   0x00190003,
	VAC_TIME_OUT                    =   0x00190004,
    CALLEE_BILLITEM_NULL            =   0x00190005,
	PRODUCT_NOT_EXIST               =   0x00190006,

};


enum ALARM_LEVEL
{
	AL_NORMAL	= 0,
	AL_WARNING	= 1,
	AL_FATAL	= 2,
};
enum ALARM_SYSTEM_MSGID
{
	AS_MSGID_MAX_BILL_TIME				= 10060001,
	AS_MSGID_MAX_TAS_SESSION_COUNT		= 10060002,
};
