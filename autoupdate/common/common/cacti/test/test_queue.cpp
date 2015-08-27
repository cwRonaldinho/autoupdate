#include "stdafx.h"
#include <iostream>

#include <cacti/kernel/SysObj.h>
#include <cacti/util/BoundedQueue.h>

using namespace std;
using namespace cacti;

static Mutex g_mutex;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class App1
{
public:
	App1()
		: m_queue(10)
	{
	}
	void test()
	{
		Thread thrd1;
		thrd1.start(this, &App1::sender);
		Thread thrd2;
		thrd2.start(this, &App1::receiver);
		thrd1.join();
		thrd2.join();
	}
private:
	BoundedQueue<int> m_queue;
	void sender() {
		int n = 0;
		while (n < 100) {
			m_queue.put(n);
			++n;
			Thread::sleep(100);
		}
		m_queue.put(-1);
	}

	void receiver() {
		int n = 0;
		do {
			m_queue.get(n);
			Thread::sleep(100);
		} while (n != -1); // -1 indicates m_end of buffer
	}
};

void QueueTest3()
{
	App1 app;
	app.test();
}
