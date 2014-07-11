////*************************************************************************
//	创建日期:	2013-4-6   23:48
//	文件名称:	DataContent.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	多种数据类型存储
//*************************************************************************
#ifndef _DATACONTENT_H_
#define _DATACONTENT_H_

#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include <string>
#include "StringUtility.h"

NS_CC_ENGINE_BEGIN

/// 数据类型
enum EData_Type
{
	EDATA_TYPE_UNKOWN	= 0,
	EDATA_TYPE_INT32,			/// 整型
	EDATA_TYPE_UINT32,			/// 无符号整型
	EDATA_TYPE_FLOAT,			/// 浮点型
	EDATA_TYPE_STRING,			/// 字符串
	EDATA_TYPE_BUFF,			/// buff型
};

/// 数据实体
struct SDataContent
{
	SDataContent() : eDataType(EDATA_TYPE_UNKOWN), nDataLen(0), pData(NULL) {}

	explicit SDataContent( int nValue ) : eDataType(EDATA_TYPE_INT32), nDataLen(sizeof(int)), pData((char*)nValue) {}

	explicit SDataContent( uint32 uValue ) : eDataType(EDATA_TYPE_UINT32), nDataLen(sizeof(uint32)), pData((char*)uValue) {}

	explicit SDataContent( float fValue ) : eDataType(EDATA_TYPE_FLOAT), nDataLen(sizeof(float)), pData(NULL)
	{
		memcpy( (void*)&pData, (void*)&fValue, sizeof(float) );
	}

	explicit SDataContent( const char* pszString ) : eDataType(EDATA_TYPE_STRING), pData(NULL),nDataLen(0)
	{
		if( pszString )
		{
			nDataLen = strlen( pszString ) + 1;

			pData = new char[nDataLen];
			cocos2d::engine::StringUtility::sstrcpyn( (char*)pData, pszString, nDataLen );
		}
	}

	explicit SDataContent( const std::string& str ) : eDataType(EDATA_TYPE_STRING), pData(NULL),nDataLen(0)
	{
		if( !str.empty() )
		{
			nDataLen = str.length()+1;
			pData = new char[nDataLen];
			cocos2d::engine::StringUtility::sstrcpyn( (char*)pData, str.c_str(), nDataLen );
		}
	}

	SDataContent( const void* pBuff, int nLen ) : eDataType(EDATA_TYPE_BUFF), pData(NULL),nDataLen(0)
	{
		if( pBuff && nLen > 0 )
		{
			nDataLen = nLen;
			pData = new char[nDataLen];
			memcpy( (void*)pData, pBuff, nLen );
		}
	}

	SDataContent( const SDataContent& data )
	{
		if( eDataType == EDATA_TYPE_STRING || eDataType == EDATA_TYPE_BUFF )
		{
			clear();
		}

		if( data.eDataType == EDATA_TYPE_STRING || data.eDataType == EDATA_TYPE_BUFF )
		{
			//SAFE_DELARR( pData );
			nDataLen = data.nDataLen;
			pData = new char[nDataLen];
			memcpy( (void*)pData, data.pData, nDataLen );
		}
		else
		{
			pData = data.pData;
			nDataLen = data.nDataLen;
		}

		eDataType = data.eDataType;
		//throw( "SDataContent( SDataContent& data ): 类型不匹配！" );
	}

	~SDataContent()
	{
		clear();
	}

	EData_Type	getType() { return eDataType; }

	int			length()	{ return nDataLen; }			

	SDataContent& operator =( int nValue )
	{
		clear();

		eDataType = EDATA_TYPE_INT32;
		nDataLen = sizeof(int);
		pData = (char*)nValue;

		return *this;
	}

	SDataContent& operator =( uint32 uValue )
	{
		clear();

		eDataType = EDATA_TYPE_UINT32;
		nDataLen = sizeof(uint32);
		pData = (char*)uValue;

		return *this;
	}

	SDataContent& operator =( float fValue )
	{
		clear();

		eDataType = EDATA_TYPE_FLOAT;
		nDataLen = sizeof(float);
		memcpy( (void*)&pData, (void*)&fValue, sizeof(float) );

		return *this;
	}

	SDataContent& operator =( const char* pszString )
	{
		clear();

		eDataType = EDATA_TYPE_STRING;

		if( pszString )
		{
			nDataLen = strlen( pszString );
			nDataLen++;

			pData = new char[nDataLen];
			//strcpy( (char*)pData, pszString);
			cocos2d::engine::StringUtility::sstrcpyn( (char*)pData, pszString, nDataLen );
		}

		return *this;
	}

	SDataContent& operator =( const std::string& str )
	{
		clear();

		eDataType = EDATA_TYPE_STRING;

		if( !str.empty() )
		{
			nDataLen = str.length();
			nDataLen++;
			pData = new char[nDataLen];
			//strcpy( (char*)pData,  str.c_str() ); 
			cocos2d::engine::StringUtility::sstrcpyn( (char*)pData, str.c_str(), nDataLen );
		}

		return *this;
	}

	const int toInt()
	{
		if( eDataType == EDATA_TYPE_INT32 )
		{
			return (int)pData;
		}

		//throw( "SDataContent::ToInt(): 类型不正确！" );
		return 0;
	}

	const uint32 toUInt()
	{
		if( eDataType == EDATA_TYPE_UINT32 )
		{
			return (uint32)pData;
		}

		//throw( "SDataContent::ToUInt(): 类型不正确！" );
		return 0;
	}

	const float toFloat()
	{
		if( eDataType == EDATA_TYPE_FLOAT )
		{
			return *(float*)(&pData);
		}

		//throw( "SDataContent::ToFloat(): 类型不正确！" );
		return 0;
	}

	const char* toString()
	{
		if( eDataType == EDATA_TYPE_STRING )
		{
			return (const char*)pData;
		}

		//throw( "SDataContent::ToString(): 类型不正确！" );
		return NULL;
	}

	const void* toData()
	{
		if( eDataType == EDATA_TYPE_BUFF )
		{
			return (const void*)pData;
		}

		//throw( "SDataContent::ToString(): 类型不正确！" );
		return NULL;
	}

	void	clear()
	{
		if( eDataType == EDATA_TYPE_STRING || eDataType == EDATA_TYPE_BUFF )
		{
			SAFE_DELARR( pData );
		}

		eDataType = EDATA_TYPE_UNKOWN;
		nDataLen = 0;
	}

	EData_Type	eDataType;
	int			nDataLen;
	char*		pData;
};

NS_CC_ENGINE_END

#endif // _DATACONTENT_H_