//
// Created by wzh.
//

#ifndef NDKAPPECG_TYPEDEF_H
#define NDKAPPECG_TYPEDEF_H

namespace SurrealTypes
{
#if defined(_MSC_VER)
	//
	// Windows/Visual C++
	//
	typedef unsigned __int8		STbool;		//布尔变量
	typedef unsigned __int8     STu8;		// 无符号8位整型变量
	typedef signed __int8		STs8;		// 有符号8位整型变量
	typedef signed __int16      STs16;		// 有符号16位整型变量
	typedef unsigned __int16    STu16;		// 无符号16位整型变量
	typedef signed __int32      STs32;		// 有符号32位整型变量
	typedef unsigned __int32    STu32;		//无符号32位整型变量
	typedef signed __int64      STs64;		// 有符号64位整型变量
	typedef unsigned __int64    STu64;		//无符号64位整型变量
	typedef float				STfp32;		// 单精度浮点数（32位长度）
	typedef double				STfp64;		// 双精度浮点数（64位长度）
#else
	//定义平台无关的类型(注意 typedef 并不创建新的类型,它仅仅为现有类型添加一个同义字。)
	typedef unsigned char  STbool;		//布尔变量
	typedef unsigned char  STu8;		// 无符号8位整型变量
	typedef char           STs8;		// 有符号8位整型变量
	typedef unsigned short STu16;		// 无符号16位整型变量
	typedef signed short   STs16;		// 有符号16位整型变量
	typedef unsigned int   STu32;		//无符号32位整型变量
	typedef signed int     STs32;		// 有符号32位整型变量
	typedef signed long long	STs64;	// 有符号64位整型变量
	typedef unsigned long long	STu64;	//无符号64位整型变量
	typedef float          STfp32;		// 单精度浮点数（32位长度）
	typedef double         STfp64;		// 双精度浮点数（64位长度）
#endif

#define TRUE 1
#define FALSE 0

//字节序转换宏
#define STswab64(x) ((x&0x00000000000000ff) << 56|(((x&0x000000000000ff00) << 40)|((x&0x0000000000ff0000) << 24)|((x&0x00000000ff000000) <<  8)|((x&0x000000ff00000000) >>  8)|((x&0x0000ff0000000000) >> 24)|((x&0x00ff000000000000) >> 40)|((x&0xff00000000000000) >> 56)))
#define STswab32(x) ((x&0x000000ff) << 24 | (x&0x0000ff00) << 8 | (x&0x00ff0000) >> 8 | (x&0xff000000) >> 24)
#define STswab16(x) ((x&0x00ff) << 8 | (x&0xff00) >> 8)
}

#endif


