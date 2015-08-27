#include <stdafx.h>

#include <cacti/util/IniFile.h>

#include <cacti/util/StringUtil.h>

#include <assert.h>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace cacti 
{
#if _MSC_VER >=1400
	const std::wstring to_wcs( const std::string& src )
	{
		std::string prev = setlocale(LC_CTYPE,NULL);	// 保存原来的locale
		setlocale( LC_CTYPE, "chs" );					// 设置当前locale为chs，这在非简体中文平台上不可缺少

		size_t count1 = mbstowcs(NULL, src.c_str(), 0); // 计算新字符串长度
		std::wstring des(count1, L' ');
		size_t count2 = mbstowcs(&des[0], src.c_str(), count1 ); // 转化
		assert(count1 == count2);

		setlocale( LC_CTYPE, prev.c_str()); // 恢复到原来的locale
		return des;
	}
#endif

	IniFile::IniFile()
	{
		m_prevInsertIt = m_lines.end();
	}

	IniFile::~IniFile (void)
	{
		clear();
	}

	bool IniFile::open(const char* filename)
	{
		assert(filename);

		clear();
#if _MSC_VER >=1400
		std::wstring wcsfile = to_wcs(filename);
		std::ifstream fin(wcsfile.c_str());	
#else
		std::ifstream fin(filename);
#endif
		if(fin.is_open())
		{
			while(!fin.eof()) 
			{
				std::string tmp;
				getline(fin, tmp, '\n');
				
				if(fin.eof() && tmp.empty())
					break;

				// if *NIX file format, remove the CR
				if(tmp.length() >= 1 && tmp[tmp.length()-1] == '\r')
				{
					tmp.resize(tmp.length()-1);
				}
				parseLine(tmp);
			}
			makeIndex();
			return true;
		}
		return false;
	}

	bool IniFile::save(const char* filename)
	{
		assert(filename);
#if _MSC_VER >=1400
		std::wstring wcsfile = to_wcs(filename);
		std::ofstream fout(wcsfile.c_str());
#else
		std::ofstream fout(filename);
#endif
		if(fout.is_open())
		{
			std::list<LineFrame*>::iterator it = m_lines.begin();
			for(; it != m_lines.end(); ++it)
			{
				LineFrame* frame = *it;
				switch(frame->m_type) 
				{
				case TYPE_COMMENT:
					fout << frame->m_comment << std::endl;
					break;
				case TYPE_SECTION:
					fout << "[" << frame->m_section << "]" << frame->m_comment << std::endl;
					break;
				case TYPE_KEYVALUE:
					{
						if(frame->m_valueHasComment)
							fout << frame->m_key << "=\"" << frame->m_value << "\"" << frame->m_comment << std::endl;
						else
							fout << frame->m_key << "=" << frame->m_value << frame->m_comment << std::endl;
						
					}
					break;
				default:
					break;
				}
			}
			return true;
		}
		return false;
	}

	void IniFile::clear()
	{
		std::list<LineFrame*>::iterator it = m_lines.begin();
		for(; it != m_lines.end(); ++it)
			delete *it;
		m_lines.clear();
		m_lineindex.clear();
	}

	bool IniFile::readBool(const char* section, const char* key, bool dft)
	{
		assert(section);
		assert(key);

		LineFrame *frame = findNotEmptyFrame(section, key);
		if(frame)
		{
			if(StringUtil::ignoreCaseCompare(frame->m_value, "true"))
				return true;
			else if(StringUtil::ignoreCaseCompare(frame->m_value, "false"))
				return false;
			else
				return dft;
		}
		return dft;
	}

	int IniFile::readInt(const char* section, const char* key, int dft)
	{
		assert(section);
		assert(key);

		LineFrame *frame = findNotEmptyFrame(section, key);
		if(frame)
		{
			if ( frame->m_value.length() > 2 && frame->m_value[0] == '0' && frame->m_value[1] == 'x' )
			{
				int value = dft;
				sscanf(frame->m_value.c_str(), "0x%x", &value );
				return value;
			}
			else if ( frame->m_value.length() > 2 && frame->m_value[0] == '0' && frame->m_value[1] == 'X' )
			{
				int value = dft;
				sscanf(frame->m_value.c_str(), "0X%x", &value );
				return value;
			}
			else
			{
				int value = dft;
				sscanf(frame->m_value.c_str(), "%d", &value );
				return value;
			}
		}
		else
		{
			return dft;
		}
	}

	double IniFile::readDouble(const char* section, const char* key, double dft)
	{
		assert(section);
		assert(key);

		LineFrame *frame = findNotEmptyFrame(section, key);
		if(frame)
		{
			return atof(frame->m_value.c_str());			
		}
		else
		{
			return dft;
		}
	}

	const char* IniFile::readString(const char* section, const char* key, const char* dft)
	{
		assert(section);
		assert(key);

		LineFrame *frame = findFrame(section, key);
		if(frame)
		{
			return frame->m_value.c_str();
		}
		else
		{
			return dft;
		}
	}

	void IniFile::writeBool(const char* section, const char* key, bool val)
	{
		assert(section);
		assert(key);

		if(val)
			writeString(section, key, "true");
		else
			writeString(section, key, "false");
	}
	void IniFile::writeInt(const char* section, const char* key, int val)
	{
		assert(section);
		assert(key);

		char tmp[32];
		sprintf(tmp, "%d", val);
		writeString(section, key, tmp);
	}

	void IniFile::writeHex(const char* section, const char* key, u32 val)
	{
		assert(section);
		assert(key);

		char tmp[32];
		sprintf(tmp, "0x%X", val);
		writeString(section, key, tmp);
	}

	void IniFile::writeDouble(const char* section, const char* key, double val)
	{
		assert(section);
		assert(key);

		char tmp[32];
		sprintf(tmp, "%g", val);
		writeString(section, key, tmp);
	}

	void IniFile::writeString(const char* section, const char* key, const char* val)
	{
		assert(section);
		assert(key);
		assert(val);

		LineFrame *frame = findFrame(section, key);
		if(frame)
		{
			// already exist, update it!
			frame->m_value = val;
			frame->m_valueHasComment = hasComment(val);
		}
		else
		{	
			std::list<LineFrame*>::iterator it;
			// try cache, if failed then locate the end of this section
			if(m_prevSection == section && m_prevInsertIt != m_lines.end())
				it = m_prevInsertIt;
			else
				it = findSectionEnd(section);
			 
			// new key insert into near by section, it will speed up all write method;
			// std::list<LineFrame*>::iterator it = findSection(section);
			if(it != m_lines.end())
			{
				frame = new LineFrame;
				frame->m_type = TYPE_KEYVALUE;
				frame->m_key = key;
				frame->m_value = val;
				frame->m_valueHasComment = hasComment(val);
				m_lines.insert(++it, frame);

				m_prevSection = section;
				m_prevInsertIt = --it;
			}
			else
			{
				// no section yet! create it!
				frame = new LineFrame;
				frame->m_type = TYPE_SECTION;
				frame->m_section = section;
				m_lines.push_back(frame);

				frame = new LineFrame;
				frame->m_type = TYPE_KEYVALUE;
				frame->m_key = key;
				frame->m_value = val;
				frame->m_valueHasComment = hasComment(val);
				m_lines.push_back(frame);
			}
			// update index
			m_lineindex[section][key] = frame;
		}
	}

	bool IniFile::hasSection(const char* section)
	{
		return m_lineindex.find(section) != m_lineindex.end();
	}

	bool IniFile::deleteSection(const char* section)
	{
		m_lineindex.erase(section);

		std::list<LineFrame*>::iterator it = findSection(section);
		if(it != m_lines.end())
		{
			// delete the section at first
			delete *it;
			m_lines.erase(it++);
			// and delete all key-value until occurs next section(or end of file)
			while(it != m_lines.end() && (*it)->m_type != TYPE_SECTION)
			{
				delete *it;
				m_lines.erase(it++);
			}
			return true;
		}
		return false;
	}
	bool IniFile::deleteKey(const char* section, const char* key)
	{
		assert(section);
		assert(key);

		std::list<LineFrame*>::iterator it = findSection(section);
		if (it == m_lines.end())
		{
			return false;
		}
		// find from next line
		++it;
		for( ; it != m_lines.end(); ++it)
		{
			if((*it)->m_type == TYPE_SECTION)
			{	// occur next section
				break;
			}
			if((*it)->m_type == TYPE_KEYVALUE &&
				StringUtil::ignoreCaseCompare((*it)->m_key, key))
			{
				m_lineindex[section].erase(key);
				delete *it;
				m_lines.erase(it);
				return true;
			}
		}
		return false;
	}

	void IniFile::readSection(const char* section, std::map<std::string, std::string>& kvset)
	{
		assert(section);

		std::list<LineFrame*>::iterator it = findSection(section);
		if(it != m_lines.end())
			++it;
		for( ; it != m_lines.end(); ++it)
		{
			if((*it)->m_type == TYPE_SECTION)
			{	// occur next section
				break;
			}
			if((*it)->m_type == TYPE_KEYVALUE)
			{
				kvset.insert(std::make_pair((*it)->m_key, (*it)->m_value));
			}
		}
	}

	void IniFile::parseLine(const std::string& line) 
	{
		LineFrame* frame = new LineFrame;
		m_lines.push_back(frame);
		frame->m_type = TYPE_COMMENT; // dft is comment

		// 1. check full commented line 
		if(isCommentLine(line))
		{
			frame->m_type = TYPE_COMMENT;
			frame->m_comment = line;
			return ;
		}

		// 2. try the find section
		std::string::size_type first = line.find('[');
		std::string::size_type last = line.rfind(']');;

		if(first != std::string::npos && last != std::string::npos && first != last+1)
		{
			frame->m_section = line.substr(first+1,last-first-1);
			if(hasComment(frame->m_section))
			{
				frame->m_type = TYPE_COMMENT;
				frame->m_comment = line;
			}
			else
			{
				frame->m_type = TYPE_SECTION;
				// remember the possible comments
				frame->m_comment = line.substr(last+1);
			}
			return ;
		}

		// 3. find key-value pattern
		first = line.find('=');
		if(first == std::string::npos)
			return ;

		std::string strtmp1 = line.substr(0, first);
		if(hasComment(strtmp1))
		{
			// such case
			// something # key=value
			// we consider such meaningless line as comment
			frame->m_type = TYPE_COMMENT;
			frame->m_comment = line;
			return ;
		}

		std::string key = StringUtil::constTrim(strtmp1);
		if(key.empty())
		{
			// such case
			// = value
			frame->m_type = TYPE_COMMENT;
			frame->m_comment = line;
			return ;
		}

		frame->m_type = TYPE_KEYVALUE;
		// remember the original form
		frame->m_key = key;
		std::string strtmp2 = line.substr(first+1);
		std::string::size_type comment = findComment(strtmp2);
		std::string value = StringUtil::constTrim(strtmp2.substr(0, comment));
	
		if(value.size()>1 && value[0] == '\"' && value[value.size()-1] == '\"')
		{
			frame->m_value = value.substr(1, value.size()-2);
			frame->m_valueHasComment = true;
		}
		else
		{
			frame->m_value = value;
		}

		if(comment != std::string::npos)
			frame->m_comment = StringUtil::constTrim(strtmp2.substr(comment));

		return ;
	}

	std::string::size_type IniFile::findComment(const std::string& line)
	{
		bool inString = false;
		for(size_t i=0; i<line.size(); i++)
		{
			if(line[i] == '\"')
			{
				inString = true;
			}
			
			if(inString)
				continue;

			if(line[i] == '/')
			{
				// "//"
				if(i+1 != line.length() && line[i+1] == '/')
					return i;
			}
			else if(line[i] == '#' || line[i] == ';')
			{
				return i;
			}
		}
		return std::string::npos;

	}

	bool IniFile::isCommentLine(const std::string& line)
	{
		// skip the space and tab chars
		std::string::size_type charBegin = line.find_first_not_of("\t ");
		return charBegin != std::string::npos && charBegin == findComment(line);
	}

	bool IniFile::hasComment(const std::string& line)
	{
		return findComment(line) != std::string::npos;
	}

	std::list<LineFrame*>::iterator IniFile::findSection(const char* section)
	{
		std::list<LineFrame*>::iterator it = m_lines.begin();
		for( ; it != m_lines.end(); ++it)
		{
			LineFrame* frame = *it;
			if(frame->m_type == TYPE_SECTION 
				&& StringUtil::ignoreCaseCompare(frame->m_section, section))
			{
				break;
			}
		}
		return it;
	}
	
	std::list<LineFrame*>::iterator IniFile::findSectionEnd(const char* section)
	{
		std::list<LineFrame*>::iterator it = findSection(section);
		if(it != m_lines.end())
		{
			// find from next line
			++it;
			while(it != m_lines.end() && (*it)->m_type != TYPE_SECTION)
			{
				++it;
			}
			// the end of section is one step back of next section line(or end())
			--it;
		}
		return it;
	}

	LineFrame* IniFile::findFrame(const char*section, const char* key)
	{
		KeyValueIndex::iterator it = m_lineindex.find(section);
		if(it != m_lineindex.end())
		{
			KeyValueMap::iterator it2 = it->second.find(key);
			if(it2 != it->second.end())
				return it2->second;
		}
		return 0;
	}

	LineFrame* IniFile::findNotEmptyFrame(const char*section, const char* key)
	{
		LineFrame* frame = findFrame(section, key);
		if(frame && !frame->m_value.empty())
			return frame;
		else
			return 0;
	}

	void IniFile::makeIndex()
	{
		std::list<LineFrame*>::iterator it = m_lines.begin();
		LineFrame* frame = NULL;
		KeyValueIndex::iterator kvmit = m_lineindex.end();
		while(it != m_lines.end())
		{
			frame = *it;
			++it;

			// 找到了一个SECTION的开始
			if(frame->m_type == TYPE_SECTION)
			{
				kvmit = (m_lineindex.insert(std::make_pair(frame->m_section, KeyValueMap()))).first;
			}
			else if(frame->m_type == TYPE_KEYVALUE && kvmit != m_lineindex.end())
			{
				(kvmit->second)[frame->m_key]= frame;
			}
		}
	}
}

bool GetPrivateProfileBoolEx(const char *section, const char *key, bool dft, const char *filename)
{
	cacti::IniFile ini;
	ini.open(filename);
	return ini.readBool(section, key, dft);
}

int GetPrivateProfileIntEx(const char *section, const char *key, int dft, const char *filename)
{
	cacti::IniFile ini;
	ini.open(filename);
	return ini.readInt(section, key, dft);
}
double GetPrivateProfileDoubleEx(const char *section, const char *key, double dft, const char *filename)
{
	cacti::IniFile ini;
	ini.open(filename);
	return ini.readDouble(section, key, dft);
}

const char* GetPrivateProfileStringEx(const char *section, const char *key, const char  *dft, std::string& retstr, const char *filename)
{
	cacti::IniFile ini;
	ini.open(filename);
	retstr = ini.readString(section, key, dft);
	return retstr.c_str();
}

void WritePrivateProfileBoolEx(const char *section, const char *key, bool val, const char *filename)
{
	cacti::IniFile ini;
	ini.open(filename);
	ini.writeBool(section, key, val);
	ini.save(filename);
}

void WritePrivateProfileIntEx(const char *section, const char *key, int val, const char *filename)
{
	cacti::IniFile ini;
	ini.open(filename);
	ini.writeInt(section, key, val);
	ini.save(filename);
}
void WritePrivateProfileDoubleEx(const char *section, const char *key, double val, const char *filename)
{
	cacti::IniFile ini;
	ini.open(filename);
	ini.writeDouble(section, key, val);
	ini.save(filename);
}
void WritePrivateProfileStringEx(const char *section, const char *key, const char *val, const char *filename)
{
	cacti::IniFile ini;
	ini.open(filename);
	ini.writeString(section, key, val);
	ini.save(filename);
}

void  WritePrivateProfileHexEx(const char *section, const char *key, u32 val, const char *filename)
{
	cacti::IniFile ini;
	ini.open(filename);
	ini.writeHex(section, key, val);
	ini.save(filename);
}


