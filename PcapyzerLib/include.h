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

#include<Packetyzer/Packetyzer.h>
#ifdef _DEBUG 
#pragma comment(lib, "debug\\Packetyzer.lib")
#else
#pragma comment(lib, "release\\Packetyzer.lib")
#endif

using namespace Packetyzer::Traffic::Connections;