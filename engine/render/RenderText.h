//*************************************************************************
//	创建日期:	2013-8-20   16:08
//	文件名称:	RednerText.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	字体渲染
//*************************************************************************
#ifndef _REDNERTEXT_H__
#define _REDNERTEXT_H__

#include "cocos2dx_types.h"
#include "IRenderText.h"
#include "cocos2dx_macros.h"
//#include "CCLabelTTFStroke.h"

NS_CC_ENGINE_BEGIN

class CRenderText : public IRenderText
{
public:
	CRenderText( unsigned int uid );
	virtual ~CRenderText(void);

	/**
	@brief 创建
	@param szFontName 字体名称
	@param nFontSize 字体大小
	*/
	void			create( const char* szString, const char* szFontName, int nFontSize, CCSize dimentions = CCSizeZero );

	/**
	@brief 创建
	@param szFontName 字体名称
	@param nFontSize 字体大小
	*/
	void			createByBmpFont( const char* szString, const char* szFontName, int nFontSize, CCSize dimentions = CCSizeZero );

	// 获取ID
	unsigned int	getID() { return m_uID; }

	/**
	@brief 关闭
	@param
	*/
	virtual void	release();

	/**
	@brief 绑定到场景上
	@param
	*/
	virtual void	attach( CCNode* pParentNode, int nZOrder, int nTag = 0 );

	/**
	@brief 从场景上解除
	@param
	*/
	virtual void	detach();

	/**
	@brief 设置位置
	@param ptLoc 位置
	*/
	virtual void	setPos( const CCPoint& ptLoc );

	/**
	@brief 设置字串
	@param
	*/
	virtual void	setString( const char* szText );

	/**
	@brief 设置字体
	@param
	*/
	virtual void	setFont( const char* szFont );

	/**
	@brief 给对象加色
	@param
	*/
	virtual void	setColor( const ccColor3B& color );

	/**
	@brief 设置透明度
	@param
	*/
	virtual void	setOpacity( BYTE byAlpha );

	/**
	@brief 设置显示区域
	@param
	*/
	virtual void	setDimensions( const CCSize& dimentions );

	/**
	@brief 设置对齐方式
	@param
	*/
	virtual void	setHorizontalAlignment( ETextHorizontalAlignment hAlignment );

	/**
	@brief 设置对齐方式
	@param
	*/
	virtual void	setVerticalAlignment( ETextVerticalAlignment vAlignment );

private:

	unsigned int				m_uID;			/// 对象ID
	CCLabelTTF*					m_pLabel;		/// 字体对象
	CCLabelBMFont*				m_pLabelBmp;	/// 字体对象

	std::string					m_strContent;	/// 显示内容

	CCPoint						m_pPos;			/// 相对位置，相对于父节点
	CCNode*						m_pParent;		/// 父节点
	CCSize						m_Dimentions;	/// 显示区域
	ccColor3B					m_color;		/// 颜色
	GLubyte						m_byAlpha;		/// 透明度

	ETextHorizontalAlignment	m_hAlignment;	/// 水平对齐方式, 仅当显示区域不为0,0时有效
	ETextVerticalAlignment		m_vAlignment;	/// 垂直对齐方式, 仅当显示区域不为0,0时有效
};

NS_CC_ENGINE_END

#endif // _REDNERTEXT_H__

