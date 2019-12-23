#pragma once
#include<list>
#include<algorithm>
#include"SyncStream.h"
#include<process.h>
#include<LogLib\DebugLog.h>
#include<enkiTS\TaskScheduler.h>
using namespace enki;

typedef void(*message_call_handler)(void* _this, void *uParam,unsigned int code);

class CSessions
{
public:

	CSessions()
	{
		_message_func = 0;
		_this = 0;
		g_TS.Initialize();
	}
	virtual ~CSessions()
	{
		g_TS.WaitforAllAndShutdown();
	}
private:
	std::list<CSyncStream> mCacheStreams;
	message_call_handler _message_func;
	void *_this;
	TaskScheduler g_TS;
private:
	struct ParallelTaskSet : enki::ITaskSet {
		//ParallelTaskSet() { m_SetSize = 1000; }
		void *param1;
		void *param2;
		unsigned int code;
		virtual void    ExecuteRange(enki::TaskSetPartition range, uint32_t threadnum) {
			// do something here, can issue tasks with g_TS
			CSessions *argu1 = static_cast<CSessions*>(param1);
			CSyncStream *argu2 = static_cast<CSyncStream*>(param2);
			argu1->_message_func(argu1->_this, param2, code);
		}
	};
	struct PinnedTask : enki::IPinnedTask {
		void *param1;
		void *param2;
		unsigned int code;
		virtual void    Execute() {
			// do something here, can issue tasks with g_TS
			CSessions *argu1 = static_cast<CSessions*>(param1);
			CSyncStream *argu2 = static_cast<CSyncStream*>(param2);
			argu1->_message_func(argu1->_this, param2, code);
		}
	};
private:
	void AddStream(CSyncStream stream)
	{
		mCacheStreams.push_back(stream);
		if (_message_func)
		{
			PinnedTask task;
			//ParallelTaskSet task; // default constructor has a set size of 1
			task.param1 = this;
			task.param2 = FindStreamByGuid(stream.guid);
			task.code = 0;
			//g_TS.AddTaskSetToPipe(&task);
			g_TS.AddPinnedTask(&task);
			g_TS.RunPinnedTasks();

			g_TS.WaitforTask(&task);
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
			PinnedTask task;
			//ParallelTaskSet task; // default constructor has a set size of 1
			task.param1 = this;
			task.param2 = stream;
			task.code = 1;
			//g_TS.AddTaskSetToPipe(&task);
			g_TS.AddPinnedTask(&task);
			g_TS.RunPinnedTasks();

			g_TS.WaitforTask(&task);
		}
	}
	void AddNewPacket(CSyncPacket packet)
	{
		CSyncStream m;
		m.net = packet.mNetInfo;
		m.time = packet.time;
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

