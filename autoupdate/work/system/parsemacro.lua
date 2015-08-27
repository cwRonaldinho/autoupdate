-- UTM宏定义解析脚本

-- 全局变量
intValue = 0			-- 等号右边的数值
strKeyName = ""			-- 等号左边的宏，字符串

-- 解析行内容
-- 返回：
--		参数1： 0--成功 其它失败
--		参数2： 字串
--		参数3： 整型值
function ParseLine(line)
	result = 1
	strKeyName = ""
	intValue = 0

	local lineType = CheckLineType(line)
	if lineType == 0 then
		local ret = ParseEqual(line)
		if ret == 0 then
			result = 0
		end
	else
		result = 1
	end
	
	if strKeyName == nil then strKeyName = "" end

	return result, strKeyName, intValue
end

-- 是否为空白行
function IsSpaceLine(aline)
	local retLine = string.match(aline, "(%s*)")
	if retLine == aline then
		return true
	end

	return false
end

-- 是否为#号开关行
function IsNumbegSignLine(aline)
end

-- 判断前2个字符的类型
-- 返回: 0--继续处理  非0－--不用处理
function CheckLineType(aline)
	strBegin2 = string.match(aline, "%s*(..)")
	if strBegin2 == nil then			-- 无两个字符
		return 1
	elseif strBegin2 == "//" then		-- 为"//"注释行
		return 2
	else
		return 0						-- 正常
	end
end

-- 取=号两边的值
-- 返回: 0--成功  非0--失败
function ParseEqual(aline)
	strKeyName, sTmpValue = string.match(aline, "%s*([%w_]*)%s*=%s*0[Xx]([%d%x]-),")

	-- __TW__DEBUG
	--print(strKeyName)
	--print(sTmpValue)

	if strKeyName == nil or strKeyName == "" or sTmpValue == nil or sTmpValue == "" then
		return 1
	end

	intValue = tonumber(sTmpValue, 16)

	-- __TW__DEBUG
	--print(intValue)

	return 0
end
