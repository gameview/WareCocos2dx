#include "InputManager.h"

#define TOUCH_LONGPRESS_TIME  0.5f

NS_CC_ENGINE_BEGIN

CInputManager::CInputManager()
: m_pMessageCallback(NULL)
, m_fPressTime(0)
, m_bPress(false)
, m_bMoved(false)
, m_bLongPress(false)
, m_ptPressPos(CCPointZero)
, m_nTouchID(0)
{
	CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate((CCKeypadDelegate*)this);
}
//-------------------------------------------------------------------------
/**
@brief 设置是否启动触摸
@param
*/
void CInputManager::setTouchEnabled( bool bTouch )
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	if( pDirector == NULL )
	{
		return;
	}

	if( bTouch )
	{
		/// 触摸代理设置
		pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	}
	else
	{
		pDirector->getTouchDispatcher()->removeDelegate(this);
	}
}
//-------------------------------------------------------------------------
/// 触摸开始回调
bool CInputManager::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if( m_pMessageCallback == NULL || touch == NULL )
	{
		return false;
	}

	m_ptPressPos = touch->getLocation();
	m_nTouchID = touch->getID();
	m_pMessageCallback->onMessageCallBack( MSG_TOUCH_BEGIN, (uint32)m_ptPressPos.x, (uint32)m_ptPressPos.y, m_nTouchID );

	m_bPress = true;
	m_fPressTime = 0.0f;
	m_bLongPress = false;

	return true;
}
//-------------------------------------------------------------------------
/// 触摸移动回调
void CInputManager::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	if( m_pMessageCallback == NULL || touch == NULL )
	{
		return;
	}

	CCPoint touchPoint = touch->getLocation();
	m_pMessageCallback->onMessageCallBack( MSG_TOUCH_MOVE, (uint32)touchPoint.x, (uint32)touchPoint.y, touch->getID() );
	m_bMoved = true;
	m_fPressTime = 0.0f;
	m_bLongPress = false;
}
//-------------------------------------------------------------------------
/// 触摸结束回调
void CInputManager::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	if( m_pMessageCallback == NULL || touch == NULL )
	{
		return;
	}

	CCPoint touchPoint = touch->getLocation();
	m_pMessageCallback->onMessageCallBack( MSG_TOUCH_END, (uint32)touchPoint.x, (uint32)touchPoint.y, touch->getID() );

	m_bPress = false;
	m_fPressTime = 0.0f;
	m_bMoved = false;
	m_bLongPress = false;
}
//-------------------------------------------------------------------------
//  The back key clicked
void CInputManager::keyBackClicked()
{
	if( m_pMessageCallback == NULL )
	{
		return;
	}

	m_pMessageCallback->onMessageCallBack( MSG_KEY_BACK, (uint32)0, (uint32)0, -1 );
}
//-------------------------------------------------------------------------
// The menu key clicked. only available on wophone & android
void CInputManager::keyMenuClicked() 
{
	if( m_pMessageCallback == NULL )
	{
		return;
	}

	m_pMessageCallback->onMessageCallBack( MSG_KEY_MENU, (uint32)0, (uint32)0, -1 );
}
//-------------------------------------------------------------------------
/**
@brief 更新
@param 
*/
void CInputManager::update( float dt )
{
	if( m_pMessageCallback == NULL )
	{
		return;
	}

	if( m_bPress && !m_bMoved )
	{
		m_fPressTime += dt;
		if( m_fPressTime > TOUCH_LONGPRESS_TIME && !m_bLongPress )
		{
			m_bLongPress = true;
			m_pMessageCallback->onMessageCallBack( MSG_TOUCH_LONGPRESS, (uint32)m_ptPressPos.x, (uint32)m_ptPressPos.y, m_nTouchID );
		}
	}
}

NS_CC_ENGINE_END
