#ifndef INCLUDED_CACTI_UTIL_FILESYSTEM_H
#define INCLUDED_CACTI_UTIL_FILESYSTEM_H

#include <string>
#include <vector>

#ifndef MAX_PATH
	#define	MAX_PATH	4096
#endif
#ifndef WIN32
	#define	_O_RDWR		O_RDWR
	#define	_O_RDONLY	O_RDONLY
	#define	_O_WRONLY	O_WRONLY
#endif

namespace cacti
{
	class FileSystem
	{
	public:
		static bool copyfile(const char* src, const char* dst);
		static std::string tempFile();
		static int fileSize(const char* file);
		static time_t fileUpdateTime(const char *file);
		static int  mkdir(const char* dir, int mode = 0777);
		static bool isFileExist(const char* filepath);
		static bool isFileCanR(const char* filepath);
		static bool isFileCanW(const char* filepath);
		static bool isFileCanRW(const char* filepath);
		static std::string getModulePath();
		static bool createMulDir(const std::string& dir);
	};

	class AbstractPath
	{
	public:
		static size_t initRols(const char* inifile);
		static std::string exportPath(const char* path);
		static const char* exportPath(const char* path, std::string& exp);
		
		static std::string importPath(const char* path);
		static const char* importPath(const char* path, std::string& imp);
	private:
		static const int MAX_ROLES = 100;
		struct RoleEntity{
			friend class AbstractPath;
			std::string m_local;		// 本地路径头部
			std::string m_import;		// 输出替换头
			std::string m_export;		// 输入检查头
		};
		static std::vector<RoleEntity> m_rols;
	};
}

#endif // INCLUDED_CACTI_UTIL_FILESYSTEM_H

/*
 * 规则说明书：
 * 1、初始化：给出一个配置文件进行初始化
 *	[APC]
 *		local1=h:
 *		export1=file:	# empty means no change
 *		import1=file:	# empty means no change
 *
 *		local2=d:
 *		export2=			# empty means no change
 *		import2=/media/cdrom	# empty means no change
 *		#...
 *
 * 2、需要发送到其他service的路径必须使用AbstractPath.export转换成输出的路径
 * 3、从其他service得到的路径必须试验AbstractPath.import还原本地路径
 * 4、支持仅单一模块的修改，例如tssg(windows)使用本机制，dmu(linux)不使用，则配置如下：
 *		[APC] 
 *			local1=h:
 *			export1=/mnt/h
 *			import1=/mnt/h
 *		这样，tssg发出一个路径“h:\ttscache\abc.vox (即：local1\ttscache\abc.vox)”给dmu的时候，路径被转换成
 *		“/mnt/h/ttscache/abc.vox (即：local1 --> export1)”
 *		dmu返回这个路径通知tssg删除文件“/mnt/h/ttscache/abc.vox”的时候，
 *		tssg将路径还原为“h:\ttscache/abc.vox (即：import1 --> local1)”
 * 5、路径中的“\”全部统一使用“/”
 * 6、三方通信的方案A(windows)， B（windows），C（linux），文件服务器到windows的假设映射到H盘，到linux的mount到/mnt/h，
 *    A同时需要把一个路径发送给B和C，则
 *  A配置如下
 *	[APC]
 *		local[1]=h:
 *		export]=file:
 *		import1=file:
 *	B配置如下
 *	[APC]
 *		local1=h:
 *		export1=file:
 *		import1=file:
 *	C配置如下
 *	[APC]
 *		local1=/mnt/H
 *		export1=file:
 *		import1=file:
 * 7、当全部模块都使用本方案之后，只需要两个配置，一个windows，一个linux即可，统一有Startup初始化。
 * 8、可以支持多个配置规则同时存在，按照先后顺序匹配成功即选择此配置，最多支持100个配置项目
 * 9、export 算法
		if thePath contains theLocal then
			replace theLocal with theExport
		loop until no entities
	  import 算法
		if theRemovePath contains theImport then
			replace theImport with theLocal
		loop until no entities
 */
