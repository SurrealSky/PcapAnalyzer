#pragma once
#ifdef _DEBUG 
#pragma comment(lib, "debug\\wpcap.lib")
#else
#pragma comment(lib, "release\\wpcap.lib")
#endif
#ifdef _DEBUG 
#pragma comment(lib, "debug\\Packet.lib")
#else
#pragma comment(lib, "release\\Packet.lib")
#endif

#include<iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")

#include <pcapplusplus20.08/TcpReassembly.h>
#include<pcapplusplus20.08\IPv4Layer.h>
#include<pcapplusplus20.08\IPv6Layer.h>
#include<pcapplusplus20.08\Packet.h>
#include<pcapplusplus20.08\PcapFileDevice.h>
#include<pcapplusplus20.08\TcpReassembly.h>
#include<pcapplusplus20.08\PcapLiveDevice.h>
#include<pcapplusplus20.08\LRUList.h>
#include<pcapplusplus20.08\PcapLiveDeviceList.h>
#include<pcapplusplus20.08\IpUtils.h>
#include<pcapplusplus20.08\SystemUtils.h>
#include<pcapplusplus20.08\PlatformSpecificUtils.h>
#include<pcapplusplus20.08\UdpLayer.h>

using namespace pcpp;

#ifdef _DEBUG 
#pragma comment(lib,"debug\\pthreadVC2.lib")
#pragma comment(lib, "debug\\Common++.lib")
#pragma comment(lib, "debug\\Packet++.lib")
#pragma comment(lib, "debug\\Pcap++.lib")
#else
#pragma comment(lib,"release\\pthreadVC2.lib")
#pragma comment(lib, "release\\Common++.lib")
#pragma comment(lib, "release\\Packet++.lib")
#pragma comment(lib, "release\\Pcap++.lib")
#endif
