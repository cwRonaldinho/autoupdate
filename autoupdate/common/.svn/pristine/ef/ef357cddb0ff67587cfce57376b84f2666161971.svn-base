#ifndef INCLUDED_CACTI_UTIL_INIFILE_H
#define INCLUDED_CACTI_UTIL_INIFILE_H

#include <string>
#include <list>
#include <map>
#include <functional>
#include "cacti/util/StringUtil.h"
#include "cacti/message/TypeDef.h"

namespace cacti
{
	enum LINETYPE {
		TYPE_SECTION    = 1,
		TYPE_KEYVALUE   = 2,
		TYPE_COMMENT    = 3,
	};

	class LineFrame
	{
	public:
		LineFrame()
			: m_valueHasComment(false)
		{}
		LINETYPE m_type;
		std::string m_section;		// when TYPE_SECTION
		std::string m_key;			// when TYPE_KEYVALUE
		std::string m_value;		// when TYPE_KEYVALUE
		bool m_valueHasComment;		// value 字段是否包含有注释字符，用引号引住的value可以包含注释字符
		std::string m_comment; 		// all case
	};

	struct icaseless
		: public std::binary_function<std::string, std::string, bool>
	{	
		// functor for operator<
		inline bool operator()(const std::string& left, const std::string& right) const
		{	
			// apply operator< to operands
			std::string tmpleft = StringUtil::constTrim(left);
			std::string tmpright= StringUtil::constTrim(right);
			StringUtil::toLower(tmpleft);
			StringUtil::toLower(tmpright);
			
			return tmpleft < tmpright;
		}
	};

	typedef std::map<std::string, LineFrame*, icaseless> KeyValueMap; // key--->frame
	typedef std::map<std::string, KeyValueMap, icaseless> KeyValueIndex;
	class IniFile
	{		
	public:
		IniFile();
		~IniFile(void);
		bool open(const char* filename);
		bool save(const char* filename);
		void clear();
		
		bool readBool(const char* section, const char* key, bool dft);
		int readInt(const char* section, const char* key, int dft);
		double readDouble(const char* section, const char* key, double dft);
		const char* readString(const char* section, const char* key, const char* dft);

		void writeBool(const char* section, const char* key, bool val);
		void writeInt(const char* section, const char* key, int val);
		void writeDouble(const char* section, const char* key, double val);
		void writeString(const char* section, const char* key, const char* val);
		void writeHex(const char* section, const char* key, u32 val);
		
		bool hasSection(const char* section);
		bool deleteSection(const char* section);
		bool deleteKey(const char* section, const char* key);
		void readSection(const char* section, std::map<std::string, std::string>& kvset);
	private:
		void parseLine(const std::string& line);
		std::string::size_type findComment(const std::string& line);
		std::string::size_type findValueComment(const std::string& line);
		bool isCommentLine(const std::string& line);
		bool hasComment(const std::string& line);
		std::list<LineFrame*>::iterator findSection(const char* section);
		std::list<LineFrame*>::iterator findSectionEnd(const char* section);
		LineFrame* findFrame(const char*section, const char* key);
		LineFrame* findNotEmptyFrame(const char*section, const char* key);
		void makeIndex();

	private:
		std::list<LineFrame*> m_lines;
		KeyValueIndex m_lineindex;		// section --> KeyValueMap
		std::string m_prevSection;
		std::list<LineFrame*>::iterator m_prevInsertIt;
	};
}

bool    GetPrivateProfileBoolEx			(const char *section, const char *key, bool dft, const char *filename);
int     GetPrivateProfileIntEx			(const char *section, const char *key, int    dft, const char *filename);
double  GetPrivateProfileDoubleEx		(const char *section, const char *key, double dft, const char *filename);
const char* GetPrivateProfileStringEx	(const char *section, const char *key, const char  *dft, std::string& retstr, const char *filename);
void    WritePrivateProfileBoolEx		(const char *section, const char *key, bool val, const char *filename);
void    WritePrivateProfileIntEx		(const char *section, const char *key, int val, const char *filename);
void    WritePrivateProfileDoubleEx		(const char *section, const char *key, double val, const char *filename);
void    WritePrivateProfileStringEx		(const char *section, const char *key, const char* val, const char *filename);
void    WritePrivateProfileHexEx		(const char *section, const char *key, u32 val, const char *filename);


#endif // INCLUDED_CACTI_UTIL_INIFILE_H
