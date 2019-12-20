
#include "coreapi.h"
#include "pluginapi.h"

void RegisterAnalyzerer(const char *type,
					  AnalyzererInitFunc init_cb,
					  AnalyzererFreeFunc free_cb)
{
	AnalyzerFactory::RegisterAnalyzerer(type, init_cb);
	(void) free_cb; // we never free these objects
}

