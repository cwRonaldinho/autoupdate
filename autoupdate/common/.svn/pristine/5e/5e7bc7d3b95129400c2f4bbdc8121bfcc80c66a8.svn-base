// xzqtest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cacti/mtl/ServiceSkeleton.h"

using namespace cacti;


#define EVT_Test   1


class MyService : public ServiceSkeleton
{
	virtual void OnMessage(const ServiceIdentifier& sender, UserTransferMessage& utm)  ;
	void OnTest(const ServiceIdentifier& sender, UserTransferMessage& utm) ;

};

void MyService::OnTest(const ServiceIdentifier& sender, UserTransferMessage& utm) 
{
	return;
}



MTL_BEGIN_MESSAGE_MAP(MyService)
MTL_ON_MESSAGE(EVT_Test,OnTest)
MTL_END_MESSAGE_MAP



	
int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

