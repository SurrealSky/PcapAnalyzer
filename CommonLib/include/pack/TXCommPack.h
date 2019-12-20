#pragma once
#include"ByteBuffer.h"

#define BUFFER_SIZE	0x200
#pragma pack(1)
class CTXCommPack
{
public:
	CTXCommPack(void);
	CTXCommPack(size_t);
	virtual ~CTXCommPack(void);
public:
	void AddByte(const char);
	void PreAddByte(const char);
	void AddWord(const short);
	void PreAddWord(const short);
	void AddDWord(const int);
	void PreAddDWord(const int);
	void AddDDWord(const long long );
	void PreAddDDWord(const long long);
	void AddBuf(const char *,const int);
	void PreAddBuf(const char *, const int);
	void AddBufLenWord();
	void AddBufLenDWord();
	void AddFieldCode(short);
	void ChangeByte(const int,const char*);
	void ChangeWord(const int,const short);
	void ChangeDWord(const int,const int);
	void ChangeDDWord(const int, const long long);
	void ChangeBuffer(const int,const char*,const int);
	void GetBufferOut();
public:
	ByteBuffer buffer;
};
