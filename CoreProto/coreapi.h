
#ifndef COREAPI_H
#define COREAPI_H

#include "defines.h"
#include "IAnalyzer.h"

#include <string>
#include <map>

///
/// A factory object in the Core API
///
class CORE_API AnalyzerFactory
{
public:
	typedef IAnalyzer *(*CreateCallback)();

	static void RegisterAnalyzerer(const std::string &type,
								 CreateCallback cb);
	static void UnregisterAnalyzerer(const std::string &type);

	static IAnalyzer *CreateAnalyzerer(const std::string &type);

private:
	typedef std::map<std::string, CreateCallback> CallbackMap;
	static CallbackMap mAnalyzerers;
};

#endif
