#include "stdafx.h"
#include "TestUnit.h"
#include "utmsenderSrvservice.h"

void TASTestUnit::StartSession()
{
	UserTransferMessagePtr utm(new UserTransferMessage);
	
	// 设置请求需要的相关参数
	utm->setMessageId(_EvtStartSession);
	utm->setReq(ServiceIdentifier(4, AppPort::_apAlarmProxy, 0));
	utm->setRes(m_owner->myIdentifier());
	utm->setReturn(99);

	(*utm)[_TagFlowId] = (u32)1;
	(*utm)[_TagSessionID] = "kkkk";
		
	m_owner->postSelfMessage(utm);
}

void TASTestUnit::SnapShot()
{
	m_owner->m_sessionManager.Snapshot(NULL);
}

void TASTestUnit::PostRecognizeResult()
{
	UserTransferMessagePtr utm(new UserTransferMessage);

	// 设置请求需要的相关参数
	utm->setMessageId(_EvtRecognizeResult);
	utm->setReturn(99);

	(*utm)[_TagSessionID] = (u32)1;
	(*utm)[_TagCommand] = "ssss"; 
	//(*utm)[_TagCondition] = "ssss&|&|222";

	m_owner->postSelfMessage(utm);
}

void TASTestUnit::TestTlv()
{
	UserTransferMessage utm;

	// 队列信息
	//TKvsetTLV ktQueues[3];
	int count = 3000;

// 	TKvsetTLVArray ktQueues;
// 	for (int i=0; i<count; i++)
// 	{
// 		char tmp[100];
// 
// 		// vector方式
// 		TKvsetTLV tmpTlv;
// 		tmpTlv[1] = u32(i);
// 		sprintf_s(tmp, 100, "queuename-%d", i);
// 		tmpTlv[2] = tmp;
// 		u32array skills;
// 		skills.push_back(100);
// 		skills.push_back(101);
// 		tmpTlv[3] = (u32array)skills;
// 
// 		TKvsetTLV sub2Kt;
// 		sub2Kt[1] = u32(2000+i);
// 		sprintf_s(tmp, 100, "subkey-%d", i);
// 		sub2Kt[2] = tmp;
// 		tmpTlv.PutKvsetData(4, sub2Kt);
// 
// 		ktQueues.push_back(tmpTlv);
// 	}
// 
// 	//utm.PutKvsetArray(1, ktQueues, 3);
// 	//delete []ktQueues;
// 
// 	utm.PutKvsetArray(1, ktQueues);
// 


// 	utm.setMessageId(0x9999);
// 	m_owner->postMessage(ServiceIdentifier(1, 241, 0), utm);	// 手动把接收地址写死
}

void TASTestUnit::TestTlvPressure()
{
	int count = 10;
	for (int i=0; i<count; i++)
	{
		Sleep(100);
		TestTlv();
	}
}