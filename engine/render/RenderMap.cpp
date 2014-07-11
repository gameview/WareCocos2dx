#include "RenderMap.h"
#include "SceneLayer.h"
#include "ImageLayer.h"
#include "TerrainBlock.h"
#include "RectangleObj.h"
#include "WareTime.h"
#include "RenderCommon.h"
#include "WareFileWrite.h"
#include "WareFileRead.h"
#include "tinyxml.h"
#include "RenderObj.h"
#include "RenderSystem.h"

#include <algorithm>

#define SCENE_FILE_VER_1		10000	/// 初始版本
#define SCENE_FILE_VER_2		10001   /// 增加地图类型 tile和笔刷
#define SCENE_FILE_VER_NEW		10002	/// tile地图支持视差

NS_CC_ENGINE_BEGIN

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((uint32)(BYTE)(ch0) | ((uint32)(BYTE)(ch1) << 8) |   \
	((uint32)(BYTE)(ch2) << 16) | ((uint32)(BYTE)(ch3) << 24 ))
#endif

CRenderMap::CRenderMap( uint32 uid )
: m_bUpdateEnable(false)
, m_uID( uid )
{
	m_nTerrainChunkWidth = 0;	
	m_nTerrainChunkHeight = 0;
	m_nTerrainWidth = 0;
	m_nTerrainHeight = 0;

	m_nTerrainGridWidth = 0;
	m_nTerrainGridHeight = 0;
	m_bParallax = false;		
	m_fParallexFactor[0] = 0.0f;
	m_fParallexFactor[1] = 0.0f;

	m_vecTerrainChunk.clear();
	m_vecVisibleTerrainChunk.clear();

	m_mapSceneLayer.clear();

	m_ptView.x = 0;
	m_ptView.y = 0;

	m_uSeed = 0;

	m_fScale = 1.0f;

	m_strSceneName = "";

	m_pRenderMapCallBack = NULL;
}


CRenderMap::~CRenderMap(void)
{
}
//------------------------------------------------------------------------------
void CRenderMap::update( float dt )
{
	//CCLOG( "update scene" );

	//if( m_pGameSceneCallBack )
	//{
	//	m_pGameSceneCallBack->update( this, dt );
	//}

	/// 只是调用可见对象集中update 这里需要优化
	RenderObjSet::iterator iter = m_setRenderObject.begin();
	for( ; iter != m_setRenderObject.end(); ++iter )
	{
		TerrainObjMap::iterator it = m_mapTerrainObj.find( (*iter) );
		if( it == m_mapTerrainObj.end() )
		{
			continue;
		}

		CRenderObj* pRenderObj = it->second.pRenderObj;
		if( pRenderObj )
		{
			pRenderObj->update( dt );
		}
	}

	if( m_bUpdateTerrainObj )
	{
		cullVisibleObject();
		updateVisibleObject();

		m_bUpdateTerrainObj = false;
	}
}
//-------------------------------------------------------------------------
/**
@brief 进入场景
@param
*/
void CRenderMap::onEnter()
{
	//CCLOG( "onEnter scene" );

	cocos2d::CCNode::onEnter();

	//CCLOG( "Node onEnter scene %d", m_pGameSceneCallBack );
	//if( m_pGameSceneCallBack )
	//{
	//	m_pGameSceneCallBack->IGameSceneCallBack::onEnter( this );
	//}

	//CCLOG( "onEnter end" );
}
//------------------------------------------------------------------------------
/**
@brief 退出
@param
*/
void CRenderMap::onExit()
{
	//if( m_pGameSceneCallBack )
	//{
	//	m_pGameSceneCallBack->onExit( this );
	//}

	//GameObjSet::iterator iter = m_setGameObj.begin();
	//for( ; iter != m_setGameObj.end(); )
	//{
	//	CGameObjManager::Instance().removeGameObj( (*iter) );
	//	++iter;
	//}

	//m_setGameObj.clear();

	//RenderObjSet::iterator iter = m_setRenderObj.begin();
	//for( ; iter != m_setRenderObj.end(); ++iter )
	//{
	//	//TerrainObjMap::iterator it = m_mapTerrainObj.find( (*iter) );
	//	//if( it == m_mapTerrainObj.end() )
	//	//{
	//	//	continue;
	//	//}

	//	removeObj( (*iter) );
	//}

	cocos2d::CCNode::onExit();
}
////------------------------------------------------------------------------------
///**
//@brief 创建游戏对象
//@param
//*/
//uint32 CRenderMap::addRenderObj( const char* szGameObjName, int nTag )
//{
//	if( szGameObjName == NULL )
//	{
//		return 0;
//	}
//
//	/// 整个地图分为很多层，按高度从低到高来排序 凹陷地表 < 地表 < 草 < 地表杂物(石块等) < 对象层(可以和人物构成遮挡的物件)
//	/// 加入到对象层
//
//	CCRect rect;
//	CCPoint ptLoc;// = pRenderObj->getPosition();
//	//addTerrainObj( TERRAIN_OBJ_LAYER, szGameObjName, rect, ptLoc );
//	CRenderObj* pRenderObj = CRenderObjManager::Instance().createRenderObj( szGameObjName );
//	if( pRenderObj == NULL )
//	{
//		return 0;
//	}
//
//	return addRenderObj( TERRAIN_OBJ_LAYER, pRenderObj );
//
//	//m_setRenderObject.insert( pRenderObj->getID() );
//
//	//if( nTag != 0 )
//	//{
//	//	m_mapRenderObjTag[nTag] = pRenderObj;
//	//}
//
//	//addChild( pRenderObj, 10 );
//
//	/// 便于做出遮挡效果
//}
//------------------------------------------------------------------------------
/**
@brief 场景树上摘除对象
@param
*/
void CRenderMap::removeRenderObj( uint32 uid )
{
	RenderObjToSceneMap::iterator iter = m_mapRenderObjToScene.find( uid );
	if( iter != m_mapRenderObjToScene.end() )
	{
		removeObj( iter->second );
	}
}
//------------------------------------------------------------------------------
/**
@brief 获取渲染对象
@param
*/
CRenderObj* CRenderMap::getRenderObj( uint32 uid )
{
	TerrainObjMap::iterator it = m_mapTerrainObj.find( uid );
	if( it == m_mapTerrainObj.end() )
	{
		return NULL;
	}

	return it->second.pRenderObj;
}
//------------------------------------------------------------------------------
/**
@brief 是否开启Update
@param
*/
void CRenderMap::setUpdateEnable( bool bEnable )
{
	if( m_bUpdateEnable == bEnable )
	{
		return;
	}

	if( m_bUpdateEnable )
	{
		unscheduleUpdate();
	}
	else
	{
		scheduleUpdate();
	}

	m_bUpdateEnable = bEnable;
}
//------------------------------------------------------------------------------
/**
@brief 转换屏幕坐标到场景坐标
@param
*/
void CRenderMap::convertScreenToScene( const CCPoint& ptScreenPos, const CCSize& size, CCPoint& ptScenePos )
{
	CCPoint ptCenter;;
	ptCenter.x = size.width /2;
	ptCenter.y = size.height /2;

	float fScale = m_fScale;
	if( fScale < 0.000f )
	{
		fScale = 0.001f;
	}

	ptScenePos.x = ( ptScreenPos.x - ptCenter.x )/fScale + m_ptView.x;
	ptScenePos.y = ( ptScreenPos.y - ptCenter.y )/fScale + m_ptView.y;
}
//-------------------------------------------------------------------------
/**
@brief 转换屏幕坐标到场景坐标
@param
*/
void CRenderMap::convertSceneToScreen( const CCPoint& ptScenePos, const CCSize& size, CCPoint& ptScreenPos )
{
	CCPoint ptCenter;;
	ptCenter.x = size.width /2;
	ptCenter.y = size.height /2;

	float fScale = m_fScale;
	if( fScale < 0.000f )
	{
		fScale = 0.001f;
	}

	ptScreenPos.x = ( ptScenePos.x - m_ptView.x ) * fScale + ptCenter.x;
	ptScreenPos.y = ( ptScenePos.y - m_ptView.y ) * fScale + ptCenter.y;
}
//------------------------------------------------------------------------------
/**
@brief 从文件初始化
@param
*/
bool CRenderMap::initWithFile( const char* szSceneFileName )
{
	if( szSceneFileName == NULL || szSceneFileName[0] == 0 )
	{
		return false;
	}

	CCLOG( "CRenderMap::initWithFile %s", szSceneFileName );

	std::string strPath = szSceneFileName;
	strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(strPath.c_str());
	if (strPath.size() == 0)
	{
		return NULL;
	}

	m_strSceneName = szSceneFileName;

	unsigned long lSize = 0;
	unsigned char* pBuff = CCFileUtils::sharedFileUtils()->getFileData( strPath.c_str(), "rb", &lSize );

	cocos2d::engine::CWareFileRead File;
	CCLOG( "CRenderMap::initWithFile %s", strPath.c_str() );
	//if( !File.open(strPath.c_str()) )
	//{
	//	return false;
	//}

	File.open( pBuff, lSize );

	CCLOG( "CRenderMap::readFile" );

	int nFlag = 0;
	File.read( nFlag );
	if( nFlag != MAKEFOURCC( 'B', 'X','S', 0 ) )
	{
		CCLOG( "场景文件格式不对!" );
		return false;
	}

	CCLOG( "CRenderMap::readVersion" );

	int nVersion = 0;
	File.read( nVersion );

	readSceneFileVer( File, nVersion );

	std::string strName = szSceneFileName;
	StringUtility::stringReplace( strName, ".scene", ".mask" );
	m_TerrainMask.loadObjFromFile( strName );

	CCLOG( "CRenderMap::readEnd" );

	delete [] pBuff;

	CCLOG( "CRenderMap::delete end" );

	return true;
}
//------------------------------------------------------------------------------
/**
@brief 设置场景数据(宽高,地表图块宽高)
@param
*/
void CRenderMap::setTerrainInfo( int nTerrainWidth, int nTerrainHeight, int nTerrainChunkWidth, int nTerrainChunkHeight )
{
	if( nTerrainHeight == 0 || nTerrainWidth == 0 )
	{
		return;
	}

	if( nTerrainChunkWidth == 0 || nTerrainChunkHeight == 0 )
	{
		return;
	}

	m_nTerrainChunkWidth = nTerrainChunkWidth;	/// 地表块宽度
	m_nTerrainChunkHeight = nTerrainChunkHeight;	/// 地表块高度
	m_nTerrainWidth = nTerrainWidth;		/// 地表宽度
	m_nTerrainHeight = nTerrainHeight;		/// 地表高度

	int nTerrainPixelWidth = m_nTerrainWidth*m_nTerrainChunkWidth;
	int nTerrainPixelHeihgt = m_nTerrainHeight*m_nTerrainChunkHeight;
	m_nTerrainGridWidth = (nTerrainPixelWidth % TERRAIN_GRID_WIDTH) == 0 ? nTerrainPixelWidth / TERRAIN_GRID_WIDTH : nTerrainPixelWidth / TERRAIN_GRID_WIDTH + 1;
	m_nTerrainGridHeight = (nTerrainPixelHeihgt % TERRAIN_GRID_HEIGHT) == 0 ? nTerrainPixelHeihgt / TERRAIN_GRID_HEIGHT : nTerrainPixelHeihgt / TERRAIN_GRID_HEIGHT + 1;
	for( int i = 0; i < m_nTerrainGridHeight; ++i )
	{
		for( int j = 0; j < m_nTerrainGridWidth; ++j )
		{
			m_mapTerrainGrid[i*m_nTerrainGridWidth+j] = new CTerrainGrid();
		}
	}

	/// 计算tile宽高
	m_nTerrainTileWidth = ((nTerrainPixelWidth % TERRAIN_TILE_WIDTH) == 0 ) ? nTerrainPixelWidth / TERRAIN_TILE_WIDTH : nTerrainPixelWidth / TERRAIN_TILE_WIDTH + 1;
	m_nTerrainTileHeight = ((nTerrainPixelHeihgt % TERRAIN_TILE_HEIGHT) == 0 ) ? nTerrainPixelHeihgt / TERRAIN_TILE_HEIGHT : nTerrainPixelHeihgt / TERRAIN_TILE_HEIGHT + 1;

	///// 添加地表块
	int nTerrainChunkNum = nTerrainWidth * nTerrainHeight;
	m_vecTerrainChunk.resize( nTerrainChunkNum );
	for( int i = 0; i < nTerrainChunkNum; ++i )
	{
		m_vecTerrainChunk[i].nXIndex = i % nTerrainWidth;
		m_vecTerrainChunk[i].nYIndex = i / nTerrainWidth;
	}

	/// 添加Terrainblock layer
	CSceneLayer* pSceneLayer = CSceneLayer::create();
	if( pSceneLayer )
	{
		pSceneLayer->setLayerID( TERRAIN_CHUNK_LAYER );
		addChild( pSceneLayer, TERRAIN_CHUNK_LAYER, TERRAIN_CHUNK_LAYER );
		m_mapSceneLayer[TERRAIN_CHUNK_LAYER] = pSceneLayer;
	}

	/// 创建对象层
	pSceneLayer = CSceneLayer::create();
	if( pSceneLayer )
	{
		pSceneLayer->setLayerID( TERRAIN_OBJ_LAYER );
		addChild( pSceneLayer, TERRAIN_OBJ_LAYER, TERRAIN_OBJ_LAYER );
		m_mapSceneLayer[TERRAIN_OBJ_LAYER] = pSceneLayer;
	}
}
//------------------------------------------------------------------------------
/**
@brief 设置地表块信息
@param
*/
void CRenderMap::setTerrainChunk( int nXIndex, int nYIndex, const std::string& strTextureName, const CCRect rc )
{
	if( nXIndex >= m_nTerrainWidth || nYIndex >= m_nTerrainHeight )
	{
		return;
	}

	int nIndex = nYIndex * m_nTerrainWidth + nXIndex;
	//CCAssert( m_vecTerrainBlock[nIndex].nXIndex == nXIndex && m_vecTerrainBlock[nIndex].nYIndex == nYIndex, "地表块坐标数据错误" );
	//CCAssert( m_nSelectedIndex == nIndex, "地表块不是当前选择地表块" );

	m_vecTerrainChunk[nIndex].strTexture = strTextureName;
	m_vecTerrainChunk[nIndex].rect = rc;

	CSceneLayer* pSceneLayer = (CSceneLayer*)getChildByTag( TERRAIN_CHUNK_LAYER );
	if( pSceneLayer )
	{
		CCPoint ptLoc;
		ptLoc.x = (float)(nXIndex * m_nTerrainChunkWidth + m_nTerrainChunkWidth / 2);
		ptLoc.y = (float)(nYIndex * m_nTerrainChunkHeight + m_nTerrainChunkHeight / 2);
		addTerrainObj( TERRAIN_CHUNK_LAYER, m_vecTerrainChunk[nIndex].strTexture, m_vecTerrainChunk[nIndex].rect, ptLoc );
	}
}
////------------------------------------------------------------------------------
///**
//@brief 设置Block颜色
//@param
//*/
//void CRenderMap::setBlockColor( cocos2d::ccColor4B& c )
//{
//	m_BlockColor = c;
//
//	CSceneLayer* pLayer = (CSceneLayer*)getChildByTag( BLOCK_GRID );
//	if( pLayer )
//	{
//		pLayer->setRectangleObjColor( c );
//	}
//}
//------------------------------------------------------------------------------
/**
@brief 设置视点
@param
*/
void CRenderMap::setViewPoint( const CCPoint& pt )
{
	if( m_bParallax )
	{
		CCPoint ptCenter = ccp( ( m_nTerrainChunkWidth * m_nTerrainWidth ) / 2, ( m_nTerrainChunkHeight * m_nTerrainHeight ) / 2 );
		CCSize visibleSize = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
		CCPoint ptViewCenter;

		ptViewCenter.x = visibleSize.width / 2;
		ptViewCenter.y = visibleSize.height / 2;

		// 计算视差位移
		for( int n = TERRAIN_BACKGROUND_LAYER; n <= TERRAIN_EMBE_LAYER; ++n )
		{
			CImageLayer* pLayer = (CImageLayer*)getChildByTag( n );
			if( pLayer )
			{
				float x = -( pt.x - ptCenter.x ) * m_fParallexFactor[n - TERRAIN_BACKGROUND_LAYER] + ptViewCenter.x;
				float y = -( pt.y - ptCenter.y ) * m_fParallexFactor[n - TERRAIN_BACKGROUND_LAYER] + ptViewCenter.y;
				pLayer->setPosition( ccp(x,y) );
			}
		}
	}

	m_ptView = pt;

	//cocos2d::CCCamera* pCamera = getCamera();
	//if( pCamera )
	//{
	//	float fEyeX,fEyeY,fEyeZ;
	//	pCamera->getEyeXYZ(&fEyeX, &fEyeY, &fEyeZ);
	//	pCamera->setEyeXYZ(pt.x,pt.y,fEyeZ);
	//}
	//setPosition(-pt.x,-pt.y);

	//std::map< int, CSceneLayer* >::iterator iter = m_mapSceneLayer.begin();
	//for( ; iter != m_mapSceneLayer.end(); ++iter )
	//{
	//	if( iter->second )
	//	{
	//		iter->second->setScale( m_fScale );
	//		iter->second->setPosition( m_ptView - pt );
	//	}
	//}

	m_bUpdateTerrainObj = true;
}
//------------------------------------------------------------------------------
/**
@brief 获取场景宽高
@param
*/
void CRenderMap::getSize( CCSize& size )
{
	size.width = m_nTerrainWidth * m_nTerrainChunkWidth;
	size.height = m_nTerrainHeight * m_nTerrainChunkHeight;
}
//------------------------------------------------------------------------------
/**
@brief 获取tile的宽高
@param
*/
void CRenderMap::getTileSize( CCSize& size )
{
	size.width = m_nTerrainTileWidth;
	size.height = m_nTerrainTileHeight;
}
//------------------------------------------------------------------------------
/**
@brief 设置缩放比例
@param
*/
void CRenderMap::setRenderScale(float fScale)
{
	m_fScale *= fScale;

	//setScale( m_fScale ); /// 以坐标原点为中心缩放

	/// 场景层筛选地表物件
	std::map< int, CSceneLayer* >::iterator iter = m_mapSceneLayer.begin();
	for( ; iter != m_mapSceneLayer.end(); ++iter )
	{
		if( iter->second )
		{
			iter->second->setScale( m_fScale );
		}
	}
}
////------------------------------------------------------------------------------
///**
//@brief 画地表块
//@param
//*/
//void CRenderMap::drawBlock()
//{
//	cocos2d::CCSize visibleSize = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
//	cocos2d::CCPoint ptCenter;
//
//	ptCenter.x = visibleSize.width / 2;
//	ptCenter.y = visibleSize.height / 2;
//
//	int nRenderWidth = (int)(0.5f+m_nTerrainBlockWidth * m_fScale);
//	int nRenderHeight = (int)(0.5f+m_nTerrainBlockHeight * m_fScale);
//
//	cocos2d::CCPoint ptView;
//	ptView.x = m_ptView.x * m_fScale;
//	ptView.y = m_ptView.y * m_fScale;
//
//	/// 根据视点筛选
//	std::size_t size = m_vecVisibleTerrainBlock.size();
//	for( size_t i = 0; i < size; ++i )
//	{
//		int nBlockIndex = m_vecVisibleTerrainBlock[i];
//
//		drawSigelBlock( nBlockIndex, nRenderWidth, nRenderHeight, m_BlockColor, ptCenter, ptView );
//	}
//
//	/// 画选择block
//	int nBlockIndex = m_nSelectedIndex;
//	if( m_nSelectedIndex >= 0 && m_nSelectedIndex < m_vecTerrainBlock.size() )
//	{
//		drawSigelBlock( nBlockIndex, nRenderWidth, nRenderHeight, cocos2d::ccc4(255, 0, 0, 255), ptCenter, ptView );
//	}
//}
////------------------------------------------------------------------------------
///**
//@brief 画单个Block
//@param
//*/
//void CRenderMap::drawSigelBlock( int nBlockIndex, int nBlockWidth, int nBlockHeight, cocos2d::ccColor4B& color, cocos2d::CCPoint& ptCenter, cocos2d::CCPoint& ptView )
//{
//	std::size_t size = m_vecTerrainBlock.size();
//	/// 画选择block
//	//int nBlockIndex = m_nSelectedIndex;
//	if( nBlockIndex >= 0 && nBlockIndex < size )
//	{
//		cocos2d::CCRect blockRect( m_vecTerrainBlock[nBlockIndex].nXIndex * nBlockWidth - ptView.x,
//			m_vecTerrainBlock[nBlockIndex].nYIndex * nBlockHeight - ptView.y, nBlockWidth, nBlockHeight );
//
//		int nViewLeft = blockRect.getMinX() + ptCenter.x;
//		int nViewRight = blockRect.getMaxX() + ptCenter.x;
//		int nViewTop = blockRect.getMaxY() + ptCenter.y;
//		int nViewBottom = blockRect.getMinY() + ptCenter.y;
//
//		CHECK_GL_ERROR_DEBUG();
//
//		cocos2d::ccDrawColor4B(color.r,color.g,color.b,color.a);
//
//		glLineWidth(1);
//		cocos2d::CCPoint vertices[] = { ccp(nViewLeft,nViewBottom), ccp(nViewRight,nViewBottom), ccp(nViewRight,nViewTop), ccp(nViewLeft,nViewTop), ccp(nViewLeft,nViewBottom) };
//		cocos2d::ccDrawPoly( vertices, 5, false);
//	}
//}
////------------------------------------------------------------------------------
///**
//@brief 筛选可见对象
//@param
//*/
//void CRenderMap::cullVisibleObject()
//{
//	/// 场景层筛选地表物件
//	std::map< int, CSceneLayer* >::iterator iter = m_mapSceneLayer.begin();
//	for( ; iter != m_mapSceneLayer.end(); ++iter )
//	{
//		if( iter->second )
//		{
//			iter->second->updateTerrainObj( m_ptView, m_fScale, m_nTerrainChunkWidth, m_nTerrainChunkHeight );
//		}
//	}
//}

//------------------------------------------------------------------------------
#ifndef MAX
#define MAX( a, b )  (a) > (b) ? (a) : (b)
#endif
//------------------------------------------------------------------------------
/**
@brief 筛选可见对象
@param
*/
void CRenderMap::cullVisibleObject()
{
	IRenderSystem* pRenderrSys = getRenderSystem();
	if( pRenderrSys == NULL )
	{
		return;
	}

	/// 场景层筛选地表物件
	/// 计算扫描宽度
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	int nCullWidth = visibleSize.width / 2;
	int nCullHeight = visibleSize.height / 2;

	int nScanWidth = MAX( 1, ( 0.5f + ( nCullWidth + TERRAIN_GRID_WIDTH ) / ( TERRAIN_GRID_WIDTH * m_fScale ) ) );
	int nScanHeight = MAX( 1, ( 0.5f + ( nCullHeight + TERRAIN_GRID_HEIGHT )  / (TERRAIN_GRID_HEIGHT * m_fScale ) ) );

	int nCurrentGridX = m_ptView.x / TERRAIN_GRID_WIDTH;
	int nCurrentGridY = m_ptView.y / TERRAIN_GRID_HEIGHT;

	/// 查找可视对象集
	m_setVisibleObj.clear();
	//std::set<uint32> ObjSet;
	for( int i = -nScanHeight; i <= nScanHeight; ++i )
	{
		for( int j = -nScanWidth; j <= nScanWidth; ++j )
		{
			int x = j + nCurrentGridX;
			int y = nCurrentGridY + i;
			if( x < 0 || x >= m_nTerrainGridWidth || y < 0 || y >= m_nTerrainGridHeight )
			{
				continue;
			}

			int id = y * m_nTerrainGridWidth + x;
			TerrainGridMap::iterator iter = m_mapTerrainGrid.find( id );
			if( iter != m_mapTerrainGrid.end() )
			{
				if( iter->second )
				{
					std::set<unsigned int>& pObjSet = iter->second->getTerrainObjs();

					std::set<unsigned int>::iterator itID = pObjSet.begin();
					for( ; itID != pObjSet.end(); ++itID )
					{
						m_setVisibleObj.insert((*itID));
					}
				}
			}
		}
	}

	/// 不参与筛选的物件
	//std::set<uint32>::iterator itObj = m_setNoCullObj.begin();
	//for( ; itObj != m_setNoCullObj.end(); ++itObj )
	//{
	//	m_setVisibleObj.insert( (*itObj) );
	//}

	//m_setNoCullObj

	/// 筛选活动对象
	std::set<uint32>::iterator itObj = m_setRenderObject.begin();
	for( ; itObj != m_setRenderObject.end(); ++itObj )
	{
		m_setVisibleObj.insert( (*itObj) );
	}

	/// 删除已经不在视野的物件
	std::vector<uint32> removeList;

	itObj = m_setRenderObj.begin();
	for( ; itObj != m_setRenderObj.end(); ++itObj )
	{
		if( m_setVisibleObj.find( (*itObj) ) == m_setVisibleObj.end() )
		{
			removeList.push_back( (*itObj) );
		}
	}

	/// 删除不可见的对象 是否需要立即删除，以后再做优化调整
	size_t size = removeList.size();
	for( size_t i = 0; i < size; ++i )
	{
		TerrainObjMap::iterator iter = m_mapTerrainObj.find( removeList[i] );
		if( iter != m_mapTerrainObj.end() )
		{
			if( iter->second.pRenderObj )
			{
				//CRenderObjManager::Instance().removeRenderObj( iter->second.pRenderObj->getID() );
				//iter->second.pRenderObj = NULL;

				CSceneLayer* pLayer = getSceneLayer( iter->second.nLayerID );
				if( pLayer )
				{
					pLayer->removeChild( iter->second.pRenderObj );
				}

				/// 场景上摘除时间
				iter->second.dwTime = WareTime::getTickCount();

				pRenderrSys->removeRenderObj( iter->second.pRenderObj );
				iter->second.pRenderObj = NULL;
			}
		}
	}

	m_setRenderObj = m_setVisibleObj;
}
//------------------------------------------------------------------------------
/**
@brief 更新可视集
@param
*/
void CRenderMap::updateVisibleObject()
{
	/// 清理对象
	//SceneLayerMap::iterator itLayer = m_mapSceneLayer.begin();
	//for( ; itLayer != m_mapSceneLayer.end(); ++itLayer )
	//{
	//	if( itLayer->second )
	//	{
	//		itLayer->second->removeAllChildren();
	//	}
	//}

	IRenderSystem* pRenderrSys = getRenderSystem();
	if( pRenderrSys == NULL )
	{
		return;
	}

	cocos2d::CCSize visibleSize = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
	cocos2d::CCPoint ptCenter;

	ptCenter.x = visibleSize.width / 2;
	ptCenter.y = visibleSize.height / 2;

	/// 新找出来的全部添加到渲染树
	std::set<uint32>::iterator itObj = m_setVisibleObj.begin();
	for( ; itObj != m_setVisibleObj.end(); ++itObj )
	{
		unsigned int uid = (*itObj);
		TerrainObjMap::iterator it = m_mapTerrainObj.find( (*itObj) );
		if( it == m_mapTerrainObj.end() )
		{
			continue;
		}

		if( it->second.pRenderObj == NULL )
		{
			//if( it->second.bRectObj )
			//{
			//	CRectangleObj* pRect = CRectangleObj::create();
			//	pRect->setRect( it->second.rect.size.width, it->second.rect.size.height );
			//	pRect->setBorderColor( it->second.color );
			//	//pRect->setFillColor( it->second.fillColor );
			//	//pRect->enableFillColor( it->second.bFillColor );
			//	it->second.pSprite = (CCSprite*)pRect;
			//}
			//else
			{
				//if( it->second.rect.equals( cocos2d::CCRect() ) )
				//{
				//	it->second.pSprite = cocos2d::CCSprite::create( it->second.strObjName.c_str() );
				//}
				//else
				//{
				//	it->second.pSprite = cocos2d::CCSprite::create( it->second.strObjName.c_str(), it->second.rect );
				//}

				it->second.pRenderObj = (CRenderObj*)pRenderrSys->createRenderObj( it->second.strObjName.c_str() );

				//if( it->second.pRenderObj )
				//{
				//	it->second.pRenderObj->retain();
				//}
			}
		}

		//CCAssert( it->second.pSprite != NULL, "精灵对象为空" );
		if( it->second.pRenderObj )
		{
			CCPoint ptLoc = ( it->second.ptPos - m_ptView ) + ptCenter;
			if( it->second.pRenderObj->getParent() == NULL )
			{
				CSceneLayer* pLayer = getSceneLayer(it->second.nLayerID );
				if( pLayer )
				{
					pLayer->addChild( it->second.pRenderObj, it->second.nTag, it->second.nTag );
					it->second.pRenderObj->setScale( it->second.fScale );
					it->second.pRenderObj->setColor( ccc3( it->second.color.r, it->second.color.g, it->second.color.b ) );
					it->second.pRenderObj->setOpacity( it->second.byAlpha );
					it->second.dwTime = 0;
				}
			}

			//if( it->second.uid == 57 )
			//{
			//	CCLOG( "Player Move:%f,%f", ptLoc.x, ptLoc.y );
			//}

			it->second.pRenderObj->setPosition( ptLoc );
			it->second.pRenderObj->setZOrder( -ptLoc.y );
		}
	}
}
//------------------------------------------------------------------------------
/**
@brief 筛选地表块
@param
*/
//void CRenderMap::cullTerrainBlock()
//{
//m_vecVisibleTerrainBlock.clear();

///// 筛选地表
//int nViewLeft = -VIEW_CULL_WIDTH;
//int nViewRight = VIEW_CULL_WIDTH;
//int nViewTop = VIEW_CULL_HEIGHT;
//int nViewBottom = -VIEW_CULL_HEIGHT;

//cocos2d::CCRect cullRect( nViewLeft, nViewBottom, nViewRight - nViewLeft, nViewTop - nViewBottom );

//int nRenderWidth = (int)(0.5f+m_nTerrainBlockWidth * m_fScale);
//int nRenderHeight = (int)(0.5f+m_nTerrainBlockHeight * m_fScale);

//cocos2d::CCPoint ptView;
//ptView.x = m_ptView.x * m_fScale;
//ptView.y = m_ptView.y * m_fScale;

//int nTerrainBlockNum = m_nTerrainWidth * m_nTerrainHeight;
//for( int i = 0; i < nTerrainBlockNum; ++i )
//{
//	cocos2d::CCRect blockRect( m_vecTerrainBlock[i].nXIndex * nRenderWidth - ptView.x,
//		m_vecTerrainBlock[i].nYIndex * nRenderHeight  - ptView.y, nRenderWidth, nRenderHeight );

//	if( cullRect.intersectsRect( blockRect ) )
//	{
//		m_vecVisibleTerrainBlock.push_back( i );
//	}
//}

/// 场景层筛选地表物件
//	std::map< int, CSceneLayer* >::iterator iter = m_mapSceneLayer.begin();
//	for( ; iter != m_mapSceneLayer.end(); ++iter )
//	{
//		if( iter->second )
//		{
//			iter->second->updateTerrainObj( m_ptView, m_fScale, m_nTerrainBlockWidth, m_nTerrainBlockHeight );
//		}
//	}
//}
//------------------------------------------------------------------------------
/**
@brief 碰撞检测
@param
*/
//void CRenderMap::hitTestTerrainBlock( const cocos2d::CCPoint& pt, int& nXIndex, int& nYIndex )
//{
//	cocos2d::CCPoint ptView;
//	ptView.x = m_ptView.x * m_fScale;
//	ptView.y = m_ptView.y * m_fScale;
//
//	int nRenderWidth = (int)(0.5f+m_nTerrainBlockWidth * m_fScale);
//	int nRenderHeight = (int)(0.5f+m_nTerrainBlockHeight * m_fScale);
//
//	std::size_t size = m_vecTerrainBlock.size();
//	for( size_t i = 0; i < size; ++i )
//	{
//		cocos2d::CCRect blockRect( m_vecTerrainBlock[i].nXIndex * nRenderWidth - ptView.x,
//			m_vecTerrainBlock[i].nYIndex * nRenderHeight  - ptView.y, nRenderWidth, nRenderHeight );
//
//		if( blockRect.containsPoint( pt ) )
//		{
//			nXIndex = m_vecTerrainBlock[i].nXIndex;
//			nYIndex = m_vecTerrainBlock[i].nYIndex;
//
//			CSceneLayer* pLayer = (CSceneLayer*)getChildByTag( BLOCK_GRID );
//			if( pLayer )
//			{
//				CRectangleObj* pObj = (CRectangleObj*)pLayer->getChildByTag( BLOCK_GRID_SECLECTED );
//				if( pObj )
//				{
//					//pObj->release();
//					//pLayer->removeChildByTag( BLOCK_GRID_SECLECTED );
//					//pObj->setPosition( ccp(nXIndex * m_nTerrainBlockWidth, nYIndex * m_nTerrainBlockHeight) );
//					pLayer->setObjPos( m_uSelectedBlockID, ccp(nXIndex * m_nTerrainBlockWidth, nYIndex * m_nTerrainBlockHeight) );
//				}
//				else
//				{
//					cocos2d::CCRect rc( m_vecTerrainBlock[i].nXIndex * m_nTerrainBlockWidth,
//						m_vecTerrainBlock[i].nYIndex * m_nTerrainBlockHeight, m_nTerrainBlockWidth, m_nTerrainBlockHeight );
//					m_uSelectedBlockID = pLayer->addRectangleObj( rc, cocos2d::ccc4(255,0,0,255), BLOCK_GRID_SECLECTED );
//				}
//			}
//
//			m_nSelectedIndex = i;
//
//			return;
//		}
//	}
//}
//------------------------------------------------------------------------------
/**
@brief 碰撞检测（物件）
@param
*/
CRenderObj* CRenderMap::hitTestObj( const CCPoint& pt )
{
	std::map< int, std::list<CRenderObj*> > objMap;

	//m_setRenderObject
	/// 只在渲染对象集里面检测
	std::set<uint32>::iterator itObj = m_setRenderObject.begin();
	for( ; itObj != m_setRenderObject.end(); ++itObj )
	{
		TerrainObjMap::iterator it = m_mapTerrainObj.find( (*itObj) );
		if( it == m_mapTerrainObj.end() )
		{
			continue;
		}

		if( it->second.pRenderObj == NULL )
		{
			continue;
		}

		if( it->second.nLayerID != TERRAIN_OBJ_LAYER )
		{
			continue;
		}

		//cocos2d::CCRect rc = it->second.rect;
		//if( rc.containsPoint( pt ) )
		//{
		//	objMap[it->second.nLayerID].push_back( it->second.pRenderObj );
		//}

		if( it->second.pRenderObj == NULL )
		{
			continue;
		}

		if( it->second.pRenderObj->hitTest(pt) )
		{
			objMap[it->second.nLayerID].push_back( it->second.pRenderObj );
		}
	}

	if( objMap.empty() )
	{
		return NULL;
	}

	std::map< int, std::list<CRenderObj*> >::reverse_iterator itObjMap = objMap.rbegin();
	std::list<CRenderObj*>& objList = itObjMap->second;

	if( objList.empty() )
	{
		return NULL;
	}

	/// 查找最顶层的一个
	int nZOrder = 0x80000000; /// 最大的负数
	std::list<CRenderObj*>::iterator itObjSel = objList.begin();
	for( ; itObjSel != objList.end(); )
	{
		CRenderObj* pObj = (*itObjSel);

		if( pObj== NULL )
		{
			continue;
		}

		if( pObj->getZOrder() > nZOrder )
		{
			nZOrder = pObj->getZOrder();
			++itObjSel;
		}
		else
		{
			itObjSel = objList.erase( itObjSel );
		}
	}

	return (*objList.begin());
}
//// 渲染对象排序
inline bool RenderObjZOrderCmp( const CRenderObj* lhs, const CRenderObj* rhs )
{
	if( lhs == NULL || rhs == NULL )
	{
		return false;
	}

	return lhs->getZOrder() > rhs->getZOrder();
}
//-------------------------------------------------------------------------
/**
@brief 碰撞检测（物件）
@param
*/
void CRenderMap::hitTestObj( const CCPoint& pt, std::list<IRenderObj*>& ObjList )
{
	std::map< int, std::list<CRenderObj*> > objMap;

	//m_setRenderObject
	/// 只在渲染对象集里面检测
	std::set<uint32>::iterator itObj = m_setRenderObject.begin();
	for( ; itObj != m_setRenderObject.end(); ++itObj )
	{
		TerrainObjMap::iterator it = m_mapTerrainObj.find( (*itObj) );
		if( it == m_mapTerrainObj.end() )
		{
			continue;
		}

		if( it->second.pRenderObj == NULL )
		{
			continue;
		}

		if( it->second.nLayerID != TERRAIN_OBJ_LAYER )
		{
			continue;
		}

		if( it->second.pRenderObj == NULL )
		{
			continue;
		}

		if( it->second.pRenderObj->hitTest(pt) )
		{
			objMap[it->second.nLayerID].push_back( it->second.pRenderObj );
		}
	}

	if( objMap.empty() )
	{
		return;
	}

	std::map< int, std::list<CRenderObj*> >::reverse_iterator itObjMap = objMap.rbegin();
	std::list<CRenderObj*>& objList = itObjMap->second;

	if( objList.empty() )
	{
		return;
	}

	/// 按Z顺序排列
	objList.sort(RenderObjZOrderCmp);

	std::list<CRenderObj*>::iterator itObjSel = objList.begin();
	for( ; itObjSel != objList.end(); ++itObjSel )
	{
		CRenderObj* pObj = (*itObjSel);

		if( pObj== NULL )
		{
			continue;
		}

		ObjList.push_back( (IRenderObj*)pObj );
	}
}
//-------------------------------------------------------------------------
/**
@brief 是否是遮挡层
@param
*/
bool CRenderMap::isMask( const CCPoint& pt )
{
	return m_TerrainMask.isMask( pt );
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
@brief 设置对象位置
@param
*/
void CRenderMap::setObjPos( uint32 uid, const CCPoint& ptLoc )
{
	RenderObjToSceneMap::iterator iter = m_mapRenderObjToScene.find( uid );
	if( iter != m_mapRenderObjToScene.end() )
	{
		TerrainObjMap::iterator it = m_mapTerrainObj.find( iter->second );
		if( it != m_mapTerrainObj.end() )
		{
			it->second.ptPos = ptLoc;
		}
	}

	m_bUpdateTerrainObj = true;
}
////------------------------------------------------------------------------------
///**
//@brief 设置对象位置
//@param
//*/
//void CRenderMap::setObjOpacity( uint32 uid, BYTE bOpacity )
//{
//	TerrainObjMap::iterator iter = m_mapTerrainObj.find( uid );
//	if( iter != m_mapTerrainObj.end() )
//	{
//		if( iter->second.pRenderObj )
//		{
//			iter->second.pRenderObj->setOpacity( bOpacity );
//		}
//		iter->second.byAlpha = bOpacity;
//	}
//
//	m_bUpdateTerrainObj = true;
//}
////------------------------------------------------------------------------------
///**
//@brief 设置对象位置
//@param
//*/
//void CRenderMap::setObjColor( uint32 uid, cocos2d::ccColor4B& color )
//{
//	TerrainObjMap::iterator iter = m_mapTerrainObj.find( uid );
//	if( iter != m_mapTerrainObj.end() )
//	{
//		if( iter->second.pRenderObj )
//		{
//			iter->second.pRenderObj->setColor( cocos2d::ccc3(color.r,color.g,color.b) );
//		}
//		iter->second.color = color;
//	}
//
//	m_bUpdateTerrainObj = true;
//}
////------------------------------------------------------------------------------
///**
//@brief 设置对象的缩放
//@param
//*/
//void CRenderMap::setObjScale( uint32 uid, float fScale )
//{
//	TerrainObjMap::iterator iter = m_mapTerrainObj.find( uid );
//	if( iter != m_mapTerrainObj.end() )
//	{
//		if( iter->second.pRenderObj )
//		{
//			iter->second.pRenderObj->setScale( fScale );
//		}
//
//		iter->second.fScale = fScale;
//	}
//
//	m_bUpdateTerrainObj = true;
//}
//------------------------------------------------------------------------------
/**
@brief 设置对象位置
@param
*/
void CRenderMap::setTerrainGridObjColor( cocos2d::ccColor4B& color )
{
	//TerrainObjMap::iterator iter = m_mapTerrainObj.begin();
	//for( ; iter != m_mapTerrainObj.end(); ++iter )
	//{
	//	if( iter->second.bRectObj && iter->second.nLayerID == TERRAIN_GRID &&  iter->second.nTag != BLOCK_GRID_SECLECTED )
	//	{
	//		iter->second.color = color;
	//		if( iter->second.pSprite )
	//		{
	//			((CRectangleObj*)iter->second.pSprite)->setBorderColor( color );
	//		}
	//	}
	//}

	//m_bUpdateTerrainObj = true;
}
//------------------------------------------------------------------------------
/**
@brief 删除对象
@param
*/
void CRenderMap::removeObj( uint32 uid )
{
	IRenderSystem* pRenderrSys = getRenderSystem();
	if( pRenderrSys == NULL )
	{
		return;
	}

	TerrainObjMap::iterator iter = m_mapTerrainObj.find( uid );
	if( iter != m_mapTerrainObj.end() )
	{
		if( iter->second.pRenderObj )
		{
			CSceneLayer* pLayer = getSceneLayer( iter->second.nLayerID );
			if( pLayer )
			{
				pLayer->removeChild( iter->second.pRenderObj );
			}
			
			pRenderrSys->removeRenderObj( iter->second.pRenderObj );
			iter->second.pRenderObj = NULL;
		}

		m_mapTerrainObj.erase( iter );
	}

	m_setRenderObject.erase( uid );

	//m_setNoCullObj.erase( uid );
}
//------------------------------------------------------------------------------
/**
@brief 添加阻挡
@param x,y  tile格坐标
@param nBlockValue 阻挡值
*/
void CRenderMap::addBlock( int x, int y, int nBlockValue )
{
	if( x < 0 || x >= m_nTerrainTileWidth || y < 0 || y >= m_nTerrainTileHeight )
	{
		return;
	}

	m_TerrainBlock.addBlock( x * TERRAIN_TILE_WIDTH + TERRAIN_TILE_WIDTH /2, y * TERRAIN_TILE_HEIGHT + TERRAIN_TILE_HEIGHT/2, nBlockValue );
}
//------------------------------------------------------------------------------
/**
@brief 设置地表高度
@param
*/
//void CRenderMap::setTerrainHeight( int x, int y, short nHeight )
//{
//	if( x < 0 || x >= m_nTerrainTileWidth || y < 0 || y >= m_nTerrainTileHeight )
//	{
//		return;
//	}
//
//	m_TerrainBlock.setTerrainHeight( x * TERRAIN_TILE_WIDTH + TERRAIN_TILE_WIDTH /2, y * TERRAIN_TILE_HEIGHT + TERRAIN_TILE_HEIGHT/2, nHeight );
//}
//------------------------------------------------------------------------------
/**
@brief 删除阻挡
@param
*/
void CRenderMap::removeBlock( int x, int y, int nBlockValue )
{
	if( x < 0 || x >= m_nTerrainTileWidth || y < 0 || y >= m_nTerrainTileHeight )
	{
		return;
	}

	m_TerrainBlock.removeBlock( x * TERRAIN_TILE_WIDTH + TERRAIN_TILE_WIDTH /2, y * TERRAIN_TILE_HEIGHT + TERRAIN_TILE_HEIGHT/2, nBlockValue );
}
//------------------------------------------------------------------------------
uint32 CRenderMap::addTerrainObj( int nLayerID, const std::string& strTerrainObjName, cocos2d::CCRect& rc, cocos2d::CCPoint& ptPos, bool bCullObj )
{
	STerrainObj obj;
	//obj.pSprite = NULL;
	obj.ptPos = ptPos;
	obj.strObjName = strTerrainObjName;
	obj.rect = rc;
	obj.nLayerID = nLayerID;

	if( ++m_uSeed == 0 )
	{
		++m_uSeed;
	}

	obj.dwTime = WareTime::getTickCount();
	obj.uid = m_uSeed;

	m_mapTerrainObj[m_uSeed] = obj;

	//if( !bCullObj )
	//{
	//	m_setNoCullObj.insert( m_uSeed );
	//}

	int nCurrentGridX = (int)(obj.ptPos.x / TERRAIN_GRID_WIDTH);
	int nCurrentGridY = (int)(obj.ptPos.y / TERRAIN_GRID_HEIGHT);
	int id = nCurrentGridY * m_nTerrainGridWidth + nCurrentGridX;
	TerrainGridMap::iterator iter = m_mapTerrainGrid.find( id );
	if( iter != m_mapTerrainGrid.end() )
	{
		if( iter->second )
		{
			iter->second->addTerrainObj( m_uSeed );
		}
	}

	m_bUpdateTerrainObj = true;

	return obj.uid;
}
//------------------------------------------------------------------------------
/**
@brief 添加矩形对象
@param
*/
uint32 CRenderMap::addRectangleObj( int nLayerID, cocos2d::CCRect& rc, cocos2d::ccColor4B& color, int nTag, cocos2d::ccColor4B fillColor, bool bFillColor )
{
	STerrainObj obj(true);
	//obj.pSprite = NULL;
	obj.ptPos = ccp(rc.getMinX(),rc.getMinY());
	obj.strObjName = "";
	obj.rect = rc;
	obj.color = color;
	obj.nTag = nTag;
	obj.nLayerID = nLayerID;
	obj.fillColor = fillColor;
	obj.bFillColor = bFillColor;
	obj.dwTime = WareTime::getTickCount();

	//CSceneLayer* pLayer = getSceneLayer( nLayerID );
	//if( pLayer )
	//{
	//	//CRenderObj* pRenderObj = CRenderObjManager::Instance().createRenderObj();
	//	pLayer->addChild( pRenderObj, obj.nTag, obj.nTag );
	//	pRenderObj->retain();
	//	pRenderObj->setScale( obj.fScale );
	//	pRenderObj->setColor( ccc3( obj.color.r, obj.color.g, obj.color.b ) );
	//	pRenderObj->setOpacity( obj.byAlpha );
	//	obj.fTime = 0;//WareTime::getTickCount();
	//}

	if( ++m_uSeed == 0 )
	{
		++m_uSeed;
	}

	obj.uid = m_uSeed;

	m_mapTerrainObj[m_uSeed] = obj;

	int nCurrentGridX = obj.ptPos.x / TERRAIN_GRID_WIDTH;
	int nCurrentGridY = obj.ptPos.y / TERRAIN_GRID_HEIGHT;
	int id = nCurrentGridY * m_nTerrainGridWidth + nCurrentGridX;
	TerrainGridMap::iterator iter = m_mapTerrainGrid.find( id );
	if( iter != m_mapTerrainGrid.end() )
	{
		if( iter->second )
		{
			iter->second->addTerrainObj( m_uSeed );
		}
	}
	//else
	//{
	//	OutputDebugString( "addRectangleObj Failed!" );
	//}

	m_bUpdateTerrainObj = true;

	return obj.uid;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
/**
@brief 添加图片层
@param nLayerID 层ID
@param vecImageFileName jpg文件列表
*/
void CRenderMap::addImageLayer( int nLayerID, std::vector<std::string>& vecImageFileName )
{
	if( NULL != getChildByTag( nLayerID ) )
	{
		removeChildByTag( nLayerID );

		ImageLayerMap::iterator iter = m_mapImageLayer.find(nLayerID);
		if( iter != m_mapImageLayer.end() )
		{
			m_mapImageLayer.erase( nLayerID );
		}
	}

	CImageLayer* pImageLayer = CImageLayer::create(vecImageFileName);
	addChild( pImageLayer, nLayerID, nLayerID );
}
//------------------------------------------------------------------------------
/**
@brief 添加矩形对象
@param
*/
uint32 CRenderMap::addRenderObj( int nLayerID, CRenderObj* pRenderObj )
{
	if( pRenderObj == NULL )
	{
		return 0;
	}

	if( pRenderObj->isInScene() )
	{
		return 0;
	}

	STerrainObj obj;
	//obj.pSprite = NULL;
	obj.ptPos = ccp(0,0);
	obj.strObjName = pRenderObj->getFileName();
	//obj.rect = rc;
	obj.color = ccc4(255,255,255,255);
	obj.nTag = 0;
	obj.nLayerID = nLayerID;
	obj.fillColor = ccc4(255,255,255,255);
	obj.bFillColor = false;
	obj.dwTime = WareTime::getTickCount();
	obj.pRenderObj = pRenderObj;
	obj.nLayerID = nLayerID;

	CSceneLayer* pLayer = getSceneLayer( nLayerID );
	if( pLayer )
	{
		pLayer->addChild( pRenderObj, obj.nTag, obj.nTag );
		//pRenderObj->retain();
		pRenderObj->setScale( obj.fScale );
		pRenderObj->setColor( ccc3( obj.color.r, obj.color.g, obj.color.b ) );
		pRenderObj->setOpacity( obj.byAlpha );
		pRenderObj->setInScene( true );
		obj.dwTime = 0;//WareTime::getTickCount();
	}

	if( ++m_uSeed == 0 )
	{
		++m_uSeed;
	}

	obj.uid = m_uSeed;

	m_mapTerrainObj[obj.uid] = obj;
	m_mapRenderObjToScene[pRenderObj->getID()] = obj.uid;
	m_setRenderObject.insert( obj.uid );

	/// 设置下位置
	pRenderObj->setPos( pRenderObj->getPosition() );

	m_bUpdateTerrainObj = true;

	return obj.uid;
}
//------------------------------------------------------------------------------
/**
@brief 读取场景文件
@param
*/
bool CRenderMap::readSceneFileVer( CWareFileRead& File, int nVersion )
{
	/// 地图信息
	File.read ( m_nTerrainWidth );
	File.read( m_nTerrainHeight );
	File.read( m_nTerrainChunkWidth );
	File.read( m_nTerrainChunkHeight );

	/// 地表块网格设置
	setTerrainInfo( m_nTerrainWidth, m_nTerrainHeight, m_nTerrainChunkWidth, m_nTerrainChunkHeight );

	if( nVersion > SCENE_FILE_VER_1 ) /// 10001 增加地图类型
	{
		int nType = 0;
		File.read( nType );

		switch( nType )
		{
		case eTerrainType_Tile:
			{
				if( nVersion > SCENE_FILE_VER_2 ) /// 10002 增加了视差数据
				{
					// 读取视差数据
					readParallax(File);
				}

				readTerrainChunck( File );
				readTerrainObject( File );
				break;
			}
		case eTerrainType_Brush:
			{
				break;
			}
		default:
			{
				break;
			}
		}

	}
	else
	{
		readTerrainChunck( File );
		readTerrainObject( File );
	}

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 读取地表块数据
@param 
*/
void CRenderMap::readTerrainChunck( CWareFileRead& File )
{
	std::size_t size = 0;
	File.read( size );

	for( size_t i = 0; i < size; ++i )
	{
		STerrainChunk block;
		File.read( block.nXIndex );
		File.read( block.nYIndex );
		File.readStr( block.strTexture );

		float x,y,w,h;
		File.read( x );
		File.read( y );
		File.read( w );
		File.read( h );
		block.rect.setRect( x, y, w, h );

		if( !block.strTexture.empty() )
		{
			std::string strTextPathName = "";
			//strTextPathName.append( "/" );
			strTextPathName.append( block.strTexture );
			setTerrainChunk( block.nXIndex, block.nYIndex, strTextPathName, block.rect );
		}
	}
}
//-------------------------------------------------------------------------
/**
@brief 读取地表对象
@param 
*/
void CRenderMap::readTerrainObject( CWareFileRead& File )
{
	std::size_t size = 0;
	File.read( size ); /// 物件层数

	for( size_t i = 0; i < size; ++i )
	{
		//if( itLayer->first != BLOCK_GRID )
		//{
		int nLayerID = 0;
		File.read( nLayerID );

		CSceneLayer* pSceneLayer = CSceneLayer::create();
		if( pSceneLayer )
		{
			addChild( pSceneLayer, nLayerID, nLayerID );
			m_mapSceneLayer[nLayerID] = pSceneLayer;

			loadObjFromFile( nLayerID, File );
		}
	}
}
//-------------------------------------------------------------------------
/**
@brief 读取视差数据
@param 
*/
void CRenderMap::readParallax( CWareFileRead& File )
{
	File.read( m_bParallax );
	File.read( m_fParallexFactor[0] );
	File.read( m_fParallexFactor[1] );

	int nImageLayerSize = 0;
	File.read( nImageLayerSize );

	for( int i = 0; i < nImageLayerSize; ++i )
	{
		SImageLayer sImageLayer;
		File.read( sImageLayer.nImageLayerID );

		int nImageSize = 0;
		File.read( nImageSize );

		for( int j = 0; j < nImageSize; ++j )
		{
			std::string strImageName;
			File.readStr( strImageName );
			sImageLayer.m_vecImageList.push_back( strImageName );
		}

		m_mapImageLayer[sImageLayer.nImageLayerID] = sImageLayer;

		addImageLayer( sImageLayer.nImageLayerID, sImageLayer.m_vecImageList );
	}
}
//------------------------------------------------------------------------------
/**
@brief 获取场景层
@param
*/
CSceneLayer* CRenderMap::getSceneLayer( int id )
{
	SceneLayerMap::iterator itLayer = m_mapSceneLayer.find( id );
	if( itLayer == m_mapSceneLayer.end() )
	{
		return NULL;
	}

	return itLayer->second;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
@brief 加载物件
@param
*/
void CRenderMap::loadObjFromFile( int nLayerID, CWareFileRead& File )
{
	std::size_t size;
	File.read( size );

	for( size_t i = 0; i < size; ++i )
	{
		STerrainObj obj;

		File.readStr( obj.strObjName );

		float x,y,w,h;
		File.read( x );
		File.read( y );
		File.read( w );
		File.read( h );
		obj.rect.setRect( x,y,w,h );

		File.read( obj.ptPos );
		File.read( obj.fScale );
		File.read( obj.fAngle );
		File.read( obj.color.r );
		File.read( obj.color.g );
		File.read( obj.color.b );
		File.read( obj.color.r );

		addTerrainObj( nLayerID, obj.strObjName, obj.rect, obj.ptPos );
	}
}

NS_CC_ENGINE_END
