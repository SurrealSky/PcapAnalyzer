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

//#include<Packetyzer/Packetyzer.h>
//#ifdef _DEBUG 
//#pragma comment(lib, "debug\\Packetyzer.lib")
//#else
//#pragma comment(lib, "release\\Packetyzer.lib")
//#endif
//
//using namespace Packetyzer::Traffic::Connections;

#include<pcapplusplus19.12\IPv4Layer.h>
#include<pcapplusplus19.12\Packet.h>
#include<pcapplusplus19.12\PcapFileDevice.h>
#include<pcapplusplus19.12\TcpReassembly.h>
#include<pcapplusplus19.12\PcapLiveDevice.h>
#include<pcapplusplus19.12\LRUList.h>
#include<pcapplusplus19.12\PcapLiveDeviceList.h>
#include<pcapplusplus19.12\IpUtils.h>
#include<pcapplusplus19.12\SystemUtils.h>
#include<pcapplusplus19.12\PlatformSpecificUtils.h>

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
