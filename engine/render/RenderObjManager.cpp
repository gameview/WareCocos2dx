#include "RenderObjManager.h"
#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "RenderObj.h"
#include "RenderObjModel.h"

NS_CC_ENGINE_BEGIN

CRenderObjManager::CRenderObjManager(void)
{
	m_mapObjModel.clear();
}
//------------------------------------------------------------------------------
CRenderObjManager::~CRenderObjManager(void)
{
}
//------------------------------------------------------------------------------
/**
@brief 创建对象
@param
*/
CRenderObj* CRenderObjManager::createRenderObj( const char* szObjFileName )
{
	if( szObjFileName == NULL )
	{
		return NULL;
	}

	static uint32 s_uGmaeObjSerial = 0;
	if( ++s_uGmaeObjSerial == 0 )
	{
		++s_uGmaeObjSerial;
	}

	CRenderObj* pRenderObj = getRenderObj( s_uGmaeObjSerial );
	while( pRenderObj != NULL )
	{
		/// 出错Log
		++s_uGmaeObjSerial;
		pRenderObj = getRenderObj( s_uGmaeObjSerial );
	}

	std::string strPathName,strFileName,strExtName;
	StringUtility::parseFileName( szObjFileName, strPathName, strFileName, strExtName );
	if( strExtName != "obj" )
	{
		pRenderObj = new CRenderObj( s_uGmaeObjSerial );
		if( pRenderObj == NULL )
		{
			/// 出错Log 便于调试和测试
			CCLOG("创建Gameobj失败！");
			return NULL;
		}

		pRenderObj->create( szObjFileName );
	}
	else
	{
		RenderObjModelMap::iterator itObjModel = m_mapObjModel.find( szObjFileName );
		if( itObjModel == m_mapObjModel.end() )
		{
			CRenderObjModel* pRenderModel = new CRenderObjModel();
			if( pRenderModel == NULL )
			{
				return NULL;
			}

			std::string strObjName = szObjFileName;
			if( !pRenderModel->parseObjFile( strObjName.c_str() ) )
			{
				delete pRenderModel;
				CCLOG( "找不到%s，创建RenderObj失败！", strObjName.c_str() );
				itObjModel = m_mapObjModel.find( "Npc/0.obj" );
				if( itObjModel == m_mapObjModel.end() )
				{
					CCLOG( "找不到0.obj，创建RenderObj失败！" );
					return NULL;
				}
			}
			else
			{
				m_mapObjModel[strObjName] = pRenderModel;
				itObjModel = m_mapObjModel.find( strObjName );
			}
		}

		pRenderObj = new CRenderObj( s_uGmaeObjSerial );
		if( pRenderObj == NULL )
		{
			/// 出错Log 便于调试和测试
			CCLOG("创建Gameobj失败！");
			return NULL;
		}

		pRenderObj->setObjModel( itObjModel->second );
		pRenderObj->create( szObjFileName );
	}

	m_mapRenderObj[s_uGmaeObjSerial] = pRenderObj;

	return pRenderObj;
}
//------------------------------------------------------------------------------
/**
@brief 获取游戏对象
@param
*/
CRenderObj* CRenderObjManager::getRenderObj( uint32 uID )
{
	RenderObjMap::iterator iter = m_mapRenderObj.find( uID );
	if( iter == m_mapRenderObj.end() )
	{
		return NULL;
	}

	return iter->second;
}
//------------------------------------------------------------------------------
/**
@brief 删除对象
@param
*/
void CRenderObjManager::removeRenderObj( uint32 uID )
{
	RenderObjMap::iterator iter = m_mapRenderObj.find( uID );
	if( iter == m_mapRenderObj.end() )
	{
		printf( "Not find GameObj: %d\n", uID );
		return;
	}

	CRenderObj* pObj = iter->second;
	if( pObj == NULL )
	{
		return;
	}

	if( pObj->isSingleReference() )
	{
		m_mapRenderObj.erase( iter );
	}

	pObj->release();
}
//-------------------------------------------------------------------------
/**
@brief 释放没有使用的对象数据
@param
*/
void CRenderObjManager::releaseObjModel( bool bFlag )
{
	RenderObjModelMap::iterator itModel = m_mapObjModel.begin();
    RenderObjModelMap::iterator itModel2;
	for( ; itModel != m_mapObjModel.end(); )
	{
        itModel2  = itModel++;
		if( itModel2->second && itModel2->second->isSingleReference())
		{
			itModel2->second->release();
			m_mapObjModel.erase( itModel2 );
		} 
	}

	if( !bFlag && !m_mapObjModel.empty() )
	{
		CCLOG( "CRenderObjManager::releaseObjModel: RenderObj already exist memory!!!!" );
	}
}
////-------------------------------------------------------------------------
///**
//@brief 获取游戏对象列表
//@param
//*/
//void CRenderObjManager::getGameObjListAll( std::list<CRenderObj*>& lstGameObj )
//{
//	GameObjMap::iterator iter =	m_mapGameObj.begin();
//	for( ; iter != m_mapGameObj.end(); ++iter )
//	{
//		lstGameObj.push_back( iter->second );
//	}	
//}
//------------------------------------------------------------------------------
/**
@brief 销毁对象
@param
*/
//void CRenderObjManager::destroyAllRenderObj()
//{
//	if( !m_mapRenderObj.empty() )
//	{
//		//printf( "" );
//	}
//
//	RenderObjIdleMap::iterator iter =	m_mapIdleRenderObj.begin();
//	for( ; iter != m_mapIdleRenderObj.end(); ++iter )
//	{
//		/// 需要重新指定ID
//		RenderObjList& glst = iter->second;
//		RenderObjList::iterator itObj = glst.begin();
//		for( ; itObj != glst.end(); ++itObj )
//		{
//			if( (*itObj) == NULL )
//			{
//				continue;
//			}
//
//			(*itObj)->release();
//		}
//
//		glst.clear();
//	}
//
//	m_mapIdleRenderObj.clear();
//}

NS_CC_ENGINE_END
