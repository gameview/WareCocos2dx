//*************************************************************************
//	创建日期:	2013-8-1   18:47
//	文件名称:	IAnimation.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	动画播放
//*************************************************************************
#ifndef _IANIMATION_H_
#define _IANIMATION_H_

#include <string>
#include <list>
#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

struct IAnimation
{
	/**
	@brief 设置动画播放回调
	@param
	*/
	virtual void	setAniCallback( IAnimationCallback* pAniCallBack, void* pContext ) = 0;
	/**
	@brief 播放动画 如果该动作处于停止状态，则继续播放
	@param[in] szAniName 动作名称
	@param[in] fSpeed 播放速度
	@param[in] nLoops 循环次数
	@param[in] bForceUpdate 是否强制更新动作数据(动作重新播放)
	*/
	virtual void	play( const std::string& strAniName, float fSpeed = 1.0f, int nLoops = -1, bool bForceUpdate = false ) = 0;

	/**
	@brief 跳转到指定帧
	@param
	*/
	virtual void	gotoFrame( int nFrame ) = 0;

	/**
	@brief 获取当前帧
	@param
	*/
	virtual int		getCurFrame() = 0;

	/**
	@brief 设置播放速度
	@param fSpeed 播放速度
	*/
	virtual void	speed( float fSpeed ) = 0;

	/**
	@brief 获取播放速度
	@param fSpeed 播放速度
	*/
	virtual float	getSpeed() = 0;
	
	/**
	@brief 停止动画
	@param
	*/
	virtual void	stop(void) = 0;

	/**
	@brief 停止动画
	@param
	*/
	virtual void	pause(void) = 0;

	/**
	@brief 是否还在播放
	@param
	*/
	virtual bool	isPlaying() = 0; 

	/**
	@brief 是否暂停状态
	@param
	*/
	virtual bool	isPause() = 0;

	/**
	 @brief 是否存在指定名称的动作
	 @param strAniName 动作名称
	 @return 返回指定动作存在与否
	*/
	virtual bool	isExistClip( const std::string& strAniName ) = 0;

	/**
	@brief 获取动作列表
	@param
	*/
	virtual void	getAnimationList( std::list< std::string >& animations ) = 0;
};

NS_CC_ENGINE_END

#endif // _IANIMATION_H_
