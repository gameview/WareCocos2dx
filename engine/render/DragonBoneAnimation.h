//********************************************************************
//	文件名称:	CDragonBoneAnimation.h
//	创建日期:	2014-4-26 14:09
//	版权信息: 	www.cocos2dres.com
//	创 建 人:   Rare
//  功能说明:   DragonBone骨骼动画实现
//	修改记录:	
//********************************************************************
#ifndef _IDRAGONBONEANIMATION_H_
#define _IDRAGONBONEANIMATION_H_

#include "IAnimation.h"
#include "cocos-ext.h"

NS_CC_ENGINE_BEGIN

class CDragonBoneAnimation : public CCObject, public IAnimation
{
public:
	CDragonBoneAnimation(void);
	~CDragonBoneAnimation(void);

	/**
	 @brief 设置动画资源
	 @param pAnimation 动画数据
	*/
	void			setAnimationRes( extension::CCArmatureAnimation* pAnimation );

	/**
	@brief 设置动画播放回调
	@param
	*/
	virtual void	setAniCallback( IAnimationCallback* pAniCallBack, void* pContext );

	/**
	@brief 播放动画 如果该动作处于停止状态，则继续播放
	@param[in] szAniName 动作名称
	@param[in] fSpeed 播放速度
	@param[in] nLoops 循环次数
	@param[in] bForceUpdate 是否强制更新动作数据(动作重新播放)
	*/
	virtual void	play( const std::string& strAniName, float fSpeed = 1.0f, int nLoops = -1, bool bForceUpdate = false );

	/**
	@brief 跳转到指定帧
	@param
	*/
	virtual void	gotoFrame( int nFrame ) {}

	/**
	@brief 获取当前帧
	@param
	*/
	virtual int		getCurFrame() { return 0; }

	/**
	@brief 设置播放速度
	@param fSpeed 播放速度
	*/
	virtual void	speed( float fSpeed );

	/**
	@brief 获取播放速度
	@param fSpeed 播放速度
	*/
	virtual float	getSpeed();
	
	/**
	@brief 停止动画
	@param
	*/
	virtual void	stop(void);

	/**
	@brief 停止动画
	@param
	*/
	virtual void	pause(void);

	/**
	@brief 是否还在播放
	@param
	*/
	virtual bool	isPlaying() { return m_nState == 0; }

	/**
	@brief 是否暂停状态
	@param
	*/
	virtual bool	isPause() { return m_nState == 1; }

	/**
	 @brief 是否存在指定名称的动作
	 @param strAniName 动作名称
	 @return 返回指定动作存在与否
	*/
	virtual bool	isExistClip( const std::string& strAniName );

	/**
	@brief 获取动作列表
	@param[inout] animations 动画列表
	*/
	virtual void	getAnimationList( std::list< std::string >& animations );

	/**
	 @brief 动作回调
	 @param pArmature 动作
	 @param eMovementEventType 动作类型
	 @param szEventName 回调事件名称
	*/
	void			onMovementEventCallFunc(extension::CCArmature* pArmature, extension::MovementEventType eMovementEventType, const char * szEventName );

	/**
	 @brief 帧事件回调
	 @param pBone 骨骼
	 @param pFrameEventName 帧事件名称
	 @param nFrameID 帧ID
	 @param nTime 回调时间
	*/
	void			onFrameEventCallFunc(extension::CCBone* pBone, const char* pFrameEventName, int nFrameID, int nTime );

private:

	int								m_nState;			/// 0 正常播放 1 暂停 2 停止
	float							m_fSpeed;			/// 动画播放速度
	std::string						m_strCurActionName;	/// 当前动作名称

	extension::CCArmatureAnimation*	m_pArmatureAni;		/// 骨骼动画数据

	IAnimationCallback*				m_pAniCallBack;		/// 动画播放回调
	void*							m_pCallBackContext;	/// 动画回调参数
};

NS_CC_ENGINE_END
	
#endif // _IDRAGONBONEANIMATION_H_

