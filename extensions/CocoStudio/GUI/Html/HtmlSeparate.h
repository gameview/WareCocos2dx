//*************************************************************************
//	创建日期:	2013-9-15   15:18
//	文件名称:	HtmlSeparate.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	html分隔符
//*************************************************************************
#ifndef _HTMLSEPRATE_H__
#define _HTMLSEPRATE_H__

#include "IHtmlElement.h"
#include <string>
#include "HtmlTagAttribute.h"

NS_CC_BEGIN

class CHtmlSeparate : public IHtmlElement
{
public:
	CHtmlSeparate();
	~CHtmlSeparate(void);

	/**
	@brief 
	@param
	*/
	virtual void	draw( const CCPoint& ptLoc );

	/**
	@brief 元素类型
	@param
	*/
	virtual int		getType() { return HtmlElement_Separate; }

};

NS_CC_END

#endif // _HTMLSEPRATE_H__

