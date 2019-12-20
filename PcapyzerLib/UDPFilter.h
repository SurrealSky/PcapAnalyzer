#pragma once
#include"packet.h"
#include "LayerFilter.h"
class UDPFilter : public LayerFilter
{
public:
	UDPFilter();
	~UDPFilter();
public:
	STu16	SourcePort;
	STu16	DestinationPort;
	STbool	isSourcePortFilter;
	STbool	isDestinationPortFilter;
};

