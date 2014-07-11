#include "SceneManager.h"
#include "RenderObj.h"
#include "WareAppDelegate.h"
#include "RenderMap.h"

NS_CC_ENGINE_BEGIN

CSceneManager::CSceneManager(void)
: m_pActiveScene(NULL)
, m_pRoot(NULL)
, m_uSceneSeed(0)
{
}
//-------------------------------------------------------------------------
/**
@brief 析构函数
@param
*/
CSceneManager::~CSceneManager(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 进入场景
@param
*/
uint32 CSceneManager::enterScene( const char* szSceneConfigName, IRenderMapCallBack* pRenderMapCallBack )
{
	if( m_pRoot == NULL )
	{
		return 0;
	}

	/// 根据地图配置文件创建地图
	CCDirector *pDirector = CCDirector::sharedDirector();
	if( pDirector == NULL )
	{
		return 0;
	}

	if( szSceneConfigName == NULL || szSceneConfigName[0] == 0 )
	{
		return 0;
	}


	CCLayer* pLayer = (CCLayer*)m_pRoot->getChildByTag( EGameLayerDepth_BackGround_Ground );
	if( pLayer == NULL )
	{
		pLayer = CCLayer::create();
		if( pLayer == NULL )
		{
			return 0;
		}

		m_pRoot->addChild( pLayer, EGameLayerDepth_BackGround_Ground, EGameLayerDepth_BackGround_Ground );
	}

	CRenderMap* pRenderMap = NULL;
	if( m_pActiveScene != NULL )
	{
		pLayer->removeChild( m_pActiveScene );
		m_pActiveScene = NULL;
	}

	if( ++m_uSceneSeed == 0 )
	{
		++m_uSceneSeed;
	}

	pRenderMap = new CRenderMap(m_uSceneSeed);
	if( pRenderMap == NULL )
	{
		return 0;
	}

	if( !pRenderMap->initWithFile( szSceneConfigName ) )
	{
		SAFE_DELETE( pRenderMap );
		return 0;
	}

	pRenderMap->setSceneCallBack( pRenderMapCallBack );

	pRenderMap->autorelease();

	pLayer->addChild( pRenderMap );

	m_pActiveScene = pRenderMap;

	m_pActiveScene->setUpdateEnable( true );

	return 1;
}
//-------------------------------------------------------------------------
/**
@brief 更新函数 为资源释放和缓存留下执行机会
@param
*/
void CSceneManager::update( float dt )
{
	if( m_pActiveScene )
	{
		m_pActiveScene->update( dt );
	}
}

NS_CC_ENGINE_END