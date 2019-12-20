#pragma once
#include<vector>
#include"packet.h"
#include "LayerFilter.h"

#define MAC_LEN 6

class EthernetFilter : public LayerFilter
{
public:
	EthernetFilter();
	~EthernetFilter();
public:
	STu8	SourceMAC[MAC_LEN];
	STu8	DestinationMAC[MAC_LEN];
	STu16	Protocol;
	STbool	isSourceMACFilter;
	STbool	isDestinationMACFilter;
	STbool	isProtocolFilter;
};

