#include "stdafx.h"

#include "cacti/message/TLV.h"
#include "cacti/message/TransferMessage.h"

#include <iterator>
#include <vector>
#include <iostream>

#pragma once

using namespace cacti;
using namespace std;

void message_test()
{
	TLV t1 = (u8)8;
	TLV t2 = (u32)32;

	u8 v1 = t1;
	u8 v2 = t2;
	u32 v3= t2;

	TLV t3 = t2;
	t1 = t2;

	v3= t1;	

	u32array u1(10);
	u1[5] = 5;
	copy(u1.begin(), u1.end(), ostream_iterator<int>(cout, "$"));
	cout << endl;

	TLV t4 = u1;
	
	TLV t5 = u1;

	u8array u2 = t4;
	copy(u2.begin(), u2.end(), ostream_iterator<int>(cout, "$"));
	cout << endl;
	u32array u3 = t5;
	copy(u3.begin(), u3.end(), ostream_iterator<int>(cout, "$"));
	cout << endl;

	TLV t6 = "string test";
	string str1 = t5;
	TLV t7 = str1;

	ptrarray p1;
	p1.assign(10, "");	
	p1[2] = "2test ptr array";
	p1[9] = "9test ptr array";

	copy(p1.begin(), p1.end(), ostream_iterator<char*>(cout, "$"));
	cout << endl;

	TLV t8 = p1;

	
	UserTransferMessage utm;

	utm[1] = (u8) 9;
	utm[2] = (u32)10;
	utm[2] = (u8)12;
	utm[3] = p1;

	strarray s2 = utm[3];
	strarray s3 = utm[4];

	copy(s2.begin(), s2.end(), ostream_iterator<string>(cout, "$"));
	cout << endl;
	copy(s3.begin(), s3.end(), ostream_iterator<string>(cout, "$"));
	cout << endl;
	u8 v8 = utm[1];
	u32 v9 = utm[2];

	const UserTransferMessage utm2;
	u32 v10 = utm2[1];

	strarray str2 = t8;
	ptrarray p2 = t8;
	str2[3] = "3test string array";

	TLV t9 = str2;

	copy(p2.begin(), p2.end(), ostream_iterator<string>(cout, "$"));
	cout << endl;
	copy(str2.begin(), str2.end(), ostream_iterator<string>(cout, "$"));
	cout << endl;

	strarray str3 = t9;
	copy(str3.begin(), str3.end(), ostream_iterator<string>(cout, "$"));
	cout << endl;

	u8array u7(10);
	u7[1] = 10;
	u7[5] = 15;
	u7[9] = 19;
	utm[3] = u7;
	u7 = utm[3];
}