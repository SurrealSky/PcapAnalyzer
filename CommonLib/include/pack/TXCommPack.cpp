#include "TXCommPack.h"
#include<stdlib.h>
#include<WinSock.h>
#include<ObjBase.h>
#include<shlobj.h>   
#include<Windows.h>
#include<WinIoCtl.h>

#pragma   comment(lib,"shell32.lib") 


CTXCommPack::CTXCommPack(void)
{
}

CTXCommPack::CTXCommPack(size_t size)
{
}

CTXCommPack::~CTXCommPack(void)
{
}

void CTXCommPack::AddByte(const char b)
{
	buffer.append(b);
}

void CTXCommPack::PreAddByte(const char b)
{
	buffer.insert(0, b);
}

void CTXCommPack::AddWord(const short data)
{
	buffer.append<uint16>(data);
}

void CTXCommPack::PreAddWord(const short data)
{
	buffer.insert<uint16>(0, data);
}

void CTXCommPack::AddDWord(const int data)
{
	buffer.append<uint32>(data);
}

void CTXCommPack::PreAddDWord(const int data)
{
	buffer.insert<uint32>(0,data);
}

void CTXCommPack::AddDDWord(const long long data)
{
	buffer.append<uint64>(data);
}

void CTXCommPack::PreAddDDWord(const long long data)
{
	buffer.insert<uint64>(0, data);
}

void CTXCommPack::AddBuf(const char *buf,const int buflen)
{
	buffer.append((uint8*)buf,buflen);
}

void CTXCommPack::PreAddBuf(const char *buf, const int buflen)
{
	buffer.insert(0,(uint8*)buf, buflen);
}

void CTXCommPack::AddBufLenWord()
{
	unsigned short len=buffer.size();
	//应该前置插入
	buffer.insert<uint16>(0,htons(len));
}

void CTXCommPack::AddBufLenDWord()
{
	unsigned int len = buffer.size();
	//应该前置插入
	buffer.insert<uint32>(0, htonl(len));
}

//插入前置特征码
void CTXCommPack::AddFieldCode(short value)
{
	buffer.insert<uint16>(0,value);
}

void CTXCommPack::GetBufferOut()
{
	buffer.clear();
}

void CTXCommPack::ChangeByte(const int pos,const char *byte)
{
	buffer.put<uint8>(pos, (uint8)byte);
}

void CTXCommPack::ChangeWord(const int pos, const short s)
{
	buffer.put<uint16>(pos, s);
}

void CTXCommPack::ChangeDWord(const int pos, const int s)
{
	buffer.put<uint32>(pos, s);
}

void CTXCommPack::ChangeDDWord(const int pos, const long long v)
{
	buffer.put<uint64>(pos, v);
}

void CTXCommPack::ChangeBuffer(const int pos, const char *b,const int s)
{
	buffer.put(pos, (uint8*)b,s);
}