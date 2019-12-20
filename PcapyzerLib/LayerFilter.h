#pragma once
#include"packet.h"
class LayerFilter
{
public:
	LayerFilter();
	~LayerFilter();
private:
	LayerType	type;
	STbool				isFilter;
public:
	LayerType GetLayerType() { return type; }
	void SetLayerType(LayerType t) { type = t; }
	STbool GetFilter() { return isFilter; }
	void SetFilter(STbool b) { isFilter = b; }
};

