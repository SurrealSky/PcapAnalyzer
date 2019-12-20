/*
 *
 *  Copyright (C) 2013  Anwar Mohamed <anwarelmakrahy[at]gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to Anwar Mohamed
 *  anwarelmakrahy[at]gmail.com
 *
 */

#pragma once
#include "Packetyzer.h"

using namespace Packetyzer::Traffic::Connections;

struct NETWORK_ADAPTERS_CAPTURE
{
	CHAR Name[200];
	CHAR ID[200];
};

class DLLEXPORT Packetyzer::Capture::cWinpcapCapture
{
	//BOOL CheckAdapter(UINT id);
	//BOOL InitializeAdaptersList();
	void AnalyzeTraffic();

	#define LINE_LEN 16
	pcap_if_t *alldevs, *d;
	pcap_t *fp;
	int res;
	struct pcap_pkthdr * PacketHeader;
	const u_char * PacketData;
	CHAR errbuf[PCAP_ERRBUF_SIZE];

	BOOL InitializeAdapters();
public:
	BOOL isReady;
	BOOL CapturePackets(UINT AdapterIndex, UINT MaxNumOfPackets, const CHAR* Filter = NULL);

	NETWORK_ADAPTERS_CAPTURE *Adapters;
	UINT nAdapters;

	//cPacket* CapturedPackets;
	UINT nCapturedPackets;
	
	cTraffic Traffic;

	cWinpcapCapture();
	~cWinpcapCapture();
};
