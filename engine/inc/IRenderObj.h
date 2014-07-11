//*************************************************************************
//	创建日期:	2013-7-19   14:25
//	文件名称:	IRenderObj.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	渲染对象接口
//*************************************************************************
#ifndef _IRENDEROBJ_H_
#define _IRENDEROBJ_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

struct IAnimationCallback;
struct IAnimation;
struct IRenderText;

struct IRenderObj
{
	/**
	@brief 获取ID
	@param
	*/
	virtual uint32	getID() = 0;

	/**
	@brief 设置用户数据
	@param
	*/
	virtual void	setUserData( void* pData ) = 0;

	/**
	@brief 获取用户数据
	@param
	*/
	virtual void*	getUserData() = 0;

	/**
	@brief 绑定到场景上
	@param
	*/
	virtual void	attach( CCNode* pParentNode, int nZOrder, int nTag = 0 ) = 0;

	/**
	@brief 从场景上解除
	@param
	*/
	virtual void	detach(bool bRemove = true ) = 0;

	/**
	@brief 释放
	@param
	*/
	virtual void	drop() = 0;

	/**
	@brief 设置位置
	@param
	*/
	virtual void	setPos( const CCPoint& ptLoc ) = 0;

	/**
	@brief 获取位置
	@param
	*/
	virtual void	getPos( CCPoint& ptLoc ) = 0;

	/**
	@brief 设置旋转
	@param
	*/
	virtual void	setRotate( float fRotate ) = 0;

	/**
	@brief 获取旋转值
	@param
	*/
	virtual float	getRotate() = 0;

	/**
	@brief 设置缩放
	@param
	*/
	virtual void	setZoom( float fZoom ) = 0;

	/**
	@brief 获取缩放
	@param
	*/
	virtual float	getZoom() = 0;

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
	@brief 设置是否允许update
	@param
	*/
	virtual void	setEnableUpdate( bool bEnable ) = 0;

	/**
	@brief 设置是否可见
	@param
	*/
	virtual void	visible( bool bVisible ) = 0;

	/**
	@brief 获取该对象的特征串(全局唯一)
	@param
	*/
	virtual void	getFeatureString( std::string& strFeature ) = 0;

	/**
	@brief 获取游戏对象配置文件
	@param
	*/
	virtual const std::string& getFileName() = 0;

	/**
	@brief 碰撞检测
	@param
	*/
	virtual bool	hitTest( const CCPoint& pt ) = 0;

	/**
	@brief 碰撞检测
	@param
	*/
	virtual bool	hitTest( const CCRect& rect ) = 0;

	/**
	@brief 设置能否被选中
	@param
	*/
	virtual void	setEnableSelect( bool bEnable ) = 0;

	/**
	@brief 获取游戏对象高度
	*/
	virtual float	getHeight() = 0;
	
	/**
	@brief 获取绑定点的偏移
	@param szLocator 绑定名
	@param ptOffset 偏移
	*/
	virtual void	getLocator( const char* szLocator, CCPoint& ptOffset ) = 0;

	/**
	@brief 挂接对象
	@param[in] szLinkobjName link对象名 必须是obj
	@param[in] szLocator  绑定点名称
	@param[in] nDepth 深度 
	@param[in] ptOffset 与宿主对象偏移
	*/
	virtual unsigned int addLinkObj( const char* szLinkobjName, const char* szLocator, int nDepth = 0, float fScale = 1.0f, ccColor4B color = ccc4(255,255,255,255) ) = 0;

	/**
	@brief 挂接对象
	@param[in] szLinkobjName link对象名 必须是obj
	@param[in] szLocator  绑定点名称
	@param[in] nDepth 深度 
	@param[in] ptOffset 与宿主对象偏移
	*/
	virtual unsigned int addLinkObj( IRenderObj* pRenderObj, const char* szLocator, int nDepth = 0, float fScale = 1.0f, cocos2d::ccColor4B color = ccc4(255,255,255,255) ) = 0;

	/**
	@brief 删除挂接对象
	@param[in] uid 游戏对象ID
	@param[in] szLinkobjName link对象名 可以是 oms oss或者obj
	*/
	virtual void	removeLinkObj( unsigned int uLinkObjID ) = 0;

	/**
	@brief 删除挂接对象
	@param[in] uid 游戏对象ID
	@param[in] szLinkobjName link对象名 可以是 oms oss或者obj
	*/
	virtual IRenderObj* removeLinkObj( unsigned int uLinkObjID, bool bRemove ) = 0;

	/**
	@brief 删除挂接对象
	@param[in] uid 游戏对象ID
	@param[in] szLinkobjName link对象名 可以是 oms oss或者obj
	*/
	virtual void	removeAllLinkObj() = 0;

	/**
	@brief 获取挂接对象
	@param
	*/
	virtual IRenderObj*	getLinkObj( unsigned int uLinkObjID ) = 0;

	/**
	@brief 更新部件
	@param szPartObjName 部件对象名
	@param szPartPlaceName 部件位置名称
	*/
	virtual void	changePart( const char* szPartObjName, const char* szPartPlaceName, int nDepth ) = 0;

	/**
	@brief 删除特效
	*/
	virtual void	removeEffectByName( const char* szEffectObjName ) = 0;

	/**
	@brief 删除特效
	*/
	virtual void	removeAllEffect() = 0;

	/**
	@brief 设置动画播放速度
	@param
	*/
	virtual void	setAnimationSpeed( float fSpeed ) = 0;

	/**
	@brief 设置动画播放回调
	@param
	*/
	virtual void	setAnimationCallback( IAnimationCallback* pAniCallBack, void* pContext ) = 0;

	/**
	@brief 播放动画 如果该动作处于停止状态，则继续播放
	@param[in] szAniName 动作名称
	@param[in] fSpeed 播放速度
	@param[in] nLoops 循环次数
	@param[in] bForceUpdate 是否强制更新动作数据(动作重新播放)
	*/
	virtual void	playAnimation( const char* szAniName, float fSpeed = 1.0f, int nLoops = -1, bool bForceUpdate = false ) = 0;

	/**
	@brief 播放动画
	@param[in] uid 游戏对象ID
	@param[in] szAniName 动作名称
	@param[in] pAniCallBack 动画播放回调
	@param[in] nStopFrame 停止的帧号
	@param[in] fSpeed 播放速度
	*/
	virtual void	playAndStop( const char* szAniName, int nStopFrame, float fSpeed = 1.0f ) = 0;

	/**
	@brief 获取当前帧
	@param
	*/
	virtual int		getCurFrame() = 0;

	/**
	@brief 动画暂停(游戏中不会使用,工具中使用)
	@param
	*/
	virtual void	pause() = 0;

	/**
	@brief 获取动画列表
	@param
	*/
	virtual void	getAnimations( std::list<std::string>& lstAnimation ) = 0;

	/**
	@brief 添加文字渲染对象
	@param
	*/
	virtual bool	addRenderText( IRenderText* pRenderText, std::string& strLocator, const CCPoint& ptOffset ) = 0;

	/**
	@brief 删除文字渲染对象
	@param
	*/
	virtual void	removeRenderText( IRenderText* pRenderText ) = 0;

	/**
	@brief 清理对象(部件，特效) 只保留空对象
	@param
	*/
	virtual void	clear() = 0;

};

NS_CC_ENGINE_END

#endif // _IRENDEROBJ_H_
