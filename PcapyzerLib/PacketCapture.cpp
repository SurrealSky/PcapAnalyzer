#include "PacketCapture.h"
#include<LogLib\DebugLog.h>
#include<iphlpapi.h>

#pragma comment(lib,"Iphlpapi.lib")

using namespace Packetyzer::Analyzers;
using namespace Packetyzer::Capture;
using namespace std;

std::vector<NetCardInfo> CPacketCapture::devs;

CPacketCapture::CPacketCapture()
{
	isSniffing = false;
}


CPacketCapture::~CPacketCapture()
{
}

void CPacketCapture::LoadNetDevs(std::vector<NetCardInfo> &tempdevs)
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i;

	/* variables used to print DHCP time info */
	struct tm newtime;
	char buffer[32];
	errno_t error;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			NetCardInfo info;
			info.name = "rpcap://\\Device\\NPF_";
			info.name.append(pAdapter->AdapterName);
			info.description = pAdapter->Description;
			//info.netmask = pAdapter->IpAddressList.IpMask.String;
			info.netmask = 0;
			pAdapter = pAdapter->Next;
			CPacketCapture::devs.push_back(info);
		}
	}
	tempdevs = CPacketCapture::devs;

	//以下代码不知为什么有时候无法获取到完整的网卡列表
	//pcap_if_t *allDevs;
	//CHAR errbuf[PCAP_ERRBUF_SIZE] = {0};
	////pcap_findalldevs(&allDevs, errbuf);
	//pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &allDevs, errbuf);
	//pcap_if_t *dev;
	//for (dev = allDevs; dev->next != NULL; dev = dev->next)
	//{
	//	NetCardInfo info;
	//	info.name = dev->name;
	//	std::string str = dev->description;
	//	size_t first = str.find_first_of('\'');
	//	size_t last = str.find_last_of('\'');
	//	str = str.substr(first+1, last - first-1);
	//	info.description = str;
	//	if (dev->addresses != NULL)
	//		info.netmask = ((struct sockaddr_in*)(dev->addresses->netmask))->sin_addr.S_un.S_addr;
	//	else
	//		info.netmask = 0xffffff;
	//	CPacketCapture::devs.push_back(info);
	//}
	//pcap_freealldevs(allDevs);
	//tempdevs = CPacketCapture::devs;
}

BOOL CPacketCapture::CapturePackets(LPVOID uParam, packet_call_handler pFunc, UINT AdapterIndex, UINT MaxNumOfPackets, const CHAR* Filter = NULL)
{
	INT retValue;UINT n = 0; nCapturedPackets = 0;

	if (AdapterIndex< 0 || AdapterIndex > devs.size()-1) return FALSE;
	if ((fp = pcap_open(devs[AdapterIndex].name.c_str(), 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, NULL)) == NULL) return FALSE;

	UINT netmask = devs[AdapterIndex].netmask;
	struct bpf_program fcode;
	if (pcap_compile(fp, &fcode, Filter, 1, netmask) < 0) return FALSE;
	if (pcap_setfilter(fp, &fcode) < 0) return FALSE;

	m_hSnifferEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hListenEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	while ((retValue = pcap_next_ex(fp, &PacketHeader, &PacketData)) >= 0 && n < MaxNumOfPackets)
	{
		isSniffing = true;
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
	isSniffing = false;
	if (retValue == -1) return FALSE;
	return TRUE;
}

BOOL CPacketCapture::StopCapture()
{
	SetEvent(m_hSnifferEvent);
	DWORD dwRetVal = WaitForSingleObject(m_hListenEvent, INFINITE);
	CloseHandle(m_hListenEvent);
	CloseHandle(m_hSnifferEvent);
	isSniffing = false;
	return TRUE;
}