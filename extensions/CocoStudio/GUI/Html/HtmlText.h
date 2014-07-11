//*************************************************************************
//	创建日期:	2013-9-15   18:43
//	文件名称:	HtmlText.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	html文本
//*************************************************************************
#ifndef _HTMLTEXT_H__
#define _HTMLTEXT_H__

#include "IHtmlElement.h"
#include <string>
#include "HtmlTagAttribute.h"

NS_CC_BEGIN

class CHtmlText : public IHtmlElement
{
public:
	CHtmlText(void);
	~CHtmlText(void);

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
	virtual int		getType() { return HtmlElement_Text; }

	/**
	@brief 解析属性 设置字符串
	@param
	*/
	virtual void	setText( const std::string& strText );

	/**
	@brief 设置字体属性
	@param
	*/
	void			setFontProp( const std::string strFontName, const ccColor3B& color, int nFontSize );

private:

	FontProp		m_FontProp;
	std::string		m_strText;
};

NS_CC_END

#endif // _HTMLTEXT_H__

