//*************************************************************************
//	创建日期:	2013-9-16   20:28
//	文件名称:	HtmlExp.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	表情动画
//*************************************************************************
#ifndef _HTMLEXP_H__
#define _HTMLEXP_H__

#include "IHtmlElement.h"
#include <string>
#include "HtmlTagAttribute.h"

NS_CC_BEGIN

class CHtmlExp : public IHtmlElement
{
public:
	CHtmlExp( int nID );
	~CHtmlExp(void);

	///**
	//@brief 解析属性
	//@param
	//*/
	//virtual void	parseAttribute( const std::string& strAttr );

	/**
	@brief 元素类型
	@param
	*/
	virtual int		getType() { return HtmlElement_Exp; }

	/**
	@brief 
	@param
	*/
	virtual void	draw( const CCPoint& ptLoc ) {}
};

NS_CC_END

#endif // _HTMLEXP_H__

