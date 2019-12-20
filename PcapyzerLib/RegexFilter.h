#pragma once
#include<typedef.h>
#include<list>
#include<regex>
using namespace SurrealTypes;

enum REGEX_FILTER_TYPE
{
	PAYLOAD_OFFSET_BYTES=0,
	PAYLOAD_OFFSET_STRING,
	PAYLOAD_OFFSET_LEN_PACKET,
	PAYLOAD_CONTAINS_BYTES,
	PAYLOAD_CONTAINS_STRING,
};

class CRegexFilter
{
public:
	CRegexFilter();
	~CRegexFilter();
public:
	std::list<std::string> LoadRegexExamples();
	/*
	*���ã�����������ʽ����ת���ɵ�ǰ��LayerFilter
	*���أ����ص�ǰ�Ĺ��˹�������,-1ʧ��
	*/
	STu32 LoadExpression(std::string);
	STu32 GetRegexFilterType(std::string);
};

