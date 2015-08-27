#include "stdafx.h"
#include <iostream>

#include <cacti/kernel/SysObj.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace cacti;

static Mutex g_mutex;

volatile static int g_counter = 0;
// usage 1 : derived from Thread class, overriding the run() method
class AppThread1 : public Thread
{
public:
	void run()
	{
		for(g_counter = 0; g_counter < 3 ; g_counter++)
		{
			g_mutex.lock();
			cout << "AppThread1: g_counter = " << g_counter << endl ;  
			g_mutex.unlock();
			Thread::sleep(1);
		}
	}
};

// usage 2 : derived from Runable interface
class AppThread2 : public Runable{
public:
	virtual void run()
	{
		while(g_counter < 3) 
		{ 
			g_mutex.lock();
			cout << "AppThread2: checking g_counter = " << g_counter << endl ;
			g_mutex.unlock();
			Thread::sleep(Timestamp(0, 100*1000)); 
		}          
		g_mutex.lock();
		cout << "AppThread2: g_counter reached, exiting" << endl ;
		g_mutex.unlock();
	}
};

void ThreadTest1(){
	AppThread1* thread1 = new AppThread1();
	thread1->start();
	Thread* thread2 = new Thread();
	thread2->start(new AppThread2(), true);
	thread1->join();
	thread2->join();
	delete thread1;
	delete thread2;
}

// usage 3 : run in a Object's function 
class App2{
	volatile int m_counter ;
public:
	App2(){ m_counter = 0 ;}
	
	void test()
	{
		Thread t1;
		t1.start(this, &App2::thread1);
		Thread  t2;
		t2.start(this, &App2::thread2);
		t1.join();
		t2.join();
	}
public:
	void thread1()
	{
		for(m_counter = 0; m_counter < 3 ; m_counter++)
		{
			g_mutex.lock();
			cout << "thread1: g_counter = " << m_counter << endl ;
			g_mutex.unlock();
			Thread::sleep(Timestamp(0, 300*1000));
		}
	}

	void thread2()
	{
		while(m_counter < 3) 
		{
			g_mutex.lock();
			cout << "thread2: checking g_counter = " << m_counter << endl;
			g_mutex.unlock();
			Thread::sleep(Timestamp(0, 100*1000));
		}
		g_mutex.lock();
		cout << "Thread1: g_counter reached, exiting" << endl;
		g_mutex.unlock();
	}
};

void ThreadTest2()
{
	App2 appObj;
	appObj.test();
}

