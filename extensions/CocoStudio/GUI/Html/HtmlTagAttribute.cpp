#include "HtmlTagAttribute.h"
#include "StringUtility.h"
#include "StringConvert.h"

NS_CC_BEGIN

CHtmlTagAttribute::CHtmlTagAttribute(void)
{
	m_mapAttribute.clear();
}

CHtmlTagAttribute::~CHtmlTagAttribute(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 属性解析 目前不支持 = 旁边有空格 以后改进
@param
*/
void CHtmlTagAttribute::parseAttribute( const std::string& strAttribute )
{
	const char* pBuff = strAttribute.c_str();
	char szOutBuff[256];
	do 
	{
		memset( szOutBuff, 0, sizeof(szOutBuff) );
		pBuff = cocos2d::engine::StringUtility::parseSubString( pBuff, ' ', szOutBuff );
		if( szOutBuff[0] != NULL )
		{
			std::string strOut = szOutBuff;
			cocos2d::engine::StringUtility::trim( strOut );
			const char* pPropText = strOut.c_str();
			memset( szOutBuff, 0, sizeof(szOutBuff) );
			pPropText = cocos2d::engine::StringUtility::parseSubString( pPropText, '=', szOutBuff );
			std::string strName = szOutBuff;
			std::string strProp = pPropText;
			cocos2d::engine::StringUtility::trim(strName);
			cocos2d::engine::StringUtility::trim(strProp);

			/// 去掉左右的 ' "
			size_t pos = strProp.find( '\'' );
			if( pos != std::string::npos )
			{
				strProp = strProp.substr( pos + 1, strProp.length() - pos );
			}
			pos = strProp.rfind( '\'');
			if( pos != std::string::npos )
			{
				strProp = strProp.substr( 0, strProp.length() - 1 );
			}

			pos = strProp.find( '\"' );
			if( pos != std::string::npos )
			{
				strProp = strProp.substr( pos + 1, strProp.length() - pos );
			}
			pos = strProp.rfind( '\"');
			if( pos != std::string::npos )
			{
				strProp = strProp.substr( 0, strProp.length() - 1 );
			}

			cocos2d::engine::StringUtility::trim( strProp );
	
			m_mapAttribute[strName] = strProp;
		}
	} while ( pBuff[0] != NULL );
}
//-------------------------------------------------------------------------
/**
@brief 获取整数属性
@param
*/
int CHtmlTagAttribute::getIntValue( const char* key )
{
	if( key == NULL )
	{
		return NULL;
	}

	std::map< std::string, std::string>::iterator iter = m_mapAttribute.find( key );
	if( iter == m_mapAttribute.end() )
	{
		return NULL;
	}

	return cocos2d::engine::StringConvert::parseInt( iter->second );
}
//-------------------------------------------------------------------------
/**
@brief 获取整数属性
@param
*/
const char*	CHtmlTagAttribute::getStringValue( const char* key )
{
	if( key == NULL )
	{
		return NULL;
	}

	std::map< std::string, std::string>::iterator iter = m_mapAttribute.find( key );
	if( iter == m_mapAttribute.end() )
	{
		return NULL;
	}

	return iter->second.c_str();
}
//-------------------------------------------------------------------------
/**
@brief 获取整数属性
@param
*/
float CHtmlTagAttribute::getFloatValue( const char* key )
{
	if( key == NULL )
	{
		return NULL;
	}

	std::map< std::string, std::string>::iterator iter = m_mapAttribute.find( key );
	if( iter == m_mapAttribute.end() )
	{
		return NULL;
	}

	return cocos2d::engine::StringConvert::parseFloat( iter->second );
}
//-------------------------------------------------------------------------
/**
@brief 获取整数属性
@param
*/
ccColor3B CHtmlTagAttribute::getColorValue( const char* key )
{
	if( key == NULL )
	{
		return ccc3( 255,255,255);
	}

	std::map< std::string, std::string>::iterator iter = m_mapAttribute.find( key );
	if( iter == m_mapAttribute.end() )
	{
		return ccc3( 255,255,255);
	}

	return cocos2d::engine::StringConvert::parseSharpColor( iter->second );
}
//-------------------------------------------------------------------------
/**
@brief 获取整数属性
@param
*/
bool CHtmlTagAttribute::getBoolValue( const char* key )
{
	if( key == NULL )
	{
		return false;
	}

	std::map< std::string, std::string>::iterator iter = m_mapAttribute.find( key );
	if( iter == m_mapAttribute.end() )
	{
		return false;
	}

	if( iter->second == "" )
	{
		return true;
	}
	else
	{
		return cocos2d::engine::StringConvert::parseInt( iter->second ) == 1;
	}

}

NS_CC_END