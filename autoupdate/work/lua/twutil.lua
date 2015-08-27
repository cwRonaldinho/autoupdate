-- 通用功能函数

local http=require("socket.http");

-- 解析出全路径中的文件名
function ParseFileNameInPath(path)
	local beginPos = 1
	local keyPos = string.find(path, "[/\\]", beginPos)
	while(keyPos ~= nil) do
		beginPos = keyPos + 1
		keyPos = string.find(path, "[/\\]", beginPos)
	end
	savedPath = string.sub(path, beginPos)
	return savedPath
end

-- 下载http文件
-- 返回
--		0:成功  其它失败
function DownLoadHttpFile(url, savedPath)
	local output = io.open(savedPath, "wb");
	if nil == output then
		print("open saved file faild: " .. savedPath)
		return 1
	end

	print("begin downloading at " .. os.date())
	local response, status, respHeaders, statusLine = http.request(url)
	--print(response)
	--print(status)
	--print(respHeaders)
	--print(statusLine)
	output:write(response)
	print("end downloading at " .. os.date())

	output:close()

	return 0
end

-- 获取文件大小
function GetFileSize(filePath)
	local hFile = io.open(filePath, "rb")
	if nil == hFile then
		return 0
	end

    --local currentPos = hFile:seek() -- 获取当前位置
    local size = hFile:seek("end") -- 获取文件大小
    --file:seek("set", currentPos)
	hFile:close()
    return size
end

-- 获取文件扩展名
function GetExtNameofFile(filename)
	local ext = string.match(filename, ".+%.(.+)")
	return string.lower(ext or "")
end
