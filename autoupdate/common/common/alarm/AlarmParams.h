#ifndef INCLUDED_ALARM_PARAMS_H
#define INCLUDED_ALARM_PARAMS_H

#include "cacti/message/TypeDef.h"

class AlarmParams
{
public: 
	AlarmParams(u32  ival)
	{
		value = cacti::StringUtil::format("%d", ival);
	}

	AlarmParams(const char* str)
	{
		value = str;
	}
	AlarmParams(const std::string& str)
	{
		value = str;
	}
public:
	std::string value;
};
typedef std::vector<AlarmParams> APV;

inline std::string u32tos(u32 val)
{
	char buff[16];
	sprintf(buff, "%d", val);
	return buff;	// return the copy of a buff, not the pointer.
}

inline std::string u32tohex(u32 val)
{
	char buff[16];
	sprintf(buff, "%X", val);
	return buff;
}

#endif
