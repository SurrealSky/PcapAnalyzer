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


//message
//CAutoComplete
#define WM_SHOWDROP		WM_USER + 201
#define WM_UPDATEUI		WM_USER + 202

#define WM_STREAMVIEW_ADDSTREAM		WM_USER+100
#define WM_STREAMVIEW_ADDPACKET		WM_USER+101
#define WM_PACKETVIEW_ADDPACKET		WM_USER+102
#define WM_HEXVIEW_PACKET			WM_USER+103
#define WM_ANALYSISVIEW_MAP			WM_USER+104
#define WM_VIEW_CLEAR				WM_USER+105
#define WM_HEXVIEW_CLEAR			WM_USER+106