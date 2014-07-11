
#include "cocos2d.h"
#include "cocos-ext.h"
#include "MainGameApp.h"
#include "script_support/CCScriptSupport.h"
#include "CCLuaEngine.h"
#include <vector>
#include "IRenderObj.h"
#include "IRenderSystem.h"
#include "IRenderText.h"
#include "WareTime.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

CMainGameApp::CMainGameApp()
{
}

CMainGameApp::~CMainGameApp()
{
}
//-------------------------------------------------------------------------
/**
@brief 初始化
@param
*/
bool CMainGameApp::init()
{
	cocos2d::engine::WareAppDelegate::init();
	srand(cocos2d::engine::WareTime::getTickCount());

	// 设置游戏资源 跟目录
	std::vector<std::string> searchPaths;
	//searchPaths.push_back("/");
	searchPaths.push_back("UI/");
	
	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize designSize = CCSizeMake(960, 640);
	if( screenSize.width/screenSize.height < 1.4f )
	{ 
		designSize = CCSizeMake(1024, 768); 
		searchPaths.push_back("Json/1024768/");
	}
	else 
	{
		designSize = CCSizeMake(960, 640);
		searchPaths.push_back("Json/960640/");
	}

	CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

	// 渲染系统初始化
	cocos2d::engine::IRenderSystem* pRenderSys = cocos2d::engine::getRenderSystem();
	if( pRenderSys == NULL )
	{
		return false;
	}
	pRenderSys->init(designSize);

	cocos2d::CCScene* pRootScene = pRenderSys->getRootScene();
	if( pRootScene == NULL )
	{
		return false;
	}

	cocos2d::engine::IRenderObj* pRenderObj = pRenderSys->createRenderObj( "Player/123.obj" );
	if( pRenderObj == NULL )
	{
		return false;
	}

	cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
	pRenderObj->attach( pRootScene, 0 );
	pRenderObj->setPos( ccp(size.width/2,size.height/2-100) );
	pRenderObj->playAnimation( "walk", 0.3f );

	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
	CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
	std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("Scripts/Main.lua");
	pEngine->executeScriptFile(path.c_str());

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 游戏释放
@param
*/
void CMainGameApp::release()
{
	cocos2d::engine::WareAppDelegate::release();
}
//-------------------------------------------------------------------------
/**
@brief 消息回调
@param uMsgId 消息ID
@param wParam
@param lParam
@return true 表示可以忽略后面调用 false 则表示还要继续执行
*/
bool CMainGameApp::onMessageCallBack( unsigned long uMsgId, unsigned long wParam, unsigned long lParam, int nTouchID )
{
	/// UI消息已经发送到底层扩展库
	if( !WareAppDelegate::onMessageCallBack( uMsgId, wParam, lParam, nTouchID ) )
	{
		/// 渲染和逻辑层对输入的处理
		return true;//pController->MessageProc( uMsgId, wParam, lParam );
	}

	return true;
}


//-------------------------------------------------------------------------
/**
@brief 帧更新
@param
*/
void CMainGameApp::updateFrame( float dt )
{
	WareAppDelegate::updateFrame( dt );
} 
