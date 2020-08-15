// MobileQQPlugin.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include"TeaCrypt.h"
#include<typedef.h>
using namespace SurrealTypes;
#include<sstream>

class MobileQQAnalyzer : public IAnalyzer
{
public:
	~MobileQQAnalyzer() 
	{
	}
private:
	unsigned int Skip4LV(const char *data, const unsigned int offset, const unsigned int len, std::string &value)
	{
		value = "";
		if (offset >= len) return 0;

		STu32 size = STswab32(*(int*)data);
		if (size > 4)
		{
			if (size + offset > len) size = len - offset;
			value.append(data + 4, size - 4);
		}
		return size;
	}
	unsigned int Skip2LV(const char *data, const unsigned int offset, const unsigned int len, std::string &value)
	{
		value = "";
		if (offset >= len) return 0;
		STu16 size = STswab16(*(STu16*)data);
		if (size>2)
			value.append(data + 2, size - 2);
		return size;
	}
	void ForStruct(const char *data, unsigned int len, std::map<std::string, std::string> &result, const bool isClient2server)
	{
		const char *p = data;
		int offset = 0;
		std::string first = "";
		offset += Skip4LV(data + offset, offset, len, first);
		if (first.size())
			ForFirst(first, result,isClient2server);
		std::string second = "";
		offset += Skip4LV(data + offset, offset, len, second);
		if (second.size())
			ForSecond(second, result, isClient2server);
	}
	void ForFirst(std::string &value, std::map<std::string, std::string> &result, const bool isClient2server)
	{
		const char *data = value.c_str();
		const unsigned int len = value.size();
		int offset = 0;
		//多个包
		int seq = STswab32(*(int*)(data + offset));
		//seq id判断
		if ((seq & 0x0000FF00) != 0)
		{
			//seq id包含两种情况
			//00 00 XX XX
			//00 ## XX XX 
			offset += 4;
			if (isClient2server)
				offset += 0x10;
			offset += 4;
			std::string value;
			offset += Skip4LV((char*)data + offset, offset, len, value);
			offset += Skip4LV((char*)data + offset, offset, len, value);
			result.insert(std::pair<std::string, std::string>("serviceCmd", value));
			offset += Skip4LV((char*)data + offset, offset, len, value);
			offset += Skip4LV((char*)data + offset, offset, len, value);
			if (value.size() != 0)
				result.insert(std::pair<std::string, std::string>("imei", value));
			offset += Skip4LV((char*)data + offset, offset, len, value);
			offset += Skip2LV((char*)data + offset, offset, len, value);
			if (value.size() != 0)
				result.insert(std::pair<std::string, std::string>("version", value));
		}
		else
		{
			std::string value;
			offset += Skip4LV((char*)data + offset, offset, len, value);
			result.insert(std::pair<std::string, std::string>("serviceCmd", value));
			offset += Skip4LV((char*)data + offset, offset, len, value);
			offset += Skip4LV((char*)data + offset, offset, len, value);
			if (value.size() != 0)
				result.insert(std::pair<std::string, std::string>("imei", value));
			offset += Skip4LV((char*)data + offset, offset, len, value);
			offset += Skip2LV((char*)data + offset, offset, len, value);
			if (value.size() != 0)
				result.insert(std::pair<std::string, std::string>("version", value));
		}
	}
	void ForSecond(std::string &value, std::map<std::string, std::string> &result, const bool isClient2server)
	{
		//jce协议,暂不支持解析
		const char *data = value.c_str();
		const unsigned int len = value.size();
		int offset = 0;
		//多个包
		int seq = STswab32(*(int*)(data + offset));
		//seq id判断
	}
public:
	bool IsClient2Server(const unsigned int srcPort, const unsigned int dstPrt)
	{
		if (srcPort == 8080 || srcPort == 14000 || srcPort == 443|| srcPort==80)
			return false;
		else if (dstPrt == 8080 || dstPrt == 14000 || dstPrt == 443|| dstPrt==80)
			return true;
		return true;
	}
	bool ForFilter(const unsigned int srcPort, const unsigned int dstPrt,const char *pbody, const unsigned int bodylen)
	{
		//长度过滤
		if (bodylen < 0x14)
		{
			return false;
		}
		//在端口过滤
		bool isFilter = false;
		if (srcPort == 8080 || srcPort == 14000 || srcPort == 443|| srcPort == 80)
			isFilter = true;
		if (!isFilter)
		{
			if (dstPrt == 8080 || dstPrt == 14000 || dstPrt == 443|| dstPrt == 80)
				isFilter = true;
		}
		if (isFilter)
		{
			//判断是否是MSF数据包
			if (pbody[4] == 0x1 && pbody[5] == 0x33 && pbody[6] == 0x52 && pbody[7] == 0x39 &&
				pbody[8] == 0x0 && pbody[9] == 0x0 && pbody[0xA] == 0x0 && pbody[0xB] == 0x0 &&
				pbody[0xC] == 0x4 && pbody[0xD] == 0x4d && pbody[0xE] == 0x53 && pbody[0xF] == 0x46)
			{
				return true;
			}
			else
			{
				//常规会话流特征判断
				if (pbody[4] == 0 && pbody[5] == 0 && pbody[6] == 0 && (pbody[7] >= 0x8 && pbody[7] <= 0xF) && (pbody[8] >= 0x0 && pbody[8] <= 0x2))
				{
					return true;
				}
			}
		}
		return false;
	}
	unsigned int ActualLen(const char *pbody, const unsigned int bodylen,const bool isClient2Server)
	{
		unsigned int len = STswab32(*(int*)pbody);
		return len;
	}
	std::map<std::string, std::string> Analysis(const char *pbody, const unsigned int bodylen, const bool isClient2Server)
	{
		std::map<std::string, std::string> mapresult;
		if (isClient2Server)
			mapresult.insert(std::pair<std::string, std::string>("client2server", "C->S"));
		else
			mapresult.insert(std::pair<std::string, std::string>("client2server", "S->C"));

		//先判断MSF数据包
		if (pbody[0xC] == 0x4 && pbody[0xD] == 0x4d && pbody[0xE] == 0x53 && pbody[0xF] == 0x46)
		{
			//MSF数据包
			mapresult.insert(std::pair<std::string, std::string>("serviceCmd", "MSF"));
			STu32 id = STswab32(*(int*)(pbody + 8));
			std::stringstream ss;
			ss << id;
			mapresult.insert(std::pair<std::string, std::string>("qq", ss.str()));
			ss.clear();
			ss.str("");
			ss << STu32(0);
			mapresult.insert(std::pair<std::string, std::string>("ciphertype", ss.str()));
			return mapresult;
		}

		//设置加密类型
		std::stringstream ss;
		ss << STu8(pbody[0x8]);
		mapresult.insert(std::pair<std::string, std::string>("ciphertype", ss.str()));

		//取出四字节，客户端发出的数据包多出一个LV的数据，分以下情况
		int value = STswab32(*(int*)(pbody + 9));
		int offset = 9;
		if (isClient2Server)
		{
			if (value == 0x44)
			{
				//附带token
				offset += 0x44;
			}
			else if (value == 0x4)
			{
				//空LV
				offset += 4;
			}
			else if ((value & 0x0000FF00) != 0x0)
			{
				//seq id包含两种情况
				//00 00 XX XX
				//00 ## XX XX
				//其中XX为seq id
				offset += 4;
			}
			else if ((value & 0xFF000000) != 0x0)
			{
				//未知情况
				//XX 00 00 00
				offset += 4;
			}
			else if (value == 0x00010001)
			{
				//未知情况
				offset += 4;
			}else
			{
				MessageBox(0, "error", "unknow", MB_OK);
				mapresult.clear();//不还原此数据包
				return mapresult;
			}

		}
		else
		{
			if (value == 0)
			{
				//00 00 00 00
			}
			else if ((value & 0xFF000000) != 0x0)
			{
				//未知情况
				//XX 00 00 00
			}else
			{
				MessageBox(0, "error", "unknow", MB_OK);
				mapresult.clear();//不还原此数据包
				return mapresult;
			}
		}
		offset += 1;//跳过一字节0
		//开始取QQ号码
		std::string qq = "";
		offset += Skip4LV(pbody + offset, offset, bodylen, qq);
		mapresult.insert(std::pair<std::string, std::string>("qq", qq));
		if (mapresult["ciphertype"][0] == 0x00)
		{
			// 明文数据，直接解析
			ForStruct((char*)pbody + offset, bodylen - offset, mapresult, isClient2Server);
		}
		else if (mapresult["ciphertype"][0] == 0x01)
		{
			//SessionKey
		}
		else if (mapresult["ciphertype"][0] == 0x02)
		{
			//全0解密
			int declen = bodylen - offset;
			unsigned char *dst = NULL;
			unsigned int bytes = 0;
			CTeaCrypt tea;
			tea.Tencent_xxTea_decrypt((unsigned char*)pbody + offset, declen, &dst, &bytes);
			std::string ciphertext = "";
			ciphertext.insert(0, (char*)dst, bytes);
			mapresult.insert(std::pair<std::string, std::string>("ciphertext", ciphertext));
			ForStruct((char*)dst, bytes, mapresult, isClient2Server);
			free(dst);
			dst = NULL;
		}
		else
		{
			//错误类型，尚未遇到
			mapresult.insert(std::pair<std::string, std::string>("ciphertype", "error"));
		}
		return mapresult;
	}
	std::map<std::string, std::string> AnalysisList(const std::list<IAnalyzerData>& packets)
	{
		std::map<std::string, std::string> mapresult;
		mapresult.insert(std::pair<std::string, std::string>("error", "N/A"));
		return mapresult;
	}
};

PLUGIN_FUNC IAnalyzer *CreateAnalyzer()
{
	return new MobileQQAnalyzer;
}

PLUGIN_FUNC void DestroyAnalyzer(IAnalyzer *r)
{
	delete r;
}

PLUGIN_DISPLAY_NAME("MobileQQ Analyzer");

PLUGIN_INIT()
{
	// register our new renderer
	RegisterAnalyzerer("MobileQQ Analyzer", CreateAnalyzer, DestroyAnalyzer);
	return 0;
}


