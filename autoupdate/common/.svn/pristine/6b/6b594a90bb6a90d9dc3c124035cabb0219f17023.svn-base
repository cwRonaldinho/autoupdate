#include "stdafx.h"

#include <cacti/config/Portability.h>
#include <cacti/util/IniFile.h>

#include <vector>

#include <sys/types.h>
#include <sys/stat.h>

#include <assert.h>
#include <time.h>

#ifndef _WIN32
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/stat.h>
	#include <sys/types.h>
#else
	#include <Windows.h>
	#include <direct.h>
	#include <io.h>
	#include <fcntl.h>
#endif

#include <cacti/util/FileSystem.h>

#define new DEBUG_NEW

namespace cacti
{
	void appendSlash(std::string& str) {
		if (str.length() > 0)
		{
			if (   str[str.length()-1] != '\\' 
				&& str[str.length()-1] != '/')
			{
				str += '/';
			}
		}
	}

	void replaceSlash(std::string& str) {
		for(size_t i=0; i<str.length(); ++i) {
			if(str[i] == '\\')
				str[i] = '/';
		}
	}

	bool FileSystem::copyfile(const char* src, const char* dst)
	{
#ifdef _WIN32
		return CopyFileA(src, dst, FALSE) == TRUE;
#else
		char buf[32768];
		int rfp = open(src, O_RDONLY);
		if(rfp == -1)
		{
			return false;
		}

		int wfp = open(dst, O_CREAT|O_WRONLY|O_TRUNC, 0666);
		if(wfp == -1)
		{
			close(rfp);
			return false;
		}

		int length;
		while( (length = read(rfp, buf, sizeof(buf))) >0 )
		{
			if(write(wfp, buf, length) != length)
			{
				close(rfp);
				close(wfp);
				unlink(dst);
				return false;
			}
		}
		close(rfp);
		close(wfp);
		return true;
#endif
	}

	std::string FileSystem::tempFile()
	{
		char buf[512];
		static int index = 0;
		sprintf(buf,"TF%d_%d_%d",(int)time(NULL),index++,rand());
		return buf;
	}

	int FileSystem::mkdir(const char *dir, int mode)
	{
#ifdef _WIN32
		return _mkdir(dir);
#else
		return ::mkdir(dir, (mode_t)mode);
#endif
	}

	bool FileSystem::isFileExist(const char* filepath)
	{
#ifdef _WIN32
		return _access(filepath, 00) == 0;
#else
		return access(filepath, F_OK) == 0;
#endif
	}

	bool FileSystem::isFileCanR(const char* filepath)
	{
#ifdef _WIN32
		return _access(filepath, 02) == 0;
#else
		return access(filepath, R_OK) == 0;
#endif
	}
	bool FileSystem::isFileCanW(const char* filepath)
	{
#ifdef _WIN32
		return _access(filepath, 04) == 0;
#else
		return access(filepath, W_OK) == 0;
#endif
	}

	bool FileSystem::isFileCanRW(const char* filepath)
	{
#ifdef _WIN32
		return _access(filepath, 06) == 0;
#else
		return access(filepath, R_OK | W_OK ) == 0;
#endif
	}
	int FileSystem::fileSize(const char* file)
	{
		struct stat stbuf;
		if (stat(file, &stbuf) == -1)
			return -1;
		else
			return stbuf.st_size;
	}

	time_t FileSystem::fileUpdateTime(const char *file)
	{
		struct stat stbuf;
		if (stat(file, &stbuf) == -1)
			return -1;
		else
			return stbuf.st_mtime;
	}
	
	std::string FileSystem::getModulePath()
	{
#ifdef _WIN32
		char modname[_MAX_PATH] = {};
		GetModuleFileNameA(NULL, modname, sizeof(modname));
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		_splitpath(modname, drive, dir, fname, ext );
		std::string basepath = drive;
		basepath += dir;
		return basepath;
#else
		char modname[MAX_PATH] = {};
		getcwd(modname, MAX_PATH);
		std::string basepath = modname;
		return basepath+"/";
#endif
	}
//not support \\xxx.xxx.xxx.xxx\abc
	bool	FileSystem::createMulDir(const std::string& dir)
	{
		std::string strDir(dir);

		if (0 == strDir.size())
		{
			return false;
		}

		if (strDir[strDir.size()-1]!='\\' || strDir[strDir.size()-1]!='/')
		{
			strDir += '/';
		}

		std::vector<std::string>	vPath;
		std::string					strTemp;
		bool						ret = true;	//0 = success; 1 = fail;
		for (int i=0; i<(int)strDir.size(); i++)
		{
			if ((strDir[i] != '/') && (strDir[i] != '\\'))
			{
				strTemp += strDir[i];
			}
			else
			{
				if (0 == i)
					vPath.push_back((std::string)("/"));
				else
					vPath.push_back(strTemp);
				strTemp += '/';
			}
		}

		std::vector<std::string>::const_iterator vIter;
		for (vIter=vPath.begin(); vIter!=vPath.end(); vIter++)
		{

			//printf("test:%s\n", (*vIter).c_str());

			if (!isFileExist((*vIter).c_str()))
			{
				if (mkdir((*vIter).c_str()) != 0)
				{
					//printf("[ERR] <createMulDir> create subPath:%s fail, fullPath:%s.\n",
					//	(*vIter).c_str(), dir.c_str());
					ret = false;
					break;
				}	
			}
			else
			{
				//printf("<createMulDir>subPath:%s is Exist, fullPath:%s.\n",
				//	(*vIter).c_str(), dir.c_str());
			}


		}

		return ret;
	}

	std::vector<AbstractPath::RoleEntity> AbstractPath::m_rols;

	size_t AbstractPath::initRols(const char* inifile)
	{
		assert(inifile);
		IniFile ini;
		ini.open(inifile);
		char tmp[64];
		RoleEntity role;
		for(int i=1; i<=MAX_ROLES; ++i) {
			snprintf(tmp, sizeof(tmp)-1, "local%d", i);
			role.m_local  = ini.readString("APC", tmp, "");
			snprintf(tmp, sizeof(tmp)-1, "import%d", i);
			role.m_import = ini.readString("APC", tmp, "");
			snprintf(tmp, sizeof(tmp)-1, "export%d", i);
			role.m_export = ini.readString("APC", tmp, "");
			if(role.m_local.empty())
			{
				continue;
			}
			else
			{
				m_rols.push_back(role);
			}
		}
		return m_rols.size();
	}
	
	std::string AbstractPath::exportPath(const char* path)
	{
		std::string exp;
		exportPath(path, exp);
		return exp;
	}
	/*
		if thePath contains theLocal then
			replace theLocal with theExport
		loop until no entities
	 */
	const char* AbstractPath::exportPath(const char* path, std::string& exp)
	{
		assert(path);
		for(size_t i=0; i<m_rols.size(); ++i)
		{
			if(strncmp(path, m_rols[i].m_local.c_str(), m_rols[i].m_local.size()) == 0
				&& strlen(path) > m_rols[i].m_local.size() )
			{
				exp = m_rols[i].m_export;
				exp.append(path+m_rols[i].m_local.size());
				break;
			}
		}
		if(exp.empty())
			exp = path;
		replaceSlash(exp);
		return exp.c_str();
	}

	std::string AbstractPath::importPath(const char* path)
	{
		std::string imp;
		exportPath(path, imp);
		return imp;
	}

	/*
		if theRemovePath contains theImport then
			replace theImport with theLocal
		loop until no entities
	*/
	const char* AbstractPath::importPath(const char* path, std::string& imp)
	{
		assert(path);
		for(size_t i=0; i<m_rols.size(); ++i)
		{
			if(strncmp(path, m_rols[i].m_import.c_str(), m_rols[i].m_import.size()) == 0
				&& strlen(path) > m_rols[i].m_import.size() )
			{
				imp = m_rols[i].m_import;
				imp.append(path+m_rols[i].m_import.size());
				break;
			}
		}

		if(imp.empty())
			imp = path;

		replaceSlash(imp);
		return imp.c_str();
	}
}
