#ifndef __TEST_UNIT_H
#define __TEST_UNIT_H

class USService;

class TASTestUnit
{
public:
	TASTestUnit() {  }
	void setOwner(USService *owner) { m_owner = owner; }

public:
	void StartSession();
	void SnapShot();
	void PostRecognizeResult();
	void TestTlv();
	void TestTlvPressure();

private:
	USService *m_owner;
};
#endif