
#include "cocos2d.h"
#include "cocos2dx_engine.h"
#include "CCEGLView.h"
#include "SimpleAudioEngine.h"
#include "WareAppDelegate.h"
#include "SceneManager.h"
#include "cocos2dx_engine.h"
#include "RenderObjManager.h"
#include "ShaderManager.h"
#include "InputManager.h"
#include "UITouchGroup.h"
using namespace CocosDenshion;

NS_CC_ENGINE_BEGIN

WareAppDelegate::WareAppDelegate()
: m_bShowFPS(true)
, m_bTouch(true)
{
	//m_pRoot = NULL;
	////m_DesignSize.setSize( 480,320 );
}
//------------------------------------------------------------------------------
WareAppDelegate::~WareAppDelegate()
{
    SimpleAudioEngine::end();

	//CRenderObjManager::Instance().releaseObjModel( false );
}
//-------------------------------------------------------------------------
/**
@brief 消息回调
@param uMsgId 消息ID
@param wParam
@param lParam
@param nTouchID
@return true 表示可以忽略后面调用 false 则表示还要继续执行
*/
bool WareAppDelegate::onMessageCallBack( unsigned long uMsgId, unsigned long wParam, unsigned long lParam, int nTouchID )
{
	IRenderSystem* pRenderSystem = getRenderSystem();
	if( pRenderSystem == NULL )
	{
		return false;
	}

	CCScene* pRootScene = pRenderSystem->getRootScene();

	/// 10000 系统定义的UILayerID
	ui::TouchGroup* pUILayer = (ui::TouchGroup*)pRootScene->getChildByTag( EGameLayerDepth_GUI );
	if( pUILayer == NULL )
	{
		return false;
	}

	return pUILayer->onMessageProc(uMsgId,wParam,lParam,nTouchID);
}
//-------------------------------------------------------------------------
/**
@brief 初始化
@param
*/
bool WareAppDelegate::init()
{
	/// 设置缩放大小
	if( m_bTouch )
	{
		CInputManager::Instance().setMessageCallback( this );
		CInputManager::Instance().setTouchEnabled( m_bTouch );
	}

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 帧更新
@param 
*/
void WareAppDelegate::updateFrame( float dt )
{
	CInputManager::Instance().update( dt );
}
//-------------------------------------------------------------------------
/**
@brief 场景更新
@param
*/
void WareAppDelegate::release()
{
	if( m_bTouch )
	{
		CInputManager::Instance().setTouchEnabled( false );
	}

	CCDirector::sharedDirector()->end();
}
//------------------------------------------------------------------------------
/**
@brief 是否显示FPS信息
@param bShow 显示开关
*/
void WareAppDelegate::setShowFPS( bool bShow )
{
	CCDirector *pDirector = CCDirector::sharedDirector();

	if( pDirector == NULL )
	{
		return;
	}

	if( m_bShowFPS == bShow )
	{
		return;
	}

	m_bShowFPS = bShow;
	// turn on display FPS
	pDirector->setDisplayStats(bShow);	
}
//------------------------------------------------------------------------------
/**
@brief 设置设计大小
*/
void WareAppDelegate::setDesignSize( CCSize& size )
{
	m_DesignSize = size;
}
//-------------------------------------------------------------------------
/**
@brief    Implement CCDirector and CCScene init code here.
@return true    Initialize success, app continue.
@return false   Initialize failed, app terminate.
*/
bool WareAppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
	if( pDirector == NULL )
	{
		CCLOG( "CCDirector == NULL" );
		return false;
	}

    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
//     pDirector->enableRetinaDisplay(true);

    // turn on display FPS
    pDirector->setDisplayStats(m_bShowFPS);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	/// 初始化
	init();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void WareAppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void WareAppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

NS_CC_ENGINE_END
