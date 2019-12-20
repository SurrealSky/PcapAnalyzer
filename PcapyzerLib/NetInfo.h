#pragma once
#include<typedef.h>
using namespace SurrealTypes;

class CNetInfo
{
public:

	CNetInfo()
	{
	}

	virtual ~CNetInfo()
	{
	}
public:
	STu32	srcip;
	STu16	srcport;
	STu32	dstip;
	STu16	dstport;
	STu32	proto;
public:
	//是否同为上行流，或同为下行流
	STbool operator==(const CNetInfo &t1)const {
		return (srcip == t1.srcip&&srcport == t1.srcport&&
			dstip == t1.dstip&&dstport == t1.dstport&&proto == t1.proto);
	}
	bool operator!=(const CNetInfo &t1)const {
		return (srcip == t1.srcip || srcport == t1.srcport ||
			dstip == t1.dstip || dstport == t1.dstport || proto == t1.proto);
	}
public:
	STbool isSameStream(const CNetInfo &t1) const
	{
		return (srcip == t1.srcip&&srcport == t1.srcport&&
			dstip == t1.dstip&&dstport == t1.dstport&&proto == t1.proto)|| (srcip == t1.dstip&&srcport == t1.dstport&&
				dstip == t1.srcip&&dstport == t1.srcport&&proto == t1.proto);
	}
	STbool isOtherSide(const CNetInfo &t1) const
	{
		return (srcip == t1.dstip&&srcport == t1.dstport&&
			dstip == t1.srcip&&dstport == t1.srcport&&proto == t1.proto);
	}
};

