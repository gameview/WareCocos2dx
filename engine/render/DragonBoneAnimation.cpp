#include "DragonBoneAnimation.h"
#include "StringUtility.h"

NS_CC_ENGINE_BEGIN

CDragonBoneAnimation::CDragonBoneAnimation(void)
: m_nState(0)
, m_fSpeed(0.0f)
, m_pArmatureAni(NULL)
{

}
//-------------------------------------------------------------------------
CDragonBoneAnimation::~CDragonBoneAnimation(void)
{

}
//-------------------------------------------------------------------------
/**
@brief 设置动画资源
@param pAnimation 动画数据
*/
void CDragonBoneAnimation::setAnimationRes( extension::CCArmatureAnimation* pAnimation )
{
	m_pArmatureAni = pAnimation;
}
//-------------------------------------------------------------------------
/**
@brief 设置动画播放回调
@param
*/
void CDragonBoneAnimation::setAniCallback( IAnimationCallback* pAniCallBack, void* pContext )
{
	m_pAniCallBack = pAniCallBack;
	m_pCallBackContext = pContext;

	if( NULL != m_pArmatureAni )
	{
		m_pArmatureAni->setMovementEventCallFunc( this, (extension::SEL_MovementEventCallFunc)(&CDragonBoneAnimation::onMovementEventCallFunc) );
		m_pArmatureAni->setFrameEventCallFunc( this, (extension::SEL_FrameEventCallFunc)(&CDragonBoneAnimation::onFrameEventCallFunc) );
	}
}
//-------------------------------------------------------------------------
/**
@brief 播放动画 如果该动作处于停止状态，则继续播放
@param[in] szAniName 动作名称
@param[in] fSpeed 播放速度
@param[in] nLoops 循环次数
@param[in] bForceUpdate 是否强制更新动作数据(动作重新播放)
*/
void CDragonBoneAnimation::play( const std::string& strAniName, float fSpeed, int nLoops, bool bForceUpdate )
{
	if( m_pArmatureAni == NULL )
	{
		return;
	}

	m_pArmatureAni->play(strAniName.c_str());
	m_pArmatureAni->setSpeedScale( fSpeed );

	m_strCurActionName = strAniName;
}
//-------------------------------------------------------------------------
/**
@brief 设置播放速度
@param fSpeed 播放速度
*/
void CDragonBoneAnimation::speed( float fSpeed )
{
	if( m_pArmatureAni == NULL )
	{
		return;
	}

	m_pArmatureAni->setSpeedScale( fSpeed );
}
//-------------------------------------------------------------------------
/**
@brief 获取播放速度
@param fSpeed 播放速度
*/
float CDragonBoneAnimation::getSpeed()
{
	if( m_pArmatureAni == NULL )
	{
		return 1.0f;
	}

	return m_pArmatureAni->getSpeedScale();
}

/**
@brief 停止动画
@param
*/
void CDragonBoneAnimation::stop(void)
{
	if( m_pArmatureAni == NULL )
	{
		return;
	}

	return m_pArmatureAni->stop();
}

/**
@brief 停止动画
@param
*/
void CDragonBoneAnimation::pause(void)
{
	if( m_pArmatureAni == NULL )
	{
		return;
	}

	return m_pArmatureAni->pause();
}
//-------------------------------------------------------------------------
/**
@brief 是否存在指定名称的动作
@param strAniName 动作名称
@return 返回指定动作存在与否
*/
bool CDragonBoneAnimation::isExistClip( const std::string& strAniName )
{
	if( m_pArmatureAni == NULL )
	{
		return false;
	}

	extension::CCAnimationData* pAniData = m_pArmatureAni->getAnimationData();
	if( pAniData == NULL )
	{
		return false;
	}

	if( pAniData->movementNames.empty() )
	{
		return false;
	}

	size_t size = pAniData->movementNames.size();
	for( size_t i = 0; i < size; ++i )
	{
		if( pAniData->movementNames[i] == strAniName )
		{
			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------
/**
@brief 获取动作列表
@param[inout] animations 动画列表
*/
void CDragonBoneAnimation::getAnimationList( std::list< std::string >& animations )
{
	//animations.clear();

	if( m_pArmatureAni == NULL )
	{
		return;
	}

	extension::CCAnimationData* pAniData = m_pArmatureAni->getAnimationData();
	if( pAniData == NULL )
	{
		return;
	}

	if( pAniData->movementNames.empty() )
	{
		return;
	}

	size_t size = pAniData->movementNames.size();
	for( size_t i = 0; i < size; ++i )
	{
		animations.push_back( pAniData->movementNames[i] );
	}
}
//-------------------------------------------------------------------------
/**
@brief 动作回调
@param pArmature 动作
@param eMovementEventType 动作类型
@param szEventName 回调事件名称
*/
void CDragonBoneAnimation::onMovementEventCallFunc(extension::CCArmature* pArmature, extension::MovementEventType eMovementEventType, const char * szEventName )
{
	if( m_pAniCallBack )
	{
		SAnimationCallbackInfo sAnimationCallbackInfo;

		sAnimationCallbackInfo.fFrameTime = 0;
		switch( eMovementEventType )
		{
		case extension::START:
			{
				StringUtility::sstrcpyn( sAnimationCallbackInfo.szName, "start", sizeof(sAnimationCallbackInfo.szName) );
				break;
			}
		case extension::COMPLETE:
		case extension::LOOP_COMPLETE:
			{
				StringUtility::sstrcpyn( sAnimationCallbackInfo.szName, "end", sizeof(sAnimationCallbackInfo.szName) );
				break;
			}
		}

		StringUtility::sstrcpyn( sAnimationCallbackInfo.szActName, m_strCurActionName.c_str(), sizeof(sAnimationCallbackInfo.szActName) );
		
		m_pAniCallBack->onAnimaitonCallback( sAnimationCallbackInfo, m_pCallBackContext );
	}
}
//-------------------------------------------------------------------------
/**
@brief 帧事件回调
@param pBone 骨骼
@param pFrameEventName 帧事件名称
@param nFrameID 帧ID
@param nTime 回调时间
*/
void CDragonBoneAnimation::onFrameEventCallFunc(extension::CCBone* pBone, const char* pFrameEventName, int nFrameID, int nTime )
{
	if( m_pAniCallBack )
	{
		SAnimationCallbackInfo sAnimationCallbackInfo;

		sAnimationCallbackInfo.fFrameTime = nTime;
		StringUtility::sstrcpyn( sAnimationCallbackInfo.szActName, m_strCurActionName.c_str(), sizeof(sAnimationCallbackInfo.szActName) );
		StringUtility::sstrcpyn( sAnimationCallbackInfo.szActName, pFrameEventName, sizeof(sAnimationCallbackInfo.szActName) );

		m_pAniCallBack->onAnimaitonCallback( sAnimationCallbackInfo, m_pCallBackContext );
	}
}

NS_CC_ENGINE_END