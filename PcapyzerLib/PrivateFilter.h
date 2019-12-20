#pragma once
#include<list>
#include "LayerFilter.h"
class PrivateFilter : public LayerFilter
{
public:
	PrivateFilter();
	~PrivateFilter();
public:
	std::list<SigCode>	sigcodes;
	std::list<BackFill>	backfills;
public:
	//std::list<std::string> expressions;
private:
	void AddBytesByOffset(std::string value);
	void AddStringByOffset(std::string value);
	void AddPacketLenByOffset(std::string value);
	void AddContainsBytes(std::string value);
	void AddContainsString(std::string value);
public:
	bool LoadRegexExp(std::string);
};

