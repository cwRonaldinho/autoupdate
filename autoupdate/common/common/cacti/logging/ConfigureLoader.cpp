#include "stdafx.h"

#include "ConfigureLoader.h"

#include "cacti/util/StringUtil.h"

#include <cacti/logging/ConsoleHandler.h>
#include <cacti/logging/FileHandler.h>

#include <iostream>
using std::cout;
using std::endl;

#include "sxml/tinyxml.h"

namespace cacti
{
	// don't talk about design pattern, here we just do it with the most easy way
	bool ConfigureLoader::loadXML(const char* xml)
	{
		TiXmlDocument doc;
		if(!doc.LoadFile(xml))
		{
			return false;
		}
		TiXmlElement* root = doc.RootElement();
		if(root)
		{
			// read handlers
			std::map<std::string, LogHandlerPtr> definedHandler;
			TiXmlElement* elemHandler = root->FirstChildElement("handler");
			while(elemHandler)
			{
				const char* name = elemHandler->Attribute("name");
				const char* clss = elemHandler->Attribute("class");
				if(name && clss)
				{
					TiXmlElement* elemParam = elemHandler->FirstChildElement("param");
					PARAMMAP params;
					while(elemParam)
					{
						const char* name = elemParam->Attribute("name");
						const char* value = elemParam->Attribute("value");
						params.insert(std::make_pair(name, value));
						elemParam = elemParam->NextSiblingElement();
					}

					LogHandlerPtr handler = createHandler(clss, params);
					if(handler)
					{
						TiXmlElement* elemFormatter = elemHandler->FirstChildElement("formatter");	
						if(elemFormatter)
						{
							const char* clss = elemFormatter->Attribute("class");
							if(clss)
							{
								TiXmlElement* elemParam = elemFormatter->FirstChildElement("param");
								PARAMMAP params;
								while(elemParam)
								{
									const char* name = elemParam->Attribute("name");
									const char* value= elemParam->Attribute("value");
									params.insert(std::make_pair(name, value));
									elemParam = elemParam->NextSiblingElement();
								}
								FormatterPtr formatter = createFormatter(clss, params);
								if(formatter)
									handler->setFormatter(formatter);
							}
						}
						definedHandler.insert(std::make_pair(name, handler));
					}
					else
					{
						cout << "<<log configure>> class :" << clss << " not found!" << endl;
					}
				}
				elemHandler = elemHandler->NextSiblingElement();
			}
			
			// read loggers
			TiXmlElement* elemLogger = root->FirstChildElement("logger");
			while(elemLogger)
			{
				const char* name = elemLogger->Attribute("name");
				if(name)
				{
					Logger& logger = Logger::getInstance(name);
					TiXmlElement* elemLevel = elemLogger->FirstChildElement("level");
					if(elemLevel)
					{
						LogControlID level = createLevel(elemLevel->Attribute("value"));
						if(level != LogLevel::NOTSET)
							logger.setLevel(level);
					}
					TiXmlElement* elemRef = elemLogger->FirstChildElement("handler-ref");
					while(elemRef)
					{
						const char* ref = elemRef->Attribute("value");
						if(ref)
						{
							LogHandlerPtr handler = definedHandler[ref];
							if(handler)
								logger.addHandler(handler);
						}
						elemRef = elemRef->NextSiblingElement();
					}
				}
				elemLogger = elemLogger->NextSiblingElement();
			}
		}
		return true;
	}

	bool ConfigureLoader::updateLevel(const char* xml)
	{
		TiXmlDocument doc;
		if(!doc.LoadFile(xml))
		{
			return false;
		}
		TiXmlElement* root = doc.RootElement();
		if(root)
		{
			TiXmlElement* elemLogger = root->FirstChildElement("logger");
			while(elemLogger)
			{
				const char* name = elemLogger->Attribute("name");
				if(name)
				{
					Logger& logger = Logger::getInstance(name);
					TiXmlElement* elemLevel = elemLogger->FirstChildElement("level");
					if(elemLevel)
					{
						LogControlID orglevel = logger.getLevel();
						LogControlID level = createLevel(elemLevel->Attribute("value"));

						if(level != LogLevel::NOTSET && orglevel != level)
						{
							logger.setLevel(level);
							cout << "logger [" << name << "] change level from [" 
								<< orglevel.getLabel() << "] to [" 
								<< level.getLabel() << "]" << endl;
						}
					}
				}
				elemLogger = elemLogger->NextSiblingElement();
			}
		}
		return true;
	}

	LogHandlerPtr ConfigureLoader::createHandler(const char* bystr, PARAMMAP& params)
	{
		if(!bystr)
			return LogHandlerPtr();

		if(StringUtil::ignoreCaseCompare(bystr, "ConsoleHandler"))
			return LogHandlerPtr(new ConsoleHandler(params));
		else if(StringUtil::ignoreCaseCompare(bystr, "FileHandler"))
			return LogHandlerPtr(new FileHandler(params));
		else if(StringUtil::ignoreCaseCompare(bystr, "RollingFileHandler"))
			return LogHandlerPtr(new RollingFileHandler(params));
		else if(StringUtil::ignoreCaseCompare(bystr, "StarFileHandler"))
			return LogHandlerPtr(new StarFileHandler(params));
		else
			return LogHandlerPtr();
	}

	FormatterPtr ConfigureLoader::createFormatter(const char* bystr, PARAMMAP& params)
	{
		if(!bystr)
			return FormatterPtr();

		if(StringUtil::ignoreCaseCompare(bystr, "SimpleFormatter"))
			return FormatterPtr(new SimpleFormatter(params));
		else if(StringUtil::ignoreCaseCompare(bystr, "TTICFormatter"))
			return FormatterPtr(new TTICFormatter(params));
		else if(StringUtil::ignoreCaseCompare(bystr, "StarFormatter"))
			return FormatterPtr(new StarFormatter(params));

		return FormatterPtr();
	}

	LogControlID ConfigureLoader::createLevel(const char* bystr)
	{
		if(!bystr)
			return LogLevel::NOTSET;

		if(StringUtil::ignoreCaseCompare("DBG", bystr))
			return LogLevel::DBG;
		else if(StringUtil::ignoreCaseCompare("INFO", bystr))
			return LogLevel::INFO;
		else if(StringUtil::ignoreCaseCompare("WARN", bystr))
			return LogLevel::WARN;
		else if(StringUtil::ignoreCaseCompare("ERROR", bystr))
			return LogLevel::ERR;
		else if(StringUtil::ignoreCaseCompare("ALERT", bystr))
			return LogLevel::ALERT;
		else if(StringUtil::ignoreCaseCompare("FATAL", bystr))
			return LogLevel::FATAL;
		return true;
	}
}

// example logger.xml
//<?xml version="1.0" encoding="UTF-8" ?>
//
//<logging>
//
//  <!-- 定义handler -->
//  <handler name="CONSOLE" class="ConsoleHandler">
//    <formatter class="TTICFormatter"/>
//  </handler>
//
//  <handler name="SYSFILE" class="StarFileHandler">
//    <formatter class="TTICFormatter"/>
//    <param name="filename" value="./log/sys.log"/>
//    <param name="maxsize" value="50000000"/>
//    <param name="maxbackup" value="10"/>
//    <param name="flush" value="true"/>
//  </handler>
//
//  <handler name="CONSOLEFILE" class="StarFileHandler">
//    <formatter class="StarFileHandler"/>
//    <param name="filename" value="./log/console.log"/>
//    <!-- 下面的参数都可以忽略，缺省大小是50M，10个backup，立刻刷新 -->
//  </handler>
//
//  <handler name="MTLFILE" class="StarFileHandler">
//    <formatter class="StarFormatter"/>
//    <param name="filename" value="./log/mtl.log"/>
//  </handler>
//
//  <handler name="SKTFILE" class="StarFileHandler">
//    <formatter class="StarFormatter"/>
//    <param name="filename" value="./log/mtl-skt.log"/>
//  </handler>
//
//  <!-- 定义level，关联handler -->
//  <logger name="console">
//    <handler-ref value="CONSOLE"/>
//    <handler-ref value="CONSOLEFILE"/>
//  </logger>
//
//  <logger name="sys">
//    <handler-ref value="SYSFILE"/>
//  </logger>
//  
//  <logger name="mtl">
//    <level value="INFO"/>
//    <handler-ref value="MTLFILE"/>
//  </logger>
//
//  <logger name="mtl.dispatcher">
//    <level value="DBG"/>
//    <handler-ref value="MTLFILE"/>
//  </logger>
//
//  <logger name="mtl.skeleton">
//    <level value="DBG"/>
//    <handler-ref value="MTLFILE"/>
//  </logger>
//
//  <logger name="mtl.peers">
//    <level value="DBG"/>
//    <handler-ref value="MTLFILE"/>
//  </logger>
//
//  <logger name="mtl.timer">
//    <level value="INFO"/>
//    <handler-ref value="MTLFILE"/>
//  </logger>
//
//  <logger name="socket.client">
//    <level value="INFO"/>
//    <handler-ref value="SKTFILE"/>
//  </logger>
//
//  <logger name="socket.server">
//    <level value="INFO"/>
//    <handler-ref value="SKTFILE"/>
//  </logger>
//  
//</logging>
//
