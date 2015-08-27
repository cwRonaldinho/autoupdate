-20080312
1. DbConnect Sybase版本引擎支持effectCount()实现，表示insert，update，delete等语句影响的记录数

-20070527
1. PostMessageToPeer的时候判断如果peer没有active的话，则返回上层发送失败
	日志打印peer(%d.%d) not active
2. 把更新passive connection ID的调用放在更新了active标志之后，
   现在可以正确打印出日志显示那些链接没有配置到[peers]里面
   没有配置到[peers]的对端连接会打印出anonymous service active
3. 启动参数支持不按顺序，并且支持arg=value的形式
4. windows的启动控制台，修改了标题栏的显示信息

-20080414
1. TransferClient在postMessage给接口的时候讲自己的m_dstappid放在dst.m_appref
	以便上层接口可以知道是那个client触发的消息
2. 接口Logger在程序退出时候可能导致的非法操作

-20080421
1. void TransferClient::stop() 向对端尝试发送一个断开连接的协议消息，让对端关闭此TCP连接
2. ActiveClient::threadProcess() 修改重连等待策略，讲等待时间分成几个小段等待
3. TLV& TLV::operator = (const ServiceIdentifier& sid) 修正一个内存泄露的问题
4. Startup/Main.cpp 添加WINDOWS的内存泄露调试代码
5. ActiveServer && ActiveClient 添加构造函数的缺省参数，支持指定logger的名字

-20080422
1. 修改MessageDispatcher::onSystemMessage对_EvtServiceInited消息的处理
	先给对方广播active消息，然后再向自己模块报告active。
	保证模块重启之后发送第一个消息之前，对端模块已经收到该模块的reactive事件。

-20080424
1. 修改Startup的Main.cpp stopWin32Proc方法给进程发出WM_QUIT消息3分钟之后进程不能自己结束的话，强制TerminateProcess