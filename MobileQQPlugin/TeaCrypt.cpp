#include<WinSock2.h>
#include "TeaCrypt.h"
#include<stdlib.h>
#include<math.h>

#pragma comment(lib,"Ws2_32.lib")


CTeaCrypt::CTeaCrypt(void) :dwTeaRound(TEA_ROUNDS_ONE), dwDelta(TEA_DELTA_ONE)
{
	memset(key, 0, TEA_KEY_LEN);
}
CTeaCrypt::~CTeaCrypt(void)
{
}

void CTeaCrypt::SetKey(unsigned char *k, unsigned int len)
{
	if (len > TEA_KEY_LEN)
	{
		memcpy(key, k, TEA_KEY_LEN);
	}
	else
		memcpy(key, k, len);
}

/*
*随机密钥初始化函数
*/
void CTeaCrypt::Randomkey(unsigned char *key, unsigned int len)
{
	memset(key, 0, TEA_KEY_LEN);
	srand(0x5f4567);
	srand(GetTickCount() + rand());
	for (int i = 0; i<len; i++)
	{
		int seed = rand();
		unsigned char r = seed;
		memcpy(key + i, &r, 0x1);
	}
}

/*
*加密函数
*/
void CTeaCrypt::encrypt_tea(unsigned long *in, unsigned long *key, unsigned long *out)
{
	unsigned long code[4];
	register unsigned long i = dwTeaRound, j = 0, m, n;

	m = htonl(in[0]);
	n = htonl(in[1]);
	code[0] = htonl(key[0]); code[1] = htonl(key[1]);
	code[2] = htonl(key[2]); code[3] = htonl(key[3]);
	while (i-->0) {
		j += dwDelta;
		m += ((n >> 5) + code[1]) ^ ((n << 4) + code[0]) ^ (j + n);
		n += ((m >> 5) + code[3]) ^ ((m << 4) + code[2]) ^ (j + m);
	}
	out[0] = htonl(m);
	out[1] = htonl(n);
}
unsigned int CTeaCrypt::Tencent_xxTea_encrypt(unsigned char *in, int inlen, unsigned char **out, unsigned int *outlen)
{
	register int m, i, j, count, p = 1;
	unsigned char q[12], *q1, *q2, *inp;
	unsigned char mkey[8];

	m = (inlen + 10) % 8;
	if (m)  m = 8 - m;

	*outlen = m + 3 + inlen;
	if (*outlen % 8 != 0)
	{
		*outlen = (*outlen / 8 + 1) * 8;
	}
	*out = (unsigned char*)malloc(*outlen);
	memset(*out, 0, *outlen);

	q[0] = (rand() & 0xf8) | m;
	i = j = 1;
	while (m>0) {
		q[i++] = rand() & 0xff;
		m--;
	}
	count = 0;
	q2 = q1 = *out;
	memset(mkey, 0, sizeof(mkey));
	while (p <= 2) {
		if (i < 8) {
			q[i++] = rand() & 0xff;
			p++;
		}
		if (i == 8) {
			for (i = 0; i < 8; i++)
				q[i] ^= mkey[i];
			encrypt_tea((unsigned long *)q, (unsigned long *)key, (unsigned long *)(*out));
			for (i = 0; i < 8; i++)
				q1[i] ^= mkey[i];
			q2 = q1;
			q1 += 8;
			count += 8;
			memcpy(mkey, q, 8);
			j = i = 0;
		}
	}
	inp = in;
	while (inlen > 0) {
		if (i < 8) {
			q[i] = inp[0];
			inp++;
			i++;
			inlen--;
		}
		if (i == 8) {
			for (i = 0; i < 8; i++) {
				if (j) q[i] ^= mkey[i];
				else q[i] ^= q2[i];
			}
			j = 0;
			encrypt_tea((unsigned long *)q, (unsigned long *)key, (unsigned long *)q1);
			for (i = 0; i < 8; i++)
				q1[i] ^= mkey[i];
			count += 8;
			memcpy(mkey, q, 8);
			q2 = q1;
			q1 += 8;
			i = 0;
		}
	}
	p = 1;
	while (p < 8) {
		if (i < 8) {
			memset(q + i, 0, 4);
			p++;
			i++;
		}
		if (i == 8) {
			for (i = 0; i < 8; i++)
				q[i] ^= q2[i];
			encrypt_tea((unsigned long *)q, (unsigned long *)key, (unsigned long *)q1);
			for (i = 0; i < 8; i++)
				q1[i] ^= mkey[i];
			memcpy(mkey, q, 8);
			count += 8;
			q2 = q1;
			q1 += 8;
			i = 0;
		}
	}
	return 1;
}

/*
*
解密函数
*
*/
void CTeaCrypt::decrypt_tea(unsigned long *in, unsigned long *key, unsigned long *out)
{

	unsigned long code[4];
	register unsigned long n = dwTeaRound, sum, y, z, delta = dwDelta;

	sum = delta * n;
	y = htonl(in[0]);
	z = htonl(in[1]);

	code[0] = htonl(key[0]); code[1] = htonl(key[1]);
	code[2] = htonl(key[2]); code[3] = htonl(key[3]);

	while (n-->0)
	{
		z -= ((y >> 5) + code[3]) ^ ((y << 4) + code[2]) ^ (sum + y);
		y -= ((z >> 5) + code[1]) ^ ((z << 4) + code[0]) ^ (sum + z);
		sum -= delta;
	}
	out[0] = htonl(y);
	out[1] = htonl(z);
}
unsigned int CTeaCrypt::Tencent_xxTea_decrypt(unsigned char *in, int inlen, unsigned char **out, unsigned int *bytes)
{
	unsigned char q[8], mkey[8], *q1, *q2, *outp;
	register int count, i, j, p;

	if (inlen % 8 || inlen<16) return 0;
	/* get basic information of the packet */
	decrypt_tea((unsigned long *)in, (unsigned long *)key, (unsigned long *)q);
	j = q[0] & 0x7;
	count = inlen - j - 10;
	*bytes = count;
	*out = (unsigned char *)malloc(*bytes);
	if (*out == NULL) return 0;
	memset(*out, 0, *bytes);


	if (count < 0) return 0;

	memset(mkey, 0, 8);
	q2 = mkey;
	i = 8; p = 1;
	q1 = in + 8;
	j++;
	while (p <= 2)
	{
		if (j < 8)
		{
			j++;
			p++;
		}
		else if (j == 8)
		{
			q2 = in;
			for (j = 0; j < 8; j++)
			{
				if (i + j >= inlen) return 0;
				q[j] ^= q1[j];
			}
			decrypt_tea((unsigned long *)q, (unsigned long *)key,
				(unsigned long *)q);
			i += 8;
			q1 += 8;
			j = 0;
		}
	}
	outp = *out;
	while (count != 0)
	{
		if (j < 8)
		{
			outp[0] = q2[j] ^ q[j];
			outp++;
			count--;
			j++;
		}
		else if (j == 8)
		{
			q2 = q1 - 8;
			for (j = 0; j < 8; j++)
			{
				if (i + j >= inlen)
					return 0;
				q[j] ^= q1[j];
			}
			decrypt_tea((unsigned long *)q, (unsigned long *)key,
				(unsigned long *)q);
			i += 8;
			q1 += 8;
			j = 0;
		}
	}
	for (p = 1; p < 8; p++)
	{
		if (j < 8)
		{
			if (q2[j] ^ q[j])
				return 0;
			j++;
		}
		else if (j == 8)
		{
			q2 = q1;
			for (j = 0; j < 8; j++)
			{
				if (i + j >= inlen)
					return 0;
				q[j] ^= q1[j];
			}
			decrypt_tea((unsigned long *)q, (unsigned long *)key,
				(unsigned long *)q);
			i += 8;
			q1 += 8;
			j = 0;
		}
	}
	return 1;

}


void CTeaCrypt::Base_Tea_encrypt(uint32_t* v, uint32_t* k)
{
	uint32_t v0 = v[0], v1 = v[1], sum = 0, i;           /* set up */
	uint32_t delta = dwDelta;                     /* a key schedule constant */
	uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];   /* cache key */
	for (i = 0; i < 32; i++) {                       /* basic cycle start */
		sum += delta;
		v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
	}                                              /* end cycle */
	v[0] = v0; v[1] = v1;
}

void CTeaCrypt::Base_Tea_decrypt(uint32_t* v, uint32_t* k) {
	uint32_t v0 = v[0], v1 = v[1], sum = 0xC6EF3720, i;  /* set up */
	uint32_t delta = dwDelta;                     /* a key schedule constant */
	uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];   /* cache key */
	for (i = 0; i<32; i++) {                         /* basic cycle start */
		v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
		v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		sum -= delta;
	}                                              /* end cycle */
	v[0] = v0; v[1] = v1;
}

void CTeaCrypt::Base_xtea_encrypt(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
	unsigned int i;
	uint32_t v0 = v[0], v1 = v[1], sum = 0, delta = dwDelta;
	for (i = 0; i < num_rounds; i++) {
		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
		sum += delta;
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
	}
	v[0] = v0; v[1] = v1;
}

void CTeaCrypt::Base_xtea_decrypt(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
	unsigned int i;
	uint32_t v0 = v[0], v1 = v[1], delta = dwDelta, sum = delta*num_rounds;
	for (i = 0; i < num_rounds; i++) {
		v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
		sum -= delta;
		v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
	}
	v[0] = v0; v[1] = v1;
}

void CTeaCrypt::Base_xxtea_encrypt(uint32_t *v, int n, uint32_t const key[4])
{
	uint32_t y, z, sum;
	unsigned p, rounds, e;
	rounds = 6 + 52 / n;
	sum = 0;
	z = v[n - 1];
	do
	{
		sum += dwDelta;
		e = (sum >> 2) & 3;
		for (p = 0; p<n - 1; p++)
		{
			y = v[p + 1];
			z = v[p] += MX;
		}
		y = v[0];
		z = v[n - 1] += MX;
	} while (--rounds);

}

void CTeaCrypt::Base_xxtea_decrypt(uint32_t *v, int n, uint32_t const key[4])
{
	uint32_t y, z, sum;
	unsigned p, rounds, e;
	rounds = 6 + 52 / n;
	sum = rounds*dwDelta;
	y = v[0];
	do
	{
		e = (sum >> 2) & 3;
		for (p = n - 1; p>0; p--)
		{
			z = v[p - 1];
			y = v[p] -= MX;
		}
		z = v[n - 1];
		y = v[0] -= MX;
		sum -= dwDelta;
	} while (--rounds);

}

void CTeaCrypt::tea_encrypt(unsigned char* v, unsigned int len)
{
	for (int i = 0; i < len / 8; i++)
	{
		Base_Tea_encrypt((unsigned int*)(v + i * 8), (uint32_t*)key);
	}
}

void CTeaCrypt::tea_decrypt(unsigned char* v, unsigned int len)
{
	for (int i = 0; i < len / 8; i++)
	{
		Base_Tea_decrypt((unsigned int*)(v + i * 8), (uint32_t*)key);
	}
}

void CTeaCrypt::xtea_encrypt(unsigned char *v, unsigned int len)
{
	for (int i = 0; i < len / 8; i++)
	{
		Base_xtea_encrypt(dwTeaRound, (unsigned int*)(v + i * 8), (uint32_t*)key);
	}
}

void CTeaCrypt::xtea_decrypt(unsigned char *v, unsigned int len)
{
	for (int i = 0; i < len / 8; i++)
	{
		Base_xtea_decrypt(dwTeaRound, (unsigned int*)(v + i * 8), (uint32_t*)key);
	}
}

void CTeaCrypt::xxtea_encrypt(unsigned char *v, unsigned int len)
{
	Base_xxtea_encrypt((unsigned int*)v, len / 4, (uint32_t*)key);
}

void CTeaCrypt::xxtea_decrypt(unsigned char *v, unsigned int len)
{
	Base_xxtea_decrypt((unsigned int*)v, len / 4, (uint32_t*)key);
}