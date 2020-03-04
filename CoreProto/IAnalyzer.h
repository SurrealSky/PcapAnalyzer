#ifndef ANALYZER_H
#define ANALYZER_H

#include<string>
#include<map>
#include<list>


class IAnalyzerData
{
	//用于插件和调用模块进行数据传递
public:
	IAnalyzerData()
	{
		payload = "";
		srcPort = 0;
		dstPort = 0;
	}
	virtual ~IAnalyzerData() {}
public:
	std::string payload;
	unsigned int srcPort;
	unsigned int dstPort;
};

class IAnalyzer
{
public:
	virtual ~IAnalyzer() {}
	/*
	*函数作用：判断当前数据包是C->S，还是S->C
	*参数一/srcPort：源端口
	*参数二/dstPrt：目的端口
	*返回值：C->S/true,S->C/false
	*/
	virtual bool IsClient2Server(const unsigned int srcPort, const unsigned int dstPrt) = 0;
	/*
	*函数作用：过滤需要处理的数据包
	*参数一/srcPort：源端口
	*参数二/dstPrt：目的端口
	*参数三/pBody：报文数据指针
	*参数四/len：报文数据长度
	返回值：true（此数据需要处理），false（此数据不进行处理）
	*/
	virtual bool ForFilter(const unsigned int srcPort, const unsigned int dstPrt,const char *pBody,const unsigned int len) = 0;
	/*
	*函数作用：计算业务数据包的实际大小
	*参数一/pBody：数据指针
	*参数二/len：数据长度
	*参数三/isClient2Server：ture（C->S数据），false（S->C数据）
	*返回值：业务数据包的大小
	*/
	virtual unsigned int ActualLen(const char *pBody, const unsigned int len, const bool isClient2Server)=0;
	/*
	*函数作用：分析单个业务数据包
	*参数一/pBody：业务包数据指针
	*参数二/len：业务数据包长度
	*参数三/isClient2Server：ture（C->S数据），false（S->C数据）
	*返回值：业务数据包分析结果，以map结构存储
	*/
	virtual std::map<std::string, std::string> Analysis(const char *pBody, const unsigned int len, const bool isClient2Server) = 0;
	/*
	*函数作用：分析多个业务数据包
	*参数一：业务数据包集合
	*返回值：业务数据包分析结果，以map结构存储
	*/
	virtual std::map<std::string, std::string> AnalysisList(const std::list<IAnalyzerData>&) = 0;
};

#endif

