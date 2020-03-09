#pragma once
#include<pcap.h>
//#include<Packetyzer/Packetyzer.h>
#include"SyncStream.h"
#include"Sessions.h"

typedef void(*packet_call_handler)(void* uParam, const unsigned char *pkt_data, unsigned int len,unsigned long long time);

class CPacketCapture
{
public:
	CPacketCapture();
	~CPacketCapture();
private:
	pcap_t *fp;
	int res;
	struct pcap_pkthdr * PacketHeader;
	const u_char * PacketData;
private:
	HANDLE m_hSnifferEvent;
	HANDLE m_hListenEvent;
public:
	static void LoadNetDevs(std::vector<NetCardInfo> &devs);
	BOOL CapturePackets(LPVOID uParam, packet_call_handler pFunc, UINT AdapterIndex, UINT MaxNumOfPackets, const CHAR* Filter);
	BOOL StopCapture();
public:
	CSessions	*pSession;
	STu8	devIndex;
	CSyncStream mSnifferPackets;
	STu64	nCapturedPackets;
	static std::vector<NetCardInfo> devs;
	STbool	isSniffing;
};

