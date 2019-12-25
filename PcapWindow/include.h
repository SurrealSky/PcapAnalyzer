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

#include<CodedConvert.h>
#include<Hex.h>

#pragma comment(lib,"..\\Debug\\CorePlugin.lib")


#include<LogLib\DebugLog.h>

//#include<LibUIDK\LibUIDK.h>
//using namespace LibUIDK;
//using namespace Render;
//using namespace ImageManager;
//using namespace FontManager;
//using namespace ControlResMgr;
//
//#ifdef _DEBUG
//#pragma comment(lib, "Debug\\LibUIDK100d.lib")
//#else
//#pragma comment(lib, "Release\\LibUIDK100.lib")
//#endif

//message
//CAutoComplete
#define WM_SHOWDROP		WM_USER + 201
#define WM_UPDATEUI		WM_USER + 202

#define WM_STREAMVIEW_ADDSTREAM		WM_USER+100
#define WM_STREAMVIEW_ADDPACKET		WM_USER+101
#define WM_HEXVIEW_PACKET			WM_USER+102
#define WM_ANALYSISVIEW_MAP			WM_USER+103