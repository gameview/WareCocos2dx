//*************************************************************************
//	创建日期:	2013-7-23   15:36
//	文件名称:	StringConvert.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	字符转换
//*************************************************************************
#ifndef _STRINGCONVERT_H_
#define _STRINGCONVERT_H_
#include <stdlib.h>
#include "cocos2dx_types.h"
#include "cocos2d.h"
#include "cocos2dx_macros.h"
#include <string>

NS_CC_ENGINE_BEGIN
class StringConvert
{
public:
	/** Converts a string to a float. 
	@returns
	0.0 if the value could not be parsed, otherwise the float version of the string.
	*/
	static float parseFloat(const std::string& val);

	/** Converts a string to a whole number. 
	@returns
	0.0 if the value could not be parsed, otherwise the numeric version of the string.
	*/
	static int parseInt(const std::string& val);

	/**
	@brief convert a string to color
	@param
	*/
	static ccColor4B parseColor(const std::string& val);

	/**
	@brief convert a string to point
	@param
	*/
	static cocos2d::CCPoint	parsePoint( const std::string& val );

	/**
	@brief convert a string to point 以点为分隔符
	@param 
	*/
	static cocos2d::CCPoint	parseDotPoint( const std::string& val );

	/**
	@brief convert color with #
	@param
	*/
	static cocos2d::ccColor3B parseSharpColor( const std::string& val );
};

NS_CC_ENGINE_END


#endif // _STRINGCONVERT_H_


