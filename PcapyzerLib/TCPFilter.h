#pragma once
#include"packet.h"
#include "LayerFilter.h"
class TCPFilter : public LayerFilter
{
public:
	TCPFilter();
	~TCPFilter();
public:
	STu16	SourcePort;
	STu16	DestinationPort;
	STbool	isSourcePortFilter;
	STbool	isDestinationPortFilter;
};

