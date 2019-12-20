#pragma once
#include<stdint.h>
#define TEA_KEY_LEN	0x10

#define TEA_ROUNDS_ONE	0x10
#define TEA_DELTA_ONE	0x9e3779b9    //0x9E3779B9 - 0x100000000 = -0x61C88647

#define TEA_ROUNDS_TWO	0x20
#define TEA_DELTA_TWO	0x57E89147

#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))


class CTeaCrypt
{
public:
	CTeaCrypt(void);
	~CTeaCrypt(void);
private:
	unsigned int dwTeaRound;
	unsigned int dwDelta;
	unsigned char key[TEA_KEY_LEN];
private:
	void decrypt_tea(unsigned long *in, unsigned long *key, unsigned long *out);
	void encrypt_tea(unsigned long *in, unsigned long *key, unsigned long *out);
public:
	void SetKey(unsigned char*, unsigned int len);
	void CommonInitTea(unsigned int mRound, unsigned int mDelta) { dwTeaRound = mRound; dwDelta = mDelta; };
public:
	void TencentInitTeaOne() { dwTeaRound = TEA_ROUNDS_ONE; dwDelta = TEA_DELTA_ONE; }
	void TencentInitTeaTwo() { dwTeaRound = TEA_ROUNDS_TWO; dwDelta = TEA_DELTA_TWO; }
public:
	unsigned int Tencent_xxTea_encrypt(unsigned char *s, int len, unsigned char **dst, unsigned int *bytes);
	unsigned int Tencent_xxTea_decrypt(unsigned char *s, int len, unsigned char **dst, unsigned int *bytes);
public:
	static void Randomkey(unsigned char *key, unsigned int len);
private:
	//标准的
	void Base_Tea_encrypt(uint32_t* v, uint32_t* k);
	void Base_Tea_decrypt(uint32_t* v, uint32_t* k);
	void Base_xtea_encrypt(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);
	void Base_xtea_decrypt(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]);
	void Base_xxtea(uint32_t *v, int n, uint32_t const key[4]);
	void Base_xxtea_encrypt(uint32_t *v, int n, uint32_t const key[4]);
	void Base_xxtea_decrypt(uint32_t *v, int n, uint32_t const key[4]);
public:
	void tea_encrypt(unsigned char* v, unsigned int len);
	void tea_decrypt(unsigned char* v, unsigned int len);
	void xtea_encrypt(unsigned char* v, unsigned int len);
	void xtea_decrypt(unsigned char* v, unsigned int len);
	//xxTea的优势在于它无需操作模式（CBC，OFB，CFB 等）
	void xxtea_encrypt(unsigned char *v, unsigned int len);
	void xxtea_decrypt(unsigned char *v, unsigned int len);
};