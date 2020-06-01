// AnalysisCap.cpp : 实现文件
//


#include "AnalysisCap.h"
#include<fstream>
#include<iomanip>
#include <process.h>
#include<pcap.h>
#include"NetInfo.h"
#include"XmlFilter.h"
#include"../CoreProto/pluginmanager.h"
#include"../CoreProto/IAnalyzer.h"
#include"../CoreProto/coreapi.h"


std::vector<NetCardInfo> CAnalysisCap::devs;

// CAnalysisCap

CAnalysisCap::CAnalysisCap()
{
	//fp = NULL;
	plugin = "";
	//加载协议解析器
	PluginManager::GetInstance().LoadAll();
	mMgr.dev = NULL;
	mMgr.tcpReassembly = NULL;
	mMgr.thisdata = this;
	mMgr.sessions = NULL;
	// set global config singleton with input configuration
	GlobalConfig::getInstance().writeMetadata = false;
	GlobalConfig::getInstance().writeToConsole = true;
	GlobalConfig::getInstance().separateSides = true;
	isSniffing = false;
}

CAnalysisCap::~CAnalysisCap()
{
	//if (fp)
	//{
	//	pcapClose(fp);
	//	fp = NULL;
	//}
	//卸载协议解析器
	if (IsSniffing()) StopOpenSniffer();
	PluginManager::GetInstance().UnloadAll();
}

std::vector<std::string> CAnalysisCap::GetAllPlugins()
{
	std::vector<std::string> result;
	std::vector<PluginInstance *> plugins = PluginManager::GetInstance().GetAllPlugins();
	std::vector<PluginInstance *>::iterator it;
	for (it = plugins.begin(); it != plugins.end(); ++it)
	{
		PluginInstance *pi = *it;
		result.push_back(pi->GetDisplayName());
	}
	return result;
}

/**
* The callback being called by the TCP reassembly module whenever new data arrives on a certain connection
*/
void CAnalysisCap::tcpReassemblyMsgReadyCallback(int sideIndex, const TcpStreamData& tcpData, void* userCookie)
{
	// extract the connection manager from the user cookie
	TcpReassemblyMgr* mMgr = (TcpReassemblyMgr*)userCookie;

	// check if this flow already appears in the connection manager. If not add it
	TcpReassemblyConnMgrIter iter = (mMgr->connMgr).find(tcpData.getConnectionData().flowKey);
	if (iter == mMgr->connMgr.end())
	{
		mMgr->connMgr.insert(std::make_pair(tcpData.getConnectionData().flowKey, TcpReassemblyData()));
		iter = mMgr->connMgr.find(tcpData.getConnectionData().flowKey);
	}

	int side;

	// if the user wants to write each side in a different file - set side as the sideIndex, otherwise write everything to the same file ("side 0")
	if (GlobalConfig::getInstance().separateSides)
		side = sideIndex;
	else
		side = 0;

	// if the file stream on the relevant side isn't open yet (meaning it's the first data on this connection)
	if (iter->second.fileStreams[side] == NULL)
	{
		// add the flow key of this connection to the list of open connections. If the return value isn't NULL it means that there are too many open files
		// and we need to close the connection with least recently used file(s) in order to open a new one.
		// The connection with the least recently used file is the return value
		uint32_t flowKeyToCloseFiles;
		int result = GlobalConfig::getInstance().getRecentConnsWithActivity()->put(tcpData.getConnectionData().flowKey, &flowKeyToCloseFiles);

		// if result equals to 1 it means we need to close the open files in this connection (the one with the least recently used files)
		if (result == 1)
		{
			// find the connection from the flow key
			TcpReassemblyConnMgrIter iter2 = mMgr->connMgr.find(flowKeyToCloseFiles);
			if (iter2 != mMgr->connMgr.end())
			{
				// close files on both sides (if they're open)
				for (int index = 0; index < 2; index++)
				{
					if (iter2->second.fileStreams[index] != NULL)
					{
						// close the file
						GlobalConfig::getInstance().closeFileSteam(iter2->second.fileStreams[index]);
						iter2->second.fileStreams[index] = NULL;

						// set the reopen flag to true to indicate that next time this file will be opened it will be opened in append mode (and not overwrite mode)
						iter2->second.reopenFileStreams[index] = true;
					}
				}
			}
		}

		// get the file name according to the 5-tuple etc.
		std::string fileName = GlobalConfig::getInstance().getFileName(tcpData.getConnectionData(), sideIndex, GlobalConfig::getInstance().separateSides) + ".txt";

		// open the file in overwrite mode (if this is the first time the file is opened) or in append mode (if it was already opened before)
		iter->second.fileStreams[side] = GlobalConfig::getInstance().openFileStream(fileName, iter->second.reopenFileStreams[side]);
	}

	// if this messages comes on a different side than previous message seen on this connection
	if (sideIndex != iter->second.curSide)
	{
		// count number of message in each side
		iter->second.numOfMessagesFromSide[sideIndex]++;

		// set side index as the current active side
		iter->second.curSide = sideIndex;
	}

	// count number of packets and bytes in each side of the connection
	iter->second.numOfDataPackets[sideIndex]++;
	iter->second.bytesFromSide[sideIndex] += (int)tcpData.getDataLength();

	// write the new data to the file
	iter->second.fileStreams[side]->write((char*)tcpData.getData(), tcpData.getDataLength());

	//处理数据包
	unsigned char *pbody = NULL;
	unsigned int bodylen = 0;
	CNetInfo nTemp;
	PacketAttach attach;
	pbody = (unsigned char*)tcpData.getData();
	bodylen = tcpData.getDataLength();
	nTemp.proto = TCP;
	if (side == 0)
	{
		//c->s
		if (tcpData.getConnectionData().srcIP->getType() == IPAddress::IPv4AddressType)
		{
			nTemp.srcip = ((IPv4Address*)(tcpData.getConnectionData().srcIP))->toInt();
		}
		else if (tcpData.getConnectionData().srcIP->getType() == IPAddress::IPv6AddressType)
		{
			//nTemp.srcip = ((IPv6Address*)(tcpData.getConnectionData().srcIP))->toIn6Addr();
		}
		nTemp.srcport = tcpData.getConnectionData().srcPort;
		if (tcpData.getConnectionData().dstIP->getType() == IPAddress::IPv4AddressType)
		{
			nTemp.dstip = ((IPv4Address*)(tcpData.getConnectionData().dstIP))->toInt();
		}
		else if (tcpData.getConnectionData().dstIP->getType() == IPAddress::IPv6AddressType)
		{
			//nTemp.srcip = ((IPv6Address*)(tcpData.getConnectionData().dstIP))->toIn6Addr();
		}
		nTemp.dstport = tcpData.getConnectionData().dstPort;
	}
	else
	{
		//s->c
		if (tcpData.getConnectionData().dstIP->getType() == IPAddress::IPv4AddressType)
		{
			nTemp.srcip = ((IPv4Address*)(tcpData.getConnectionData().dstIP))->toInt();
		}
		else if (tcpData.getConnectionData().dstIP->getType() == IPAddress::IPv6AddressType)
		{
			//nTemp.srcip = ((IPv6Address*)(tcpData.getConnectionData().dstIP))->toIn6Addr();
		}
		nTemp.srcport = tcpData.getConnectionData().dstPort;
		if (tcpData.getConnectionData().srcIP->getType() == IPAddress::IPv4AddressType)
		{
			nTemp.dstip = ((IPv4Address*)(tcpData.getConnectionData().srcIP))->toInt();
		}
		else if (tcpData.getConnectionData().srcIP->getType() == IPAddress::IPv6AddressType)
		{
			//nTemp.dstip = ((IPv6Address*)(tcpData.getConnectionData().srcIP))->toIn6Addr();
		}
		nTemp.dstport = tcpData.getConnectionData().srcPort;
	}
	attach.time = ((time_t)tcpData.getConnectionData().startTime.tv_sec) * 1000000 + tcpData.getConnectionData().startTime.tv_usec;
	CAnalysisCap *p = static_cast<CAnalysisCap*>(mMgr->thisdata);
	//attach.isStepFilter = false;
	p->EnterPacket(*(CSessions*)(mMgr->sessions), pbody, bodylen, nTemp, attach);
}

/**
* The callback being called by the TCP reassembly module whenever a new connection is found. This method adds the connection to the connection manager
*/
void CAnalysisCap::tcpReassemblyConnectionStartCallback(const ConnectionData& connectionData, void* userCookie)
{
	// get a pointer to the connection manager
	TcpReassemblyConnMgr* connMgr = (TcpReassemblyConnMgr*)userCookie;

	// look for the connection in the connection manager
	TcpReassemblyConnMgrIter iter = connMgr->find(connectionData.flowKey);

	// assuming it's a new connection
	if (iter == connMgr->end())
	{
		// add it to the connection manager
		connMgr->insert(std::make_pair(connectionData.flowKey, TcpReassemblyData()));
	}
}

/**
* The callback being called by the TCP reassembly module whenever a connection is ending. This method removes the connection from the connection manager and writes the metadata file if requested
* by the user
*/
void CAnalysisCap::tcpReassemblyConnectionEndCallback(const ConnectionData& connectionData, TcpReassembly::ConnectionEndReason reason, void* userCookie)
{
	// get a pointer to the connection manager
	TcpReassemblyConnMgr* connMgr = (TcpReassemblyConnMgr*)userCookie;

	// find the connection in the connection manager by the flow key
	TcpReassemblyConnMgrIter iter = connMgr->find(connectionData.flowKey);

	// connection wasn't found - shouldn't get here
	if (iter == connMgr->end())
		return;

	// write a metadata file if required by the user
	if (GlobalConfig::getInstance().writeMetadata)
	{
		std::string fileName = GlobalConfig::getInstance().getFileName(connectionData, 0, false) + "-metadata.txt";
		std::ofstream metadataFile(fileName.c_str());
		metadataFile << "Number of data packets in side 0:  " << iter->second.numOfDataPackets[0] << std::endl;
		metadataFile << "Number of data packets in side 1:  " << iter->second.numOfDataPackets[1] << std::endl;
		metadataFile << "Total number of data packets:      " << (iter->second.numOfDataPackets[0] + iter->second.numOfDataPackets[1]) << std::endl;
		metadataFile << std::endl;
		metadataFile << "Number of bytes in side 0:         " << iter->second.bytesFromSide[0] << std::endl;
		metadataFile << "Number of bytes in side 1:         " << iter->second.bytesFromSide[1] << std::endl;
		metadataFile << "Total number of bytes:             " << (iter->second.bytesFromSide[0] + iter->second.bytesFromSide[1]) << std::endl;
		metadataFile << std::endl;
		metadataFile << "Number of messages in side 0:      " << iter->second.numOfMessagesFromSide[0] << std::endl;
		metadataFile << "Number of messages in side 1:      " << iter->second.numOfMessagesFromSide[1] << std::endl;
		metadataFile.close();
	}

	// remove the connection from the connection manager
	connMgr->erase(iter);
}

void CAnalysisCap::PacketCenter(RawPacket &rawPacket, TcpReassembly &tcpReassembly,CSessions &mSessions)
{
	Packet packet(&rawPacket);

	if ((IPv6Layer*)packet.getLayerOfType(IPv6) != NULL)
	{
		//不支持ipv6
		return;
	}

	if (packet.getLayerOfType(TCP) != NULL)
	{
		//tcp协议
		tcpReassembly.reassemblePacket(&rawPacket);
	}
	else if (packet.getLayerOfType(UDP) != NULL)
	{
		//udp协议
		UdpLayer *udp = (UdpLayer*)packet.getLayerOfType(UDP);
		IPv4Layer *ip= (IPv4Layer*)packet.getLayerOfType(IPv4);
		if (ip != NULL&&udp != NULL)
		{
			//处理数据包
			unsigned char *pbody = NULL;
			unsigned int bodylen = 0;
			CNetInfo nTemp;
			PacketAttach attach;
			pbody = (unsigned char*)udp->getLayerPayload();
			bodylen = udp->getLayerPayloadSize();
			nTemp.proto = UDP;
			nTemp.srcip = ip->getIPv4Header()->ipSrc;
			nTemp.srcport = STswab16(udp->getUdpHeader()->portSrc);
			nTemp.dstip = ip->getIPv4Header()->ipDst;
			nTemp.dstport = STswab16(udp->getUdpHeader()->portDst);
			attach.time = (time_t)rawPacket.getPacketTimeStamp().tv_sec * 1000000 + rawPacket.getPacketTimeStamp().tv_usec;
			//attach.isStepFilter = false;
			EnterPacket(mSessions, pbody, bodylen, nTemp, attach);
		}
	}
	else
	{
		//不处理
	}
}

bool CAnalysisCap::doTcpReassemblyOnPcapFile(const char *fileName, CSessions &mSessions, std::string _plugin,std::string bpfFiler)
{
	plugin = _plugin;	
	mMgr.connMgr.clear();
	mMgr.sessions = &mSessions;

	// create the TCP reassembly instance
	mMgr.tcpReassembly=new TcpReassembly(tcpReassemblyMsgReadyCallback, &mMgr, tcpReassemblyConnectionStartCallback, tcpReassemblyConnectionEndCallback);

	// open input file (pcap or pcapng file)
	IFileReaderDevice* reader = IFileReaderDevice::getReader(fileName);

	// try to open the file device
	if (!reader->open())
	{
		//EXIT_WITH_ERROR("Cannot open pcap/pcapng file");
		return false;
	}

	// set BPF filter if set by the user
	if (bpfFiler != "")
	{
		if (!reader->setFilter(bpfFiler))
		{
			//EXIT_WITH_ERROR("Cannot set BPF filter to pcap file");
			return false;
		}
			
	}

	// run in a loop that reads one packet from the file in each iteration and feeds it to the TCP reassembly instance
	RawPacket rawPacket;
	while (reader->getNextPacket(rawPacket))
	{
		PacketCenter(rawPacket, *(mMgr.tcpReassembly), *(CSessions*)(mMgr.sessions));
	}
	// extract number of connections before closing all of them
	size_t numOfConnectionsProcessed = mMgr.tcpReassembly->getConnectionInformation().size();

	////处理流
	//if (mMgr.connMgr.size() > 0)
	//{
	//	TcpReassemblyConnMgrIter iter = mMgr.connMgr.begin();
	//	for (; iter != mMgr.connMgr.end(); iter++)
	//	{
	//		EnterConnection(iter->second, tcpReassembly.getConnectionInformation().at(iter->first), mSessions);
	//	}
	//}

	// after all packets have been read - close the connections which are still opened
	mMgr.tcpReassembly->closeAllConnections();

	delete mMgr.tcpReassembly;
	mMgr.tcpReassembly = NULL;

	// close the reader and free its memory
	reader->close();
	delete reader;

	return true;
}

/**
* packet capture callback - called whenever a packet arrives on the live device (in live device capturing mode)
*/
void CAnalysisCap::onPacketArrives(RawPacket* rawPacket, PcapLiveDevice* dev, void* userCookie)
{
	// get a pointer to the TCP reassembly instance and feed the packet arrived to it
	TcpReassemblyMgr* mMgr = (TcpReassemblyMgr*)userCookie;
	CAnalysisCap *p = static_cast<CAnalysisCap*>(mMgr->thisdata);
	p->PacketCenter(*rawPacket, *(mMgr->tcpReassembly),*(CSessions*)(mMgr->sessions));
}

/**
* The callback to be called when application is terminated by ctrl-c. Stops the endless while loop
*/
void CAnalysisCap::onApplicationInterrupted(void* cookie)
{
	bool* shouldStop = (bool*)cookie;
	*shouldStop = true;
}

bool CAnalysisCap::doTcpReassemblyOnLiveTraffic(const char *interfaceNameOrIP, CSessions &mSessions, std::string _plugin, std::string bpfFiler)
{
	plugin = _plugin;
	mMgr.connMgr.clear();
	mMgr.sessions = &mSessions;

	// create the TCP reassembly instance
	mMgr.tcpReassembly = new TcpReassembly(tcpReassemblyMsgReadyCallback, &mMgr, tcpReassemblyConnectionStartCallback, tcpReassemblyConnectionEndCallback);

	mMgr.dev = NULL;
	IPv4Address interfaceIP(interfaceNameOrIP);
	if (interfaceIP.isValid())
	{
		mMgr.dev = PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(interfaceIP);
		if (mMgr.dev == NULL)
		{
			return false;
			//EXIT_WITH_ERROR("Couldn't find interface by provided IP");
		}
			
	}
	else
	{
		mMgr.dev = PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(interfaceNameOrIP);
		if (mMgr.dev == NULL)
		{
			return false;
			//EXIT_WITH_ERROR("Couldn't find interface by provided name");
		}
	}
	// try to open device
	if (!mMgr.dev->open())
	{
		//EXIT_WITH_ERROR("Cannot open interface");
		return false;
	}

	// set BPF filter if set by the user
	if (bpfFiler != "")
	{
		if (!mMgr.dev->setFilter(bpfFiler))
		{
			//EXIT_WITH_ERROR("Cannot set BPF filter to interface");
			return false;
		}
	}

	return StartOpenSniffer("", mSessions, plugin);

	//// start capturing packets. Each packet arrived will be handled by onPacketArrives method
	//dev->startCapture(onPacketArrives, mMgr.tcpReassembly);

	//// register the on app close event to print summary stats on app termination
	//bool shouldStop = false;
	//ApplicationEventHandler::getInstance().onApplicationInterrupted(onApplicationInterrupted, &shouldStop);

	//// run in an endless loop until the user presses ctrl+c
	//while (!shouldStop)
	//	PCAP_SLEEP(1);

	//// stop capturing and close the live device
	//dev->stopCapture();
	//dev->close();

	//// close all connections which are still opened
	//mMgr.tcpReassembly->closeAllConnections();

	//delete mMgr.tcpReassembly;
	//mMgr.tcpReassembly = 0;
}

bool CAnalysisCap::isFileLoaded()
{
	return true;
}

bool CAnalysisCap::pcapOpen(const char *File,CSessions &mSessions,std::string _plugin)
{
	plugin = _plugin;
	char errbuf[100] = { 0 };
	pcap_t *fp= pcap_open_offline(File, errbuf);
	if (fp==NULL) return false;

	//开始处理packet
	bool result = true;
	int res = 0;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	while ((res = pcap_next_ex(fp, &header, &pkt_data)) >= 0)
	{
		/*cPacket* TestPacket = new cPacket((UCHAR*)pkt_data, header->len);
		unsigned char *pbody = NULL;
		unsigned int bodylen = 0;
		CNetInfo nTemp;
		PacketAttach attach;
		if (TestPacket->isUDPPacket)
		{
			pbody = TestPacket->UDPData;
			bodylen = TestPacket->UDPDataSize;
			nTemp.proto = TestPacket->IPHeader->Protocol;
			nTemp.srcip = TestPacket->IPHeader->SourceAddress;
			nTemp.srcport = STswab16(TestPacket->UDPHeader->SourcePort);
			nTemp.dstip = TestPacket->IPHeader->DestinationAddress;
			nTemp.dstport = STswab16(TestPacket->UDPHeader->DestinationPort);
			attach.time = ((STu64)header->ts.tv_sec)* 1000000+ header->ts.tv_usec;
		}
		else if (TestPacket->isTCPPacket)
		{
			pbody = TestPacket->TCPData;
			bodylen = TestPacket->TCPDataSize;
			nTemp.proto = TestPacket->IPHeader->Protocol;
			nTemp.srcip = TestPacket->IPHeader->SourceAddress;
			nTemp.srcport = STswab16(TestPacket->TCPHeader->SourcePort);
			nTemp.dstip = TestPacket->IPHeader->DestinationAddress;
			nTemp.dstport = STswab16(TestPacket->TCPHeader->DestinationPort);
			attach.time = ((STu64)header->ts.tv_sec)* 1000000+ header->ts.tv_usec;
		}
		else
		{
			delete(TestPacket);
			continue;
		}
		EnterPacket(mSessions,pbody, bodylen, nTemp,attach);
		delete(TestPacket);*/
	}
	if (fp)
	{
		pcap_close(fp);
		fp = NULL;
	}
	return true;
}

bool CAnalysisCap::EnterPacket(CSessions &mSessions,unsigned char *pbody, unsigned int bodylen, CNetInfo n, PacketAttach &attach)
{
	bool result = false;
	if (bodylen == 0) return true;

	bool isFilter = true;
	IAnalyzer *qq = AnalyzerFactory::CreateAnalyzerer(plugin.c_str());
	if (qq)
	{
		isFilter=qq->ForFilter(n.srcport, n.dstport,(char*)pbody, bodylen);
		delete qq;
	}
	if (isFilter)
	{
		//头部已经匹配到特征码的数据包
		ForInterfaceData(mSessions,pbody, bodylen, n,attach);
	}
	else
	{
		//头部没有匹配到的数据包，根据五元组查找是否在已经缓存的流中
		CSyncStream *p = mSessions.FindSameStreamByQuintet(n);
		if (p != NULL)
		{
			//在已缓存中流中查找到，查找最后一个包，是否已经结束
			if (p->FindLastPacket(n) != NULL)
			{
				//找到最后一个数据包
				if (p->FindLastPacket(n)->isStreamEnd ==true)
				{
					//最后一个数据包已经结束,重传的或者错误数据包
					//bug：还有一种情况是bug，就是包含特征码的数据包被截断在下一个包中，导致特征码判断失败
					return true;
				}
				else
				{
					//最后一个数据包未结束
					if (bodylen < p->FindLastPacket(n)->streamResiduelen)
					{
						p->FindLastPacket(n)->_payload.append(pbody, bodylen);
						p->FindLastPacket(n)->isStreamEnd = false;
						p->FindLastPacket(n)->streamResiduelen -= bodylen;
					}
					else if (bodylen == p->FindLastPacket(n)->streamResiduelen)
					{
						p->FindLastPacket(n)->_payload.append(pbody, bodylen);
						p->FindLastPacket(n)->isStreamEnd = true;
					}
					else if (bodylen > p->FindLastPacket(n)->streamResiduelen)
					{
						p->FindLastPacket(n)->_payload.append(pbody, p->FindLastPacket(n)->streamResiduelen);
						p->FindLastPacket(n)->isStreamEnd = true;
						//新的数据包
						//attach.isStepFilter = true;
						EnterPacket(mSessions, pbody + p->FindLastPacket(n)->streamResiduelen, bodylen - p->FindLastPacket(n)->streamResiduelen, n, attach);
					}
				}
			}
			else
			{
				//第一包为断头流，则丢弃
				return true;
			}
		}
	}
	return result;
}

bool CAnalysisCap::ForInterfaceData(CSessions &mSessions,unsigned char *pbody, unsigned int bodylen, CNetInfo n, PacketAttach &attach)
{
	unsigned int len = bodylen;
	IAnalyzer *qq = AnalyzerFactory::CreateAnalyzerer(plugin.c_str());
	if (qq)
	{
		bool isClient2Server = qq->IsClient2Server(n.srcport, n.dstport);
		len = qq->ActualLen((char*)pbody,bodylen,isClient2Server);
		delete qq;
	}
	if (len == bodylen)
	{
		CSyncStream *p = mSessions.FindSameStreamByQuintet(n);
		if (p != NULL)
		{
			CSyncPacket mSyncPacket;
			mSyncPacket.mNetInfo = n;
			mSyncPacket.isStreamEnd = true;
			mSyncPacket.time = attach.time;
			mSyncPacket._payload.append(pbody, bodylen);
			mSessions.AddPacket(p->guid, mSyncPacket);
		}
		else
		{
			CSyncPacket mSyncPacket;
			mSyncPacket.mNetInfo = n;
			mSyncPacket.isStreamEnd = true;
			mSyncPacket.time = attach.time;
			mSyncPacket._payload.append(pbody, bodylen);
			mSessions.AddNewPacket(mSyncPacket);
		}
	}
	else if (len < bodylen)
	{
		CombinPacket(mSessions,pbody, bodylen, n,attach);
	}
	else
	{
		//合并包
		CSyncStream *p = mSessions.FindSameStreamByQuintet(n);
		if (p != NULL)
		{
			CSyncPacket mSyncPacket;
			mSyncPacket.mNetInfo = n;
			mSyncPacket.isStreamEnd = false;
			mSyncPacket.time = attach.time;
			mSyncPacket.streamResiduelen = len - bodylen;
			mSyncPacket._payload.append(pbody, bodylen);
			//p->AddPacket(mSyncPacket);
			mSessions.AddPacket(p->guid, mSyncPacket);
		}
		else
		{
			CSyncPacket mSyncPacket;
			mSyncPacket.mNetInfo = n;
			mSyncPacket.isStreamEnd = false;
			mSyncPacket.time = attach.time;
			mSyncPacket.streamResiduelen = len - bodylen;
			mSyncPacket._payload.append(pbody, bodylen);
			mSessions.AddNewPacket(mSyncPacket);
		}
	}

	return true;
}

bool CAnalysisCap::CombinPacket(CSessions &mSessions,unsigned char *pbody, unsigned int bodylen, CNetInfo n, PacketAttach& attach)
{
	//len<bodylen
	CSyncStream *p = mSessions.FindSameStreamByQuintet(n);
	unsigned int len = bodylen;
	IAnalyzer *qq = AnalyzerFactory::CreateAnalyzerer(plugin.c_str());
	if (qq)
	{
		bool isClient2Server = qq->IsClient2Server(n.srcport, n.dstport);
		len = qq->ActualLen((char*)pbody,bodylen, isClient2Server);
		delete qq;
	}
	if (p != NULL)
	{
		CSyncPacket mSyncPacket;
		mSyncPacket.mNetInfo = n;
		mSyncPacket.isStreamEnd = true;
		mSyncPacket.time = attach.time;
		mSyncPacket._payload.append(pbody, len);
		//attach.isStepFilter = true;
		EnterPacket(mSessions,pbody + len, bodylen - len, n,attach);
	}
	else
	{
		CSyncPacket mSyncPacket;
		mSyncPacket.mNetInfo = n;
		mSyncPacket.isStreamEnd = true;
		mSyncPacket.time = attach.time;
		mSyncPacket.streamResiduelen = 0;
		mSyncPacket._payload.append(pbody, len);
		mSessions.AddNewPacket(mSyncPacket);
		//attach.isStepFilter = true;
		EnterPacket(mSessions,pbody + len, bodylen - len, n, attach);
	}
	return true;
}

std::map<std::string, std::string> CAnalysisCap::PacketAnalysis(const char *pbody, const unsigned int bodylen, const unsigned int srcPort, const unsigned int dstPort)
{
	std::map<std::string, std::string> result;
	IAnalyzer *qq = AnalyzerFactory::CreateAnalyzerer(plugin.c_str());
	if (qq)
	{
		bool isClient2Server = qq->IsClient2Server(srcPort, dstPort);
		result = qq->Analysis((char*)pbody, bodylen, isClient2Server);
		delete qq;
	}
	return result;
}

std::map<std::string, std::string> CAnalysisCap::PacketAnalysis(std::list<CSyncPacket> &packets)
{
	std::map<std::string, std::string> result;
	IAnalyzer *qq = AnalyzerFactory::CreateAnalyzerer(plugin.c_str());
	if (qq)
	{
		std::list<IAnalyzerData> mypackets;
		std::list<CSyncPacket>::iterator iter = packets.begin();
		for (; iter != packets.end(); iter++)
		{
			IAnalyzerData packet;
			packet.payload.append((char*)iter->_payload.contents(), iter->_payload.size());;
			packet.srcPort = iter->mNetInfo.srcport;
			packet.dstPort = iter->mNetInfo.dstport;
			mypackets.push_back(packet);
		}
		result = qq->AnalysisList(mypackets);
		delete qq;
	}
	return result;
}

std::vector<NetCardInfo>& CAnalysisCap::LoadNetDevs()
{
	const std::vector<PcapLiveDevice*> list = PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
	for (int i = 0; i < list.size(); i++)
	{
		NetCardInfo info;
		info.name = list[i]->getName();
		info.description = list[i]->getDesc();
		info.netmask = list[i]->getDefaultGateway().toInt();
		devs.push_back(info);
	}
	return devs;
}

bool CAnalysisCap::StartOpenSniffer(const char *name,CSessions &mSessions, std::string _plugin)
{
	isSniffing = mMgr.dev->startCapture(onPacketArrives, &mMgr);
	return isSniffing;

	//plugin = _plugin;
	//mSniffer.mSnifferPackets.ClearPackets();
	//int sel = 0;
	//for (UINT i = 0; i< mSniffer.devs.size(); i++)
	//{
	//	if (strcmp(mSniffer.devs[i].description.c_str(), name) == 0)
	//		sel = i;
	//}
	//mSniffer.pSession = &mSessions;
	//mSniffer.devIndex = sel;
	////需要开启一个线程
	//_beginthreadex(NULL, 0, snifferThreadFunc, this, 0, 0);
	//return true;
}

void CAnalysisCap::StopOpenSniffer()
{
	// stop capturing and close the live device
	mMgr.dev->stopCapture();
	mMgr.dev->close();

	// close all connections which are still opened
	mMgr.tcpReassembly->closeAllConnections();

	delete mMgr.tcpReassembly;
	mMgr.tcpReassembly = 0;

	isSniffing = false;
	////先结束线程
	//mSniffer.StopCapture();
}

bool CAnalysisCap::IsSniffing()
{
	return isSniffing;
}