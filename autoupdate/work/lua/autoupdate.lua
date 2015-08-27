-----------------------------------------------------------
-- 自动更新脚本
-----------------------------------------------------------

-- 设置导入模块路径
local p = "./lua/"
local m_package_path = package.path
package.path = string.format("%s;%s?.lua;%s?/init.lua", m_package_path, p, p)
--package.cpath = string.format("%s;%s?.dll", m_package_path, p, p)
--C_PrintLog(package.cpath)

-----------------------------------------------------------
http=require("socket.http");
require("twutil")

-- 全局宏
-- 本模块配置文件
SYSTEM_CONFIG = "./autoupdate.ini"
-- 主exe配置文件
MAIN_CLIENT_CONFIG = "../Update_cfg.dat"


-----------------------------------------------------------
-- 全局变量
-- 请求url
g_dstUrl = ""
-- 客户端id
g_key = ""

-- 查询版本号时间间隔，单位秒
g_checkVersionInterval = 900

-- 本次需要下载列表 key:id value:{"sizeSourceFile"=sizeSourceFile, "path"=path}
g_downloadList = {}
g_needDownLoadCount = 0      -- 需要下载文件总数
g_downloadedCount = 0        -- 已下载文件数

g_savedPath = ".\\downloaded\\"
g_unrarPath = g_savedPath -- .. "unrar\\"
g_mediaPath = "../video/"
saveFilePath = ""            -- 下载文件到本地的全路径
sizeSourceFile = 0           -- 源文件大小
g_localFileName = ""         -- 下载到本地的文件名
-----------------------------------------------------------
-- 功能函数

-- 发送http post数据
-- 请求参数:
--   url: 路径
--   body: 包体
-- 返回:
--   回复内容，异常时返回空
function HttpPost(url, body)
	--C_PrintLog("http post: " .. url .. "?" .. body)

	local request_body = body
	local response_body = {}

	local res, code, response_headers = http.request{
	  url = url,
	  method = "POST",
	  headers =
		{
			["Content-Type"] = "application/x-www-form-urlencoded";
			["Content-Length"] = #request_body;
		},
		source = ltn12.source.string(request_body),
		sink = ltn12.sink.table(response_body),
	}


	if 200 == code then
		if type(response_body) == "table" then
			--C_PrintLog(response_body[1])
			return response_body[1]
		else  -- 当服务端地址不能访问时，走该分支
			C_PrintLog("[ERROR] http respone content isn't table value:" .. type(response_body))
			--C_PrintLog(response_body)
		end
	else
		C_PrintLog('http response code: ' .. code)
	end

	return ""
end

--
function Sleep(n)
   if n > 0 then os.execute("ping -n " .. tonumber(n + 1) .. " localhost > NUL") end
end

-----------------------------------------------------------
-- 主逻辑
function BEGIN()
	-- 读取服务端地址
	C_ReadConfig("port", "httpurl", MAIN_CLIENT_CONFIG)
	g_dstUrl = _Result
	if "" == g_dstUrl then
		C_PrintLog("[ERROR] server url nil, exit...")
		return
	end
	C_PrintLog("server url: " .. g_dstUrl)

	-- 读取客户端ID
	C_ReadConfig("port", "key", MAIN_CLIENT_CONFIG)
	g_key = _Result
	if "" == g_key then
		C_PrintLog("[ERROR] client id is nil, exit...")
		return
	end
	C_PrintLog("client id: " .. g_key)

	-- 读取检查更新时间间隔
	C_ReadConfig("system", "checkVersionInterval", SYSTEM_CONFIG)
	g_checkVersionInterval = tonumber(_Result)   -- 注意：如未配置，左值为nil
	if nil == g_checkVersionInterval or g_checkVersionInterval < 1 then
		g_checkVersionInterval = 900
	end
	C_PrintLog("check version interval: " .. g_checkVersionInterval)


	-- 判断下载目录是否存在
	if 0 ~= os.execute("cd " .. g_savedPath) then
		-- 创建下载目录
		os.execute("md " .. g_savedPath)
	end

	-- 判断解压目录是否存在
	if 0 ~= os.execute("cd " .. g_unrarPath) then
		-- 创建下载目录
		os.execute("md " .. g_unrarPath)
	end

	stWaitForCheckVersion()
end

-- 等待下一次查询版本
function stWaitForCheckVersion()
	C_PrintLog("stWaitForCheckVersion:")

	-- 进入该状态，表示需要重置参数
	g_downloadList = {}
	g_needDownLoadCount = 0      -- 需要下载文件总数
	g_downloadedCount = 0        -- 已下载文件数

	C_StartTimer(g_checkVersionInterval)
	C_SetNextState("_EvtTimeOut", -1, "stCheckVersion")
end

-- 辅助函数：往全局文件列表中添加一条文件信息
function AddFile(line)
	local path, sizeSourceFile = string.match(line, "(.+),(%d+)")
	if nil== path or ""==path then
		C_PrintLog("file info error: " .. line)
		return
	end

	local fileInfo = {["path"] = path, ["sizeSourceFile"] = sizeSourceFile}
	g_downloadList[g_needDownLoadCount] = fileInfo
	g_needDownLoadCount = g_needDownLoadCount + 1
end

-- 显示待下载列表
function ShowDownloadLiest()
	C_PrintLog("--------- download list ---------")
	local i = 0
	while i < g_needDownLoadCount do
		C_PrintLog(g_downloadList[i]["path"] .. "," .. g_downloadList[i]["sizeSourceFile"])
		i = i + 1
	end
end


-- 辅助函数: 解析出更新文件列表信息
function ParseNeedUpdateFileList(str)
	local beginPos = 1
	local commaPos = string.find(str, ";", beginPos)

	local line = ""
	while(commaPos ~= nil) do
		--C_PrintLog(commaPos)
		line = string.sub(str, beginPos, commaPos)

		-- 模拟
		--line = "http://demo.zksr.cn/upload/priceTag/ccc250.mp4,255929468"

		AddFile(line)
		--C_PrintLog(line)
		beginPos = commaPos + 1
		commaPos = string.find(str, ";", beginPos)
	end

	-- 最后一个文件或者唯一的一个文件
	line = string.sub(str, beginPos)

	-- 模拟
	--line = "http://demo.zksr.cn/upload/priceTag/bbb.wmv,32069384"

	--C_PrintLog(line)
	AddFile(line)

	ShowDownloadLiest()
end

-- 检查是否有更新
function stCheckVersion()
	C_PrintLog("stCheckVersion:")

	-- 发送http，分析结果
	local body = "method=checkVersion&key=" .. g_key
	local resp = HttpPost(g_dstUrl, body)

	if nil == resp then
		C_PrintLog("response content is nil")
		-- 视为查询失败
		C_StartTimer(1)
		C_SetNextState("_EvtTimeOut", -1, "stWaitForCheckVersion")

		return
	end
	
	C_PrintLog("check version resp: " .. resp)

	-- 解析结果
	local status, filelist = string.match(resp, "(%d+)|(.*)")
	if nil == status or nil == filelist then
		C_PrintLog('error response: ' .. resp)
		C_StartTimer(1)
		C_SetNextState("_EvtTimeOut", -1, "stWaitForCheckVersion")
		return
	end

	-- 判断状态
	local nStatus = tonumber(status)
	if 0 == nStatus then       -- 没有更新
		C_StartTimer(1)
		C_SetNextState("_EvtTimeOut", -1, "stWaitForCheckVersion")
	elseif 1 == nStatus then  -- 有更新，并且可以立即下载
		-- 解析下载列表
		ParseNeedUpdateFileList(filelist)
		C_StartTimer(1)
		C_SetNextState("_EvtTimeOut", -1, "stDownload")
	elseif 2 == nStatus then  -- 有更新，但不能立即下载
		-- 解析下载列表
		ParseNeedUpdateFileList(filelist)
		C_StartTimer(1)
		C_SetNextState("_EvtTimeOut", -1, "stCheckForDownload")
	else
		C_PrintLog("Unknown status " .. status)
		C_StartTimer(1)
		C_SetNextState("_EvtTimeOut", -1, "stWaitForCheckVersion")
	end
end

-- 查询是否可更新
function stCheckForDownload()
	C_PrintLog("stCheckForDownload:")

	local body = "method=checkUpdate&key=" .. g_key
	local resp = HttpPost(g_dstUrl, body)

	if nil == resp then
		C_PrintLog("response content is nil")
		-- 视为查询失败
		C_StartTimer(1)
		C_SetNextState("_EvtTimeOut", -1, "stCheckForDownload")

		return
	end

	--
	C_PrintLog("check download resp: " .. resp)

	-- 解析结果
	local status = tonumber(resp)
	if 1 == status then  -- 可以下载
		C_StartTimer(1)
		C_SetNextState("_EvtTimeOut", -1, "stDownload")
		return
	else                 -- 不可以下载
		C_StartTimer(1)
		C_SetNextState("_EvtTimeOut", -1, "stCheckForDownload")
	end
end

-- 下载文件
function stDownload()
	C_PrintLog("stDownload:")

	--
	stIsDownloadOver()
end

-- 是否下载完成
function stIsDownloadOver()
	C_PrintLog("stIsDownloadOver:")

	if g_downloadedCount < g_needDownLoadCount then
		stDownloadOne()
	else -- 下载完成
		stNotifyServerOver()
	end
end

-- 处理一个下载文件
function HandleDownoadedFile(fileFullPath, filename)
	--
	--filename = "test.rar"
	--fileFullPath = ".\\test.rar"

	local ext = GetExtNameofFile(filename)
	if "" == ext then
		C_PrintLog("no ext name " .. filename)
		return
	end

	
	if "rar" == ext then   -- 压缩文件
		os.execute("rar x -o+ " .. fileFullPath .. " " .. g_unrarPath)
		-- 删除压缩文件
		os.remove(fileFullPath)
	else
		-- 其它文件暂时全部视为媒体文件，直接拷贝到媒体目录  TODO: 需要确定除了rar之外都是媒体文件
		ret = C_CopyFile(fileFullPath, g_mediaPath .. filename) -- 用c代码进行拷贝，目标必须包含文件名
		-- 删除媒体文件
		os.remove(fileFullPath)
	end
end

-- 下载下一个文件
function stDownloadOne()
	C_PrintLog("stDownloadOne:")

	-- 组装下载文件名
	local path = g_downloadList[g_downloadedCount]["path"]
	sizeSourceFile = g_downloadList[g_downloadedCount]["sizeSourceFile"]
	C_PrintLog("download file: " .. path .. " sizeSourceFile: " .. sizeSourceFile)

	g_localFileName = ParseFileNameInPath(path)
	saveFilePath = g_savedPath .. g_localFileName
	C_PrintLog("save file: " .. saveFilePath)

	local ret = C_HttpDownload(path, saveFilePath)	-- 异步下载http文件
	if 0 ~= ret then  -- 如果同步返回失败，需要看情况决定是否需要重新下载
		C_PrintLog("[ERROR] download fail! " .. path)
		C_StartTimer(1)
		C_SetNextState("_EvtTimeOut", -1, "stDownloadOne")
		return
	end

	-- 判断是否下载完成
	C_StartTimer(1)
	C_SetNextState("_EvtTimeOut", -1, "stIsDownloadOK")
end

-- 判断是否下载完成
function stIsDownloadOK()
	C_PrintLog("stIsDownloadOK:")

	if 1 ~= C_IsHttpDownloadOK() then -- 下载未完成
		-- 发送心跳
		local body = "method=heartbeat&key=" .. g_key
		local resp = HttpPost(g_dstUrl, body)
		C_PrintLog("heart beat resp " .. resp)

		-- 间隔60秒，再判断是否下载完成
		C_StartTimer(60)
		C_SetNextState("_EvtTimeOut", -1, "stIsDownloadOK")
		return
	end

	-- 下载完成
	-- 通过下载文件大小判断是否下载成功
	local savedFileSize = GetFileSize(saveFilePath)
	C_PrintLog("download ok, savedFileSize = " .. savedFileSize)
	if tonumber(savedFileSize) ~= tonumber(sizeSourceFile) then                     -- 下载文件与源文件大小不一致，需要重新下载
		C_PrintLog("[ERROR] download file sizeSourceFile error! re download")
		C_StartTimer(5)
		C_SetNextState("_EvtTimeOut", -1, "stDownloadOne")
		return
	end

	-- 下载后的处理
	HandleDownoadedFile(saveFilePath, g_localFileName)

	-- 已下载数自增
	g_downloadedCount = g_downloadedCount + 1

	-- 设置本地更新标志 在文件中定内容
	local pfUpdateFlagFile = io.open("updateflag.dat", "w")
	if nil == pfUpdateFlagFile then
		C_PrintLog("[ERROR] Write update flag failed")
	end
	pfUpdateFlagFile:write("[info]\nneedupdate=1\n")
	pfUpdateFlagFile:close()

	C_StartTimer(1)
	C_SetNextState("_EvtTimeOut", -1, "stIsDownloadOver")
end

-- 通知服务端下载完成
function stNotifyServerOver()
	C_PrintLog("stNotifyServerOver:")

	-- 发送通知接口
	local body = "method=updateFinish&key=" .. g_key
	local resp = HttpPost(g_dstUrl, body)

	if nil == resp or resp ~= "1" then
		C_PrintLog("response content is nil")
		-- 视为失败
		C_StartTimer(5)
		C_SetNextState("_EvtTimeOut", -1, "stNotifyServerOver")

		return
	end

	C_PrintLog("notify result: " .. resp)

	-- 开始下一次更新查询等待
	C_StartTimer(1)
	C_SetNextState("_EvtTimeOut", -1, "stWaitForCheckVersion")
end


function stEnd()
	C_PrintLog("END:")

	C_Exit()
end


BEGIN()
