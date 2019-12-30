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
	//ʹ��Packetyzer::Analyzers::cPcapFile�Զ������ٷ���
	bool capOpen(const char *, CSessions &,std::string plugin);
private:
	void EnterConnect(cConnection *, CSessions &);
public:
	//ʹ��pcapԭʼ�⺯�����з���
	bool pcapOpen(const char *File, CSessions&, std::string plugin);
public:
	bool isFileLoaded();
private:
	/***********************�������԰����кϲ��ͷֿ�********************************/
	//������CombinPacket���õģ���������ʼ�Ѿ�������ForInterfaceMBData���ж�
	bool EnterPacket(CSessions &mSessions,unsigned char *data, unsigned int size, CNetInfo n, PacketAttach&);
	bool ForInterfaceData(CSessions &mSessions,unsigned char *data, unsigned int size, CNetInfo n, PacketAttach&);
	bool CombinPacket(CSessions &mSessions,unsigned char *data, unsigned int size, CNetInfo, PacketAttach&);
	/***********************�������԰����кϲ��ͷֿ�********************************/
public:
	//���ݲ���������ݰ�
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


