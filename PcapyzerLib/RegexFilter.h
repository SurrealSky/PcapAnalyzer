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
	*作用：加载正则表达式，并转换成当前的LayerFilter
	*返回：返回当前的过滤规则类型,-1失败
	*/
	STu32 LoadExpression(std::string);
	STu32 GetRegexFilterType(std::string);
};

