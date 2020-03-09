#pragma once
#include<string>
#include<xml\Markup.h>
#include<list>
#include"packet.h"
#include"EthernetFilter.h"
#include"IPv4Filter.h"
#include"TCPFilter.h"
#include"UDPFilter.h"
#include"PrivateFilter.h"
#include"DataFilter.h"
#include<typedef.h>
using namespace SurrealTypes;

//namespace SurrealPacket
//{
	class XmlFilter
	{
	public:
		XmlFilter();
		~XmlFilter();
	public:
		DataFilter mDataFilter;
	private:
		CodeType	ValueCodeType(LayerType,std::string);
		VALUE_TYPE ValueValueType(std::string);
		std::string ValueValue(VALUE_TYPE,std::string);
		bool ValueisFilter(std::string value);
		AppLayerStyle ValueAppLayerStyle(std::string);
		LayerType ValueLayerType(std::string);
		OffsetType ValueOffsetType(std::string);
		void ValueMAC(VALUE_TYPE valuetype,std::string value, STu8 MAC[]);
		STu16 ValueEthernetProtocol(std::string value);
		STu32 ValueIP(VALUE_TYPE valuetype, std::string value);
		STu16 ValuePort(VALUE_TYPE valuetype,std::string value);
		STu32 ValueOffset(std::string value);
		STu32 ValueStep(std::string value);
		STu16 ValueIPProtocol(std::string value);
		unsigned char ValueEndian(std::string);
	private:
		void ReadLayerFilter(CMarkup, LayerFilter*);
		void ReadSigCode(CMarkup, std::list<SigCode>&);
		void ReadRegexExp(CMarkup, PrivateFilter*);
		void ReadBackFill(CMarkup, std::list<BackFill>&);
	public:
		STu32	CalcOffset(OffsetType type, STu32 offset, STu32 step);
		std::string FormatTime(time_t time1);
		std::string IPProtocol2String(STu16 protocol);
		bool ReadFiltersFromXML(std::string file,std::string action,std::string filterindex);
		bool ReadActionsFromXML(std::string file, std::list<CommonAction> &mActions);
	private:
		bool IsHexadecimal(std::string);
		bool IsOctal(std::string);
		bool IsBinary(std::string);
		bool IsDecimal(std::string);
		STu32 ValueDigit(std::string);
		/*
		*���أ�true�����ݰ������ƥ��
		*���أ�false�����ݰ������ƥ��
		*/
		bool ForSigCodeFilter(const SigCode &sig, const STu8 *, const STu32);
		/*
		*���أ�true�����ݰ������ƥ��
		*���أ�false�����ݰ������ƥ��
		*/
		bool ForBackFillFilter(const BackFill &sig, const STu8 *, const STu32);
		/*
		*���أ�true�����ݰ������ƥ��
		*���أ�false�����ݰ������ƥ��
		*/
		bool ForPrivateFilter(const PrivateFilter &, const STu8 *, const STu32 size);
	public:
		/*
		*���أ�true�����ݰ������ƥ��
		*���أ�false�����ݰ������ƥ��
		*/
		bool ForFilter(const STu8 *,const STu32 size, std::string exp);
		/*
		*���أ�true�����ݰ������ƥ��
		*���أ�false�����ݰ������ƥ��
		*/
		bool ForPrivateFilter(const STu8 *, const STu32 size);
		/*
		*���أ�true����ƥ����
		*���أ�false����ƥ����
		*/
		bool IsValidateeExpression(std::string);
		/*
		*���أ����˱��ʽʾ��
		*/
		std::list<std::string> LoadRegexExamples();
	};
//}


