#pragma once
#include<string>
#include<vector>
#include<list>
#include<typedef.h>
using namespace SurrealTypes;


typedef struct _NetCardInfo
{
	std::string name;
	std::string description;
	STu32	netmask;
}NetCardInfo, *PNetCardInfo;

enum VALUE_TYPE				//type属性标记
{
	VALUE_CHILD,			//子元素
	VALUE_BYTE = 0x1000,	//字段值为1字节数据(endian属性无效)
	VALUE_WORD,				//2字节
	VALUE_DWORD,			//4字节
	VALUE_DDWORD,			//8字节
	VALUE_STRING,			//串类型数据(endian属性无效),默认
	VALUE_HEXSTR,			//十六进制串数据(endian属性无效)
};

#define VALUE_BYTE_LEN		1
#define VALUE_WORD_LEN		2
#define VALUE_DWORD_LEN		4
#define VALUE_DDWORD_LEN	8

enum LayerType
{
	EthernetType,
	IPv4Type,
	IPv6Type,
	TCPType,
	UDPType,
	Private,			//LayerType="Private"
	Protobuffer,		//LayerType="Protobuffer"
	JCE,				//LayerType="JCE"
};

enum CodeType
{
	EthernetSourceMAC,
	EthernetDestinationMAC,
	EthernetProtocol,
	IPv4SourceIP,
	IPv4DestinationIP,
	IPv4Protocol,
	IPv6SourceIP,
	IPv6DestinationIP,
	IPv6Protocol,
	UDPSourcePort,
	UDPDestinationPort,
	TCPSourcePort,
	TCPDestinationPort,
	PrivateSigCode,
	PrivateBackFill,
	PrivateGray,
	PrivateTlv,
};

enum OffsetType
{
	OFFSET_FRONT,		//offsettype="FRONT"
	OFFSET_BACK,		//offsettype="BACK"
};

enum BACK_FILL_TYPE
{
	BACK_FILL_LEN = 0x1000,		//数据包总长度（属性：LEN）
	BACK_FILL_ERROR,			//未知
};

enum CompareOperate
{
	TYPE_EQUAL,			//==
	TYPE_NOT_EQUAL,		//!=
	TYPE_GREATER_THAN,	//>
	TYPE_LESS_THAN,		//<
	TYPE_GREATER_OR_EQUAL,	//>=
	TYPE_LESS_THAN_OR_EQUAL,//<=
	TYPE_CONTAINS,			//包含
	TYPE_NOT_CONTAINS,		//不包含
};

typedef struct _SigCode
{
	STbool			isFilter;
	VALUE_TYPE		valuetype;
	unsigned char	endian;		//1=大端，0=小端(属性：endian)
	std::string		value;
	OffsetType		offsettype;
	STu32			offset;
	CompareOperate	operate;
	_SigCode()
	{
		operate = TYPE_EQUAL;
		valuetype = VALUE_STRING;
		isFilter = false;
		endian = 1;
	}
}SigCode, *PSigCode;

typedef struct _BackFill
{
	VALUE_TYPE		valuetype;
	BACK_FILL_TYPE	filltype;
	unsigned char	endian;		//1=大端，0=小端(属性：endian)
	std::string		value;
	OffsetType		offsettype;
	STu32			offset;
	STbool			isFilter;
	_BackFill()
	{
		valuetype = VALUE_STRING;
		filltype = BACK_FILL_LEN;
		offsettype = OFFSET_FRONT;
		isFilter = false;
		endian = 1;
	}
}BackFill,*PBackFill;

enum AppLayerStyle
{
	STYLE_PACKET,
	STYLE_STREAM
};

typedef struct _CommonAction
{
	std::string action;
	std::list<std::string> filters;
}CommonAction, *PCommonAction;

#define MAC_LEN	0x6
typedef struct _MacInfo
{
	unsigned char srcMac[MAC_LEN];
	unsigned char dstMac[MAC_LEN];

}MacInfo, *PMacInfo;

typedef struct _PacketAttach
{
	STu64	time;
}PacketAttach,*PPacketAttach;

