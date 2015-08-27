
#include "stdafx.h"

#include "MessageDispatcher.h"

#include "cacti/util/IniFile.h"
#include "cacti/mtl/MtlClient.h"
#include "cacti/util/FileSystem.h"
#include "alarm/AlarmSender.h"

#include "evtmsg.h"
#include "reqmsg.h"
#include "resmsg.h"
#include "sxconst.h"
#include "sxcode.h"

#include <fstream>
#include <assert.h>

#define new DEBUG_NEW

#define CFGRES_TIMEOUT 60
#define MTLCLIENT_CONF	"./mtlr.conf"

#pragma warning(disable:4355)

namespace cacti
{
	MessageDispatcher::MessageDispatcher()
		: m_logger(cacti::Logger::getInstance(LOGGER_MTL_DISPATCHER))
		, m_msgTimer(this)
		, m_tcpserver(this)
		, m_peers(this)
		, m_stopSignaled(false)
	{
		// use the parent's handler;
		m_logger.setAdditivity(true);
		m_remoteConfigure = false;
		m_appid = 0xABCDEF01;

		m_version = "ver:mtl        V2.1.20080421.1  build:";
		char month[10];
		int day;
		int year;
		sscanf(__DATE__,"%s %d %d", &month, &day, &year);

		int imonth = 0;
		if (strcmp(month, "Jan") == 0 )
			imonth = 1;
		else if (strcmp(month, "Feb") == 0 )
			imonth = 2;
		else if (strcmp(month, "Mar") == 0 )
			imonth = 3;
		else if (strcmp(month, "Apr") == 0 )
			imonth = 4;
		else if (strcmp(month, "May") == 0 )
			imonth = 5;
		else if (strcmp(month, "Jun") == 0 )
			imonth = 6;
		else if (strcmp(month, "Jul") == 0 )
			imonth = 7;
		else if (strcmp(month, "Aug") == 0 )
			imonth = 8;
		else if (strcmp(month, "Sep") == 0 )
			imonth = 9;
		else if (strcmp(month, "Oct") == 0 )
			imonth = 10;
		else if (strcmp(month, "Nov") == 0 )
			imonth = 11;
		else if (strcmp(month, "Dec") == 0 )
			imonth = 12;

		m_version += StringUtil::format("%4d-%02d-%02d ", year, imonth, day);
		m_version += __TIME__;
	}

	// URL=name:[group]@[nameserver]|ip:portid
	void  MessageDispatcher::parseURL(const std::string& url)
	{
		std::string::size_type pos = url.find_first_of("@");
		std::string tmp = url;
		if(pos != std::string::npos)
		{
			tmp = url.substr(0, pos-0);
			m_nameServer = url.substr(pos+1);
		}
		pos = tmp.find_first_of(":");
		if(pos != std::string::npos)
		{
			m_serviceName = tmp.substr(0, pos-0);
			m_groupName   = tmp.substr(pos+1);
		}
		else
		{
			// if group name missed, it means using local configure format
			// local configure format will be explained in m_serviceName later;
			m_serviceName = tmp;
			m_groupName   = "";
		}
	}

	// format is addr1[|addr2][:portid]
	void parseAddress(const std::string& fmt, 
		std::string& addr1, 
		std::string& addr2, 
		u32& portid)
	{
		std::string::size_type pos = fmt.find_first_of(":");
		if(pos != std::string::npos)
		{
			std::string addr = fmt.substr(0, pos-0);
			portid = atoi(fmt.substr(pos+1).c_str());
			pos = addr.find_first_of("|");
			if(pos != std::string::npos)
			{
				addr1 = addr.substr(0, pos-0);
				addr2 = addr.substr(pos+1);
			}
			else
			{
				addr1 = addr;
				addr2 = "";
			}
		}
		else
		{
			addr1 = fmt;
			portid  = 0;
		}
	}

	// format is : name/id | id
	void parseServiceName(const std::string& sn, 
		std::string& realName, u32& id)
	{
		std::string::size_type pos = sn.find_first_of("/");
		if(pos == std::string::npos)
		{
			// id only
			realName = sn;
			id = atoi(sn.c_str());
		}
		else
		{
			// name and id
			id = atoi(sn.substr(pos+1).c_str());
			realName = sn.substr(0, pos-0);
		}
	}

	bool MessageDispatcher::start(const char* inifile)
	{
		assert(inifile);
		m_logger.info("%s\n", versioninfo());
// 		FILE* fp = fopen("./version.txt", "at");
// 		if(fp)
// 		{
// 			fprintf(fp, "%s\n", versioninfo());
// 			fclose(fp); 
// 		}

// 		m_logger.info("Message Dispatcher\n");
// 		m_logger.info("Initialize from %s\n", inifile);
		bool ret = false;

		do
		{
			if(!bootupSocket())
			{
				m_errmsg = "Socket stack failed";
				break;
			}		
			// take the configure from file
// 			cacti::IniFile ini;
// 			ini.open(inifile);
// 
// 			//m_checkPingInterval = ini.readInt("service", "ping", 60);
// 
// 			std::string url = ini.readString("service", "URL", "");
// 			if(url.empty())
// 			{
// 				m_errmsg = "No URL configured";
// 				break;
// 			}
// 
// 			parseURL(url);
// 
// 			m_logger.info("Loading configures....\n");
// 
// 			// add myself to the portid queue;
// 			m_services.addHook(AppPort::_apMessageD);
// 			if(m_groupName.empty())
// 			{
// 				ret = loadLocalConfigure(inifile);
// 			}
// 			else
// 			{
// 				ret = loadRemoteConfigure();
// 				if(!ret)
// 				{
// 					if(FileSystem::isFileExist(MTLCLIENT_CONF))
// 					{
// 						ini.clear();
// 						ini.open(MTLCLIENT_CONF);
// 						url = ini.readString("service", "URL", "");
// 						parseURL(url);
// 						m_logger.info("Try to load configure from previous remote configure file\n");
// 						ret = loadLocalConfigure(MTLCLIENT_CONF);
// 					}
// 				}
// 			}
// 			if(!ret)
// 				break;
// 
// 			AlarmSender* alarm = AlarmSender::getInstance();
// 			if(alarm)
// 			{
// 				alarm->registerService(m_serviceName.c_str());
// 			}
// 
// 			ret = false;
// 
// 			m_logger.info("Verify the configures...\n");
// 			// @TODO: verify configures
// 			m_logger.info("Passed\n");

			m_logger.info("Running the message timer...\n");
			if(!m_msgTimer.start())
			{
				m_errmsg = "Timer start failed";
				break;
			}		

			m_stopSignaled = false;
			
// 			m_logger.info("Starting service listener...\n");
// 			// start the listen server
// 			m_logger.info("Connecting to all peers...\n");
// 			// start the connector
// 			m_peers.connectAll();
// 			if(!m_tcpserver.start((u_short)m_serviceportid, m_primAddress.c_str()))
// 			{
// 				m_errmsg = "Start the listener failed";
// 				break;
// 			}
// 			m_logger.info("Everything ready!\n");
// 			setPingTimer();
			ret = true;

		}while(false);

		if(ret)
		{			
			m_errmsg = "Initialized OK, ^_^";
			m_logger.info("%s\n", m_errmsg.c_str());
		}
		else
		{
			std::string syserr;
			m_logger.error("%s, %s\n", m_errmsg.c_str(), SysError::lastErrorMessage(syserr));
		}

		return ret;
	}

	void MessageDispatcher::stop()
	{
		m_stopSignaled = true;
		UserTransferMessagePtr utm = UTM::create(myIdentifier(), myIdentifier(), _EvtStopPull);
		postMessage(myIdentifier(), utm);

		m_peers.closeAll();
		m_tcpserver.stop();
		m_msgTimer.stop();

		m_thread.join();
		shutdownSocket();
	}

	const char* MessageDispatcher::getErrorMesssage() const
	{
		return m_errmsg.c_str();
	}

	bool MessageDispatcher::loadLocalConfigure(const char* inifile)
	{
		// reformat the m_serviceName, get the real service name and the app id
		parseServiceName(m_serviceName, m_serviceName, m_appid);
		parseAddress(m_nameServer, m_primAddress, m_backAddress, m_serviceportid);

		if(m_appid == 0)
		{
			m_errmsg = "Configure error, application ID missed";
			return false;
		}

		if(m_serviceportid == 0)
		{
			m_errmsg = "Configure error, service portid missed";
			return false;
		}
		
		IniFile ini;
		ini.open(inifile);
		m_cfgAppId = ini.readInt("Service","cfgServer", 1);

		m_peers.loadFromFile(inifile);

		m_logger.info("Service name   : %s\n", m_serviceName.c_str());
		m_logger.info("Application ID : %d\n", m_appid);
		m_logger.info("Primary address: %s\n", m_primAddress.c_str());
		m_logger.info("Backup address : %s\n", m_backAddress.c_str());
		m_logger.info("Service port   : %d\n", m_serviceportid);
		m_logger.info("CfgServer ID   : %d\n", m_cfgAppId);

		m_remoteConfigure = false;
		return true;
	}

	class ConfigureDispatcher : public ClientDispatcher
	{
	public:
		ConfigureDispatcher(MessageDispatcher* parent)
		{
			m_parent = parent;
			m_responseStatus = -1;
		}
		int waitResponse()
		{
			if(!m_responseEvent.wait(Timestamp(CFGRES_TIMEOUT, 0)))
				return -1;
			else
				return m_responseStatus;
		}
	private:
		virtual bool handleMessage(u32 clientid, UserTransferMessagePtr utm)
		{
			if(utm->getMessageId() == _ResWhoAmI)
			{ 
				m_parent->setRemoteConfigureData(utm);
				m_responseStatus = utm->getReturn();
				m_responseEvent.signal();
			}
			else if(utm->getMessageId() == _EvtDisconnectedFromPeer)
			{
				m_responseEvent.signal();
			}
			return true;
		}
	private:		
		MessageDispatcher* m_parent;
		Event m_responseEvent;
		u32 m_responseStatus;
	};

	bool MessageDispatcher::loadRemoteConfigure()
	{
		ConfigureDispatcher cfgdisp(this);
		TransferClient mtlClient(AppPort::_apMessageD, &cfgdisp);

		if(!mtlClient.start(CFGSERVER_PORT, m_nameServer.c_str(), false))
		{
			m_logger.error("Can't connect to configure server(%s)\n", m_nameServer.c_str());
			return	false;
		}
		ServiceIdentifier appD(0, AppPort::_apMessageD, 0);
		UserTransferMessagePtr whoami = UTM::create(appD, appD, _ReqWhoAmI);
		(*whoami)[_TagAppName] = m_serviceName;
		(*whoami)[_TagGroupName] = m_groupName;

		NetworkPackage np;
		np.setSrcIdentifier(appD);
		np.setDstIdentifier(appD);
		np.setUTM(whoami);
		if(!mtlClient.sendPackage(np))
		{
			m_logger.error("Send WhoAmI FAILED\n");
			mtlClient.stop();
			return false;
		}

		m_logger.info("Asking WhoAmI\n");
		int ret = cfgdisp.waitResponse();
		mtlClient.stop();
		if(ret != ERR_SUCCESS)
		{
			m_logger.info("WhoAmI response FAILED,%d\n", ret);
			return false;
		}
		else
		{
			return true;
		}
	}

	void MessageDispatcher::setRemoteConfigureData(UserTransferMessagePtr utm)
	{
		if(utm->getReturn() != ERR_SUCCESS)
			return;
		m_serviceName = (std::string)(*utm)[_TagAppName];
		m_appid = (*utm)[_TagAppID];
		std::vector<std::string> ipV4 = (*utm)[_TagIPv4];
		m_serviceportid = (*utm)[_TagPortID];
		std::string peers = (*utm)[_TagPeers];
		m_primAddress = ipV4[0];
		m_backAddress = ipV4[1];
		m_cfgAppId = (*utm)[_TagCfgServerID];

		m_logger.info("Service name   : %s\n", m_serviceName.c_str());
		m_logger.info("Application ID : %d\n", m_appid);
		m_logger.info("Primary address: %s\n", m_primAddress.c_str());
		m_logger.info("Backup address : %s\n", m_backAddress.c_str());
		m_logger.info("Service port   : %d\n", m_serviceportid);
		m_logger.info("CfgServer ID   : %d\n", m_cfgAppId);

		FILE* fp = fopen(MTLCLIENT_CONF, "wt");
		if(!fp)
		{
			m_logger.info("Open %s to write failed\n",
				MTLCLIENT_CONF);
			return;
		}

		if(m_backAddress.length() != 0 && m_primAddress != m_backAddress)
			fprintf(fp, "[service]\nurl=%s/%d@%s|%s:%d\n", m_serviceName.c_str(), m_appid, m_primAddress.c_str(), m_backAddress.c_str(), m_serviceportid);
		else
			fprintf(fp, "[service]\nurl=%s/%d@%s:%d\n", m_serviceName.c_str(), m_appid, m_primAddress.c_str(), m_serviceportid);
		fprintf(fp, "cfgServer=%d\n", m_cfgAppId);

		fprintf(fp, "\n%s\n", peers.c_str());
		fclose(fp);
		m_peers.loadFromFile(MTLCLIENT_CONF);
		m_remoteConfigure = true;
	}
	bool MessageDispatcher::hook(u32 portid)
	{		
		// must alloc portid queue before update status -- ref the portidQueue
		if(!m_services.addHook(portid))
		{
			m_logger.error("[%04d] Allocate port queue failed\n", portid);
		}

		m_logger.alert("[%04d] HOOK\n", portid);
		
		return true;
	}

	void MessageDispatcher::inited(u32 portid)
	{
		m_services.setInited(portid);
		m_logger.alert("[%04d] INITED\n", portid);
		ServiceIdentifier req(m_appid, portid, 0);
		ServiceIdentifier res(m_appid, AppPort::_apMessageD, 0);
		postMessage(req, res, UTM::create(req, res, _EvtServiceInited));
	}
	bool MessageDispatcher::ready(u32 portid)
	{
		return true;
	}
	void MessageDispatcher::unhook(u32 portid)
	{
		m_services.delHook(portid);

		m_logger.alert("[%04d] UNHOOK\n", portid);
	}

	bool MessageDispatcher::getMessage(u32 portid, ServiceIdentifier& req, UserTransferMessagePtr& utm)
	{
		NetworkPackagePtr npp = m_services.getPackage(portid);
		if(npp)
		{
			req = npp->getSrcIdentifier();
			utm = npp->getUTM();
			if(utm)
			{
				m_logger.info("[%04d] GET (%s, %s, %X) from (%s)\n", 
					portid,
					utm->getReq().toString().c_str(),
					utm->getRes().toString().c_str(),
					utm->getMessageId(),
					req.toString().c_str());
				return true;
			}
		}
		return false;
	}

	bool MessageDispatcher::postMessage(const ServiceIdentifier& req, UserTransferMessagePtr utm)
	{
		ServiceIdentifier myid(m_appid, AppPort::_apMessageD, 0);
		ServiceIdentifier myreq = req;
		myreq.m_appid = appId();
		return postMessage(myreq, myid, utm);
	}

	bool MessageDispatcher::postMessage(const ServiceIdentifier& req, 
										const ServiceIdentifier& res, 
										UserTransferMessagePtr utm,
										bool atHead /*= false*/)
	{
		// check self message
		if(res.m_appid == appId())
		{
			if(req.m_appid == appId())
			{
				// self message
				m_logger.info("[%04d] POST(%s, %s, %X) from self port(%d), to(%s)\n",
					req.m_appport,
					utm->getReq().toString().c_str(),
					utm->getRes().toString().c_str(),
					utm->getMessageId(),
					res.m_appport,
					atHead ? "head" : "tail");
			}
			else
			{
				m_logger.info("[%04d] RECV(%s, %s, %X) from (%s), to(%s)\n",
					req.m_appport,
					utm->getReq().toString().c_str(),
					utm->getRes().toString().c_str(),
					utm->getMessageId(),
					req.toString().c_str(),
					atHead ? "head" : "tail");
			}

			if(res.m_appport == AppPort::_apMessageD)
			{
				m_logger.info("[0000] System message\n");
				onSystemMessage(req, utm);
			}
			else
			{
				NetworkPackagePtr npp(new NetworkPackage());
				npp->setSrcIdentifier(req);
				npp->setDstIdentifier(res);
				npp->setUTM(utm);

				bool ret;
				if(atHead)
					ret = m_services.tryPutPackageHead(res.m_appport, npp);
				else
					ret = m_services.tryPutPackage(res.m_appport, npp);

				if(!ret)
				{
					m_logger.error("[%04d] POST(%s, %s, %X) to self FAILED\n",
						req.m_appport,
						utm->getReq().toString().c_str(),
						utm->getRes().toString().c_str(),
						utm->getMessageId());
					return false;
				}
			}
			return true;
		}
		else
		{
			return postMessageToPeer(req, res, utm);
		}
	}

	bool MessageDispatcher::isPeerActive(const ServiceIdentifier& res)
	{
		return m_peers.isServiceActive(res.m_appid, res.m_appport);
	}

	bool MessageDispatcher::postMessageToPeer(const ServiceIdentifier& req, const ServiceIdentifier& res, UserTransferMessagePtr utm)
	{
		unsigned int connid = m_peers.getPasvConnection(res);
		NetworkPackage np;
		np.setSrcIdentifier(req);
		np.setDstIdentifier(res);
		np.setUTM(utm);

		m_logger.info("[%04d] POST(%s, %s, %X) to (%s)\n",
			req.m_appport,
			utm->getReq().toString().c_str(),
			utm->getRes().toString().c_str(),
			utm->getMessageId(),
			res.toString().c_str());

		if(!isPeerActive(res))
		{
			m_logger.warn("[%04d] peer(%d.%d) not active\n", 
				req.m_appport, res.m_appid, res.m_appport);
			return false;
		}

		if(connid != TransferServer::INVALID_CONNECTION_ID)
		{
			m_logger.debug("[%04d] post by passive connection\n", req.m_appport);
			// find the connection from server at first
			if(m_tcpserver.sendPackage(connid, np))
			{
				return true;
			}
			else
			{
				m_logger.warn("[%04d] post by passive connection failed, try active connection\n", 
					req.m_appport);
			}
		}
		
		// don't else!
		m_logger.debug("[%04d] post by active connection\n", req.m_appport);
		// find the connection from client
		bool ok = m_peers.sendPackage(res.m_appid, np);
		if(!ok)
		{
			m_logger.error("[%04d] post by active connection FAILED\n", 
				req.m_appport);
		}
		return ok;
	}

	TimerID MessageDispatcher::setTimer(u32 expires, const ServiceIdentifier& sid, UserTransferMessagePtr utm)
	{
		return m_msgTimer.set(expires, sid, utm);
	}

	void MessageDispatcher::killTimer(TimerID id)
	{
		m_msgTimer.cancel(id);
	}

	ApplicationData MessageDispatcher::getApplicationInfo(u32 appid)
	{
		return m_peers.getApplicationInfo(appid);
	}

	void MessageDispatcher::onSystemMessage(const ServiceIdentifier& req, UserTransferMessagePtr utm)
	{
		switch(utm->getMessageId())
		{
		case _EvtServiceInited:
			broadcastServiceActive(req.m_appport);
			notifyMissedActive(req.m_appport);
			break;

		case _EvtConnectedToPeer:
			onConnectedToPeer(req, utm);
			break;

		case _EvtPingServer:
			onCheckPingServer(req, utm);
			break;

		case _EvtPingFromRemote:
			m_logger.info("Got a ping from (%d)\n", req.m_appid);
			break;

		case _EvtDisconnectedFromPeer:
			onPeerDisconnected(req, utm);
			break;

		case _EvtClientClosed:
			onClientClosed(req, utm);
			break;

		case _EvtModuleActive:
			onServiceActive(req, utm, false);
			break;
		
		case _EvtModuleRedundanceActive:
			onServiceActive(req, utm, true);
			break;

		case _EvtStopPull:
			break;

		default:
			break;
		}
	}

	void MessageDispatcher::onConnectedToPeer(const ServiceIdentifier& req, UserTransferMessagePtr utm)
	{
		u32 peerAppid = req.m_appref;
		m_logger.info("Connected to peer(%d)\n", peerAppid);

		// peer's message dispatcher already active
		m_peers.onServiceActive(peerAppid, AppPort::_apMessageD, false);
		m_logger.info("[%d, %04d] active\n", peerAppid, AppPort::_apMessageD);

		UserTransferMessagePtr rutm = UTM::create(req, req, _EvtModuleActive);
		m_services.setActiveService(*rutm);
		
		// post an active message to peer's message dispatcher
		NetworkPackage np;
		np.setSrcIdentifier(myIdentifier());
		np.setDstIdentifier(ServiceIdentifier(peerAppid, AppPort::_apMessageD, 0));
		np.setUTM(rutm);

		// find the connection from client
		if(!m_peers.sendPackage(peerAppid, np))
		{
			m_logger.error("send _EvtModuleActive to (%d) failed when connected to peer\n",
				peerAppid);
		}
		else
		{
			m_logger.info("send _EvtModuleActive to (%d) when connected to peer\n",
				peerAppid);
		}
	}

	void MessageDispatcher::onPeerDisconnected(const ServiceIdentifier& req, UserTransferMessagePtr utm)
	{
		u32 peerAppid = req.m_appref;
		m_logger.info("peer(%d) active connection closed\n", peerAppid);

		if(m_peers.isPeerDead(peerAppid))
		{
			m_logger.info("peer(%d) dead\n",peerAppid);
			onAppDead(peerAppid);
		}
	}

	void MessageDispatcher::onClientClosed(const ServiceIdentifier& req, UserTransferMessagePtr utm)
	{
		int cnntid = req.m_appref;
		m_logger.info("(%d)client closed\n", cnntid);

		u32 peerAppid = m_peers.clearPasvConnection(cnntid);
		if(peerAppid != 0)
		{
			m_logger.info("peer(%d) passive connection(%d) invalidate\n", 
				peerAppid, cnntid);

			if(m_peers.isPeerDead(peerAppid))
			{
				m_logger.info("peer(%d) dead\n",peerAppid);
				onAppDead(peerAppid);
			}
		}
	}

	void MessageDispatcher::onAppDead(u32 peerappid)
	{
		m_services.lock();
		// send all module the active message
		std::vector<u32> hookedservice = m_services.getInitedService();
		std::vector<ServiceIdentifier> needdeactive = m_peers.getAllActiveServices(peerappid);

		UserTransferMessagePtr evtutm = UTM::create(_EvtModuleDeactive);
		for(size_t i=0; i<hookedservice.size(); ++i)
		{
			if(hookedservice[i] == AppPort::_apMessageD)
				break;
			// first de-active
			for(size_t j=0; j<needdeactive.size(); ++j)
			{
				ServiceIdentifier ressid(m_appid, hookedservice[i], 0);
				postMessage(needdeactive[j], ressid, evtutm);
			}
		}
		m_services.unlock();
	}
	void MessageDispatcher::onServiceActive(const ServiceIdentifier& req, UserTransferMessagePtr utm, bool redundance)
	{
		u32array activeports = (*utm)[_TagPortID];
		u8array  firsttime = (*utm)[_TagFirstTimeConnected];
		u32 starttime = (*utm)[_TagAppStartTime];

		u32 connid = req.m_appref;

		m_logger.info("%d tell me it's active,connid=%d\n", 
			req.m_appid, connid);

		std::vector<u32> needactive;
		std::vector<u32> needdeactive;
		if(starttime != 0)
		{
			m_logger.info("MTL with application timestamp\n");
			for(size_t i=0; i<activeports.size(); ++i)
			{
				u32 ret = m_peers.onServiceActive(req.m_appid, activeports[i], starttime);
				switch(ret)
				{
				case 0:
					{
						m_logger.info("[%d, %04d] anonymous service active\n", req.m_appid, activeports[i]);
						needactive.push_back(activeports[i]);
					}
					break;
				case 1:
					{
						m_logger.info("[%d, %04d] active\n", req.m_appid, activeports[i]);
						needactive.push_back(activeports[i]);
					}
					break;
				case 2:
					{
						m_logger.info("[%d, %04d] reactive\n", req.m_appid, activeports[i]);
						needdeactive.push_back(activeports[i]);
						needactive.push_back(activeports[i]);
					}
					break;
				case 3:
					{
						m_logger.info("[%d, %04d] duplicated active event\n", 
							req.m_appid, activeports[i]);
					}
					break;
				default:
					m_logger.info("[%d, %04d] ?%d?\n", 
						req.m_appid, activeports[i], ret);
					break;
				}
			}
		}
		else
		{
			m_logger.info("MTL without application timestamp, update MTL please\n");
		}

		if(connid != TransferServer::INVALID_CONNECTION_ID)
		{
			m_logger.info("%d passive connid(%d) updated\n", 
				req.m_appid, connid);
			m_peers.updatePasvConnection(req, connid);
		}

		m_logger.info("need deactive list(%s)\n", 
			StringUtil::from(needdeactive).c_str());
		m_logger.info("need active list(%s)\n", 
			StringUtil::from(needactive).c_str());

		m_services.lock();

		if(!redundance)
		{
			// make an response event to peer
			UserTransferMessagePtr rutm = UTM::create(req, req, _EvtModuleRedundanceActive);
			m_services.setActiveService(*rutm);
			postMessage(myIdentifier(), req, rutm);
		}

		// send all module the active message
		std::vector<u32> hookedservice = m_services.getInitedService();
		UserTransferMessagePtr rutm1 = UTM::create(*utm);
		rutm1->setMessageId(_EvtModuleDeactive);
		UserTransferMessagePtr rutm2 = UTM::create(*utm);
		rutm2->setMessageId(_EvtModuleActive);

		for(size_t i=0; i<hookedservice.size(); ++i)
		{
			if(hookedservice[i] == AppPort::_apMessageD)
				break;

			// first de-active
			for(size_t j=0; j<needdeactive.size(); ++j)
			{
				ServiceIdentifier reqsid(req.m_appid, needdeactive[j], 0);
				ServiceIdentifier ressid(m_appid, hookedservice[i], 0);
				postMessage(reqsid, ressid, rutm1);
			}

			// and then active
			for(size_t k=0; k<needactive.size(); ++k)
			{
				ServiceIdentifier reqsid(req.m_appid, needactive[k], 0);
				ServiceIdentifier ressid(m_appid, hookedservice[i], 0);
				postMessage(reqsid, ressid, rutm2);
			}
		}

		m_services.unlock();
	}

	void MessageDispatcher::onCheckPingServer(const ServiceIdentifier& req, UserTransferMessagePtr utm)
	{
		broadcastMessage(UTM::create(ServiceIdentifier(), ServiceIdentifier(), _EvtPingFromRemote));
		setPingTimer();
	}

	void MessageDispatcher::setPingTimer()
	{
		// start a ping timer
// 		setTimer(m_checkPingInterval*1000, myIdentifier(), 
// 			UTM::create(UserTransferMessage(myIdentifier(),myIdentifier(), _EvtPingServer)));
	}

	void MessageDispatcher::broadcastMessage(UserTransferMessagePtr utm)
	{
		std::vector<u32> peerapps = m_peers.getAllPeerApplication();
		ServiceIdentifier myappid = myIdentifier();
		for(size_t i=0; i<peerapps.size(); ++i)
		{
			ServiceIdentifier peerappid(peerapps[i], AppPort::_apMessageD, 0);
			if(!postMessageToPeer(myappid, peerappid, utm))
			{
				m_logger.error("broadcast to (%d) failed\n", peerapps[i]);
			}
		}
	}

	void MessageDispatcher::broadcastServiceActive(u32 portid)
	{
		m_logger.info("[%04d] broadcast active to all peers\n", portid);

		ServiceIdentifier sid;
		UserTransferMessagePtr utm = UTM::create(sid, sid, _EvtModuleActive);
		m_services.setActiveService(*utm, portid);
		broadcastMessage(utm);
	}

	void MessageDispatcher::broadcastServiceReady(u32 portid)
	{
	}

	void MessageDispatcher::notifyMissedActive(u32 portid)
	{
		// notify the portid service what already active services
		ServiceIdentifier sid(m_appid, portid, 0);
		std::vector<ServiceIdentifier> activepeers = m_peers.getAllActiveServices();
		for(size_t i=0; i<activepeers.size();++i)
		{
			m_logger.info("[%04d] notify missed service(%d, %d) active\n", 
				portid, activepeers[i].m_appid, activepeers[i].m_appport);

			postMessage(activepeers[i], sid, UTM::create(activepeers[i], sid, _EvtModuleActive));
		}
	}
}

SelfDispatcher::SelfDispatcher(MessageDispatcher* parent, const ServiceIdentifier& sid)
	: m_parent(parent)
	, m_selfsid(sid)
{
}

bool SelfDispatcher::getMessage(u32 portid, ServiceIdentifier& req, UserTransferMessagePtr& utm)
{
	return false;
}

bool SelfDispatcher::postMessage(const ServiceIdentifier& req, UserTransferMessagePtr utm)
{
	// save the connection id to other place
	// otherwise ServiceSkeleton will remove it
	ServiceIdentifier newreq = req;
	newreq.m_appport = req.m_appref;
	return m_parent->postMessage(newreq, m_selfsid, utm);
}

bool SelfDispatcher::postMessage(const ServiceIdentifier& req, const ServiceIdentifier& res, UserTransferMessagePtr utm, bool atHead)
{
	// save the connection id to other place
	ServiceIdentifier newreq = req;
	newreq.m_appport = req.m_appref;
	return m_parent->postMessage(newreq, m_selfsid, utm, atHead);
}
