#pragma once
enum EventMessage
{
	//------------------------
	//MTL evt message
	//0x30000001 - 0x30010000
	_EvtStopPull					= 0x30000001,
	_EvtTimeOut						= 0x30000002,
	_EvtModuleActive				= 0x30000003,
	_EvtModuleDeactive				= 0x30000004,
	_EvtTraceData					= 0x30000005,
	_EvtConnectedToPeer				= 0x30000006,
	_EvtDisconnectedFromPeer		= 0x30000007,
	_EvtServiceInited				= 0x30000008,
	_EvtModuleRedundanceActive      = 0x30000009,
	_EvtModuleAssociate				= 0x3000000a,	//	cfgserver -> you
	_EvtModuleDeassociate			= 0x3000000b,	//	cfgserver -> you
	
	_EvtRedirectMessage				= 0x3000000c,
	_EvtClientClosed				= 0x3000000d,
	_EvtModuleInfo					= 0x3000000e,
	_EvtSnapshot					= 0x3000000f,
	_EvtHeartBeat					= 0x30000010,
	_EvtSwitchOver					= 0x30000011,
	_EvtSwitchBack					= 0x30000012,
	_EvtPingServer					= 0x30000013,
	_EvtPingFromRemote				= 0x30000014,
	_EvtMessageLoop					= 0x30000015,
	_EvtCommand						= 0x30000016,
	_EvtReportUI					= 0x30000017,
	_EvtChangeLoggerLevel			= 0x30000018,
	_EvtSendSMSTimeOut				=	0x30000019,
	_EvtReqGwServiceTimerOut		=	0x3000001a,
	//------------------------
	//config event message 
	//0x30010001 - 0x30020000
	_EvtMonitorTimeOut				=	0x30010001,
	_EvtStatisticData				=	0x30010002,
	_EvtNotifyMessage				=	0x30010003,
	
	
	//------------------------
	//call evt message
	//0x30020001 - 0x30030000
	_EvtSS7CallInit					=	0x30020001,
	_EvtSS7CallAddrComplete			=	0x30020002,
	_EvtSS7CallEstablish			=	0x30020003,
	_EvtSS7CallRelease				=	0x30020004,
	_EvtSS7CallSubAddr				=	0x30020005,
	_EvtSS7CallProceeding			=	0x30020006,

	_EvtChannelBlocked				=	0x30020011,
	_EvtChannelUnblocked			=	0x30020012,
	_EvtChannelReset				=	0x30020013,
	_EvtChannelOOS					=	0x30020014,
	_EvtChannelINS					=	0x30020015,

	_EvtCallInit					=	0x30020021,
	_EvtCallAddrComplete			=	0x30020022,
	_EvtCallEstablish				=	0x30020023,
	_EvtCallRelease					=	0x30020024,
	_EvtCallSubAddr					=	0x30020025,
	_EvtCallProceeding				=	0x30020026,
	_EvtReleaseCall					=	0x30020027,
	_EvtCallAlerting				=	0x30020028,
	_EvtRequestQueued				=	0x30020029,
	_EvtFlowStopped					=	0x3002002a,	

	_EvtIsdnCallInit				=	0x30020031,
	_EvtIsdnCallAddrComplete		=	0x30020032,
	_EvtIsdnCallEstablish			=	0x30020033,
	_EvtIsdnCallRelease				=	0x30020034,
	_EvtIsdnCallSubAddr				=	0x30020035,
	_EvtIsdnCallProceeding			=	0x30020036,
	_EvtIsdnCallReleaseComplete		=	0x30020037,
	

	_EvtReleaseChannel				=	0x30020038,

	_EvtIsdnCallAccept				=	0x30020039,

	
	//------------------------
	//media evt message
	//0x30030001 - 0x30040000
	_EvtPlayFileStarted					=	0x30030002,
	_EvtRecordFileStarted				=	0x30030003,
	_EvtCollectDigitsStarted			=	0x30030004,
	_EvtPlayListStarted					=	0x30030006,
	_EvtText2SpeechStarted				=	0x30030007,
	_EvtRecognizeStarted				=	0x30030008,	
	_EvtSetGrammar						=	0x30030009,
	_EvtVoiceData						=	0x3003000a,
	_EvtBargeIn							=	0x3003000b,
	_EvtStopRecognize					=	0x3003000c,
	_EvtPlayTextStarted					=	0x3003000d,
	_EvtTTSPing							=   0x3003000e,

	_EvtSendFaxStarted					=	0x30030101,
	_EvtRecvFaxStarted					=	0x30030102,

	_EvtConfPlayFileStarted					=	0x30030103,
	_EvtConfRecordFileStarted				=	0x30030104,




	//------------------------
	//connect evt message
	//0x30040001 - 0x30050000
	
	_EvtConfPlayStared					=	0x30040014,
	_EvtConfRecordStared				=	0x30040015,
	_EvtConfClosed						=	0x30040016,
	_EvtConfResourceReport				=   0x30040017,


	
	//------------------------
	//bill evt message
	//0x30050001 - 0x30060000
	_EvtTASCCBill						=	0x30050001,
	_EvtTASRegister						=	0x30050002,
	_EvtTASStopService					=	0x30050003,
	_EvtPutBill							=	0x30050004,
	_EvtCheckPriceRes					=	0x30050005,
	_EvtCheckPriceConfirmRes			=	0x30050006,
	_EvtTrafficPriceRes					=	0x30050007,
	_EvtTimeToTrafficPrice				=	0x30050008,
	_EvtRemoteStopService				=	0x30050009,
	_EvtTimeToCheckSession				=	0x3005000a,
	_EvtSessionExit						=	0x3005000b,
	_EvtTimeToSynInfo					=	0x3005000c,
	_EvtSynCompleted					=	0x3005000d,
	_EvtSynRestart						=	0x3005000e,
	_EvtSynTaskCompleted				=	0x3005000f,
	_EvtBillStopped						=	0x30050010,
	_EvtVacclientUsable               =	0x30050011,//added by hanfei
	_EvtVacclientUnUsable               =	0x30050012,//added by hanfei
	_ResVacclientunConect               =   0x30050013,//added by hanfei 
	_EvtCheckPriceSecondRes =0x30050015,
	_EvtCheckPriceOnceRes  = 0x30050016,
	_EvtCheckPriceMPRes   = 0x30050017,
	_EvtCheckPriceNorderRes = 0x30050018,
	_EvtCheckPriceorderRes  = 0x30050019,
	_EvtClientStauts	 =   0x3005001a,	//add cyp 
	_EvtTimeOutException	 =   0x3005001b,	//add cyp   

	//------------------------
	//cfgserver internal evt message
	//0x30100001 - 0x30110000
	
	
	//------------------------
	//sag internal evt message
	//0x30110001 - 0x30120000
	_EvtSyncCircuit						= 0x30110001,
	_EvtSyncBundle						= 0x30110002,
	_EvtServiceReady					= 0x30110003,
	_EvtSS7SwitchOver					= 0x30110004,
	_EvtSS7LinkStatus					= 0x30110005,
	_EvtSS7CheckBoard					= 0x30110006,
	_EvtSS7RouteStatus					= 0x30110007,
	_EvtSS7BoardStatus					= 0x30110008,
	_EvtSS7ResetTrunk					= 0x30110009,
	_EvtSS7CheckStatus					= 0x3011000a,
	_EvtSS7ConfigIPResult				= 0x3011000b,
	_EvtSS7ConfigNodeResult				= 0x3011000c,
	_EvtSS7ConfigClearResult			= 0x3011000d,
	_EvtSS7ConfigLinksetResult			= 0x3011000e,
	_EvtSS7ConfigLinkResult				= 0x3011000f,
	_EvtSS7ConfigRouteResult			= 0x30110010,
	_EvtSS7ConfigSaveResult				= 0x30110011,
	_EvtSS7ActiveLinkResult				= 0x30110012,
	_EvtSS7ActiveLinksetResult			= 0x30110013,
	_EvtSS7ConfigNext					= 0x30110014,

	//------------------------
	//dmu internal evt message
	//0x30120001 - 0x30130000
	_EvtPlayFileFailed					=	0x30120001,
	_EvtRecordFileFailed				=	0x30120002,
	_EvtCollectFailed					=	0x30120003,
	_EvtPlayListFailed					=	0x30120004,
	_EvtMediaOperOver					=	0x30120005,
	_EvtMediaNoNeedConnect				=	0x30120006,
	_EvtMediaTimeOut					=	0x30120007,
	_EvtPlayTextFailed					=	0x30120008,
	_EvtTTSFinish						=   0x30120009,

	_EvtAllocAsrSuccess					=	0x3012000a,
	_EvtAllocAsrFail					=	0x3012000b,
	_EvtStopVoiceData					=	0x3012000c,


	_EvtPlayDone						=	0x30120101,
	_EvtRecordDone						=	0x30120102,
	_EvtCollectDone						=	0x30120103,
	_EvtMediaFail						=	0x30120104,
	//for dmu
	_EvtSendFaxDone						=	0x30120105,
	_EvtRecvFaxDone						=	0x30120106,
	_EvtFaxErr							=	0x30120107,
	_EvtSendFaxFailed					=	0x30120108,
	_EvtRecvFaxFailed					=	0x30120109,
	_EvtSendFaxOver						=	0x3012010a,
	_EvtRecvFaxOver						=	0x3012010b,

	
	_EvtPlayFileOver					=	0x30120201,
	_EvtRecordFileOver					=	0x30120202,
	_EvtCollectOver						=	0x30120203,
	_EvtPlayListOver					=	0x30120204,

	_EvtDmuCallComplete					=	0x30120205,
	_EvtDmuCallRelease					=	0x30120206,
	_EvtDmuCallEstablish				=	0x30120207,
	_EvtDmuCallProceeding				=	0x30120208,
	_EvtDmuCallAddrComplete				=	0x30120209,
	_EvtDmuCallInit						=	0x30120210,

	_EvtPlayTextOver					=	0x30120211,

	_EvtDmuPhoneOnHook					=	0x30120221,
	_EvtDmuPhoneOffHook					=	0x30120222,
	_EvtDmuPhoneDTMF					=	0x30120223,
	_EvtDmuPhoneAddrComplete			=	0x30120224,

	
	_EvtCtaOpenSrvDone                  =   0x30123001,
	_EvtCtaCloseSrvDone                 =   0x30123002,
	_EvtCtaDestoryContextDone           =   0x30123003,
	_EvtCtaNullEvent                    =   0x30123004,
	_EvtCtaUpdateWaitobjs               =   0x30123005,
	_EvtCtaWaitTimeout                  =   0x30123006,

	_EvtAdiSignalDetectDone             =   0x30123007,
	_EvtAdiQuerySignalDone              =   0x30123008,
	_EvtAONBON                          =   0x30123009,
	_EvtAONBOFF                         =   0x3012300a,
	_EvtAOFFBON                         =   0x3012300b,
	_EvtAOFFBOFF                        =   0x3012300c,
	_EvtCOFF                            =   0x3012300d,
	_EvtCON		                        =   0x3012300e,
	_EvtAdiMfDetectDone                 =   0x3012300f,
	_EvtAdiMfDigitBegin                 =   0x30123010,
	_EvtAdiMfDigitEnd                   =   0x30123011,
	_EvtAdiTonesDone                    =   0x30123012,
	_EvtStartProtocolDone               =   0x30123013,
	_EvtStopProtocolDone                =   0x30123014,
	_EvtAdiBoardError                   =   0x30123015,
	_EvtAdiBoardEvent                   =   0x30123016,
	_EvtAdiDtmfCollectionDone           =   0x30123017,
	_EvtAdiDtmfDigitBegin               =   0x30123018,
	_EvtAdiDtmfDigitEnd                 =   0x30123019,
	_EvtAdiDtmfDetectDone               =   0x3012301a,
	_EvtAdiFskReceiveDone               =   0x3012301b,
	_EvtAdiFskSendDone                  =   0x3012301c,

	_EvtNfxSessionDone                  =   0x3012301d,
	_EvtNfxDocBegin                     =   0x3012301f,
	_EvtNfxDocEnd                       =   0x30123020,
	_EvtNfxPageBegin                    =   0x30123021,
	_EvtNfxPageEnd                      =   0x30123022,
	_EvtNfxPolled                       =   0x30123023,
	_EvtNfxProcedureInterrupt           =   0x30123024,
	_EvtNfxRemoteIdentified             =   0x30123025,
	_EvtNfxReceiveStarted               =   0x30123026,
	_EvtNfxSendStarted                  =   0x30123027,


	_EvtVcePlayDone                     =   0x30123028,
	_EvtVceRecordDone                   =   0x30123029,

	_EvtCdiAChangeOff                   =   0x3012302a,
	_EvtCdiAChangeOn                    =   0x3012302b,
	_EvtCdiStatusChange                 =   0x3012302c,
	_EvtCdiHeartBeatBack                =   0x3012302d,
	_EvtCdiHeartBeatLost                =   0x3012302e,
	_EvtCdiDtmfEnded                    =   0x3012302f,
	_EvtCdiDtmfStarted                  =   0x30123030,
	_EvtCdiFlashHook                    =   0x30123031,
	_EvtCdiOffHook                      =   0x30123032,
	_EvtCdiOnHook                       =   0x30123033,
	_EvtCdiTimerDone                    =   0x30123034,
	_EvtCdiTimerStart                   =   0x30123035,
	_EvtCdiTimerStop                    =   0x30123036,
	_EvtCdiTimerTick                    =   0x30123037,
	_EvtCdiToneDone                     =   0x30123038,

	_EvtCnfActiveTalkersChange          =   0x30123039,
	_EvtCnfCloseResourceDone            =   0x3012303a,
	_EvtCnfOpenResourceDone             =   0x3012303b,
	_EvtCnfToneDone                     =   0x3012303c,

	_EvtIsdnSendMessage                 =   0x3012303d,
	_EvtIsdnSetMsgCapture               =   0x3012303e,
	_EvtIsdnError                       =   0x3012303f,
	_EvtIsdnCallProcIN                  =   0x30123040,
	_EvtIsdnAlertIN                     =   0x30123041,
	_EvtIsdnConnIN                      =   0x30123042,
	_EvtIsdnConnCO                      =   0x30123043,
	_EvtIsdnClearIN                     =   0x30123044,
	_EvtIsdnClearCO                     =   0x30123045,
	_EvtIsdnDigitIN                     =   0x30123046,
	_EvtIsdnDigitCO                     =   0x30123047,
	_EvtIsdnUnknown                     =   0x30123048,

	_EvtNfxCannotOpenFile				=   0x30123049,
	_EVTNfxProcedureInterrupt			=	0x3012304a,

	_EvtStreamRecordDone				=	0x3012304b,
	_EvtStreamPlayDone					=	0x3012304c,
	_EvtStreamRecordRequest				=	0x3012304d,
	_EvtDigitCollected					=	0x3012304e,

	_EvtBoardReady                      =   0x30123100,

	_EvtMspConnectDone					=	0x30123101,
	_EvtMspCreateChannelDone			=	0x30123102,
	_EvtMspCreateEndpointDone			=	0x30123103,
	_EvtMspDestroyChannelDone			=	0x30123104,
	_EvtMspDestroyEndpointDone			=	0x30123105,
	_EvtMspDisableChannelDone			=	0x30123106,
	_EvtMspDisableEndpointDone			=	0x30123107,
	_EvtMspDisconnectDone				=	0x30123108,
	_EvtMspEnableChannelDone			=	0x30123109,
	_EvtMspEnableEndpointDone			=	0x3012310a,


	_EvtSipCallInit						=	0x30023110,
	_EvtSipCallAddrComplete				=	0x30023111,
	_EvtSipCallEstablish				=	0x30023112,
	_EvtSipCallRelease					=	0x30023113,
	_EvtSipGetDtmf						=	0x30023114,
	_EvtSipSamInfo						=	0x30023115,

	_EvtNetMediaData				= 	0x30023200,
	_EvtNetMediaBegin				=	0x30023201,
	_EvtNetMediaStop				=	0x30023202,
	//------------------------
	//isr internal evt message
	//0x30130001 - 0x30140000
	_EvtEndCall							=	0x30130001,
	_EvtCallInitWaitSubAddr				=	0x30130002,
	_EvtSubAddrCompleted				=	0x30130003,
	_EvtEstablishRequest				=	0x30130004,
	_EvtActiveCall						=	0x30130005,
	_EvtDeactiveCall					=	0x30130006,
	_EvtCallActivate					=	0x30130007,
	_EvtCallDeactivate					=	0x30130008,
	_EvtReRoute							=	0x30130009,
	_EvtNoAnswer						=	0x3013000a,
	_EvtNoSecondCall					=	0x3013000b,
	_EvtUnConfReq						=	0x3013000c,
	_EvtConfReq							=	0x3013000d,
	_EvtFreeConnection					=	0x3013000e,
	_EvtCallTransferred					=	0x3013000f,

	//------------------------
	//slee internal evt message
	//0x30140001 - 0x30150000
	_EvtActionMessage					=	0x30140001,
	_EvtUserTimerExpired				=	0x30140002,
	_EvtCallFunc						=	0x30140003,
	_EvtDriveFlow						=	0x30140004,
	_EvtIsrTimeout						=	0x30140005,
	_EvtTasTimeout						=	0x30140006,
	_EvtDagwTimeout						=	0x30140007,
	_EvtHookTimeout						=	0x30140008,
	_EvtMaxRunTimeReach					=	0x30140009,
	_EvtShareObjectNotified				=	0x3014000a,
	_EvtNotifyNext						=	0x3014000b,

	
	
	//------------------------
	//confserver internal evt message
	//0x30150001 - 0x30160000
	
	
	//------------------------
	//tas internal evt message
	//0x30160001 - 0x30170000	
	_EvtMonitorTimerOut					=0x30160001,
	

	//------------------------
	//ccs internal evt message
	//0x30170001 - 0x30180000	
	_EvtPlayIdEnd						=	0x30170001,
	_EvtAgentIdle						=	0x30170002,
	_EvtConsultationEnd					=	0x30170003,
	_EvtClientDestroy					=	0x30170004,
	_EvtMethodChanged					=	0x30170005,
	_EvtAgentMsg						=	0x30170007,
	_EvtGroupMsg						=	0x30170008,
	_EvtServiceMsg						=	0x30170009,
	_EvtCallCleared						=	0x3017000a,
	_EvtKickOuted						=	0x3017000b,
	_EvtBlocked							=	0x3017000c,
	_EvtUnBlocked						=	0x3017000d,
	_EvtRecordStarted					=	0x3017000e,
	_EvtRecordStopped					=	0x3017000f,
	_EvtInterventStarted				=	0x30170010,
	_EvtInterventStopped				=	0x30170011,
	_EvtListenStarted					=	0x30170012,
	_EvtListenStopped					=	0x30170013,
	_EvtServiceActivate					=	0x30170014,
	_EvtServiceDeactivate				=	0x30170015,

	_EvtMyInfo							=	0x30170016,
	_EvtAllService						=	0x30170017,
	_EvtAllGroup						=	0x30170018,
	_EvtGetQueueLen						=	0x30170019,

	_EvtServiceResult					=	0x3017001a,
	_EvtIRSGetted						=	0x3017001b,
	_EvtAgentNull						=	0x3017001c,

	_EvtAgentUnRegister				=	0x30170020,
	_EvtAgentStatusChanged		=	0x30170021,
	_EvtAgentStandBy					=	0x30170022,
	_EvtAgentCanceled				=	0x30170023,
	_EvtAgentCallDelivered			=	0x30170024,
	_EvtAgentCallConnected		=	0x30170025,
	_EvtAgentPhoneAlerting		=	0x30170026,
	_EvtAgentPhoneAnswered	=	0x30170027,
	_EvtAgentConsultationCall	=	0x30170028,
	//obs internal evt message
	//0x30180001 - 0x301900000
	_EvtGetTask							=	0x30180001,
	_EvtDialTask						=	0x30180002,
	_EvtDialWait						=	0x30180003,
	_EvtDialEnd							=	0x30180004,
	_EvtDialProgress					=	0x30180005,

	//ttsgw internal evt message
	//0x30190001 - 0x302000000
	_EvtStopProcess						=	0x30190001,

	//asrgw internal evt message
	//0x302000001 - 0x302100000
	_EvtVoiceStart						=	0x30200001,
	_EvtVoiceStop						=	0x30200002,
	_EvtStreamStart						=	0x30200003,
	_EvtStreamStop						=	0x30200004,
	_EvtStreamArrived					=	0x30200005,
	_EvtRecognizeResult					=	0x30200006,
	_EvtCancelRecognize					=	0x30200007,
	_EvtEngineAborted					=	0x30200008,
	_EvtEngineFailed					=	0x30200009,
	_EvtASRResult						=	0x3020000a,
	_EvtASRFailed						=	0x3020000b,
	_EvtExternalDone					=	0x3020000c,
	_EvtNotNeedData						=	0x3020000d,
	_EvtGetResult						=	0x3020000e,
	_EvtASRIdle							=	0x3020000f,
	_EvtEnrollEnd						=	0x30200010,
	_EvtEnrollWait						=	0x30200011,

	//msg gateway internal evt message
	//0x302100001 - 0x302200000
	_EvtServerConnected					=	0x30210001,
	_EvtClientConnected					=	0x30210002,
	_EvtServerDisconnected              =   0x30210003,
	_EvtClientDisconnected              =   0x30210004,
	_EvtReceiveMessage                  =   0x30210005,
	_EvtTransferMessage                 =   0x30210006,
	_EvtActiveTest                      =   0x30210007,
	_EvtActiveTimer                     =   0x30210008,
	_EvtCheckRecord                     =   0x30210009,
	_EvtReloadConfig					=   0x3021000A,
	_EvtForceLogon						=   0x3021000B,
	_EvtReportMessage					=	0x3021000C,
	//cxm internal evt message
	//0x302200001 - 0x302300000
	_EvtCxmActOver						=	0x30220001,
	_EvtCxmError						=	0x30220002,
	_EvtCxmTimeOut						=	0x30220003,
	_EvtCxmPrintTimeOut					=	0x30220004,


	//telephone Q event message
	//0x302300001 - 0x302400000
	_EvtTQLogoff						=	0x30230001,
	_EvtTQChatInvite					=	0x30230002,
	_EvtTQTalkFinish					=   0x30230003,
	_EvtTQFriendCome					=	0x30230004,
	_EvtTQFriendLeft					=   0x30230005,
	_EvtHasTalkResource					=   0x30230006,
	_EvtTQSetupRoom						=   0x30230007,
	_EvtTQCloseRoom						=   0x30230008,
	_EvtTQRoomTimeout					=   0x30230009,
	_EvtTQRoomBroadcast					=   0x3023000A,
	_EvtTQRoomInvite					=   0x3023000B,
	_EvtTQHasTalkRes					=   0x3023000C,
	_EvtTQWaitInviteTimeout				=   0x3023000E,
	_EvtTQKickOut						=   0x3023000F,
	_EvtSendFAILED						=   0x30230010,
	_EvtTQDeleteRoom					=   0x30230011, // 删除自建聊天室

	_EvtAlarmMessage					=   0x301B0001, // 告警事件
	_EvtUpdateAlarmServerAddress		=   0x301B0002, // alarm server 更新地址信息
	_EvtRegisterService					=   0x301B0003, // 注册服务名字
	_EvtRegisterAlarmListener			=   0x301B0004, // 注册接受告警信息
	_EvtAlarmConsole					=   0x301B0005,	// 界面日志
	_EvtUnregisterService				=   0x301B0006, // 注销服务名字

	_EvtProgress						= 	0x301C0002,
	_EvtC2TimeOut						=	0x301C0003,
	_EvtC1TimeOut						=	0x301C0004,

	//excel dmu internal message
	_EvtAddMatrix						=	0x301D0000,
	_EvtRmvMatrix						=	0x301D0001,
	_EvtXLConnect							=	0x301D0002,
	_EvtXLConnectAck						=	0x301D0003,
	_EvtXLPPLEventRequest					=	0x301D0004,
	_EvtXLPPLEventRequestAck				=	0x301D0005,
	_EvtXLReleaseChannel				=	0x301D0006,
	_EvtXLReleaseChannelAck				=	0x301D0007,
	_EvtXLOutseizeControl					=	0x301D0008,
	_EvtXLOutseizeControlAck				=	0x301D0009,
	_EvtXLPlayFile						=	0x301D000a,
	_EvtXLPlayFileAck					=	0x301D000b,
	_EvtXLRecordFile					=	0x301D000c,
	_EvtXLRecordFileAck					=	0x301D000d,
	_EvtXLStopPlay						=	0x301D000e,
	_EvtXLStopPlayAck					=	0x301D000f,
	_EvtXLStopRecord					=	0x301D0010,
	_EvtXLStopRecordAck					=	0x301D0011,
	_EvtXLPlayModify					=	0x301D0012,
	_EvtXLPlayModifyAck					=	0x301D0013,
	_EvtXLRecordModify					=	0x301D0014,
	_EvtXLResourceCreate				=	0x301D0015,
	_EvtXLResourceCreateAck				=	0x301D0016,
	_EvtXLResourceDelete				=	0x301D0017,
	_EvtXLResourceDeleteAck				=	0x301D0018,
	_EvtXLResourceConnect				=	0x301D0019,
	_EvtXLResourceConnectAck			=	0x301D001a,
	_EvtXLResourceModify				=	0x301D001b,
	_EvtXLResourceModifyAck				=	0x301D001c,
	_EvtXLConnectTone					=	0x301D001d,
	_EvtXLConnectToneAck				=	0x301D001e,
	_EvtXLDspServiceRequest				=	0x301D001f,
	_EvtXLDspServiceRequestAck			=	0x301D0020,
	_EvtXLDspServiceCancel				=	0x301D0021,
	_EvtXLDspServiceCancelAck			=	0x301D0022,
	_EvtXLSynListConfig					=	0x301D0023,
	_EvtXLSynListConfigAck				=	0x301D0024,
	_EvtXLCardPopulationQuery			=	0x301D0025,
	_EvtXLCardPopulationQueryAck		=	0x301D0026,
	_EvtXLServiceStateConfig			=	0x301D0027,
	_EvtXLServiceStateConfigAck			=	0x301D0028,
	_EvtXLResetConfiguration			=	0x301D0029,
	_EvtXLResetConfigurationAck			=	0x301D002a,
	_EvtXLClearSW						=	0x301D002b,
	_EvtXLClearSWAck					=	0x301D002c,
	_EvtXLAssignNodeID					=	0x301D002d,
	_EvtXLAssignNodeIDAck				=	0x301D002e,
	_EvtXLToneConfig					=	0x301D002f,
	_EvtXLToneConfigAck					=	0x301D0030,
	_EvtXLLoopTimingConfig				=	0x301D0031,
	_EvtXLLoopTimingConfigAck			=	0x301D0032,
	_EvtXLSS7StackConfig				=	0x301D0033,
	_EvtXLSS7StackConfigAck				=	0x301D0034,
	_EvtXLSS7LinksetConfig				=	0x301D0035,
	_EvtXLSS7LinksetConfigAck			=	0x301D0036,
	_EvtXLSS7LinkConfig					=	0x301D0037,
	_EvtXLSS7LinkConfigAck				=	0x301D0038,
	_EvtXLSS7RouteConfig				=	0x301D0039,
	_EvtXLSS7RouteConfigAck				=	0x301D003a,
	_EvtXLSS7CicConfig					=	0x301D003b,
	_EvtXLSS7CicConfigAck				=	0x301D003c,
	_EvtXLNodeStatusQuery				=	0x301D003d,
	_EvtXLNodeStatusQueryAck			=	0x301D003e,
	_EvtXLRingStatusQuery				=	0x301D003f,
	_EvtXLRingStatusQueryAck			=	0x301D0040,
	_EvtXLExnetRingConfig				=	0x301D0041,
	_EvtXLExnetRingConfigAck			=	0x301D0042,
	_EvtXLLicenseDownload				=	0x301D0043,
	_EvtXLLicenseDownloadAck			=	0x301D0044,
	_EvtXLLicenseQuery					=	0x301D0045,
	_EvtXLLicenseQueryAck				=	0x301D0046,
	_EvtXLMatrixConfig					=	0x301D0047,
	_EvtXLMatrixConfigAck				=	0x301D0048,
	_EvtXLResetMatrix					=	0x301D0049,
	_EvtXLResetMatrixAck				=	0x301D004a,
	_EvtXLPollRequest					=	0x301D004b,
	_EvtXLPollRequestAck				=	0x301D004c,
	_EvtXLPollIntervalConfig			=	0x301D004d,
	_EvtXLPollIntervalConfigAck			=	0x301D004e,
	_EvtXLFaultLogQuery					=	0x301D004f,
	_EvtXLFaultLogQueryAck				=	0x301D0050,
	_EvtXLExsNodeConfig					=	0x301D0051,
	_EvtXLExsNodeConfigAck				=	0x301D0052,
	_EvtXLBecomeActive					=	0x301D0053,
	_EvtXLBecomeActiveAck				=	0x301D0054,
	_EvtXLAssignSpanID					=	0x301D0055,
	_EvtXLAssignSpanIDAck				=	0x301D0056,
	_EvtXLAnswerModeConfig				=	0x301D0057,
	_EvtXLAnswerModeConfigAck			=	0x301D0058,
	_EvtXLDspSimmConfig					=	0x301D0059,
	_EvtXLDspSimmConfigAck				=	0x301D005a,
	_EvtXLPPLTimerConfig				=	0x301D005b,
	_EvtXLPPLTimerConfigAck				=	0x301D005c,
	_EvtXLPPLAssgin						=	0x301D005d,
	_EvtXLPPLAssginAck					=	0x301D005e,
	_EvtXLPPLCreate						=	0x301D005f,
	_EvtXLPPLCreateAck					=	0x301D0060,
	_EvtXLPPLTableDownInit				=	0x301D0061,
	_EvtXLPPLTableDownInitAck			=	0x301D0062,
	_EvtXLPPLTableDown					=	0x301D0063,
	_EvtXLPPLTableDownAck				=	0x301D0064,
	_EvtXLPPLConfig						=	0x301D0065,
	_EvtXLPPLConfigAck					=	0x301D0066,
	_EvtXLE1SpanConfig					=	0x301D0067,
	_EvtXLE1SpanConfigAck				=	0x301D0068,
	_EvtXLPPLDelete						=	0x301D0069,
	_EvtXLPPLDeleteAck					=	0x301D006a,
	_EvtXLParkChannel					=	0x301D006b,
	_EvtXLParkChannelAck				=	0x301D006c,
	_EvtXLTagConfiguration				=	0x301D006d,
	_EvtXLTagConfigurationAck			=	0x301D006e,
	_EvtXLRecordModifyAck				=	0x301D006f,
	_EvtXLGenericCardConfig				=	0x301D0070,
	_EvtXLGenericCardConfigAck			=	0x301D0071,

	_EvtXLRFSD							=	0x301D0201,
	_EvtXLCPE							=	0x301D0202,
	_EvtXLDS0StatusChanged				=	0x301D0203,
	_EvtXLPPLEventIndication			=	0x301D0204,
	_EvtXLChannelReleased				=	0x301D0205,
	_EvtXLChannelReleasedWithData		=	0x301D0206,
	_EvtXLNodeStatusReport				=	0x301D0207,
	_EvtXLRingStatusReport				=	0x301D0208,
	_EvtXLCardStatusReport				=	0x301D0209,
	_EvtXLPoll							=	0x301D020a,
	_EvtXLAlarm							=	0x301D020b,
	_EvtXLAlarmCleared					=	0x301D020c,
	_EvtXLMatrixStatusReport			=	0x301D020d,

	_EvtMatrixConnected					=	0x301D0301,
	_EvtMatrixDisconnected				=	0x301D0302,
	

	// ConfUnit Code
	_EvtTConfCreated					=	0x30250001,
	_EvtTConfDeleted					=	0x30250002,
	_EvtTConfJoined						=	0x30250003,
	_EvtTConfAwayed						=	0x30250004,
	_EvtTConfBacked						=	0x30250005,
	_EvtTConfLeaved						=	0x30250006,
	_EvtTConfModeChanged				=	0x30250007,
	_EvtTConfMoved						=	0x30250008,
	_EvtTConfLinked						=	0x30250009,
	_EvtTConfJoinReq					=	0x3025000a,
	_EvtTConfLeaveReq					=	0x3025000b,
	_EvtTConfAwayReq					=	0x3025000c,
	_EvtTConfBackReq					=	0x3025000d,
	_EvtTConfCMReq						=	0x3025000e,
	_EvtTConfDeleteReq					=	0x3025000f,
	_EvtTConfCanDelete					=	0x30250010,
	_EvtTConfBackToCreated			=	0x30250011,
	_EvtTConfCommand					=	0x30250012,
	_EvtTConfCheckCP					=	0x30250013,
	_EvtTConfMovingCP					=	0x30250014,
	_EvtTConfCheckTask					=	0x30250015,
	_EvtTConfStartFlow					=	0x30250016,
	_EvtTConfCalling						=	0x30250017,
	_EvtTConfNodeCallUpdate		=	0x30250018,

	//net manager
	_EvtPingStatus						=	0x30260001,
	_EvtSnmpValue						=	0x30260002,
	_EvtModuleStatus					=	0x30260003,
	_EvtTrunkStatus						=	0x30260004,
	_EvtLinkStatus						=	0x30260005,

	//alarm agent
	_EvtAlarmUpdateConfig				=	0x30270001,
	_EvtAlarmCreated					=	0x30270002,
	_EvtAlarmUpdated					=	0x30270003,
	_EvtAlarmAccountingStop				=	0x30270004,
	_EvtAccountingStart					=	0x30270005,
	_EvtAccountingStop					=	0x30270006,

	_EvtPingAcounting					=	0x30270007,
	_EvtPingTimeReach					=	0x30270008,
	_Evt_SGW_BModuleDaction				=	0x30270009,
	_EvtStartSpeech						=	0x30270010,
	_EvtSendNode						=	0x3027001a,
	_EvtNoCallAlarm						=	0x3027001b,
	//logmonitor
	_EvtLogMonitor						=	0x30280001,
	_EvtMonitorEnd						=	0x30280002,

	_EvtRadioCheckNormal				=	0x30300001,
	_EvtRadioState							=	0x30300002,
};
