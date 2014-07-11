//*************************************************************************
//	创建日期:	2013-7-8   21:04
//	文件名称:	RectangleObj.h	
//	版权所有:	www.cocos2dres.com
//	说    明:	多边形对象
//*************************************************************************
#ifndef _POLYGONOBJ_H_
#define _POLYGONOBJ_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_engine.h"

NS_CC_ENGINE_BEGIN

class CRectangleObj : public cocos2d::CCSprite
{
public:
	CRectangleObj(void);
	virtual ~CRectangleObj(void);

	/** create one layer */
	static CRectangleObj *create(void);

	/**
	@brief 设置矩形宽高
	@param
	*/
	void	setRect( int nWidth, int nHeight );

	/**
	@brief 设置颜色
	@param
	*/
	void	setBorderColor( cocos2d::ccColor4B& color ) { m_Color = color; }

	/**
	@brief 设置是否需要填充色
	@param
	*/
	void	enableFillColor( bool bEnableFillColor ) { m_bFillColor = bEnableFillColor; }

	/**
	@brief 设置FillColor
	@param
	*/
	void	setFillColor( cocos2d::ccColor4B& color ) { m_FillColor = color; }

	/**
	@brief 重载draw方法
	@param
	*/
	virtual void draw();

private:

	int						m_nWidth;		/// 宽
	int						m_nHeight;		/// 高

	bool					m_bFillColor;	/// 画填充色标识
	cocos2d::ccColor4B		m_Color;		/// 颜色
	cocos2d::ccColor4B		m_FillColor;	/// 填充颜色

	cocos2d::CCDrawNode*	m_pDrawNode;	/// 画填充矩形
};

NS_CC_ENGINE_END

#endif // _POLYGONOBJ_H_

