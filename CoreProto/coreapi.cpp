/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   coreapi.cpp
/// \author Martin Reddy
/// \brief  A factory object to create Analyzerer instances.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "coreapi.h"
#include <iostream>

// instantiate the static variable in AnalyzererFactory
AnalyzerFactory::CallbackMap AnalyzerFactory::mAnalyzerers;

void AnalyzerFactory::RegisterAnalyzerer(const std::string &type,
									   CreateCallback cb)
{
	mAnalyzerers[type] = cb;
}

void AnalyzerFactory::UnregisterAnalyzerer(const std::string &type)
{
	mAnalyzerers.erase(type);
}

IAnalyzer *AnalyzerFactory::CreateAnalyzerer(const std::string &type)
{
	CallbackMap::iterator it = mAnalyzerers.find(type);
	if (it != mAnalyzerers.end())
	{
		// call the creation callback to construct this derived type
		return (it->second)();
	}

    return NULL;
}
