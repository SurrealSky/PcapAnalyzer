#pragma once
#include"packet.h"
#include"Sessions.h"
#include<map>
#include<string>
#include<typedef.h>
#include<vector>
using namespace SurrealTypes;

//#ifdef _AFXDLL
//#define SURREAL_EXPORTS 1
//#else
//#undef  SURREAL_EXPORTS
//#endif

#ifdef SURREAL_EXPORTS
#define SURREALEXPORTS_PCAPANALYZER_API __declspec(dllexport)
#else
#define SURREALEXPORTS_PCAPANALYZER_API __declspec(dllimport)
#endif

class CAnalysisCap;
class XmlFilter;

class /*SURREALEXPORTS_PCAPANALYZER_API*/ PcapAnalyzer
{
public:
	explicit PcapAnalyzer();
	~PcapAnalyzer();
private:
	//CSessions	mSessions;
	CAnalysisCap * pImplAnalysis;
	XmlFilter * pImplFilter;
public:
	static std::vector<std::string> GetAllPlugins();
public:
	/*
	*作用：调用pcap原始库进行组流，并处理业务数据包
	*/
	STbool OpenPcapFileByPacket(CSessions&,std::string,std::string plugin="");
	/*
	*作用：调用第三方库进行组流,未处理业务数据包
	*/
	STbool OpenPcapFileByStream(CSessions&,std::string,std::string plugin="");
	STbool isFileLoaded();
private:
	//xml操作
	STbool ReadActionsFromXML(std::string file, std::list<CommonAction> &mActions);
	STbool ReadFiltersFromXML(std::string file, std::string action, std::string filterindex);
public:
	STbool ForDisplayFilter(const STu8*, const STu32 size, std::string exp);
public:
	std::list<std::string> LoadExpression();
	STbool IsValidateeExpression(std::string);
	std::string FormatTime(time_t time);
	std::string IPProtocol2String(STu16 protocol);
	std::string Protocol2String(STu16 protocol);
public:
	//sniffer
	static void LoadNetDevs(std::vector<NetCardInfo>&);
	bool StartOpenSniffer(CSessions &mSessions, const char * dev, std::string plugin = "");
	void StopOpenSniffer();
	bool IsSniffing();
public:
	//分析
	std::map<std::string, std::string> PacketAnalysis(ByteBuffer &payload, const unsigned int srcPort, const unsigned int dstPort);
	std::map<std::string, std::string> PacketAnalysis(std::list<CSyncPacket> &packets);
};

