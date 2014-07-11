#include "WareAnimate.h"
#include "AnimationRes.h"
#include "AnimationResManager.h"

NS_CC_ENGINE_BEGIN

CWareAnimate::CWareAnimate(void)
{
	m_pAniRes = NULL;
	m_pTarget = NULL;
	
	m_pAniCallBack = NULL;

	m_bResValied = false;

	reset();
}

CWareAnimate::~CWareAnimate(void)
{
	//if( m_pAniRes )
	//{
	//	//int i = 0;
	//	//if (m_pAniRes->retainCount() > 20)
	//	//{

	//	//	i++;
	//	//}
	//	//CCLOG("CWareAnimate::~CWareAnimate m_uReference= %d %x",m_pAniRes->retainCount(),m_pAniRes);
	//	//CAnimationResManager::Instance().releaseAnimationRes( m_pAniRes );
	//	m_pAniRes->release();
	//	m_pAniRes = NULL;
	//}

	reset();
}
/**
@brief 重置
@param
*/
void CWareAnimate::reset()
{
	m_fTime = 0.0f;				/// 动画播放开始时间
	m_fSpeed = 1.0f;			/// 动画播放速度
	m_nLoop = -1;				/// 动画播放次数

	m_nNextFrame = 0;			/// 当前帧
	m_bNewLoop = true;

	CC_SAFE_RELEASE( m_pAniRes );

	m_vecCallbacks.clear();
	m_vecFrameTimes.clear();
	m_pTarget = NULL;			/// 目标对象

	m_nState = 0;				/// 正常播放
	m_pCurAnimation = NULL;		/// 动画	

	m_pAniCallBack = NULL;		/// 动画回调接口
	m_pCallBackContext = NULL;	/// 动画回调参数
}
//-------------------------------------------------------------------------
///// 动作名和CAnimation的映射
//void CWareAnimate::addAnimation( std::string& strAniName, cocos2d::CCAnimation* pAnimation, bool bFlipX, bool bFlipY )
//{
//	if( strAniName.empty() || pAnimation == NULL )
//	{
//		return;
//	}
//
//	SAnimationInfo info;
//	info.pAnimation = pAnimation;
//	info.m_bFlipX = bFlipX;
//	info.m_bFlipY = bFlipY;
//
//	m_mapAnimation[strAniName] = info;
//}
/// 动作名和CAnimation的映射
void CWareAnimate::setAnimationRes( CAnimationRes* pAniRes )
{
	CC_SAFE_RELEASE( m_pAniRes );

	m_pAniRes = pAniRes;
	if( m_pAniRes )
	{
		m_bResValied = m_pAniRes->isVailed();
	}
}
//-------------------------------------------------------------------------
/// 设置目标对象
void CWareAnimate::setTarget(cocos2d::CCNode *pTarget)
{
	m_pTarget = pTarget;
}
//------------------------------------------------------------------------------
/**
@brief 设置动画播放回调
@param
*/
void CWareAnimate::setAniCallback( IAnimationCallback* pAniCallBack, void* pContext )
{
	m_pAniCallBack = pAniCallBack;
	m_pCallBackContext = pContext;
}
//-------------------------------------------------------------------------
/**
@brief 播放动画 如果该动作处于停止状态，则继续播放
@param[in] szAniName 动作名称
@param[in] fSpeed 播放速度
@param[in] nLoops 循环次数
@param[in] bForceUpdate 是否强制更新动作数据(动作重新播放)
*/
void CWareAnimate::play( const std::string& strAniName, float fSpeed, int nLoops, bool bForceUpdate )
{
	if( m_pTarget == NULL || m_pAniRes == NULL )
	{
		return;
	}

	if( !m_pAniRes->isVailed() )
	{
		m_strCurAniName = strAniName;
		m_fSpeed = fSpeed;
		m_nLoop = nLoops;
		return;
	}

	CAnimationRes::SAnimationInfo* pAniInfo = m_pAniRes->getAnimation( strAniName.c_str() );
	if( pAniInfo == NULL )
	{
		return;
	}

	cocos2d::CCAnimation* pCurAnimation = m_pCurAnimation;
	m_pCurAnimation = pAniInfo->pAnimation;
	if( m_pCurAnimation == NULL )
	{
		m_pCurAnimation = pCurAnimation;
		return;
	}

	m_strCurAniName = strAniName;

	if( m_nLoop > 0 )
	{
		m_nLoop = nLoops * m_pCurAnimation->getLoops();
	}
	else
	{
		m_nLoop = nLoops;
	}

	if( m_pCurAnimation == pCurAnimation )
	{
		if( bForceUpdate )
		{
			m_fSpeed = fSpeed;
			m_fTime = 0.0f;
			m_nNextFrame = 0;
			m_bNewLoop = true;
			m_fTime = 0.0f;
		}

		m_nState = 0;
		return;
	}

	m_vecCallbacks.clear();
	std::size_t size = pAniInfo->callbacks.size();
	for( size_t i = 0; i < size; ++i )
	{
		m_vecCallbacks.push_back( pAniInfo->callbacks[i] );
	}

	m_fSpeed = fSpeed;
	m_fTime = 0.0f;
	m_nNextFrame = 0;
	m_bNewLoop = true;
	m_vecFrameTimes.clear();
	m_nState = 0;

	((cocos2d::CCSprite*)m_pTarget)->setFlipX( pAniInfo->m_bFlipX );
	((cocos2d::CCSprite*)m_pTarget)->setFlipY( pAniInfo->m_bFlipY );

	/// 设置时间分割
	float singleDuration = m_pCurAnimation->getDuration();
	cocos2d::CCArray* pFrames = m_pCurAnimation->getFrames();
	CCARRAY_VERIFY_TYPE(pFrames, CCAnimationFrame*);

	m_vecFrameTimes.reserve(pFrames->count());

	float accumUnitsOfTime = 0;
	float newUnitOfTimeValue = singleDuration / m_pCurAnimation->getTotalDelayUnits();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(pFrames, pObj)
	{
		CCAnimationFrame* frame = (CCAnimationFrame*)pObj;
		float value = (accumUnitsOfTime * newUnitOfTimeValue);// / singleDuration;
		accumUnitsOfTime += frame->getDelayUnits();
		m_vecFrameTimes.push_back(value);
	}
}
//-------------------------------------------------------------------------
/**
@brief 跳转到指定帧
@param
*/
void CWareAnimate::gotoFrame( int nFrame )
{
	int nFrameCount = getFrameCount();
	if( nFrame < 0 )
	{
		nFrame = 0;
	}

	if( nFrame >= nFrameCount )
	{
		nFrame = nFrameCount - 1;
	}

	m_nState = 0; /// 播放状态
	m_fTime = m_vecFrameTimes[nFrame];
	update( 0 );
	m_nState = 1; /// 暂停状态
}
//-------------------------------------------------------------------------
/**
@brief 获取当前帧
@param
*/
int CWareAnimate::getCurFrame()
{
	return m_nNextFrame - 1;
}
//-------------------------------------------------------------------------
/**
@brief 设置播放速度
@param fSpeed 播放速度
*/
void CWareAnimate::speed( float fSpeed )
{
	m_fSpeed = fSpeed;
}
//-------------------------------------------------------------------------
/**
@brief 获取播放速度
@param fSpeed 播放速度
*/
float CWareAnimate::getSpeed()
{
	return m_fSpeed;
}
//-------------------------------------------------------------------------
/**
@brief 停止动画
@param
*/
void CWareAnimate::stop(void)
{
	m_nState = 2;
}
//-------------------------------------------------------------------------
/**
@brief 停止动画
@param
*/
void CWareAnimate::pause(void)
{
	m_nState = 1;
}
//-------------------------------------------------------------------------
/**
@brief 是否存在指定名称的动作
@param strAniName 动作名称
@return 返回指定动作存在与否
*/
bool CWareAnimate::isExistClip( const std::string& strAniName )
{
	if( m_pAniRes == NULL )
	{
		return false;
	}

	std::set<std::string>& aniset = m_pAniRes->getAniNameList();
	return aniset.find(strAniName) != aniset.end();
}
//------------------------------------------------------------------------------
/**
@brief 获取动作列表
@param
*/
void CWareAnimate::getAnimationList( std::list< std::string >& animations )
{
	//animations.clear();

	if( m_pAniRes == NULL )
	{
		return;
	}

	std::set<std::string>& aniset = m_pAniRes->getAniNameList();
	if( aniset.empty() )
	{
		return;
	}

	std::set<std::string>::iterator iter = aniset.begin();
	for( ; iter != aniset.end(); ++iter )
	{
		animations.push_back( (*iter) );	
	}
}
//-------------------------------------------------------------------------
/**
@brief 更新
@param
*/
void CWareAnimate::update(float t)
{
	if( m_pAniRes == NULL )
	{
		return;
	}

	if( !m_pAniRes->isVailed() )
	{
		return;
	}

	/// 支持资源异步加载
	if( m_pAniRes->isVailed() && !m_bResValied )
	{
		play( m_strCurAniName, m_fSpeed, m_nLoop );
		m_bResValied = true;
	}

	if( m_pCurAnimation == NULL )
	{
		std::set<std::string>& AninName = m_pAniRes->getAniNameList();
		if( AninName.empty() )
		{
			return;
		}

		std::set<std::string>::iterator iter = AninName.begin();
		play( (*iter), m_fSpeed, m_nLoop );
		if( m_pCurAnimation == NULL )
		{
			return;
		}

		m_strCurAniName = (*iter);
	}

	switch( m_nState )
	{
	case 0: /// playing
		{
			float fDuration = m_pCurAnimation->getDuration();
			CCArray* frames = m_pCurAnimation->getFrames();
			int nFrames = static_cast<int>(frames->count());
			float fLastTime = m_fTime;

			m_fTime += t * m_fSpeed;
			if( m_fTime >= fDuration )
			{
				if( m_nLoop == -1 )/// 循环播放
				{
					m_nNextFrame = 0;
					m_fTime = 0.0f;
				}
				else
				{
					m_nNextFrame = 0;
					m_fTime = 0.0f;
					m_nLoop--;
					if( m_nLoop == 0 )
					{
						m_nState = 2;
						/// 动画回调
						callbackAni( fLastTime, m_fTime, fDuration );
						return;
					}
				}
			}

			/// 支持反向播放
			if( m_fTime < 0.0f )
			{
				if( m_nLoop == -1 )/// 循环播放
				{
					m_nNextFrame = 0;
					m_fTime = fDuration;
				}
				else
				{
					m_nNextFrame = 0;
					m_fTime = fDuration;
					m_nLoop--;
					if( m_nLoop == 0 )
					{
						m_nState = 2;
						/// 动画回调
						callbackAni( fLastTime, m_fTime, fDuration );
						return;
					}
				}
			}

			CCSpriteFrame* pFrameToDisplay = NULL;

			if( nFrames == 0 )
			{
				((CCSprite*)m_pTarget)->setTexture(NULL);
				return;
			}

			//for( int i = m_nNextFrame; i < nFrames; i++ )
			for( int i = nFrames - 1; i >= 0; --i )
			{ 
				float splitTime = m_vecFrameTimes.at(i);

				if( splitTime <= m_fTime )
				{
					CCAnimationFrame* frame = (CCAnimationFrame*)frames->objectAtIndex(i);
					pFrameToDisplay = frame->getSpriteFrame();
					((CCSprite*)m_pTarget)->setDisplayFrame(pFrameToDisplay);
					//((CCSprite*)m_pTarget)->setContentSize(pFrameToDisplay->getRect().size);

					CCDictionary* dict = frame->getUserInfo();
					if( dict )
					{
						/// 处理动画播放回调
						//TODO: [[NSNotificationCenter defaultCenter] postNotificationName:CCAnimationFrameDisplayedNotification object:target_ userInfo:dict];
					}
					m_nNextFrame = i+1;

					break;
				}
			}

			/// 动画回调
			callbackAni( fLastTime, m_fTime, fDuration );
			
			break;
		}
	case 1:
	case 2:
		{
			break;
		}
	}
}
//-------------------------------------------------------------------------
/**
@brief 动画回调实现
@param
*/
void CWareAnimate::callbackAni( float fLastTime, float fCurTime, float fDuration )
{
	/// 现在是回调时间
	if( m_pAniCallBack )
	{
		SAnimationCallbackInfo sAnimationCallbackInfo;

		/// 查看回调数据
		std::size_t size = m_vecCallbacks.size();
		for( size_t i = 0; i < size; ++i )
		{
			float t = m_vecCallbacks[i].fCallbackTime;
			if( t >= fLastTime && t <= fCurTime )
			{
				strcpy( sAnimationCallbackInfo.szName, m_vecCallbacks[i].strName.c_str() );
				strcpy( sAnimationCallbackInfo.szActName, m_strCurAniName.c_str() );
				sAnimationCallbackInfo.fFrameTime = t;
				m_pAniCallBack->onAnimaitonCallback( sAnimationCallbackInfo, m_pCallBackContext );
				return;
			}
		}

		if( fLastTime > -0.0001f && fLastTime < 0.0001f )
		{
			strcpy( sAnimationCallbackInfo.szName, "start" );
			strcpy( sAnimationCallbackInfo.szActName, m_strCurAniName.c_str() );
			sAnimationCallbackInfo.fFrameTime = 0.0f;
			m_pAniCallBack->onAnimaitonCallback( sAnimationCallbackInfo, m_pCallBackContext );
			return;
		}

		
		if( fCurTime < fLastTime )
		{
			strcpy( sAnimationCallbackInfo.szName, "end" );
			strcpy( sAnimationCallbackInfo.szActName, m_strCurAniName.c_str() );
			sAnimationCallbackInfo.fFrameTime = fDuration;
			m_pAniCallBack->onAnimaitonCallback( sAnimationCallbackInfo, m_pCallBackContext );
			return;
		}

	}
}
//-------------------------------------------------------------------------
/**
@brief 获取当前动作帧
@param
*/
int CWareAnimate::getFrameCount()
{
	if( m_pCurAnimation == NULL )
	{
		return 0;
	}

	CCArray* frames = m_pCurAnimation->getFrames();
	if( frames == NULL )
	{
		return 0;
	}

	return static_cast<int>(frames->count());
}

NS_CC_ENGINE_END
