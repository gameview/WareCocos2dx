//*************************************************************************
//	创建日期:	2013-9-15   15:02
//	文件名称:	IHtmlItem.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	html标签
//*************************************************************************
#ifndef _IHTMLITEM_H__
#define _IHTMLITEM_H__

#include "cocos2d.h"
#include "HtmlTagAttribute.h"

NS_CC_BEGIN

#define FONT_DEFAULT_SIZE	24
#define SEPRATE_IMAGE		"UI/Common/common_line.png"

/// 图画元素类型
enum EHtmlElementType
{
	HtmlElement_Text = 0,	/// 文字
	HtmlElement_Link,		/// 文字链接
	HtmlElement_Image,		/// 图片
	HtmlElement_ImageLink,	/// 图片链接
	HtmlElement_Ani,		/// 图片带动画
	HtmlElement_Exp,		/// 表情动画
	HtmlElement_Input,		/// 输入框
	HtmlElement_Separate,	/// 分隔符
};

struct IHtmlLabelEventHandler;

struct IHtmlElement
{
	/**
	@brief 元素类型
	@param
	*/
	virtual int		getType() = 0;

	/**
	@brief 
	@param
	*/
	virtual void	draw( const CCPoint& ptLoc ) = 0;

	/**
	@brief 释放 
	@param
	*/
	virtual void	release() { delete this; }

	/**
	@brief 解析属性
	@param
	*/
	virtual void	parseAttribute( const std::string& strAttr ) { m_TagAttribute.parseAttribute( strAttr ); }

	
	/**
	@brief 获取任务指令
	@param
	*/
	virtual const char*	getCommand() { return NULL; }

	/**
	@brief 是否需要关闭
	@param
	*/
	virtual bool	isClose() { return false; }

	/**
	@brief 事件回调 超链接有效
	@param
	*/
	virtual void	onEvent( IHtmlLabelEventHandler* pEnentHandler ) {}


	/**
	@brief 对象ID
	@param
	*/
	int				getID() { return m_nID; }
 
	/**
	@brief 设置大小
	@param
	*/
	void			setContentSize( const CCSize& size ) { m_Rect.size = size; }

	/**
	@brief 获取元素大小
	@param
	*/
	CCSize			getContentSize() { return m_Rect.size; }

	/**
	@brief 
	@param
	*/
	CCPoint			getPos() { return m_Rect.origin; }

	/**
	@brief 是否包含在区域内
	@param
	*/
	bool			containsPoint( const CCPoint& point ) { return m_Rect.containsPoint( point ); }

protected:

	int					m_nID;				/// ID
	CHtmlTagAttribute	m_TagAttribute;		/// 属性
	CCRect				m_Rect;
};

NS_CC_END

#endif // _IHTMLITEM_H__

