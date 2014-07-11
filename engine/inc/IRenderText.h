//*************************************************************************
//	创建日期:	2013-8-20   15:52
//	文件名称:	IRenderText.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	游戏文字渲染
//*************************************************************************
#ifndef _IRENDERTEXT_H__
#define _IRENDERTEXT_H__

#include "cocos2d.h"
#include "cocos2dx_macros.h"
#include "RenderCommon.h"

NS_CC_ENGINE_BEGIN

struct IRenderText
{
	/**
	@brief 关闭
	@param
	*/
	virtual void	release() = 0;

	/**
	@brief 绑定到场景上
	@param
	*/
	virtual void	attach( CCNode* pParentNode, int nZOrder, int nTag = 0 ) = 0;

	/**
	@brief 从场景上解除
	@param
	*/
	virtual void	detach() = 0;

	/**
	@brief 设置位置
	@param ptLoc 位置
	*/
	virtual void	setPos( const CCPoint& ptLoc ) = 0;

	/**
	@brief 设置字串
	@param
	*/
	virtual void	setString( const char* szText ) = 0;

	/**
	@brief 给对象加色
	@param
	*/
	virtual void	setColor( const ccColor3B& color ) = 0;

	/**
	@brief 设置透明度
	@param
	*/
	virtual void	setOpacity( BYTE byAlpha ) = 0;

	/**
	@brief 设置字体
	@param
	*/
	virtual void	setFont( const char* szFont ) = 0;

	/**
	@brief 设置显示区域
	@param
	*/
	virtual void	setDimensions( const CCSize& dimentions ) = 0;

	/**
	@brief 设置水平对齐方式
	@param
	*/
	virtual void	setHorizontalAlignment( ETextHorizontalAlignment hAlignment ) = 0;

	/**
	@brief 设置垂直对齐方式
	@param
	*/
	virtual void	setVerticalAlignment( ETextVerticalAlignment vAlignment ) = 0;
};

NS_CC_ENGINE_END

#endif // _IRENDERTEXT_H__