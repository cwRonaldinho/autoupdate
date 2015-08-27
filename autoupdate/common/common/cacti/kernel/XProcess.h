#ifndef INCLUDED_PROCESS_H
#define INCLUDED_PROCESS_H

namespace cacti
{
	class XProcess
	{
	public:
#ifdef WIN32
		typedef DWORD exit_type;
#else
		typedef int exit_type;
#endif
#ifdef WIN32 
		typedef DWORD pid_t;
#endif
	public:
		// create a process and run it until it finished 
		// return 0 when success, otherwise failed
		static bool create(const std::string& cmdline, const std::string& workpath, exit_type& exitcode);
		// create a process and return
		// return 0 when success, otherwise failed
		static bool create(const std::string& cmdline, const std::string& workpath);

		// argv 是列表方式，第一个元素是进程名字，之后是进程需要的参数列表，最后跟一个NULL表示结束
		// 使用此方式比较符合linux的习惯
		static bool create(char* argv[], const std::string& workpath, exit_type& exitcode);
		static bool create(char* argv[], const std::string& workpath);

		static bool isrunning(pid_t procid);
		static bool isrunning(const std::string& procname);
	};
}
#endif // INCLUDED_PROCESS_H
