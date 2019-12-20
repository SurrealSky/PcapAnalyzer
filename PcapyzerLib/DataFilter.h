#pragma once
#include<list>
#include"LayerFilter.h"
class DataFilter
{
public:
	DataFilter();
	~DataFilter();
private:
	std::list<LayerFilter*> filters;
public:
	void AddFilter(LayerFilter*);
	STu32 FilterCount();
	std::list<LayerFilter*>::iterator GetBeginItor();
	std::list<LayerFilter*>::iterator GetEndItor();
};

