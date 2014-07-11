//*************************************************************************
//	创建日期:	2013-9-15   18:08
//	文件名称:	HtmlTagAttribute.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	html属性管理器
//*************************************************************************
#ifndef _HTMLTAGATTRIBUTE_H__
#define _HTMLTAGATTRIBUTE_H__

#include "cocos2d.h"
#include <string>
#include <map>

NS_CC_BEGIN

class CHtmlTagAttribute
{
public:
	CHtmlTagAttribute(void);
	~CHtmlTagAttribute(void);

	/**
	@brief 属性解析
	@param
	*/
	void		parseAttribute( const std::string& strAttribute );

	/**
	@brief 获取整数属性
	@param
	*/
	int			getIntValue( const char* key );

	/**
	@brief 获取整数属性
	@param
	*/
	const char*	getStringValue( const char* key );

	/**
	@brief 获取整数属性
	@param
	*/
	float		getFloatValue( const char* key );

	/**
	@brief 获取整数属性
	@param
	*/
	ccColor3B	getColorValue( const char* key );

	/**
	@brief 获取整数属性
	@param
	*/
	bool		getBoolValue( const char* key );

private:

	std::map< std::string, std::string>		m_mapAttribute;
};

NS_CC_END

#endif // _HTMLTAGATTRIBUTE_H__

