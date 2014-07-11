//*************************************************************************
//	创建日期:	2013-9-15   19:32
//	文件名称:	HtmlTextLink.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	文字超链接
//*************************************************************************
#ifndef _HTMLTEXTLINK_H__
#define _HTMLTEXTLINK_H__

#include "IHtmlElement.h"
#include <string>
#include "HtmlTagAttribute.h"

NS_CC_BEGIN

class CHtmlTextLink : public IHtmlElement
{
public:
	CHtmlTextLink(void);
	~CHtmlTextLink(void);

	struct FontProp
	{
		int			nSize;
		std::string strFontName; /// 默认 Arial
		ccColor3B	color;

		FontProp()
		{
			nSize = FONT_DEFAULT_SIZE;
			strFontName = "黑体";
			color = ccc3( 255,255,255 );
		}
	}; 

	/**
	@brief 
	@param
	*/
	virtual void	draw( const CCPoint& ptLoc );

	/**
	@brief 元素类型
	@param
	*/
	virtual int		getType() { return HtmlElement_Link; }

	/**
	@brief 解析属性 设置字符串
	@param
	*/
	virtual void	setText( const std::string& strText ) { m_strText = strText; }

	/**
	@brief 获取任务指令
	@param
	*/
	virtual const char*		getCommand() { return m_TagAttribute.getStringValue("herf"); }

	/**
	@brief 事件回调 超链接有效
	@param
	*/
	virtual void	onEvent( IHtmlLabelEventHandler* pEnentHandler );

	/**
	@brief 设置字体属性
	@param
	*/
	void			setFontProp( const std::string strFontName, const ccColor3B& color, int nFontSize );

private:

	bool			m_bClose;
	FontProp		m_FontProp;
	std::string		m_strText;
	std::string		m_strCommand;
};

NS_CC_END

#endif // _HTMLTEXTLINK_H__

