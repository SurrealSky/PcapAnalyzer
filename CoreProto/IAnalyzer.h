#ifndef ANALYZER_H
#define ANALYZER_H

#include<string>
#include<map>

class IAnalyzer
{
public:
	virtual ~IAnalyzer() {}
	virtual bool IsClient2Server(const unsigned int srcPort, const unsigned int dstPrt) = 0;
	virtual bool ForFilter(const unsigned int srcPort, const unsigned int dstPrt,const char *pBody,const unsigned int len) = 0;
	virtual unsigned int ActualLen(const char *pBody, const unsigned int len, const bool isClient2Server)=0;
	virtual std::map<std::string,std::string> Analysis(const char *pBody, const unsigned int len, const bool isClient2Server) = 0;
};

#endif

