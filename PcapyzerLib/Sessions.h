#pragma once
#include<list>
#include<algorithm>
#include"SyncStream.h"
#include<process.h>

typedef void(*message_call_handler)(void* _this, void *uParam,unsigned int code);

struct argulst
{
	void *param1;
	void *param2;
	unsigned int code;
};

class CSessions
{
public:

	CSessions()
	{
		_message_func = 0;
		_this = 0;
		hmu= CreateMutex(NULL, FALSE, NULL);
	}
	virtual ~CSessions()
	{
		CloseHandle(hmu);
	}
private:
	std::list<CSyncStream> mCacheStreams;
	message_call_handler _message_func;
	void *_this;
	HANDLE hmu;
private:
	static unsigned int __stdcall ThreadFunc(void* pParam)
	{
		argulst *p = static_cast<argulst*>(pParam);
		CSessions *s = static_cast<CSessions*>(p->param1);
		WaitForSingleObject(s->hmu, INFINITE);//等待互斥量
		s->_message_func(s->_this, (void*)(p->param2), p->code);
		delete p;
		p = 0;
		ReleaseMutex(s->hmu);
		return 0;
	}
private:
	void AddStream(CSyncStream stream)
	{
		mCacheStreams.push_back(stream);
		if (_message_func)
		{
			argulst *argus=new argulst();
			argus->param1 = this;
			argus->param2 = FindStreamByGuid(stream.guid);
			argus->code = 0;
			_beginthreadex(NULL, 0, ThreadFunc, argus, 0, 0);
		}	
	}
public:
	void initcallhander(void *main, message_call_handler handler1)
	{
		_message_func = handler1;
		_this = main;
	}
	CSyncStream *FindStreamByGuid(const std::string &_guid)
	{
		std::list<CSyncStream>::iterator itr =
			std::find_if(mCacheStreams.begin(), mCacheStreams.end(), [_guid](CSyncStream const& obj) {
			return obj.guid==_guid;
		});
		if (itr != mCacheStreams.end())
			return &(*itr);
		return NULL;
	}
	void ClearStream()
	{
		mCacheStreams.clear();
	}
	void AddPacket(std::string guid, CSyncPacket packet)
	{
		CSyncStream *stream = FindStreamByGuid(guid);
		stream->AddPacket(packet);
		if (_message_func)
		{
			argulst *argus=new argulst();
			argus->param1 = this;
			argus->param2 = stream;
			argus->code = 1;
			_beginthreadex(NULL, 0, ThreadFunc, argus, 0, 0);
		}
	}
	void AddNewPacket(CSyncPacket packet)
	{
		CSyncStream m;
		AddStream(m);
		AddPacket(m.guid, packet);
	}
	/*
	*作用：单行流查找
	*若为上行流，则查找上行流，若为下行流，则查找下行流
	*/
	//CSyncStream* FindEqualStream(const CNetInfo &n)
	//{
	//	std::list<CSyncStream>::iterator itr =
	//		std::find_if(mCacheStreams.begin(), mCacheStreams.end(), [n](CSyncStream const& obj) {
	//		return obj.mPackets.begin()->mNetInfo==n;
	//	});
	//	if (itr != mCacheStreams.end())
	//		return &(*itr);
	//	return NULL;
	//}
	/*
	*作用：查找同一条流，包含上行和下行流
	*/
	CSyncStream* FindSameStreamByQuintet(const CNetInfo &n)
	{
		std::list<CSyncStream>::iterator itr =
			std::find_if(mCacheStreams.begin(), mCacheStreams.end(), [n](CSyncStream const& obj) {
			return obj.GetStreamNetInfo().isSameStream(n);
		});
		if (itr != mCacheStreams.end())
			return &(*itr);
		return NULL;
	}
};

