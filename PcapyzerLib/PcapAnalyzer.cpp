#include "PcapAnalyzer.h"
#include"AnalysisCap.h"
#include"XmlFilter.h"
#include"PacketCapture.h"

PcapAnalyzer::PcapAnalyzer()
{
	pImplAnalysis = new CAnalysisCap();
	pImplFilter = new XmlFilter();
}

PcapAnalyzer::~PcapAnalyzer()
{
	if (pImplAnalysis)
	{
		delete pImplAnalysis;
		pImplAnalysis = 0;
	}
	if (pImplFilter)
	{
		delete pImplFilter;
		pImplFilter = 0;
	}
}

std::vector<std::string> PcapAnalyzer::GetAllPlugins()
{
	return CAnalysisCap::GetAllPlugins();
}

STbool PcapAnalyzer::OpenPcapFile(CSessions &mSessions,std::string file, std::string plugin)
{
	mSessions.ClearStream();
	return pImplAnalysis->doTcpReassemblyOnPcapFile(file.c_str(),mSessions,plugin);
}

STbool PcapAnalyzer::isFileLoaded()
{
	return pImplAnalysis->isFileLoaded();
}

STbool PcapAnalyzer::ReadActionsFromXML(std::string file, std::list<CommonAction> &mActions)
{
	return pImplFilter->ReadActionsFromXML(file, mActions);
}

STbool PcapAnalyzer::ReadFiltersFromXML(std::string file, std::string action, std::string filterindex)
{
	return pImplFilter->ReadFiltersFromXML(file, action, filterindex);
}

STbool PcapAnalyzer::ForDisplayFilter(const STu8 *data, const STu32 size, std::string exp)
{
	return pImplFilter->ForFilter(data, size, exp);
}

std::list<std::string> PcapAnalyzer::LoadExpression()
{
	return pImplFilter->LoadRegexExamples();
}

STbool PcapAnalyzer::IsValidateeExpression(std::string exp)
{
	return pImplFilter->IsValidateeExpression(exp);
}

std::string PcapAnalyzer::FormatTime(time_t time)
{
	return pImplFilter->FormatTime(time);
}

std::string PcapAnalyzer::IPProtocol2String(STu16 protocol)
{
	/*switch (protocol)
	{
	case TCP_PACKET:
	{
		return "TCP";
	}break;
	case UDP_PACKET:
	{
		return "UDP";
	}break;
	case ICMP_PACKET:
	{
		return "ICMP";
	}break;
	case IGMP_PACKET:
	{
		return "IGMP";
	}break;
	default:
	{
		return "UnknowProtocol";
	}break;
	}*/
	return "";
}

std::string PcapAnalyzer::Protocol2String(STu16 protocol)
{
	/*switch (protocol)
	{
	case ETHERTYPE_IP:
	{
		return "IPv4";
	}break;
	case ETHERTYPE_ARP:
	{
		return "ARP";
	}break;
	case ETHERTYPE_IPV6:
	{
		return "IPv6";
	}break;
	case TCP_PACKET:
	{
		return "TCP";
	}break;
	case UDP_PACKET:
	{
		return "UDP";
	}break;
	default:
	{
		return "UnknowProtocol";
	}break;
	}*/
	return "";
}

std::map<std::string,std::string> PcapAnalyzer::PacketAnalysis(ByteBuffer &payload,const unsigned int srcPort,const unsigned int dstPort)
{
	return pImplAnalysis->PacketAnalysis((char*)payload.contents(), payload.size(),srcPort,dstPort);
}

std::map<std::string, std::string> PcapAnalyzer::PacketAnalysis(std::list<CSyncPacket> &packets)
{
	return pImplAnalysis->PacketAnalysis(packets);
}

void PcapAnalyzer::LoadNetDevs(std::vector<NetCardInfo> &devs)
{
	CPacketCapture::LoadNetDevs(devs);
}

bool PcapAnalyzer::StartOpenSniffer(CSessions &mSessions,const char * name, std::string _plugin)
{
	return pImplAnalysis->StartOpenSniffer(name,mSessions, _plugin);
}

void PcapAnalyzer::StopOpenSniffer()
{
	//先结束线程
	pImplAnalysis->StopOpenSniffer();
}

bool PcapAnalyzer::IsSniffing()
{
	return pImplAnalysis->IsSniffing();
}