#include "DataFilter.h"


DataFilter::DataFilter()
{
}


DataFilter::~DataFilter()
{
	std::list<LayerFilter*>::iterator iter = filters.begin();
	for (; iter != filters.end(); iter++)
	{
		delete *iter;
		*iter = 0;
	}
	filters.clear();
}

void DataFilter::AddFilter(LayerFilter *filter)
{
	filters.push_back(filter);
}

STu32 DataFilter::FilterCount()
{
	return filters.size();
}

std::list<LayerFilter*>::iterator DataFilter::GetBeginItor()
{
	return filters.begin();
}

std::list<LayerFilter*>::iterator DataFilter::GetEndItor()
{
	return filters.end();
}
