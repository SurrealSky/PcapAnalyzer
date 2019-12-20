#pragma once
#include<pcap.h>
#include<Packetyzer/Packetyzer.h>
#include"SyncStream.h"
#include"Sessions.h"

typedef void(*packet_call_handler)(void* uParam, const unsigned char *pkt_data, unsigned int len,unsigned long long time);

class CPacketCapture
{
public:
	CPacketCapture();
	~CPacketCapture();
private:
	pcap_if_t *allDevs;
	pcap_t *fp;
	int res;
	struct pcap_pkthdr * PacketHeader;
	const u_char * PacketData;
	CHAR errbuf[PCAP_ERRBUF_SIZE];
private:
	HANDLE m_hSnifferEvent;
	HANDLE m_hListenEvent;
public:
	BOOL CapturePackets(LPVOID uParam, packet_call_handler pFunc, UINT AdapterIndex, UINT MaxNumOfPackets, const CHAR* Filter);
	BOOL StopCapture();
public:
	CSessions	*pSession;
	STu8	devIndex;
	CSyncStream mSnifferPackets;
	STu64	nCapturedPackets;
	std::vector<NetCardInfo> devs;
};

