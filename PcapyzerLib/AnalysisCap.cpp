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


// CAnalysisCap

CAnalysisCap::CAnalysisCap()
{
	//fp = NULL;
	plugin = "";
	//加载协议解析器
	PluginManager::GetInstance().LoadAll();
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

bool CAnalysisCap::capOpen(const char *File, CSessions &mSessions, std::string _plugin)
{
	plugin = _plugin;
	Packetyzer::Analyzers::cPcapFile* pckts = new Packetyzer::Analyzers::cPcapFile((char*)File);
	if (!pckts->FileLoaded)
	{
		delete pckts;
		pckts = NULL;
		return false;
	}
	for (STu32 i = 0; i < pckts->Traffic->nConnections; i++)
	{
		EnterConnect(pckts->Traffic->Connections[i], mSessions);
	}
	delete pckts;
	pckts = NULL;
	return true;
}

void CAnalysisCap::EnterConnect(cConnection *con, CSessions &mSessions)
{
	for (STu32 packetnum = 0; packetnum < con->nPackets; packetnum++)
	{
		unsigned char *pbody = NULL;
		unsigned int bodylen = 0;
		CNetInfo nTemp;
		PacketAttach attach;
		if (con->Packets[packetnum]->isUDPPacket)
		{

			pbody = con->Packets[packetnum]->UDPData;
			bodylen = con->Packets[packetnum]->UDPDataSize;
			nTemp.proto = con->Packets[packetnum]->IPHeader->Protocol;
			nTemp.srcip = con->Packets[packetnum]->IPHeader->SourceAddress;
			nTemp.srcport = STswab16(con->Packets[packetnum]->UDPHeader->SourcePort);
			nTemp.dstip = con->Packets[packetnum]->IPHeader->DestinationAddress;
			nTemp.dstport = STswab16(con->Packets[packetnum]->UDPHeader->DestinationPort);
			attach.time = con->Packets[packetnum]->Timestamp;
		}
		else if (con->Packets[packetnum]->isTCPPacket)
		{
			pbody = con->Packets[packetnum]->TCPData;
			bodylen = con->Packets[packetnum]->TCPDataSize;
			nTemp.proto = con->Packets[packetnum]->IPHeader->Protocol;
			nTemp.srcip = con->Packets[packetnum]->IPHeader->SourceAddress;
			nTemp.srcport = STswab16(con->Packets[packetnum]->TCPHeader->SourcePort);
			nTemp.dstip = con->Packets[packetnum]->IPHeader->DestinationAddress;
			nTemp.dstport = STswab16(con->Packets[packetnum]->TCPHeader->DestinationPort);
			attach.time = con->Packets[packetnum]->Timestamp;
		}
		else
		{
			continue;
		}
		EnterPacket(mSessions,pbody, bodylen, nTemp,attach);
	}
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
		cPacket* TestPacket = new cPacket((UCHAR*)pkt_data, header->len);
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
		delete(TestPacket);
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
		//五元组是否已在session中
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
		EnterPacket(mSessions,pbody + len, bodylen - len, n, attach);
	}
	return true;
}

std::map<std::string, std::string> CAnalysisCap::PacketAnalysis(const char *pbody, const unsigned int bodylen, const unsigned int srcPort, const unsigned int dstPort)
{
	std::map<std::string,std::string> result;
	IAnalyzer *qq = AnalyzerFactory::CreateAnalyzerer(plugin.c_str());
	if (qq)
	{
		bool isClient2Server = qq->IsClient2Server(srcPort, dstPort);
		result = qq->Analysis((char*)pbody, bodylen, isClient2Server);
		delete qq;
	}
	return result;
}

void CAnalysisCap::LoadNetDevs(std::vector<NetCardInfo> &devs)
{
	devs=CPacketCapture::devs;
}

bool CAnalysisCap::StartOpenSniffer(const char * name, CSessions &mSessions, std::string _plugin)
{
	plugin = _plugin;
	mSniffer.mSnifferPackets.ClearPackets();
	int sel = 0;
	for (UINT i = 0; i< mSniffer.devs.size(); i++)
	{
		if (strcmp(mSniffer.devs[i].description.c_str(), name) == 0)
			sel = i;
	}
	mSniffer.pSession = &mSessions;
	mSniffer.devIndex = sel;
	//需要开启一个线程
	_beginthreadex(NULL, 0, snifferThreadFunc, this, 0, 0);
	return true;
}

void CAnalysisCap::StopOpenSniffer()
{
	//先结束线程
	mSniffer.StopCapture();
}

bool CAnalysisCap::IsSniffing()
{
	return mSniffer.isSniffing;
}

unsigned int __stdcall CAnalysisCap::snifferThreadFunc(void* pParam)
{
	// TODO:
	CAnalysisCap *p = (CAnalysisCap*)pParam;
	p->mSniffer.CapturePackets(pParam, packet_handler, p->mSniffer.devIndex, -1, 0);
	return 0;
}

void CAnalysisCap::packet_handler(void* uParam, const unsigned char *pkt_data, unsigned int len,unsigned long long time)
{
	CAnalysisCap* pAnalysis = (CAnalysisCap*)uParam;
	//开始处理packet
	cPacket* TestPacket = new cPacket((UCHAR*)pkt_data, len);
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
		attach.time = time;
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
		attach.time = time;
	}
	else
	{
		delete(TestPacket);
		return;
	}
	pAnalysis->EnterPacket(*(pAnalysis->mSniffer.pSession), pbody, bodylen, nTemp, attach);
	delete(TestPacket);
}
