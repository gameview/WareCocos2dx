//*************************************************************************
//	创建日期:	2013-6-4   8:32
//	文件名称:	RenderObjManager.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	
//*************************************************************************
#ifndef _RENDEROBJMANAGER_H_
#define _RENDEROBJMANAGER_H_

#include "cocos2d.h"
#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"


NS_CC_ENGINE_BEGIN

struct IRenderable;
struct ISprite;
struct IAniSprite;
struct IEffect;
struct IDragonBoneSprite; /// 骨骼动画

class CRenderableManager
{
	typedef std::map< uint32, IRenderable* >			RenderableMap;
	typedef std::list< IRenderable* >				RenderaleList;
	typedef std::map< std::string, RenderaleList >	RenderableReclaim;

private:
	CRenderableManager(void);
	~CRenderableManager(void);

public:

	static CRenderableManager& Instance()
	{
		static CRenderableManager _inst;
		return _inst;
	}

	/**
	@brief 创建精灵对象
	@param szPicFileName [in] 贴图文件名
	@param rect [in] 贴图区域
	@param ptOffset 偏移
	@retrun 渲染对象
	*/
	ISprite*		createSpriteObj( const char* szPicFileName, const CCRect& rect, CCPoint ptOffset = ccp(0,0) );

	/**
	@brief 创建灰度精灵对象
	@param szPicFileName [in] 贴图文件名
	@param rect [in] 贴图区域
	@param ptOffset 偏移
	@retrun 渲染对象
	*/
	ISprite*		createGraySpriteObj( const char* szPicFileName, const CCRect& rect, CCPoint ptOffset = ccp(0,0) );

	/**
	@brief 创建精灵对象
	@param szPicFileName [in] 动画文件
	@param ptOffset 偏移
	@retrun 渲染对象
	*/
	IAniSprite*		createAniSpriteObj( const char* szAniFileName, CCPoint ptOffset = ccp(0,0) );

	/**
	@brief 创建特效
	@param szEffectFileName [in] 特效文件名
	@param ptOffset 偏移
	@retrun 渲染对象
	*/
	IEffect*		createEffectObj( const char* szEffectFileName, CCPoint ptOffset = ccp(0,0) );

	/**
	 @brief 创建支持DragonBone骨骼动画对象
	 @param szAniFileName [in] 动画文件
	 @param ptOffset 偏移
	*/
	IDragonBoneSprite*	createDragobBoneSpriteObj( const char* szAniFileName, CCPoint ptOffset = ccp(0,0) );

	/**
	*@brief 获取渲染对象
	*@param uID 对象ID
	*/
	IRenderable*	getRenderable( uint32 uID );

	/**
	@brief 删除对象
	*@param uID 对象ID
	*/
	void			removeRenderable( uint32 uID );

private:

	uint32				m_uGmaeObjSerial;								/// 渲染对象序列号

	RenderableMap		m_mapRenderable;								/// 渲染对象
	//RenderableReclaim	m_mapRenderableReclaim[RenderableType_Count];	/// 渲染对象回收容器
};

NS_CC_ENGINE_END

#endif // _RENDEROBJMANAGER_H_

