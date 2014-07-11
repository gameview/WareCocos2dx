//*************************************************************************
//	创建日期:	2013-4-29   11:15
//	文件名称:	CSVFileCommon.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	CSV文件定义
//*************************************************************************
#ifndef _CSVFILECOMMON_H_
#define _CSVFILECOMMON_H_

#include <vector>
#include <string>
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

#pragma pack( push, 1 )

//////////////////////////////////////////////////////////////////////
/// 以下是数据类型定义
/// 整型数组
struct IntArray
{
	std::vector<int> m_vecInt;

	int length() { return static_cast<int>( m_vecInt.size() ); }

	IntArray()
	{
		m_vecInt.clear();
	}

	~IntArray()
	{
		m_vecInt.clear();
	}
	
	int getData( int nIndex )
	{
		if( nIndex <= 0 || nIndex > length() )
		{
			return -1;
		}
	}
};

/// 双字节数组
struct WordArray
{
	std::vector<uint16> m_vecInt;

	int length() { return static_cast<int>( m_vecInt.size() ); }

	WordArray()
	{
		m_vecInt.clear();
	}

	~WordArray()
	{
		m_vecInt.clear();
	}

	uint16 getData( int nIndex )
	{
		if( nIndex <= 0 || nIndex > length() )
		{
			return -1;
		}
	}
};

/// 字节数组
struct ByteArray
{
	std::vector<BYTE> m_vecInt;

	int length() { return static_cast<int>( m_vecInt.size() ); }

	ByteArray()
	{
		m_vecInt.clear();
	}

	~ByteArray()
	{
		m_vecInt.clear();
	}

	BYTE getData( int nIndex )
	{
		if( nIndex <= 0 || nIndex > length() )
		{
			return -1;
		}
	}
};

/// CSV文件数据关键字范式定义
struct SCSVFileParadigm
{
	const char* szFileName;		/// CSV文件名 支持目录
	const char* szDataItemType;	/// 数据类型范式
	const char* szKeyParadigm;	/// 关键字范式

	SCSVFileParadigm()
	{
		szFileName = NULL;
		szDataItemType = NULL;
		szKeyParadigm = NULL;
	}
};

#pragma pack( pop )

NS_CC_ENGINE_END

#endif // _CSVFILECOMMON_H_