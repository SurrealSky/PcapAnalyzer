#include "RegexFilter.h"
#include<map>

const std::map<STu32, std::string> REGEXS = {
	//payload[34:2]==89:23
	std::pair<STu32,std::string>(PAYLOAD_OFFSET_BYTES,
	"^payload\\[[A-Fa-f0-9]*:[A-Fa-f0-9]+\\][\\s]*==[\\s]*[A-Fa-f0-9]{1,2}(:[A-Fa-f0-9]{1,2})*$"),
	//payload[34:]=="123"
	std::pair<STu32,std::string>(PAYLOAD_OFFSET_STRING,
	"^payload\\[[A-Fa-f0-9]*:[A-Fa-f0-9]+\\][\\s]*==[\\s]*\".*\"$"),
	std::pair<STu32,std::string>(PAYLOAD_OFFSET_LEN_PACKET,
	"^payload\\[[A-Fa-f0-9]*:[1248]{1}\\][\\s]*==packet\\.len$"),
	//payload contains 33:4f
	std::pair<STu32,std::string>(PAYLOAD_CONTAINS_BYTES,
	"^payload[\\s]*contains[\\s]*[A-Fa-f0-9]{1,2}(:[A-Fa-f0-9]{1,2})*$"),
	//payload contains "11111"
	std::pair<STu32,std::string>(PAYLOAD_CONTAINS_STRING,
	"^payload[\\s]*contains[\\s]*\".*\"$"),
};

CRegexFilter::CRegexFilter()
{
}


CRegexFilter::~CRegexFilter()
{
}

STu32 CRegexFilter::GetRegexFilterType(std::string expression)
{
	STu32 type = -1;
	std::map<STu32, std::string>::const_iterator iter = REGEXS.begin();
	while (iter != REGEXS.end())
	{
		std::regex express(iter->second);
		if (std::regex_search(expression.c_str(), express))
		{
			type = iter->first;
			break;
		};
		iter++;
	}
	return type;
}

STu32 CRegexFilter::LoadExpression(std::string exp)
{
	STu32 type = GetRegexFilterType(exp);
	if (type != -1)
	{
		return type;
	}
	return -1;
}

std::list<std::string> CRegexFilter::LoadRegexExamples()
{
	std::list<std::string> regexs = {
		"payload contains 12:34",
		"payload[0:4]==packet.len",
		"payload[0:4]==12:34:56:78",
	};
	return regexs;
}
