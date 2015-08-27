#ifndef INCLUDED_CACTI_UTILSTRING_UTIL_H
#define INCLUDED_CACTI_UTILSTRING_UTIL_H

#include <string>
#include <vector>
#include <list>
#include <climits>
#include <stdarg.h>

#include "cacti/message/TypeDef.h"
#include "cacti/message/ServiceIdentifier.h"

namespace cacti
{
	class StringUtil 
	{
	public:

		/**
		Returns a string constructed from the a format specifier
		and a va_list of arguments, analogously to vprintf(3).
		@param format the format specifier.
		@param args the va_list of arguments.
		**/
		static std::string vform(const char* format, va_list args);
		static std::vector<char> vform2(const char* format, va_list args);
		/**
		Returns a string identical to the given string but without leading
		or trailing HTABs or spaces.
		**/
		static std::string constTrim(const std::string& s);
		static void trim(std::string& s);
		/**
		splits a string into a vector of string segments based on the
		given delimiter.
		@param v The vector in which the segments will be stored. The vector
		will be emptied before storing the segments
		@param s The string to split into segments.
		@param delimiter The delimiter character
		@param maxSegments the maximum number of segments. Upon return
		v.size() <= maxSegments.  The string is scanned from left to right
		so v[maxSegments - 1] may contain a string containing the delimiter
		character.
		@return The actual number of segments (limited by maxSegments).
		**/
		static unsigned int split(std::vector<std::string>& v, 
			const std::string& s, char delimiter,
			unsigned int maxSegments = INT_MAX);

		static unsigned int split(std::vector<std::string>& v, 
			const std::string& s, const char* delimiter,
			unsigned int maxSegments = INT_MAX);
		/**
		splits a string into string segments based on the given delimiter
		and assigns the segments through an output_iterator.
		@param output The output_iterator through which to assign the string
		segments. Typically this will be a back_insertion_iterator.
		@param s The string to split into segments.
		@param delimiter The delimiter character
		@param maxSegments The maximum number of segments.
		@return The actual number of segments (limited by maxSegments).
		**/
		template<typename T> static unsigned int split(T output,
			const std::string& s, const char* delimiter,
			unsigned int maxSegments = INT_MAX) 
		{
			std::string::size_type left = 0;
			unsigned int i;
			for(i = 1; i < maxSegments; i++) 
			{
				std::string::size_type right = s.find_first_of(delimiter, left);
				if (right == std::string::npos) 
				{
					break;
				}
				*output++ = s.substr(left, right - left);
				left = right + 1;
			}

			*output++ = s.substr(left);
			return i;
		}
		
		static char* toUpper(char* str);
		static char* toLower(char* str);
		static void toUpper(std::string& str);
		static void toLower(std::string& str);
		
		//convert something to other format
		static const char* toHex(unsigned char , char* buf);
		static const char* toHex(unsigned char* buf, size_t length, std::string& hex);	
		static void fromHex(const char *buf, size_t length, std::string &oct);
		static std::string format(const char* fmt, ...);
		static std::string displayBinary(const char* bin, size_t size);
		static bool ignoreCaseCompare(const std::string& left, const std::string& right);
		static const char* formatBinary(const unsigned char *buf, int len, 
										std::string& binBuf, size_t countPerLine=INT_MAX);
		static std::string from(const std::vector<u32>& u32arr);
		static std::string from(const std::vector<u8>& u32arr);
		static void replace(std::string & content, const std::string & src, const std::string &dst);		
		static std::string encodeURL(const std::string& url);
		static std::vector<u8> getBCD(std::string value);
		static std::string StringUtil::BCD2ASC(const unsigned char *bcd, int len=0, int fmt=0);

		static std::string getKetValue(const std::string& src, const std::string& key, const std::string& delim = ",");
		template<typename Iter> 
		static std::string fromArray(Iter beg, Iter end, const char* split)
		{
			std::string str;
			Iter it = beg;
			if(it != end)
			{
				++it;
				if(it == end)
				{
					// only one element
					str += makestr(*beg);
				}
				else
				{
					Iter it2 = beg;
					// it == (it2+1)
					while(it != end)
					{
						str += makestr(*it2);
						str += split;

						++it;
						++it2;
					}
					// the last one element
					str += makestr(*it2);
				}
			}
			return str;
		}
	private:
		static inline std::string makestr(const std::string& val)
		{
			return val;
		}
	
		static inline std::string makestr(const u32 val)
		{
			char buf[16];
			sprintf(buf, "%d", val);
			return buf;
		}
	
		static inline std::string makestr(const ServiceIdentifier& sid)
		{
			return sid.toString();
		}
	};
}

#endif // INCLUDED_STRING_UTIL_H
