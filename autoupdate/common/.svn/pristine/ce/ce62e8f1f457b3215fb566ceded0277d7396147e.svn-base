#include "stdafx.h"

#include <cacti/util/StringUtil.h>

#include <iterator>
#include <algorithm>

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include <cacti/config/Portability.h>

#define new DEBUG_NEW

namespace cacti
{
	std::string StringUtil::format(const char* fmt, ...)
	{
		va_list ap;
		va_start(ap, fmt);
		return vform(fmt, ap);
		va_end(ap);
	}

	std::string StringUtil::vform(const char* format, va_list args)
	{
		std::vector<char> tmp = vform2(format, args);
		return &tmp[0];
	}

	std::vector<char> StringUtil::vform2(const char* format, va_list args)
	{
		size_t size = 1024;
		std::vector<char> buffer(size);
		
		while(true) 
		{
			int n = vsnprintf(&buffer[0], size, format, args);

			// If that worked, return a string.
			if ((n > -1) && (static_cast<size_t>(n) < size)) 
			{
				return buffer;
			}

			// Else try again with more space.
			size = (n > -1) ?
				n + 1 :   // ISO/IEC 9899:1999
			size * 2; // twice the old size

			buffer.resize(size);
		}
	}

	void StringUtil::trim(std::string& s)
	{
		s = constTrim(s);
	}
	std::string StringUtil::constTrim(const std::string& s) 
	{
		// test for null string
		if(s.empty())
			return s;

		// find first non-space character
		std::string::size_type b = s.find_first_not_of(" \t");
		if(b == std::string::npos) // No non-spaces
			return "";

		// find last non-space character
		std::string::size_type e = s.find_last_not_of(" \t");

		// return the remaining characters
		return std::string(s, b, e - b + 1);
	}

	unsigned int StringUtil::split(std::vector<std::string>& v,
		const std::string& s,
		char delimiter, unsigned int maxSegments) 
	{
		char tmp[2] = {0};
		tmp[0] = delimiter;
		return split(v, s, tmp, maxSegments);
	}

	unsigned int StringUtil::split(std::vector<std::string>& v, 
		const std::string& s, 
		const char* delimiter,
		unsigned int maxSegments)
	{
		v.clear();
		std::back_insert_iterator<std::vector<std::string> > it(v);
		return split(it, s, delimiter, maxSegments);
	}

	char* StringUtil::toUpper(char* str)
	{
		size_t len = strlen(str);
		for(size_t i=0; i<len; i++)
			str[i] = ::toupper(str[i]);
		return str;
	}

	char* StringUtil::toLower(char* str)
	{
		size_t len = strlen(str);
		for(size_t i=0; i<len; i++)
			str[i] = ::tolower(str[i]);
		return str;
	}

	void StringUtil::toUpper(std::string& str)
	{
		for(size_t i=0; i<str.length(); i++)
			str[i] = ::toupper(str[i]);
	}

	void StringUtil::toLower(std::string& str)
	{
		for(size_t i=0; i<str.length(); i++)
			str[i] = ::tolower(str[i]);
	}

	const char* StringUtil::toHex(unsigned char bb, char* buf)
	{
		assert(buf);
		sprintf(buf, "%02X", bb);
		return buf;
	}

	const char* StringUtil::toHex(unsigned char* buf, size_t length, std::string& hex)
	{
		assert(buf);
		assert(length >= 0);
		hex.reserve(2*length+3);
		hex = "";
		char tmp[16];
		while(length--)
			hex += toHex(*buf++, tmp);
		return hex.c_str();
	}

	void StringUtil::fromHex(const char *buf, size_t length, std::string &oct)
	{
		char tmp[16];
		unsigned int hexVal = 0;
		for(size_t index = 0; index+1 < length; /*在循环内改变index*/)
		{
			tmp[0] = buf[index];
			tmp[1] = buf[index+1];
			tmp[2] = '\0';
			sscanf(tmp, "%02X", &hexVal);
			oct += hexVal;
			index += 2;
		}
		// append the last char if length is odd(last bit is 1)
		if((length & 1) != 0)
			oct += buf[length-1];
	}

	std::string StringUtil::displayBinary(const char* bin, size_t size) 
	{
		std::string result;
		result.reserve(size+size/2);
		for(size_t i=0; i<size; ++i) 
		{
			if(*bin == '\0')
				result += "\\0";
			else
				result += *bin;
			++bin;
		}
		return result;
	}
	
	bool StringUtil::ignoreCaseCompare(const std::string& left, const std::string& right)
	{
		std::string tmpleft = StringUtil::constTrim(left);
		std::string tmpright= StringUtil::constTrim(right);
		toUpper(tmpleft);
		toUpper(tmpright);
		return tmpleft == tmpright;
	}

	const char* StringUtil::formatBinary(const unsigned char *buf, int len, 
										 std::string& binBuf, size_t countPerLine/*=INT_MAX*/)
	{
		binBuf.reserve(binBuf.size() + 3*len + len/32 + 16);

		int i;
		char tmp[16];
		for( i=0; i<len && i<4096; i++ ) 
		{
			snprintf(tmp, sizeof(tmp), "%02x ", buf[i]);
			binBuf += tmp;

			if(((i+1) % countPerLine) == 0 )
			{
				binBuf += "\n";
			}
		}

		if( i % countPerLine )
		{
			binBuf += "\n";
		}
		return binBuf.c_str();
	}

	std::string StringUtil::from(const std::vector<u32>& u32arr)
	{
		std::string str;
		char buf[16];
		for(size_t i=0; i<u32arr.size(); ++i)
		{
			snprintf(buf, sizeof(buf)-1, "%d ", u32arr[i]);
			str += buf;
		}
		return str;
	}
	std::string StringUtil::from(const std::vector<u8>& u32arr)
	{
		std::string str;
		char buf[16];
		for(size_t i=0; i<u32arr.size(); ++i)
		{
			snprintf(buf, sizeof(buf)-1, "%d ", (int)u32arr[i]);
			str += buf;
		}
		return str;
	}
	void StringUtil::replace(std::string & content, const std::string & src, const std::string &dst) {
		std::string::size_type pos = 0;
		std::string::size_type srclen = src.size();
		std::string::size_type dstlen = dst.size();
		while( (pos = content.find(src, pos)) != std::string::npos){
			content.replace(pos, srclen, dst);
			pos += dstlen;
		}
	}

	// convert a given char to url hex form
	static std::string convert(std::string::value_type val) 
	{
		char tmp[8];
		sprintf(tmp, "%%%02X", (unsigned char)val);
		return tmp;
	}

	static bool isUnsafe(std::string::value_type compareChar)
	{
		static std::string g_URLUnsafeChars = "\"<>%\\^[]`+$,@:;!#&";

		if(std::find(g_URLUnsafeChars.begin(), g_URLUnsafeChars.end(), compareChar) != g_URLUnsafeChars.end())
			return true;
		else if(compareChar <= 32 || compareChar >=123)
			return true;
		else
			return false;

	}
	std::string StringUtil::encodeURL(const std::string& url)
	{
		std::string ret;
		ret.reserve(url.size()+64);
		for(size_t i=0; i<url.size(); ++i)
		{
			std::string::value_type ch = url[i];
			if(isUnsafe(ch))
				ret += convert(ch);
			else
				ret += ch;
		}
		return ret;
	}
	std::vector<u8> StringUtil::getBCD( std::string value)
	{

		if (value.length() % 2)
			value =std::string("0")+value;
		size_t len = value.length()/2;

		std::vector<u8> bcdArray; 
		bcdArray.resize(len);
		for( size_t i = 0 ; i < len ; i ++ )
			bcdArray[i] = ((value[2*i]- '0')<<4 ) | (u8)(value[2*i+1]- '0');

		return bcdArray;
	}

	/* 
	参数
	bcd: 需转化BCD字符串
	len: 需转化长度
	fmt: 需转化BCD的格式，即长度为奇数时，前补还是后补零
	*/
	std::string StringUtil::BCD2ASC (const unsigned char *bcd, int len, int fmt)
	{
		int i, odd;
		unsigned char c;
		std::string ret;
	
		if (bcd == NULL || len == 0)
			return ret;

		if (-1 == len)
			len = (int)strlen((const char*)bcd);

		odd = len&0x01;

		if ( odd && !fmt)
			ret += (char)(((*bcd++)&'\x0F') + '0');

		len >>= 1;

		for (i=0; i<len; i++) {
			c = *bcd++; 
			ret += (char)((c>>4) + '0');
			ret += (char)((c&'\x0F') + '0');
		}

		if ( odd && fmt)
			ret += (char)(((*bcd)>>4) + '0');

		return ret;
	}

	std::string StringUtil::getKetValue(const std::string& src, const std::string& key, const std::string& delim)
	{

		if(!delim.compare(""))
			return "";

		std::string src1 = src;
		std::string key1 = key;

		src1.insert(0,delim);
		key1.insert(0,delim);
		key1.append("=");

		size_t start = src1.find(key1);
		if( start == std::string::npos)
			return "";

		start += key1.length();
		size_t end = src1.find(delim,start);
		if( end != std::string::npos)
			return src1.substr(start,end-start);
		else
			return src1.substr(start);
	}
}
