#include "RenderSystem.h"
#include "SceneManager.h"
#include "RenderMap.h"
#include "RenderObj.h"
#include "cocos2dx_types.h"

NS_CC_ENGINE_BEGIN

/// 全局的渲染系统
IRenderSystem*	g_pRenderSystem = NULL;
CRenderSystem g_RenderSystem;
bool CRenderSystem::m_bGrey = false; /// 默认是正常渲染

/**
*@brief 获取渲染系统
*/
IRenderSystem* getRenderSystem()
{
	return g_pRenderSystem;
}

CRenderSystem::CRenderSystem(void)
{
	CCAssert( g_pRenderSystem == NULL, "渲染接口不允许重复创建！" );

	g_pRenderSystem = this;
}

CRenderSystem::~CRenderSystem(void)
{
	m_RenderObjManager.releaseObjModel( false );
}
//-------------------------------------------------------------------------
/**
@brief 渲染系统初始化
@param sizeDesign 设计大小
*/
void CRenderSystem::init( CCSize& sizeDesign )
{
	CCScene* pRootScene = CCScene::create();
	CSceneManager::Instance().setRoot( pRootScene );
	CCDirector::sharedDirector()->runWithScene( pRootScene );

	//CCSize designSize = CCSizeMake(960, 640);

	// 设置游戏 的设计尺寸 大小
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(sizeDesign.width, sizeDesign.height, kResolutionExactFit );
}
//------------------------------------------------------------------------------
/**
@brief 获取场景根节点
@param
*/
CCScene* CRenderSystem::getRootScene()
{
	//CCScene* pRootScene
	return CSceneManager::Instance().getRoot();
}
//------------------------------------------------------------------------------
/**
@brief 进入游戏场景
@param[in] szScenename 场景配置文件名
@param[in] pRenderMapCallBack 场景进入和退出回调
*/
uint32 CRenderSystem::enterScene( const char* szScenename, IRenderMapCallBack* pRenderMapCallBack )
{
	return CSceneManager::Instance().enterScene( szScenename, pRenderMapCallBack );
}
//-------------------------------------------------------------------------
/**
@brief 是否是遮挡格
@param 场景坐标（转换后的场景坐标）
*/
bool CRenderSystem::isMask( const CCPoint& ptLoc )
{
	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return false;
	}

	return pRenderMap->isMask( ptLoc );
}
//------------------------------------------------------------------------------
/**
@brief 设置场景视点
@param[in] dwSceneID 场景运行时ID
@param[in] ptView 视点位置
*/
void CRenderSystem::setViewPoint( const CCPoint& ptView )
{
	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return;
	}

	//if( pRenderMap->getID() != dwSceneID && dwSceneID != 0 )
	//{
	//	return;
	//}

	pRenderMap->setViewPoint( ptView );
}
//-------------------------------------------------------------------------
/**
@brief 获取场景视点(场景坐标)
@param[in] ptView 视点位置
*/
void CRenderSystem::getViewPoint( CCPoint& ptView )
{
	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return;
	}

	//if( pRenderMap->getID() != dwSceneID && dwSceneID != 0 )
	//{
	//	return;
	//}

	ptView = pRenderMap->getViewPoint();	
}
//------------------------------------------------------------------------------
/**
@brief 设置场景缩放
@param
*/
void CRenderSystem::setSceneScale( uint32 dwSceneID, float fScale )
{
	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return;
	}

	if( pRenderMap->getID() != dwSceneID && dwSceneID != 0 )
	{
		return;
	}

	pRenderMap->setRenderScale( fScale );
}
//------------------------------------------------------------------------------
/**
@brief 从屏幕坐标到场景坐标
@param[in] ptScreenPos 屏幕位置
@param[out] ptScenePos 场景位置
*/
bool CRenderSystem::convertScreenToScene( const CCPoint& ptScreenPos, CCPoint& ptScenePos )
{
	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return false;
	}

	cocos2d::CCSize visibleSize = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
	pRenderMap->convertScreenToScene( ptScreenPos, visibleSize, ptScenePos );
	return true;
}
//-------------------------------------------------------------------------
/**
@brief 从场景坐标到屏幕坐标
@param[in] ptScenePos 场景位置
@param[out] ptScreenPos 屏幕位置
*/
bool CRenderSystem::convertSceneToScreen( const CCPoint& ptScenePos, CCPoint& ptScreenPos )
{
	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return false;
	}

	cocos2d::CCSize visibleSize = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
	pRenderMap->convertSceneToScreen( ptScenePos, visibleSize, ptScreenPos );
	return true;
}
//------------------------------------------------------------------------------
/**
@brief 获取视口大小(像素)
@param
*/
CCSize CRenderSystem::getCurrentSceneSize()
{
	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return CCSize(0,0);
	}

	CCSize size;
	pRenderMap->getSize( size );
	return size;
}
//------------------------------------------------------------------------------
/**
@brief 获取视口大小(像素)
@param
*/
CCSize CRenderSystem::getSceneTileSize()
{
	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return CCSize(0,0);
	}

	CCSize size;
	pRenderMap->getTileSize( size );
	return size;
}
//------------------------------------------------------------------------------
/**
@brief 获取视口大小(像素)
@param
*/
CCSize CRenderSystem::getViewSize()
{
	return cocos2d::CCDirector::sharedDirector()->getVisibleSize();
}
//-------------------------------------------------------------------------
/**
@brief 设置场景是否灰度图渲染
@param
*/
void CRenderSystem::setGrey( bool bGrey )
{
	m_bGrey = bGrey;
}
//------------------------------------------------------------------------------
/**
@brief 物体碰撞检测 
@param ptScreenPos 屏幕位置
@return 碰撞对象
*/
IRenderObj* CRenderSystem::hitRenderObj( const CCPoint& ptScreenPos )
{
	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return NULL;
	}

	//CCPoint ptLoc;
	//convertScreenToScene(ptScreenPos,ptLoc);
	return pRenderMap->hitTestObj( ptScreenPos );
}
//-------------------------------------------------------------------------
/**
@brief 物体碰撞检测 
@param ptScreenPos 屏幕位置
@return 碰撞对象
*/
void CRenderSystem::hitRenderObj( const CCPoint& ptScreenPos, std::list<IRenderObj*>& ObjList )
{
	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return;
	}

	//CCPoint ptLoc;
	//convertScreenToScene(ptScreenPos,ptLoc);
	pRenderMap->hitTestObj( ptScreenPos, ObjList );
}
//------------------------------------------------------------------------------
///////////////////// 游戏对象操作 /////////////////////////////////////////////////////
/**
@brief 创建游戏对象
@param[in] szGameobjName 游戏对象配置文件名
*/
IRenderObj* CRenderSystem::createRenderObj( const char* szGameobjName )
{
	CRenderObj* pRenderObj = m_RenderObjManager.createRenderObj( szGameobjName );
	return pRenderObj;
}
//------------------------------------------------------------------------------
/**
@brief 删除没有加入到场景中渲染对
@param
*/
void CRenderSystem::removeRenderObj( IRenderObj* pRenderObj )
{
	if( pRenderObj == NULL )
	{
		return;
	}

	m_RenderObjManager.removeRenderObj( pRenderObj->getID() );
}
//------------------------------------------------------------------------------
/**
@brief 添加到场景
@param[in] uid 游戏对象ID 对象不存在，添加失败
@param[in] dwSceneID 目标场景ID 如果目标场景不是当前场景，则添加失败
*/
bool CRenderSystem::addRenderObjToScene( IRenderObj* pRenderObj )
{
	if( pRenderObj == NULL )
	{
		return false;
	}

	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return false;
	}

	return pRenderMap->addRenderObj( TERRAIN_OBJ_LAYER, (CRenderObj*)pRenderObj ) > 0;
}
//------------------------------------------------------------------------------
/**
@brief 从场景中删除游戏对象
@param[in] 游戏对象ID
*/
void CRenderSystem::removeRenderObjFromScene( IRenderObj* pRenderObj )
{
	if( pRenderObj == NULL )
	{
		return;
	}

	CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
	if( pRenderMap == NULL )
	{
		return;
	}

	pRenderMap->removeRenderObj( pRenderObj->getID() );
}
//-------------------------------------------------------------------------
/**
@brief 画文字
@param szFontName 字体名称
@param nFontSize 字体大小
*/
IRenderText* CRenderSystem::createRenderText( const char* szString, const char* szFontName, int nFontSize, CCSize dimentions, bool bBmpFont )
{
	return m_RenderTextManager.createRenderText( szString, szFontName, nFontSize, dimentions, bBmpFont );
}
//-------------------------------------------------------------------------
/**
@brief 画文字
@param szFontName 字体名称
@param nFontSize 字体大小
*/
void CRenderSystem::removeRenderText( IRenderText* pRenderText )
{
	m_RenderTextManager.removeRenderText( pRenderText );	
}


NS_CC_ENGINE_END
