//*************************************************************************
//	创建日期:	2013-9-16   19:46
//	文件名称:	HtmlInput.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	输入框
//*************************************************************************
#ifndef _HTMLINPUT_H__
#define _HTMLINPUT_H__

#include "IHtmlElement.h"
#include <string>
#include "HtmlTagAttribute.h"

NS_CC_BEGIN

class CHtmlInput : public IHtmlElement
{
public:
	CHtmlInput( int nID );
	~CHtmlInput(void);


	/**
	@brief 解析属性
	@param
	*/
	virtual void	parseAttribute( const std::string& strAttr );

	/**
	@brief 元素类型
	@param
	*/
	virtual int		getType() { return HtmlElement_Input; }

	/**
	@brief 
	@param
	*/
	virtual void	draw( const CCPoint& ptLoc ) {}
};

NS_CC_END

#endif // _HTMLINPUT_H__

