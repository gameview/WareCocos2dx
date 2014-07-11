//*************************************************************************
//	创建日期:	2013-6-2   11:25
//	文件名称:	Animate.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:   动画播放	
//*************************************************************************
#ifndef _WAREANIMATE_H_
#define _WAREANIMATE_H_


#include <map>
#include <vector>
#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include "AnimationRes.h"
#include "IAnimation.h"

NS_CC_ENGINE_BEGIN

class CAnimationRes;
struct IAnimationCallback;

/// 动画播放管理
class CWareAnimate : public IAnimation
{
public:
	CWareAnimate(void);
	~CWareAnimate(void);

	/**
	@brief 重置
	@param
	*/
	void	reset();

	/// 动作名和CAnimation的映射
	void	setAnimationRes( CAnimationRes* pAniRes );

	/// 设置目标对象
	void	setTarget(cocos2d::CCNode *pTarget);

	/**
	@brief 更新
	@param
	*/
	void	update(float t);

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
	virtual void	gotoFrame( int nFrame );

	/**
	@brief 获取当前帧
	@param
	*/
	virtual int		getCurFrame();

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
	@param
	*/
	virtual void	getAnimationList( std::list< std::string >& animations );

protected:

	/**
	@brief 动画回调实现
	@param
	*/
	void			callbackAni( float fLastTime, float fCurTime, float fDuration );

	/**
	@brief 获取当前动作帧
	@param
	*/
	int				getFrameCount();

private:

	bool					m_bResValied;		/// 数据资源有效标志

	float					m_fTime;			/// 动画播放时间
	float					m_fSpeed;			/// 动画播放速度
	int						m_nLoop;			/// 动画播放次数

	int						m_nNextFrame;		/// 下一帧索引
	bool					m_bNewLoop;			/// 是否是新的播放循环

	std::string				m_strCurAniName;	/// 当前动作名称

	std::vector<AnimateCallback> m_vecCallbacks;/// 动画回调数据

	CAnimationRes*			m_pAniRes;			/// 动画资源

	IAnimationCallback*		m_pAniCallBack;		/// 动画播放回调
	void*					m_pCallBackContext;	/// 动画回调参数

	int						m_nState;			/// 0 正常播放 1 暂停 2 停止
	cocos2d::CCAnimation*	m_pCurAnimation;	/// 当前动作
	
	cocos2d::CCNode*		m_pTarget;			/// 目标对象

	std::vector<float>		m_vecFrameTimes;	/// 帧时间
};

NS_CC_ENGINE_END

#endif // _WAREANIMATE_H_

