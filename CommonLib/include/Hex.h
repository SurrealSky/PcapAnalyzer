#pragma once
#include<string>
#include<Windows.h>

namespace SurrealHex
{
	class HexConvert
	{
	public:
		HexConvert() {};
		~HexConvert() {};
	public:
		std::string HexToStr(BYTE *pbSrc, int nLen)
		{
			std::string str;
			char ddl, ddh;
			int i;

			for (i = 0; i<nLen; i++)
			{
				ddh = 48 + pbSrc[i] / 16;
				ddl = 48 + pbSrc[i] % 16;
				if (ddh > 57) ddh = ddh + 7;
				if (ddl > 57) ddl = ddl + 7;
				str.push_back(ddh);
				str.push_back(ddl);
			}
			return str;
		}

		std::string StrToHex(BYTE *pbSrc, int nLen)
		{
			std::string str;
			char h1, h2;
			BYTE s1, s2;
			int i;

			for (i = 0; i<nLen / 2; i++)
			{
				h1 = pbSrc[2 * i];
				h2 = pbSrc[2 * i + 1];

				s1 = toupper(h1) - 0x30;
				if (s1 > 9)
					s1 -= 7;

				s2 = toupper(h2) - 0x30;
				if (s2 > 9)
					s2 -= 7;

				str.push_back(s1 * 16 + s2);
			}
			return str;
		}
	};
}

