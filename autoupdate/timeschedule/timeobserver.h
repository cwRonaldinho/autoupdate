#pragma once
#include <stdio.h>

class TimeObserver
{
public:
	virtual void Notify() {}
};

class TestService : public TimeObserver
{
public:
	void Notify() { printf("Concentrate observer is notified\n"); }
};