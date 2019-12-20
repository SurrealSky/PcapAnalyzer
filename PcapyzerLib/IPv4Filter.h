#pragma once
#include"packet.h"
#include "LayerFilter.h"
class IPv4Filter : public LayerFilter
{
public:
	IPv4Filter();
	~IPv4Filter();
public:
	STu32	SourceIP;
	STu32	DestinationIP;
	STu16	Protocol;
	STbool	isSourceIPFilter;
	STbool	isDestinationIPFilter;
	STbool	isProtocolFilter;
};

