#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "PrivateFilter.h"
#include"RegexFilter.h"


PrivateFilter::PrivateFilter()
{
}


PrivateFilter::~PrivateFilter()
{
}

void PrivateFilter::AddBytesByOffset(std::string value)
{
	//去掉字符串中的空格
	int index = 0;
	if (!value.empty())
	{
		while ((index = value.find(' ', index)) != std::string::npos)
		{
			value.erase(index, 1);
		}
	}

	SigCode field;
	field.isFilter = true;
	field.endian = 1;
	STu32 offset1 = value.find_first_of('[');
	STu32 offset2 = value.find_first_of(':');
	if (offset1 + 1 < offset2)
	{
		std::string stroffset = "";
		stroffset.insert(0, value, offset1 + 1, offset2 - offset1-1);
		field.offsettype = OffsetType::OFFSET_FRONT;
		field.offset = strtoll(stroffset.c_str(), 0, 16);
	}
	else
	{
		field.offsettype = OffsetType::OFFSET_FRONT;
		field.offset = 0;
	}
	//判断是>=还是==还是<=
	offset1 = value.find_first_of("==");
	std::string words;
	words.insert(0, value, offset1+2,-1);
	std::istringstream ss(words);
	while (!ss.eof())
	{
		std::string word;
		std::getline(ss, word, ':');
		STu8 c = strtol(word.c_str(), 0, 16);
		field.value.push_back(c);
	}

	offset1 = value.find_first_of(':');
	offset2 = value.find_first_of(']');
	if (offset1 + 1 < offset2)
	{
		std::string strsize = "";
		strsize.insert(0, value, offset1 + 1, offset2 - offset1-1);
		field.offsettype = OffsetType::OFFSET_FRONT;
		field.value.resize(strtol(strsize.c_str(), 0, 16));
	}
	sigcodes.push_back(field);
}

void PrivateFilter::AddStringByOffset(std::string value)
{
}

void PrivateFilter::AddPacketLenByOffset(std::string value)
{
	//payload[0:4]==packet.len
	BackFill field;
	field.isFilter = true;
	field.endian = 1;
	field.offsettype = OffsetType::OFFSET_FRONT;
	STu32 offset1 = value.find_first_of('[');
	STu32 offset2 = value.find_first_of(':');
	if (offset1 + 1 < offset2)
	{
		std::string stroffset = "";
		stroffset.insert(0,value,offset1+1,offset2 - offset1-1);
		field.offsettype = OffsetType::OFFSET_FRONT;
		field.offset = strtoll(stroffset.c_str(), 0, 16);
	}
	else
	{
		field.offsettype = OffsetType::OFFSET_FRONT;
		field.offset = 0;
	}

	offset1 = value.find_first_of(':');
	offset2 = value.find_first_of(']');
	if (offset1 + 1 < offset2)
	{
		std::string stroffset = "";
		stroffset.insert(0, value, offset1 + 1, offset2 - offset1-1);
		field.offsettype = OffsetType::OFFSET_FRONT;
		if (stroffset == "1")
			field.valuetype = VALUE_BYTE;
		else if (stroffset == "2")
			field.valuetype = VALUE_WORD;
		else if(stroffset == "4")
			field.valuetype = VALUE_DWORD;
		else if(stroffset == "8")
			field.valuetype = VALUE_DDWORD;
	}
	backfills.push_back(field);
}

void PrivateFilter::AddContainsBytes(std::string value)
{
}

void PrivateFilter::AddContainsString(std::string value)
{

}

bool PrivateFilter::LoadRegexExp(std::string exp)
{
	CRegexFilter regex;
	STu32 type=regex.LoadExpression(exp);
	switch (type)
	{
		case PAYLOAD_OFFSET_BYTES:
		{
			AddBytesByOffset(exp);
		}break;
		case PAYLOAD_OFFSET_STRING:
		{
			AddStringByOffset(exp);
		}break;
		case PAYLOAD_OFFSET_LEN_PACKET:
		{
			AddPacketLenByOffset(exp);
		}break;
		case PAYLOAD_CONTAINS_BYTES:
		{
			AddContainsBytes(exp);
		}break;
		case PAYLOAD_CONTAINS_STRING:
		{
			AddContainsString(exp);
		}break;
		default:
			return false;
	}
	return true;
}
