//*************************************************************************
//	创建日期:	2013-9-15   15:18
//	文件名称:	HtmlImage.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	html图片元素
//*************************************************************************
#ifndef _HTMLIMAGE_H__
#define _HTMLIMAGE_H__

#include "IHtmlElement.h"
#include <string>
#include "HtmlTagAttribute.h"

NS_CC_BEGIN

class CHtmlImage : public IHtmlElement
{
public:
	CHtmlImage();
	~CHtmlImage(void);

	/**
	@brief 
	@param
	*/
	virtual void	draw( const CCPoint& ptLoc );

	/**
	@brief 元素类型
	@param
	*/
	virtual int		getType() { return HtmlElement_Image; }

};

NS_CC_END

#endif // _HTMLIMAGE_H__

