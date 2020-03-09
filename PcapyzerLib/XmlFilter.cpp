#include<regex>
#include "XmlFilter.h"
#include"../CommonLib/include/Hex.h"
#include"RegexFilter.h"

XmlFilter::XmlFilter()
{
}


XmlFilter::~XmlFilter()
{
}


std::string XmlFilter::FormatTime(time_t time1)
{
	//微秒的时间戳需要进行转换
	time_t ts_sec = time1 / 1000000;
	time_t ts_usec = time1 % 1000000;
	struct tm tm1;
	char * szTime = new char[128];
	memset(szTime, 0, 128);
	tm1 = *gmtime(&ts_sec);
	//sprintf(szTime, "%4.4d.%2.2d.%2.2d.%2.2d.%2.2d.%2.2d",
	//	tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday,
	//	tm1.tm_hour, tm1.tm_min, tm1.tm_sec);

	sprintf(szTime, "%2.2d.%2.2d.%2.2d.%6.6d",
		tm1.tm_hour, tm1.tm_min, tm1.tm_sec, ts_usec);
	std::string time;
	time.append(szTime, strlen(szTime));
	delete []szTime;
	return time;
}

CodeType XmlFilter::ValueCodeType(LayerType type,std::string value)
{
	if (type==LayerType::EthernetType)
	{
		if (strcmp(value.c_str(), "SourceMAC") == 0)
		{
			return CodeType::EthernetSourceMAC;
		}
		else if (strcmp(value.c_str(), "DestinationMAC") == 0)
		{
			return CodeType::EthernetDestinationMAC;
		}
		else if (strcmp(value.c_str(), "Protocol") == 0)
		{
			return CodeType::EthernetProtocol;
		}
	}
	else if (type == LayerType::IPv4Type)
	{
		if (strcmp(value.c_str(), "SourceIP") == 0)
		{
			return CodeType::IPv4SourceIP;
		}
		else if (strcmp(value.c_str(), "DestinationIP") == 0)
		{
			return CodeType::IPv4DestinationIP;
		}
		else if (strcmp(value.c_str(), "Protocol") == 0)
		{
			return CodeType::IPv4Protocol;
		}
	}
	else if (type == LayerType::IPv6Type)
	{
		if (strcmp(value.c_str(), "SourceIP") == 0)
		{
			return CodeType::IPv6SourceIP;
		}
		else if (strcmp(value.c_str(), "DestinationIP") == 0)
		{
			return CodeType::IPv6DestinationIP;
		}
		else if (strcmp(value.c_str(), "Protocol") == 0)
		{
			return CodeType::IPv6Protocol;
		}
	}
	else if (type == LayerType::TCPType)
	{
		if (strcmp(value.c_str(), "SourcePort") == 0)
		{
			return CodeType::TCPSourcePort;
		}
		else if (strcmp(value.c_str(), "DestinationPort") == 0)
		{
			return CodeType::TCPDestinationPort;
		}
	}
	else if (type == LayerType::UDPType)
	{
		if (strcmp(value.c_str(), "SourcePort") == 0)
		{
			return CodeType::UDPSourcePort;
		}
		else if (strcmp(value.c_str(), "DestinationPort") == 0)
		{
			return CodeType::UDPDestinationPort;
		}
	}
	else if (type == LayerType::Private)
	{
		if (strcmp(value.c_str(), "SigCode") == 0)
		{
			return CodeType::PrivateSigCode;
		}
		else if (strcmp(value.c_str(), "BackFill") == 0)
		{
			return CodeType::PrivateBackFill;
		}
		else if (strcmp(value.c_str(), "Gray") == 0)
		{
			return CodeType::PrivateGray;
		}
		else if (strcmp(value.c_str(), "Tlv") == 0)
		{
			return CodeType::PrivateTlv;
		}
	}
	else if (type == LayerType::Protobuffer)
	{

	}
	else if (type == LayerType::JCE)
	{

	}
}

VALUE_TYPE XmlFilter::ValueValueType(std::string value)
{
	//默认值为VALUE_STRING
	if (strcmp(value.c_str(), "BYTE") == 0)
	{
		return VALUE_TYPE::VALUE_BYTE;
	}
	else if (strcmp(value.c_str(), "WORD") == 0)
	{
		return VALUE_TYPE::VALUE_WORD;
	}
	else if (strcmp(value.c_str(), "DWORD") == 0)
	{
		return VALUE_TYPE::VALUE_DWORD;
	}
	else if (strcmp(value.c_str(), "DDWORD") == 0)
	{
		return VALUE_TYPE::VALUE_DDWORD;
	}
	else if (strcmp(value.c_str(), "HEXSTR") == 0)
	{
		return VALUE_TYPE::VALUE_HEXSTR;
	}
	else if (strcmp(value.c_str(), "CHILD") == 0)
	{
		return VALUE_TYPE::VALUE_CHILD;
	}else
		return VALUE_TYPE::VALUE_STRING;
}

std::string XmlFilter::ValueValue(VALUE_TYPE type, std::string value)
{
	std::string strResult;
	if (type == VALUE_TYPE::VALUE_BYTE)
	{
		STu8 c=ValueDigit(value);
		strResult.push_back(c);
	}
	else if (type == VALUE_TYPE::VALUE_WORD)
	{
		//判断大小端
		STu16 c = ValueDigit(value);
		strResult.push_back(c);

	}
	else if (type == VALUE_TYPE::VALUE_DWORD)
	{
		STu32 c = ValueDigit(value);
		strResult.push_back(c);
	}
	else if (type == VALUE_TYPE::VALUE_DDWORD)
	{
		STu64 c = ValueDigit(value);
		strResult.push_back(c);
	}
	else if (type == VALUE_TYPE::VALUE_STRING)
	{
		strResult = value;
	}
	else if (type == VALUE_TYPE::VALUE_HEXSTR)
	{
		SurrealHex::HexConvert mHex;
		strResult=mHex.StrToHex((BYTE*)value.c_str(), value.size());
	}
	else if (type == VALUE_TYPE::VALUE_CHILD)
	{

	}
	return strResult;
}

bool XmlFilter::ValueisFilter(std::string value)
{
	//默认值为false
	if (strcmp(value.c_str(), "1") == 0)
	{
		return true;
	}
	return false;
}

AppLayerStyle XmlFilter::ValueAppLayerStyle(std::string value)
{
	if (strcmp(value.c_str(), "packet") == 0)
	{
		return AppLayerStyle::STYLE_PACKET;
	}
	else if (strcmp(value.c_str(), "stream") == 0)
	{
		return AppLayerStyle::STYLE_STREAM;
	}
}

LayerType XmlFilter::ValueLayerType(std::string value)
{
	if (strcmp(value.c_str(), "Ethernet") == 0)
	{
		return LayerType::EthernetType;
	}
	else if (strcmp(value.c_str(), "IPv4") == 0)
	{
		return LayerType::IPv4Type;
	}
	else if (strcmp(value.c_str(), "IPv6") == 0)
	{
		return LayerType::IPv6Type;
	}
	else if (strcmp(value.c_str(), "TCP") == 0)
	{
		return LayerType::TCPType;
	}
	else if (strcmp(value.c_str(), "UDP") == 0)
	{
		return LayerType::UDPType;
	}
	else if (strcmp(value.c_str(), "Private") == 0)
	{
		return LayerType::Private;
	}
	else if (strcmp(value.c_str(), "Protobuffer") == 0)
	{
		return LayerType::Protobuffer;
	}
	else if (strcmp(value.c_str(), "JCE") == 0)
	{
		return LayerType::JCE;
	}
}

OffsetType XmlFilter::ValueOffsetType(std::string value)
{
	if (strcmp(value.c_str(), "FRONT") == 0)
	{
		return OffsetType::OFFSET_FRONT;
	}
	else if (strcmp(value.c_str(), "BACK") == 0)
	{
		return OffsetType::OFFSET_BACK;
	}
}

void XmlFilter::ValueMAC(VALUE_TYPE valuetype, std::string value, STu8 MAC[])
{
	if (valuetype== VALUE_TYPE::VALUE_HEXSTR)
	{
		SurrealHex::HexConvert mHexConvert;
		std::string hex= mHexConvert.StrToHex((BYTE*)value.c_str(), value.size());
		MAC[0] = hex[0];
		MAC[1] = hex[1];
		MAC[2] = hex[2];
		MAC[3] = hex[3];
		MAC[4] = hex[4];
		MAC[5] = hex[5];
	}
}

STu16 XmlFilter::ValueEthernetProtocol(std::string value)
{
	//if (strcmp(value.c_str(), "IPv4") == 0)
	//{
	//	return ETHERTYPE_IP;
	//}
	//else if (strcmp(value.c_str(), "IPv6") == 0)
	//{
	//	return ETHERTYPE_IPV6;
	//}
	return 0;
}

STu32 XmlFilter::ValueIP(VALUE_TYPE valuetype, std::string value)
{
	if (valuetype == VALUE_TYPE::VALUE_STRING)
	{
		//点分十进制转换为整形
		UINT dwServerIp = 0;
		//inet_pton(AF_INET, value.c_str(), &dwServerIp);
		dwServerIp = STswab32(dwServerIp);
		return dwServerIp;
	}
}

STu16 XmlFilter::ValuePort(VALUE_TYPE valuetype,std::string value)
{
	if(valuetype== VALUE_TYPE::VALUE_DWORD)
		return atoi(value.c_str());
}

STu32 XmlFilter::ValueOffset(std::string value)
{
	return atoi(value.c_str());
}

STu32 XmlFilter::ValueStep(std::string value)
{
	return atoi(value.c_str());
}

STu32 XmlFilter::CalcOffset(OffsetType type, STu32 _offset, STu32 step)
{
	DWORD offset = 0;
	if (type == OffsetType::OFFSET_FRONT)
	{
		offset = _offset;
	}
	else if (type == OffsetType::OFFSET_BACK)
	{

	}
	return offset;
}

STu16 XmlFilter::ValueIPProtocol(std::string value)
{
	/*if (strcmp(value.c_str(), "TCP") == 0)
	{
		return TCP_PACKET;
	}
	else if (strcmp(value.c_str(), "UDP") == 0)
	{
		return UDP_PACKET;
	}
	else if (strcmp(value.c_str(), "ICMP") == 0)
	{
		return ICMP_PACKET;
	}
	else if (strcmp(value.c_str(), "IGMP") == 0)
	{
		return IGMP_PACKET;
	}*/
	return 0;
}

unsigned char XmlFilter::ValueEndian(std::string value)
{
	//默认值为1
	if (strcmp(value.c_str(), "0") == 0)
	{
		return 0;
	}
	return 1;
}

std::string XmlFilter::IPProtocol2String(STu16 protocol)
{
	/*switch (protocol)
	{
		case TCP_PACKET:
		{
			return "TCP";
		}break;
		case UDP_PACKET:
		{
			return "UDP";
		}break;
		case ICMP_PACKET:
		{
			return "ICMP";
		}break;
		case IGMP_PACKET:
		{
			return "IGMP";
		}break;
		default:
		{
			return "UnknowProtocol";
		}break;
	}*/
	return "";
}

bool XmlFilter::IsHexadecimal(std::string value)
{
	const std::string pattern = "(^[A-Fa-f0-9]+(h|H)$)|(^0[xX][A-Fa-f0-9]+$)";
	std::regex express(pattern);
	return std::regex_search(value.c_str(), express);
}

bool XmlFilter::IsOctal(std::string value)
{
	const std::string pattern = "(^[0-7]+(o|O)$)|(^0[0-7]+$)";
	std::regex express(pattern);
	return std::regex_search(value.c_str(), express);
}

bool XmlFilter::IsBinary(std::string value)
{
	const std::string pattern = "(^[0-1]+(b|B)$)|(^0[bB][0-1]+$)";
	std::regex express(pattern);
	return std::regex_search(value.c_str(), express);
}

bool XmlFilter::IsDecimal(std::string value)
{
	const std::string pattern = "(^[0-9]+(d|D)$)|(^[0-9]+$)";
	std::regex express(pattern);
	return std::regex_search(value.c_str(), express);
}

STu32 XmlFilter::ValueDigit(std::string value)
{
	int radix = 10;
	if (IsHexadecimal(value))
	{
		radix = 16;
	}
	else if (IsOctal(value))
	{
		radix = 8;
	}
	else if (IsBinary(value))
	{
		radix = 2;
	}
	else if (IsDecimal(value))
	{
		radix = 10;
	}
	else
		return 0;
	return strtoll(value.c_str(), 0, radix);
}

bool XmlFilter::ReadActionsFromXML(std::string file, std::list<CommonAction> &mActions)
{
	mActions.clear();
	CMarkup xml;
	bool bLoad = false;

	bLoad = xml.Load(file.c_str());
	if (!bLoad)
	{
		//printf("没有找到XML文件!\n");
		return false;
	}
	xml.ResetMainPos();//把xml对象指向的位置初始化，使其指向文件开始
	bool ol = xml.FindElem("config");
	while (xml.FindChildElem())
	{
		xml.IntoElem();
		CommonAction action;
		action.action = xml.GetTagName();
		//读取filter
		while (xml.FindChildElem("filter"))
		{
			xml.IntoElem();
			std::string filtername = xml.GetAttrib("name");
			action.filters.push_back(filtername);
			xml.OutOfElem();
		}
		mActions.push_back(action);
		xml.OutOfElem();
	}
	return true;
}

void XmlFilter::ReadSigCode(CMarkup xml, std::list<SigCode> &sigcodes)
{
	std::string _tag = xml.GetTagName();
	std::string _valuetype = xml.GetAttrib("valuetype");
	std::string _value = xml.GetData();
	std::string _endian = xml.GetAttrib("endian");
	std::string _offsettype= xml.GetAttrib("offsettype");
	std::string _offset= xml.GetAttrib("offset");
	std::string _isfilter= xml.GetAttrib("isFilter");
	SigCode sigcode;
	sigcode.valuetype = ValueValueType(_valuetype);
	sigcode.endian = ValueEndian(_endian);
	sigcode.value = ValueValue(sigcode.valuetype,_value);
	sigcode.offsettype = ValueOffsetType(_offsettype);
	sigcode.offset = ValueOffset(_offset);
	sigcode.isFilter = ValueisFilter(_isfilter);
	sigcodes.push_back(sigcode);
}

void XmlFilter::ReadBackFill(CMarkup xml, std::list<BackFill> &backfills)
{
	std::string _tag = xml.GetTagName();
	std::string _valuetype = xml.GetAttrib("valuetype");
	std::string _value = xml.GetData();
	std::string _endian = xml.GetAttrib("endian");
	std::string _offsettype = xml.GetAttrib("offsettype");
	std::string _offset = xml.GetAttrib("offset");
	std::string _isfilter = xml.GetAttrib("isFilter");
	BackFill backfill;
	backfill.valuetype = ValueValueType(_valuetype);
	backfill.endian = ValueEndian(_endian);
	backfill.value = ValueValue(backfill.valuetype, _value);
	backfill.offsettype = ValueOffsetType(_offsettype);
	backfill.offset = ValueOffset(_offset);
	backfill.isFilter = ValueisFilter(_isfilter);
	backfills.push_back(backfill);
}

void XmlFilter::ReadRegexExp(CMarkup xml, PrivateFilter *filter)
{
	std::string _tag = xml.GetTagName();
	std::string _valuetype = xml.GetAttrib("valuetype");
	std::string _value = xml.GetData();
	std::string _isfilter = xml.GetAttrib("isFilter");
	if (IsValidateeExpression(_value))
	{
		filter->LoadRegexExp(_value);
	}	
}

void XmlFilter::ReadLayerFilter(CMarkup xml, LayerFilter *filter)
{
	while (xml.FindChildElem())
	{
		xml.IntoElem();
		std::string _codetype = xml.GetAttrib("codetype");
		std::string _valuetype = xml.GetAttrib("valuetype");
		std::string _isFilter= xml.GetAttrib("isFilter");
		if (filter->GetLayerType() == LayerType::EthernetType)
		{
			if (strcmp(_codetype.c_str(), "SourceMAC") == 0)
			{
				ValueMAC(ValueValueType(_valuetype), xml.GetData(), static_cast<EthernetFilter*>(filter)->SourceMAC);
				static_cast<EthernetFilter*>(filter)->isSourceMACFilter = ValueisFilter(_isFilter);
			}
			else if (strcmp(_codetype.c_str(), "DestinationMAC") == 0)
			{
				ValueMAC(ValueValueType(_valuetype), xml.GetData(), static_cast<EthernetFilter*>(filter)->DestinationMAC);
				static_cast<EthernetFilter*>(filter)->isDestinationMACFilter = ValueisFilter(_isFilter);
			}
			else if (strcmp(_codetype.c_str(), "Protocol") == 0)
			{
				static_cast<EthernetFilter*>(filter)->Protocol = ValueEthernetProtocol(xml.GetData());
				static_cast<EthernetFilter*>(filter)->isProtocolFilter = ValueisFilter(_isFilter);
			}
		}
		else if (filter->GetLayerType() == LayerType::IPv4Type)
		{
			if (strcmp(_codetype.c_str(), "SourceIP") == 0)
			{
				static_cast<IPv4Filter*>(filter)->SourceIP = ValueIP(ValueValueType(_valuetype), xml.GetData());
				static_cast<IPv4Filter*>(filter)->isSourceIPFilter = ValueisFilter(_isFilter);
			}
			else if (strcmp(_codetype.c_str(), "DestinationIP") == 0)
			{
				static_cast<IPv4Filter*>(filter)->DestinationIP = ValueIP(ValueValueType(_valuetype), xml.GetData());
				static_cast<IPv4Filter*>(filter)->isDestinationIPFilter = ValueisFilter(_isFilter);
			}
			else if (strcmp(_codetype.c_str(), "Protocol") == 0)
			{
				static_cast<IPv4Filter*>(filter)->Protocol = ValueIPProtocol(xml.GetData());
				static_cast<IPv4Filter*>(filter)->isProtocolFilter = ValueisFilter(_isFilter);
			}
		}
		else if (filter->GetLayerType() == LayerType::TCPType)
		{
			if (strcmp(_codetype.c_str(), "SourcePort") == 0)
			{
				static_cast<TCPFilter*>(filter)->SourcePort = ValuePort(ValueValueType(_valuetype), xml.GetData());
				static_cast<TCPFilter*>(filter)->isSourcePortFilter = ValueisFilter(_isFilter);
			}
			else if (strcmp(_codetype.c_str(), "DestinationPort") == 0)
			{
				static_cast<TCPFilter*>(filter)->DestinationPort = ValuePort(ValueValueType(_valuetype), xml.GetData());
				static_cast<TCPFilter*>(filter)->isDestinationPortFilter = ValueisFilter(_isFilter);
			}
		}
		else if (filter->GetLayerType() == LayerType::UDPType)
		{
			if (strcmp(_codetype.c_str(), "SourcePort") == 0)
			{
				static_cast<UDPFilter*>(filter)->SourcePort = ValuePort(ValueValueType(_valuetype), xml.GetData());
				static_cast<UDPFilter*>(filter)->isSourcePortFilter = ValueisFilter(_isFilter);
			}
			else if (strcmp(_codetype.c_str(), "DestinationPort") == 0)
			{
				static_cast<UDPFilter*>(filter)->DestinationPort = ValuePort(ValueValueType(_valuetype), xml.GetData());
				static_cast<UDPFilter*>(filter)->isDestinationPortFilter = ValueisFilter(_isFilter);
			}
		}
		else if (filter->GetLayerType() == LayerType::Private)
		{
			if (strcmp(_codetype.c_str(), "SigCode") == 0)
			{
				ReadSigCode(xml, static_cast<PrivateFilter*>(filter)->sigcodes);
			}
			else if (strcmp(_codetype.c_str(), "BackFill") == 0)
			{
				ReadBackFill(xml, static_cast<PrivateFilter*>(filter)->backfills);
			}
			else if (strcmp(_codetype.c_str(), "RegexExp") == 0)
			{
				ReadRegexExp(xml, static_cast<PrivateFilter*>(filter));
			}
		}
		xml.OutOfElem();
	}
}

bool XmlFilter::ReadFiltersFromXML(std::string file,std::string action,std::string filterindex)
{

	CMarkup xml;
	bool bLoad = false;

	bLoad = xml.Load(file.c_str());
	if (!bLoad)
	{
		//printf("没有找到XML文件!\n");
		return false;
	}
	xml.ResetMainPos();//把xml对象指向的位置初始化，使其指向文件开始
	
	bool ol = xml.FindChildElem(action.c_str());
	if (ol)
	{
		xml.IntoElem();
		xml.IntoElem();
		while (xml.FindElem())
		{
			std::string name = xml.GetAttrib("name");
			if (strcmp(name.c_str(), filterindex.c_str()) == 0)
			{
				while (xml.FindChildElem())
				{
					xml.IntoElem();
					std::string strLevel= xml.GetTagName();
					std::string strLayerType = xml.GetAttrib("LayerType");
					std::string strisFilter= xml.GetAttrib("isFilter");
					LayerFilter *layerfilter = 0;
					if (strLayerType=="Ethernet")
					{
						layerfilter = new EthernetFilter();
					}
					else if (strLayerType == "IPv4")
					{
						layerfilter = new IPv4Filter();
					}else if (strLayerType == "UDP")
					{
						layerfilter = new UDPFilter();
					}else if (strLayerType == "TCP")
					{
						layerfilter = new TCPFilter();
					}
					else if (strLayerType == "Private")
					{
						layerfilter = new PrivateFilter();
					}
					if (!layerfilter) return false;
					layerfilter->SetLayerType(ValueLayerType(strLayerType));
					layerfilter->SetFilter(ValueisFilter(strisFilter));
					ReadLayerFilter(xml, layerfilter);
					mDataFilter.AddFilter(layerfilter);
					xml.OutOfElem();
				}
				return true;
			}
		}
	}
	return false;
}

bool XmlFilter::ForSigCodeFilter(const SigCode &filter, const STu8 *data, const STu32 size)
{
	if (filter.offsettype == OffsetType::OFFSET_FRONT)
	{
		if (filter.offset + filter.value.size()> size) return false;
		std::string str;
		str.append((char*)data + filter.offset, filter.value.size());
		if (filter.value.compare(str) != 0) return false;
	}
	else if (filter.offsettype == OffsetType::OFFSET_BACK)
	{
		if (filter.offset + filter.value.size()> size) return false;
		std::string str;
		str.append((char*)data + (size - 1) - filter.offset, filter.value.size());
		if (filter.value.compare(str) != 0) return false;
	}
	//全部规则匹配
	return true;
}

bool XmlFilter::ForBackFillFilter(const BackFill &filter, const STu8 *data, const STu32 size)
{
	if (filter.valuetype == VALUE_BYTE)
	{
		if (filter.offset + VALUE_BYTE_LEN> size) return false;
		std::string str;
		if (filter.offsettype == OffsetType::OFFSET_FRONT)
		{
			str.append((char*)data + filter.offset, VALUE_BYTE_LEN);
		}
		else if (filter.offsettype == OffsetType::OFFSET_BACK)
		{
			str.append((char*)data + (size - 1) - filter.offset, VALUE_BYTE_LEN);
		}
		STu8 valuelen = *(STu8*)(str.c_str());
		if (size != valuelen) return false;
	}
	else if (filter.valuetype == VALUE_WORD)
	{
		if (filter.offset + VALUE_WORD_LEN> size) return false;
		std::string str;
		if (filter.offsettype == OffsetType::OFFSET_FRONT)
		{
			str.append((char*)data + filter.offset, VALUE_WORD_LEN);
		}
		else if (filter.offsettype == OffsetType::OFFSET_BACK)
		{
			str.append((char*)data + (size - 1) - filter.offset, VALUE_WORD_LEN);
		}
		STu16 valuelen = *(STu16*)(str.c_str());
		if (filter.endian == 1)
		{
			valuelen = STswab16(valuelen);
		}
		if (size != valuelen) return false;
	}
	else if (filter.valuetype == VALUE_DWORD)
	{
		if (filter.offset + VALUE_DWORD_LEN> size) return false;
		std::string str;
		if (filter.offsettype == OffsetType::OFFSET_FRONT)
		{
			str.append((char*)data + filter.offset, VALUE_DWORD_LEN);
		}
		else if (filter.offsettype == OffsetType::OFFSET_BACK)
		{
			str.append((char*)data + (size - 1) - filter.offset, VALUE_DWORD_LEN);
		}
		STu32 valuelen = *(STu32*)(str.c_str());
		if (filter.endian == 1)
		{
			valuelen = STswab32(valuelen);
		}
		if (size != valuelen) return false;
	}
	else if (filter.valuetype == VALUE_DDWORD)
	{
		if (filter.offset + VALUE_DDWORD_LEN> size) return false;
		std::string str;
		if (filter.offsettype == OffsetType::OFFSET_FRONT)
		{
			str.append((char*)data + filter.offset, VALUE_DDWORD_LEN);
		}
		else if (filter.offsettype == OffsetType::OFFSET_BACK)
		{
			str.append((char*)data + (size - 1) - filter.offset, VALUE_DDWORD_LEN);
		}
		STu64 valuelen = *(STu64*)(str.c_str());
		if (filter.endian == 1)
		{
			valuelen = STswab64(valuelen);
		}
		if (size != valuelen) return false;
	}
	////全部规则匹配
	return true;
}

bool XmlFilter::ForPrivateFilter(const PrivateFilter &filter, const STu8 *data, const STu32 size)
{
	//遍历SigCode
	std::list<SigCode>::const_iterator itor= filter.sigcodes.begin();
	for (; itor != filter.sigcodes.end(); itor++)
	{
		if (itor->isFilter)
		{
			if(!ForSigCodeFilter(*itor, data, size)) return false;
		}
	}
	//遍历backfill
	std::list<BackFill>::const_iterator itor2 = filter.backfills.begin();
	for (; itor2 != filter.backfills.end(); itor2++)
	{
		if (itor2->isFilter)
		{
			if (!ForBackFillFilter(*itor2, data, size)) return false;
		}
	}
	////遍历正则
	//std::list<std::string>::const_iterator itor3 = filter.expressions.begin();
	//for (; itor3 != filter.expressions.end(); itor3++)
	//{
	//	//if (itor3->isFilter)
	//	//{
	//	//	if (!ForBackFillFilter(*itor2, data, size)) return false;
	//	//}
	//}
	//全部规则匹配
	return true;
}

bool XmlFilter::ForPrivateFilter(const STu8 *data, const STu32 size)
{
	std::list<LayerFilter*>::iterator iter = mDataFilter.GetBeginItor();
	for (; iter != mDataFilter.GetEndItor(); iter++)
	{
		if ((*iter)->GetLayerType() == Private)
		{
			if (!ForPrivateFilter(*(PrivateFilter*)(*iter), data, size)) return false;
		}
	}
	return true;
}


bool XmlFilter::ForFilter(const STu8 *data, const STu32 size, std::string exp)
{
	SigCode sigcode;
	sigcode.isFilter = true;
	PrivateFilter filter;
	bool isLoad=filter.LoadRegexExp(exp);
	if (isLoad)
	{
		return ForPrivateFilter(filter, data, size);
	}
	return true;
}

bool XmlFilter::IsValidateeExpression(std::string exp)
{
	CRegexFilter regex;
	if (regex.GetRegexFilterType(exp) != -1)
		return true;
	return false;
}

std::list<std::string> XmlFilter::LoadRegexExamples()
{
	CRegexFilter regex;
	return regex.LoadRegexExamples();
}