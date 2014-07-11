//*************************************************************************
//	创建日期:	2013-9-16   19:24
//	文件名称:	HtmlImageLink.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	图片链接
//*************************************************************************
#ifndef _HTMLIMAGELINK_H__
#define _HTMLIMAGELINK_H__

#include "IHtmlElement.h"
#include <string>
#include "HtmlTagAttribute.h"

NS_CC_BEGIN

struct IHtmlLabelEventHandler;

class CHtmlImageLink : public IHtmlElement
{
public:
	CHtmlImageLink(void);
	~CHtmlImageLink(void);

	//struct FontProp
	//{
	//	int			nSize;
	//	std::string strFontName; /// 默认 Arial
	//	ccColor3B	color;

	//	FontProp()
	//	{
	//		nSize = FONT_DEFAULT_SIZE;
	//		strFontName = "Arial";
	//		color = ccc3( 255,255,255 );
	//	}
	//};

	/**
	@brief 
	@param
	*/
	virtual void	draw( const CCPoint& ptLoc );

	/**
	@brief 解析属性 设置字符串
	@param
	*/
	virtual void	setText( const std::string& strText ) { m_strText = strText; }

	/**
	@brief 元素类型
	@param
	*/
	virtual int		getType() { return HtmlElement_ImageLink; }

	/**
	@brief 获取任务指令
	@param
	*/
	virtual const char*	getCommand() { return m_TagAttribute.getStringValue("herf"); }

	/**
	@brief 事件回调 超链接有效
	@param
	*/
	virtual void	onEvent( IHtmlLabelEventHandler* pEnentHandler );

private:

	bool			m_bClose;
	std::string		m_strText;
};

NS_CC_END

#endif // _HTMLIMAGELINK_H__

