#include "PacketCapture.h"

using namespace Packetyzer::Analyzers;
using namespace Packetyzer::Capture;
using namespace std;

CPacketCapture::CPacketCapture()
{
	pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &allDevs, errbuf);
	pcap_if_t *dev;
	for (dev = allDevs; dev->next != NULL; dev = dev->next) 
	{
		NetCardInfo info;
		info.name = dev->name;
		info.description = dev->description;
		if (dev->addresses != NULL)
			info.netmask = ((struct sockaddr_in*)(dev->addresses->netmask))->sin_addr.S_un.S_addr;
		else
			info.netmask = 0xffffff;
		devs.push_back(info);
	}
}


CPacketCapture::~CPacketCapture()
{
}

BOOL CPacketCapture::CapturePackets(LPVOID uParam, packet_call_handler pFunc, UINT AdapterIndex, UINT MaxNumOfPackets, const CHAR* Filter = NULL)
{

	INT retValue;UINT n = 0; nCapturedPackets = 0;

	if (AdapterIndex< 0 || AdapterIndex > devs.size()-1) return FALSE;
	if ((fp = pcap_open(devs[AdapterIndex].name.c_str(), 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf)) == NULL) return FALSE;

	UINT netmask = devs[AdapterIndex].netmask;
	struct bpf_program fcode;
	if (pcap_compile(fp, &fcode, Filter, 1, netmask) < 0) return FALSE;
	if (pcap_setfilter(fp, &fcode) < 0) return FALSE;

	m_hSnifferEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hListenEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	while ((retValue = pcap_next_ex(fp, &PacketHeader, &PacketData)) >= 0 && n < MaxNumOfPackets)
	{

		DWORD dwRetVal;
		dwRetVal = WaitForSingleObject(m_hSnifferEvent, 100);
		if (dwRetVal == WAIT_TIMEOUT)
		{
			// TODO:
			if (retValue == 0)
				continue;
			n++;
			unsigned long long time = ((unsigned long long)PacketHeader->ts.tv_sec) * 1000000 + PacketHeader->ts.tv_usec;
			pFunc(uParam, PacketData, PacketHeader->len,time);
			nCapturedPackets++;
		}
		else
		{
			SetEvent(m_hListenEvent);
			break;
		}
	}
	if (retValue == -1) return FALSE;
	return TRUE;
}

BOOL CPacketCapture::StopCapture()
{
	SetEvent(m_hSnifferEvent);
	CloseHandle(m_hSnifferEvent);
	DWORD dwRetVal = WaitForSingleObject(m_hListenEvent, INFINITE);
	CloseHandle(m_hListenEvent);
	return TRUE;
}