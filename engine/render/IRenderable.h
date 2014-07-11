//*************************************************************************
//	创建日期:	2013-5-14   8:41
//	文件名称:	IRenderObj.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	渲染对象
//*************************************************************************
#ifndef _IRENDERABLE_H_
#define _IRENDERABLE_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include <string>

NS_CC_ENGINE_BEGIN

struct IAnimationCallback;
struct IAnimation;
struct IEffectProp;

/// 渲染对象
struct IRenderable : public CCNode
{
	///**
	//@brief 
	//@param
	//*/
	//virtual void			release() = 0;

	/**
	@brief 获取渲染对象类型
	@param
	*/
	virtual RenderableType	getType() = 0;

	/**
	@brief 给对象加色
	@param
	*/
	virtual void			setColor( const ccColor3B& color ) = 0;

	/**
	@brief 设置ID
	@param
	*/
	virtual void			setID( uint32 uID ) = 0;

	/**
	@brief 获取ID
	@param
	*/
	virtual uint32			getID() = 0;

	/**
	@brief 设置透明度
	@param byAlpha 透明度
	*/
	virtual void			setOpacity( BYTE byAlpha ) = 0;

	/**
	@brief 获取贴图文件名
	@param
	*/
	virtual const std::string getFileName() = 0;

	/**
	@brief 获取该对象的特征串(全局唯一)
	@param
	*/
	virtual void			getFeatureString( std::string& strFeature ) = 0;

	/**
	@brief 碰撞检测
	@param
	*/
	virtual bool			hitTest( const CCPoint& pt ) = 0;

	/**
	@brief 碰撞检测
	@param
	*/
	virtual bool			hitTest( const CCRect& rect ) = 0;

	/**
	@brief 获取对象包围盒
	@param
	*/
	virtual bool			getBoundingBox( CCRect& boundBox ) = 0;

	/**
	@brief 获取偏移
	@param
	*/
	virtual void			setOffset( CCPoint& ptOffet ) = 0;

	/**
	@brief 获取偏移
	@param
	*/
	virtual void			getOffset( CCPoint& ptOffet ) = 0;

};

/// sprite对象
struct ISprite : public IRenderable
{
	/**
	@brief 获取渲染对象类型
	@param
	*/
	virtual RenderableType		getType() { return RenderableType_Sprite; }

	/**
	@brief 获取精灵
	@param
	*/
	virtual CCSprite*			getSprite() = 0;
};

/// 动画对象
struct IAniSprite : public IRenderable
{
	/**
	@brief 获取渲染对象类型
	@param
	*/
	virtual RenderableType	getType() { return RenderableType_AniSprite; }

	/**
	@brief 获取动画播放控制
	@param
	*/
	virtual IAnimation*		getAnimation() = 0;
};

/// 骨骼动画对象
struct IDragonBoneSprite : public IRenderable
{
	/**
	@brief 获取渲染对象类型
	@param
	*/
	virtual RenderableType	getType() { return RenderableType_DragonBone; }

	/**
	@brief 获取动画播放控制
	@param
	*/
	virtual IAnimation*		getAnimation() = 0;
};

/// 特效对象
struct IEffect : public IRenderable
{
	/**
	@brief 获取渲染对象类型
	@param
	*/
	virtual RenderableType		getType() { return RenderableType_Effect; }

	/**
	 @brief 获取ID
	 @param 
	*/
	virtual uint32				getID() = 0;

	/**
	 @brief 获取特效属性
	 @param 
	*/
	virtual IEffectProp*		getEffectProp() = 0;
};

NS_CC_ENGINE_END

#endif // _IRENDERABLE_H_