#pragma once

//#include<HexControl.h>
//#ifdef _DEBUG
//#pragma comment(lib, "Debug\\HexView.lib")
//#else
//#pragma comment(lib, "Release\\HexView.lib")
//#endif

#include"../PcapyzerLib/PcapAnalyzer.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\PcapyzerLib.lib")
#else
#pragma comment(lib, "..\\Release\\PcapyzerLib.lib")
#endif

//#include<CodedConvert.h>
//#include<Hex.h>
//
#pragma comment(lib,"..\\Debug\\CorePlugin.lib")