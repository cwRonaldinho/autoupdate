#ifndef INCLUDED_CACTI_UTIL_CRC_H
#define INCLUDED_CACTI_UTIL_CRC_H

#if defined(HAS_PRAGMA_ONCE)
#pragma PRAGMA_ONCE_DECLARE
#endif

#include <string>

namespace cacti
{
	
	typedef unsigned char  u8;
	typedef unsigned short u16;
	typedef unsigned int   u32;

	class CRCITU
	{
	public:
		// CRC16
		static u16 calc(const u8 *processData, size_t dataLength);
		static u16 calc(const std::string &data)
		{
			return calc((const u8 *)data.c_str(), data.length());
		}
		static u16 s_CRC16Table[];
	};

	class CRC32
	{
	public:
		//CRC32
		static u32 calc(const u8 *processData, size_t dataLength);
		static u32 calc(const std::string &data)
		{
			return calc((const u8 *)data.c_str(), data.length());
		}
		static u32 s_CRC32Table[];
	};
}

#endif	// INCLUDED_CACTI_UTIL_CRC_H
