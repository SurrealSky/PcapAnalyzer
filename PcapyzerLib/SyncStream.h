#pragma once
#include<guiddef.h>
#include<winsock2.h>
#include<objbase.h>
#include"packet.h"
#include"NetInfo.h"
#include"SyncPacket.h"

class CSyncStream
{
public:

	CSyncStream()
	{
		GUID _guid;
		CoCreateGuid(&_guid);
		STu8 cBuffer[64] = { 0 };
		snprintf((char*)cBuffer, sizeof(cBuffer), "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
			_guid.Data1, _guid.Data2, _guid.Data3,
			_guid.Data4[0], _guid.Data4[1],
			_guid.Data4[2], _guid.Data4[3],
			_guid.Data4[4], _guid.Data4[5],
			_guid.Data4[6], _guid.Data4[7]);
		guid = std::string((char*)cBuffer);
	}

	virtual ~CSyncStream()
	{
	}
private:
	std::list<CSyncPacket> mPackets;
public:
	std::string		guid;
	STu64			time;
	CNetInfo		net;
public:
	STu32 GetCount()
	{
		return mPackets.size();
	}
	CSyncPacket *GetBack()
	{
		return &(mPackets.back());
	}
	std::list<CSyncPacket>::iterator GetBegin()
	{
		return mPackets.begin();
	}
	std::list<CSyncPacket>::iterator GetEnd()
	{
		return mPackets.end();
	}
public:
	void AddPacket(CSyncPacket packet)
	{
		mPackets.push_back(packet);
	}
	void ClearPackets()
	{
		mPackets.clear();
	}
	CNetInfo GetStreamNetInfo() const
	{
		return mPackets.begin()->mNetInfo;
	}
	CSyncPacket* FindLastPacket(const CNetInfo &n)
	{
		std::list<CSyncPacket>::reverse_iterator iter = mPackets.rbegin();
		for (; iter != mPackets.rend(); iter++)
		{
			if (iter->mNetInfo == n) return &(*iter);
		}
		return NULL;
	}
};

