#pragma once
#include"packet.h"
#include"Sessions.h"
#include"PacketCapture.h"
#include"include.h"

class CAnalysisCap
{

public:
	CAnalysisCap();
	virtual ~CAnalysisCap();
private:
	std::string plugin;
public:
	static std::vector<std::string> GetAllPlugins();
public:
	//使用Packetyzer::Analyzers::cPcapFile自动组流再分析
	bool capOpen(const char *, CSessions &,std::string plugin);
private:
	void EnterConnect(cConnection *, CSessions &);
public:
	//使用pcap原始库函数进行分析
	bool pcapOpen(const char *File, CSessions&, std::string plugin);
public:
	bool isFileLoaded();
private:
	/***********************组流并对包进行合并和分开********************************/
	//比如由CombinPacket调用的，整个包开始已经经过了ForInterfaceMBData的判断
	bool EnterPacket(CSessions &mSessions,unsigned char *data, unsigned int size, CNetInfo n, PacketAttach&);
	bool ForInterfaceData(CSessions &mSessions,unsigned char *data, unsigned int size, CNetInfo n, PacketAttach&);
	bool CombinPacket(CSessions &mSessions,unsigned char *data, unsigned int size, CNetInfo, PacketAttach&);
	/***********************组流并对包进行合并和分开********************************/
public:
	//根据插件分析数据包
	std::map<std::string, std::string> PacketAnalysis(const char *data, const unsigned int size,const unsigned int srcPort, const unsigned int dstPort);
private:
	CPacketCapture mSniffer;
public:
	static void LoadNetDevs(std::vector<NetCardInfo>&);
	bool StartOpenSniffer(const char * name, CSessions &, std::string plugin);
	void StopOpenSniffer();
	bool IsSniffing();
private:
	static unsigned int __stdcall snifferThreadFunc(void* pParam);
	static void packet_handler(void* uParam, const unsigned char *pkt_data, unsigned int len,unsigned long long time);
};


