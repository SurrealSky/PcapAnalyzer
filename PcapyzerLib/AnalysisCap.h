#pragma once
#include<map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include"packet.h"
#include"Sessions.h"
#include"PacketCapture.h"
#include"include.h"

#if defined(WIN32) || defined(WINx64)
#define SEPARATOR '\\'
#else
#define SEPARATOR '/'
#endif

// unless the user chooses otherwise - default number of concurrent used file descriptors is 500
#define DEFAULT_MAX_NUMBER_OF_CONCURRENT_OPEN_FILES 500

/**
* A singleton class containing the configuration as requested by the user. This singleton is used throughout the application
*/
class GlobalConfig
{
private:

	/**
	* A private c'tor (as this is a singleton)
	*/
	GlobalConfig() { writeMetadata = false; outputDir = ""; writeToConsole = false; separateSides = false; maxOpenFiles = DEFAULT_MAX_NUMBER_OF_CONCURRENT_OPEN_FILES; m_RecentConnsWithActivity = NULL; }

	// A least-recently-used (LRU) list of all connections seen so far. Each connection is represented by its flow key. This LRU list is used to decide which connection was seen least
	// recently in case we reached max number of open file descriptors and we need to decide which files to close
	LRUList<uint32_t>* m_RecentConnsWithActivity;

public:

	// a flag indicating whether to write a metadata file for each connection (containing several stats)
	bool writeMetadata;

	// the directory to write files to (default is current directory)
	std::string outputDir;

	// a flag indicating whether to write TCP data to actual files or to console
	bool writeToConsole;

	// a flag indicating whether to write both side of a connection to the same file (which is the default) or write each side to a separate file
	bool separateSides;

	// max number of allowed open files in each point in time
	size_t maxOpenFiles;


	/**
	* A method getting connection parameters as input and returns a filename and file path as output.
	* The filename is constructed by the IPs (src and dst) and the TCP ports (src and dst)
	*/
	std::string getFileName(ConnectionData connData, int side, bool separareSides)
	{
		std::stringstream stream;

		// if user chooses to write to a directory other than the current directory - add the dir path to the return value
		if (outputDir != "")
			stream << outputDir << SEPARATOR;

		std::string sourceIP = connData.srcIP->toString();
		std::string destIP = connData.dstIP->toString();

		// for IPv6 addresses, replace ':' with '_'
		std::replace(sourceIP.begin(), sourceIP.end(), ':', '_');
		std::replace(destIP.begin(), destIP.end(), ':', '_');

		// side == 0 means data is sent from client->server
		if (side <= 0 || separareSides == false)
			stream << sourceIP << "." << connData.srcPort << "-" << destIP << "." << connData.dstPort;
		else // side == 1 means data is sent from server->client
			stream << destIP << "." << connData.dstPort << "-" << sourceIP << "." << connData.srcPort;

		// return the file path
		return stream.str();
	}


	/**
	* Open a file stream. Inputs are the filename to open and a flag indicating whether to append to an existing file or overwrite it.
	* Return value is a pointer to the new file stream
	*/
	std::ostream* openFileStream(std::string fileName, bool reopen)
	{
		// if the user chooses to write only to console, don't open anything and return std::cout
		if (writeToConsole)
			return &std::cout;

		// open the file on the disk (with append or overwrite mode)
		if (reopen)
			return new std::ofstream(fileName.c_str(), std::ios_base::binary | std::ios_base::app);
		else
			return new std::ofstream(fileName.c_str(), std::ios_base::binary);
	}


	/**
	* Close a file stream
	*/
	void closeFileSteam(std::ostream* fileStream)
	{
		// if the user chooses to write only to console - do nothing and return
		if (!writeToConsole)
		{
			// close the file stream
			std::ofstream* fstream = (std::ofstream*)fileStream;
			fstream->close();

			// free the memory of the file stream
			delete fstream;
		}
	}


	/**
	* Return a pointer to the least-recently-used (LRU) list of connections
	*/
	LRUList<uint32_t>* getRecentConnsWithActivity()
	{
		// This is a lazy implementation - the instance isn't created until the user requests it for the first time.
		// the side of the LRU list is determined by the max number of allowed open files at any point in time. Default is DEFAULT_MAX_NUMBER_OF_CONCURRENT_OPEN_FILES
		// but the user can choose another number
		if (m_RecentConnsWithActivity == NULL)
			m_RecentConnsWithActivity = new LRUList<uint32_t>(maxOpenFiles);

		// return the pointer
		return m_RecentConnsWithActivity;
	}


	/**
	* The singleton implementation of this class
	*/
	static GlobalConfig& getInstance()
	{
		static GlobalConfig instance;
		return instance;
	}

	/**
	* d'tor
	*/
	~GlobalConfig()
	{
		delete m_RecentConnsWithActivity;
	}
};


/**
* A struct to contain all data save on a specific connection. It contains the file streams to write to and also stats data on the connection
*/
struct TcpReassemblyData
{
	// pointer to 2 file stream - one for each side of the connection. If the user chooses to write both sides to the same file (which is the default), only one file stream is used (index 0)
	std::ostream* fileStreams[2];

	// flags indicating whether the file in each side was already opened before. If the answer is yes, next time it'll be opened in append mode (and not in overwrite mode)
	bool reopenFileStreams[2];

	// a flag indicating on which side was the latest message on this connection
	int curSide;

	// stats data: num of data packets on each side, bytes seen on each side and messages seen on each side
	int numOfDataPackets[2];
	int numOfMessagesFromSide[2];
	int bytesFromSide[2];

	/**
	* the default c'tor
	*/
	TcpReassemblyData() { fileStreams[0] = NULL; fileStreams[1] = NULL; clear(); }

	/**
	* The default d'tor
	*/
	~TcpReassemblyData()
	{
		// close files on both sides if open
		if (fileStreams[0] != NULL)
			GlobalConfig::getInstance().closeFileSteam(fileStreams[0]);

		if (fileStreams[1] != NULL)
			GlobalConfig::getInstance().closeFileSteam(fileStreams[1]);
	}

	/**
	* Clear all data (put 0, false or NULL - whatever relevant for each field)
	*/
	void clear()
	{
		// for the file stream - close them if they're not null
		if (fileStreams[0] != NULL)
		{
			GlobalConfig::getInstance().closeFileSteam(fileStreams[0]);
			fileStreams[0] = NULL;
		}

		if (fileStreams[1] != NULL)
		{
			GlobalConfig::getInstance().closeFileSteam(fileStreams[1]);
			fileStreams[1] = NULL;
		}

		reopenFileStreams[0] = false;
		reopenFileStreams[1] = false;
		numOfDataPackets[0] = 0;
		numOfDataPackets[1] = 0;
		numOfMessagesFromSide[0] = 0;
		numOfMessagesFromSide[1] = 0;
		bytesFromSide[0] = 0;
		bytesFromSide[1] = 0;
		curSide = -1;
	}
};

typedef std::map<uint32_t, TcpReassemblyData> TcpReassemblyConnMgr;
typedef std::map<uint32_t, TcpReassemblyData>::iterator TcpReassemblyConnMgrIter;

struct TcpReassemblyMgr
{
	TcpReassemblyConnMgr connMgr;
	void *thisdata;
	void *sessions;
};

class CAnalysisCap
{

public:
	CAnalysisCap();
	virtual ~CAnalysisCap();
private:
	std::string plugin;
public:
	static std::vector<std::string> GetAllPlugins();
private:
	static void tcpReassemblyMsgReadyCallback(int sideIndex, const TcpStreamData& tcpData, void* userCookie);
	static void tcpReassemblyConnectionStartCallback(const ConnectionData& connectionData, void* userCookie);
	static void tcpReassemblyConnectionEndCallback(const ConnectionData& connectionData, TcpReassembly::ConnectionEndReason reason, void* userCookie);
	static void onPacketArrives(RawPacket* packet, PcapLiveDevice* dev, void* tcpReassemblyCookie);
	static void onApplicationInterrupted(void* cookie);
public:
	bool doTcpReassemblyOnPcapFile(const char *fileName,CSessions &mSessions,std::string plugin, std::string bpfFiler = "");
	bool doTcpReassemblyOnLiveTraffic(const char *interfaceNameOrIP, CSessions &mSessions, std::string plugin,std::string bpfFiler = "");
private:
	void EnterConnection(const TcpReassemblyData &tcpReassemblyData, const ConnectionData& connectionData,CSessions &);
public:
	//使用pcap原始库函数进行分析
	bool pcapOpen(const char *File, CSessions&, std::string plugin);
public:
	bool isFileLoaded();
private:
	/***********************组流并对包进行合并和分开********************************/
	//比如由CombinPacket调用的，整个包开始已经经过了ForInterfaceMBData的判断
	bool EnterPacket(CSessions &mSessions,unsigned char *data, unsigned int size, CNetInfo n, PacketAttach&);
	bool ForInterfaceData(CSessions &mSessions,unsigned char *data, unsigned int size, CNetInfo n, PacketAttach&);
	bool CombinPacket(CSessions &mSessions,unsigned char *data, unsigned int size, CNetInfo, PacketAttach&);
	/***********************组流并对包进行合并和分开********************************/
public:
	//根据插件分析数据包
	std::map<std::string, std::string> PacketAnalysis(const char *data, const unsigned int size,const unsigned int srcPort, const unsigned int dstPort);
	std::map<std::string, std::string> PacketAnalysis(std::list<CSyncPacket> &packets);
private:
	CPacketCapture mSniffer;
public:
	static void LoadNetDevs(std::vector<NetCardInfo>&);
	bool StartOpenSniffer(const char * name, CSessions &, std::string plugin);
	void StopOpenSniffer();
	bool IsSniffing();
private:
	static unsigned int __stdcall snifferThreadFunc(void* pParam);
	static void packet_handler(void* uParam, const unsigned char *pkt_data, unsigned int len,unsigned long long time);
};


