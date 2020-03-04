#ifndef ANALYZER_H
#define ANALYZER_H

#include<string>
#include<map>
#include<list>


class IAnalyzerData
{
	//���ڲ���͵���ģ��������ݴ���
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
	*�������ã��жϵ�ǰ���ݰ���C->S������S->C
	*����һ/srcPort��Դ�˿�
	*������/dstPrt��Ŀ�Ķ˿�
	*����ֵ��C->S/true,S->C/false
	*/
	virtual bool IsClient2Server(const unsigned int srcPort, const unsigned int dstPrt) = 0;
	/*
	*�������ã�������Ҫ��������ݰ�
	*����һ/srcPort��Դ�˿�
	*������/dstPrt��Ŀ�Ķ˿�
	*������/pBody����������ָ��
	*������/len���������ݳ���
	����ֵ��true����������Ҫ������false�������ݲ����д���
	*/
	virtual bool ForFilter(const unsigned int srcPort, const unsigned int dstPrt,const char *pBody,const unsigned int len) = 0;
	/*
	*�������ã�����ҵ�����ݰ���ʵ�ʴ�С
	*����һ/pBody������ָ��
	*������/len�����ݳ���
	*������/isClient2Server��ture��C->S���ݣ���false��S->C���ݣ�
	*����ֵ��ҵ�����ݰ��Ĵ�С
	*/
	virtual unsigned int ActualLen(const char *pBody, const unsigned int len, const bool isClient2Server)=0;
	/*
	*�������ã���������ҵ�����ݰ�
	*����һ/pBody��ҵ�������ָ��
	*������/len��ҵ�����ݰ�����
	*������/isClient2Server��ture��C->S���ݣ���false��S->C���ݣ�
	*����ֵ��ҵ�����ݰ������������map�ṹ�洢
	*/
	virtual std::map<std::string, std::string> Analysis(const char *pBody, const unsigned int len, const bool isClient2Server) = 0;
	/*
	*�������ã��������ҵ�����ݰ�
	*����һ��ҵ�����ݰ�����
	*����ֵ��ҵ�����ݰ������������map�ṹ�洢
	*/
	virtual std::map<std::string, std::string> AnalysisList(const std::list<IAnalyzerData>&) = 0;
};

#endif

