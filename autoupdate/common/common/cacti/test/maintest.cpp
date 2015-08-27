#include "stdafx.h"
#include <iostream>

#include <cacti/test/maintest.h>

#include <cacti/logging/Logger.h>
#include <cacti/logging/ConsoleHandler.h>

#include <cacti/util/DES.h>

#include <conio.h>

#include "message_test.h"
using namespace cacti;

#include <boost/shared_ptr.hpp>

class Test
{
public:
	Test(int i)
	{
		m_i = i;
		printf("Test(%d)\n", m_i);
	}
	~Test()
	{
		printf("~Test(%d)\n", m_i);
	}
private:
	int m_i;
};

typedef boost::shared_ptr<Test> TestPtr;
int main(int argc, char* argv[])
{
	TestPtr tp(new Test(1));
	std::map<int, TestPtr> MAP;
	MAP[1] = tp;
	MAP[2] = TestPtr(new Test(2));
	MAP[1] = TestPtr(new Test(3));
	MAP[2] = TestPtr(new Test(4));

	MAP[1] = TestPtr();
	MAP[2] = TestPtr();
	message_test();
	return 0;
}
