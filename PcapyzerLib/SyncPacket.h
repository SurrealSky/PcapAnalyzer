#pragma once
#include<string>
#include<pack/ByteBuffer.h>

class CSyncPacket
{
public:

	CSyncPacket()
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

	virtual ~CSyncPacket()
	{
	}
public:
	std::string		guid;
public:
	CNetInfo	mNetInfo;
	STu64	time;
	ByteBuffer _payload;
	bool	isStreamEnd;
	unsigned int streamResiduelen;
};

