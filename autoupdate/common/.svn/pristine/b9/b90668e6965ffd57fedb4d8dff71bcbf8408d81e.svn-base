//////////////////////////////////////////////////////////////////////////////
//FileName                : md5.cpp
//Description             : MD5报文摘要
//Version                 : 
//Date                    : 1991
//Author                  : RSA Data Security, Inc.
//Other                   : C++/object oriented translation of MD5.
//////////////////////////////////////////////////////////////////////////////

// MD5报文摘要
// 使用方法请参考文件最后部分

#ifndef INCLUDED_CACIT_UTIL_MD5_H
#define INCLUDED_CACIT_UTIL_MD5_H

#pragma once

#include <string>

namespace cacti
{
	class MD5 
	{
		// first, some types for exact size
		typedef unsigned       int  uint4;
		typedef unsigned short int  uint2;
		typedef unsigned char       uint1;

	public:
		MD5();
		// methods for controlled operation:
		void init();
		void update(unsigned char *input, unsigned int input_length);
		void final();

		void digest(unsigned char *output);
	// for private MD5 digest to change, don't use this method handy
		void changeMagicNumber(uint4 m1, uint4 m2, uint4 m3, uint4 m4);
		std::string MD5Result(uint1 [16]);//0314
		std::string MD5Encode(const char* src);   //0314
	private:
		void transform        (uint1 *buffer);  // does the real update work.  Note 
		// that length is implied to be 64.
		static void encode    (uint1 *dest, uint4 *src, uint4 length);
		static void decode    (uint4 *dest, uint1 *src, uint4 length);
		static inline void memcpy    (uint1 *dest, uint1 *src, uint4 length);
		static inline void memset    (uint1 *start, uint1 val, uint4 length);

		static inline uint4  rotate_left (uint4 x, uint4 n);
		static inline uint4  F           (uint4 x, uint4 y, uint4 z);
		static inline uint4  G           (uint4 x, uint4 y, uint4 z);
		static inline uint4  H           (uint4 x, uint4 y, uint4 z);
		static inline uint4  I           (uint4 x, uint4 y, uint4 z);
		static inline void   FF  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			uint4 s, uint4 ac);
		static inline void   GG  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			uint4 s, uint4 ac);
		static inline void   HH  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			uint4 s, uint4 ac);
		static inline void   II  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			uint4 s, uint4 ac);

	private:
		uint4 m_state[4];
		uint4 m_count[2];     // number of *bits*, mod 2^64
		uint1 m_buffer[64];   // input buffer
		uint1 m_digest[16];
		bool  m_finalized;
	};

	class MD5Ext
	{
	public:
		// do the MD5 digest with full input
		MD5Ext(const char *input, size_t length);
		// return the digest as raw data format, 
		// output must big or equal than 16 bytes
		void digest(char *output);
		// return the digest as hex string format for print etc.
		void digest(std::string &output);
	private:
		MD5 m_ctx;
	};

	void md5digest(char* output, const char* input, size_t insize);

	// RFC中第一次出现的函数接口名字
	void md5_calc(unsigned char *output, unsigned char *input, unsigned int inlen);

}

#endif // INCLUDED_CACIT_UTIL_MD5_H

/******************************************************************************
Usage                   :
1. first scenario :
// This method give you the full control of the process, 
// it fit for stream input data.
MD5 md5;
md5.init();
// read some data to "input"
md5.update(input, insize);
// read more data to "input"
md5.update(input, insize);
//...
//all data read
md5.final();
//get the digest
unsigned char output[16];
md5.digest(output);
2. second scenario :    
// This method give you an easy way to do the MD5 digest,
// all the input data must present at first.
// read all data into "input";
MD5Ext md5(input, insize);
// get the digest with original format
char output[16];
md5.digest(output);
// or get the digest with hex string format
std::string hexdigest;
md5.digest(hexdigest);
3. third scenario :
// This method give you only one API to do all things
//read all data into "input", alloc space for "output"
md5digest(output, input, insize);
******************************************************************************/
