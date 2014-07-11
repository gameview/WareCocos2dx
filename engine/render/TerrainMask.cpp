//#include "StdAfx.h"
#include "TerrainMask.h"
#include "RenderMap.h"

NS_CC_ENGINE_BEGIN

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((uint32)(BYTE)(ch0) | ((uint32)(BYTE)(ch1) << 8) |   \
	((uint32)(BYTE)(ch2) << 16) | ((uint32)(BYTE)(ch3) << 24 ))
#endif


CTerrainMask::CTerrainMask()
{
}


CTerrainMask::~CTerrainMask(void)
{
	m_mapMask.clear();
}
//------------------------------------------------------------------------------
/**
@brief 添加阻挡
@param
*/
void CTerrainMask::addMask( int x, int y, int nMaskValue )
{

	tPosition pos;
	pos.x = x;
	pos.y = y;

	//std::string strObjName;
	//switch( nBlockValue )
	//{
	//case eTerrainBlock_Walk_Forbid: /// 行走阻挡
	//	{
	//		strObjName = "red.jpg";
	//		break;
	//	}
	//case eTerrainBlock_Fly_Forbid: /// 飞行阻挡
	//	{
	//		strObjName = "blue.jpg";
	//		break;
	//	}
	//default: /// 地表高度刷
	//	{
	//		strObjName = "blue.jpg";
	//		break;
	//	}
	//}


	MaskMap::iterator iter = m_mapMask.find( pos );
	if( iter != m_mapMask.end() )
	{
		if( (iter->second.nBlockValue & nMaskValue) != 0 )
		{
			return;
		}

		iter->second.nBlockValue = nMaskValue;

		//m_pGameScene->removeObj( iter->second.uid );

		//cocos2d::CCRect rc;
		//iter->second.uid = m_pGameScene->addTerrainObj( TERRAIN_BLOCK, strObjName, rc, ccp( x , y ) );
		////if( iter->second.)
		//m_pGameScene->setObjOpacity( iter->second.uid, 128 );

		//cocos2d::CCRect rc;
		//rc.setRect( x - TERRAIN_TILE_WIDTH/2, y - TERRAIN_TILE_HEIGHT/2, TERRAIN_TILE_WIDTH, TERRAIN_TILE_HEIGHT );
		//m_pMetaScene->addRectangleObj( TERRAIN_BLOCK_GRID, rc, cocos2d::ccc4( 255,255,0,255) );
	}
	else
	{
		SBlockInfo blockInfo;
		blockInfo.pos = pos;
		blockInfo.nBlockValue = nMaskValue;
		
		//cocos2d::CCRect rc;
		//blockInfo.uid = m_pGameScene->addTerrainObj( TERRAIN_BLOCK, strObjName, rc, ccp( x , y ) );

		//m_pMetaScene->setObjOpacity( blockInfo.uid, 128 );

		m_mapMask[pos] = blockInfo;

		//cocos2d::CCRect rc;
		//rc.setRect( x - TERRAIN_BLOCK_WIDTH/2, y - TERRAIN_BLOCK_HEIGHT/2, TERRAIN_BLOCK_WIDTH, TERRAIN_BLOCK_HEIGHT );
		//m_pMetaScene->addRectangleObj( TERRAIN_BLOCK_GRID, rc, cocos2d::ccc4( 255,255,0,255) );
	}
}
//------------------------------------------------------------------------------
/**
@brief 删除阻挡
@param
*/
//void CTerrainMask::removeMask( int x, int y, int nMaskValue )
//{
	//if( m_pGameScene == NULL )
	//{
	//	return;
	//}

	//tPosition pos;
	//pos.x = x;
	//pos.y = y;

	//if( nBlockValue == 0 )
	//{
	//	BlockMap::iterator iter = m_mapBlock.find( pos );
	//	if( iter == m_mapBlock.end() )
	//	{
	//		return;
	//	}

	//	m_pGameScene->removeObj( iter->second.uid );
	//	//m_pMetaScene->removeObj( iter->second.uidGrid );

	//	m_mapBlock.erase( iter );
	//}
	//else
	//{
	//	BlockMap::iterator iter = m_mapBlock.find( pos );
	//	if( iter != m_mapBlock.end() )
	//	{
	//		iter->second.nBlockValue &= ~nBlockValue; /// 擦除标志
	//		if( iter->second.nBlockValue == 0 )
	//		{
	//			m_pGameScene->removeObj( iter->second.uid );
	//			//m_pMetaScene->removeObj( iter->second.uidGrid );
	//			iter->second.uid = 0;
	//			iter->second.uidGrid = 0;
	//			m_mapBlock.erase( iter );
	//		}
	//		else
	//		{
	//			m_pGameScene->removeObj( iter->second.uid );
	//			iter->second.uid = 0;
	//		}
	//	}
	//}
//}
//------------------------------------------------------------------------------
/**
@brief 加载物件
@param
*/
void CTerrainMask::loadObjFromFile( std::string& strMaskFileName )
{
	if( strMaskFileName.empty() )
	{
		return;
	}

	//if( m_pRenderMap == NULL )
	//{
	//	return;
	//}

	unsigned long lSize = 0;
	unsigned char* pBuff = CCFileUtils::sharedFileUtils()->getFileData( strMaskFileName.c_str(), "rb", &lSize );

	cocos2d::engine::CWareFileRead File;
	CCLOG( "CTerrainMask::initWithFile %s", strMaskFileName.c_str() );
	//if( !File.open(strPath.c_str()) )
	//{
	//	return false;
	//}

	File.open( pBuff, lSize );

	int nFlag = 0;
	File.read( nFlag );
	if( nFlag != MAKEFOURCC( 'M', 'S','K', 0 ) )
	{
		CCLOG( "阻挡文件格式不对!" );
		return;
	}

	int nVersion = 10000;
	File.read( nVersion );

	int nTileX = 0;
	int nTileY = 0;
	//m_pMetaScene->getTileSize( nTileY, nTileY );
	File.read( nTileX );
	File.read( nTileY );

	int nBlockSize = 0;
	File.read( nBlockSize );

	//BlockMap::iterator iter = m_mapBlock.begin();				/// 阻挡列表
	for( int i = 0; i < nBlockSize; ++i )
	{
		SBlockInfo block;
		
		File.read( block.pos.x );
		File.read( block.pos.y );
		File.read( block.nBlockValue );

		addMask( block.pos.x, block.pos.y, block.nBlockValue );
	}

	delete [] pBuff;
	File.close();
}
//------------------------------------------------------------------------------
/**
@brief 保存物件信息
@param
*/
//void CTerrainMask::saveToFile( std::string& strBlockFileName )
//{
	//if( strBlockFileName.empty() )
	//{
	//	return;
	//}

	//if( m_pGameScene == NULL )
	//{
	//	return;
	//}

	//cocos2d::engine::CWareFileWrite File;
	//if( !File.open( strBlockFileName.c_str() ) )
	//{
	//	return;
	//}

	//int nFlag = MAKEFOURCC( 'B', 'O','C', 0 );
	//File.write( nFlag );

	//int nTileX = 0;
	//int nTileY = 0;
	//m_pGameScene->getTileSize( nTileX, nTileY );
	//File.write( nTileX );
	//File.write( nTileY );

	//int nBlockSize = static_cast<int>(m_mapBlock.size());
	//File.write( nBlockSize );
	//BlockMap::iterator iter = m_mapBlock.begin();				/// 阻挡列表
	//for( ; iter != m_mapBlock.end(); ++iter )
	//{
	//	File.write( iter->first.x );
	//	File.write( iter->first.y );
	//	File.write( iter->second.nBlockValue );
	//}

	//File.close();
//}
//-------------------------------------------------------------------------
/**
@brief 是否是遮挡位置
@param
*/
bool CTerrainMask::isMask( const CCPoint& ptLoc )
{
	tPosition pos;
	pos.x = ptLoc.x / TERRAIN_TILE_WIDTH;
	pos.y = ptLoc.y / TERRAIN_TILE_HEIGHT;

	MaskMap::iterator iter = m_mapMask.find( pos );
	if( iter != m_mapMask.end() )
	{
		if( (iter->second.nBlockValue & eTerrainBlock_Mask_Forbid) != 0 )
		{
			return true;
		}
	}

	return false;
}

NS_CC_ENGINE_END