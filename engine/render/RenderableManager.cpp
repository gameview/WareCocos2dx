#include "RenderableManager.h"
#include "SpriteObj.h"
#include "AniSpriteObj.h"
#include "DragonBoneSpriteObj.h"
#include "CEffect.h"
#include "cocoa/CCObject.h"
#include "ccMacros.h"

NS_CC_ENGINE_BEGIN

CRenderableManager::CRenderableManager(void)
{
	m_uGmaeObjSerial = 0;
}

CRenderableManager::~CRenderableManager(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 创建精灵对象
@param szPicFileName [in] 贴图文件名
@param nXPos [in] x位置
@param nYPos [in] y位置
@param rect [in] 贴图区域
@retrun 渲染对象ID
*/
ISprite* CRenderableManager::createSpriteObj( const char* szPicFileName, const CCRect& rect, CCPoint ptOffset )
{
	if( szPicFileName == NULL || szPicFileName[0] == 0 )
	{
		return NULL;
	}

	if( ++m_uGmaeObjSerial == 0 )
	{
		++m_uGmaeObjSerial;
	}

	IRenderable* pRenderable = getRenderable( m_uGmaeObjSerial );
	while( pRenderable != NULL )
	{
		if( ++m_uGmaeObjSerial == 0 )
		{
			++m_uGmaeObjSerial;
		}
		pRenderable = getRenderable( m_uGmaeObjSerial );
	}

	CSpriteObj* pSpriteObj = NULL;
	//RenderableReclaim::iterator iter = m_mapRenderableReclaim[RenderableType_Sprite].find(szPicFileName);
	//if( iter != m_mapRenderableReclaim[RenderableType_Sprite].end() )
	//{
	//	/// 需要重新指定ID
	//	if( !iter->second.empty() )
	//	{
	//		pSpriteObj = (CSpriteObj*)iter->second.front();
	//		if( pSpriteObj != NULL )
	//		{
	//			pSpriteObj->setID(m_uGmaeObjSerial);
	//			m_mapRenderable[m_uGmaeObjSerial] = pSpriteObj;
	//		}

	//		iter->second.pop_front();
	//		return pSpriteObj;
	//	}
	//}

	/// 创建新的对象
	pSpriteObj = new CSpriteObj(m_uGmaeObjSerial);
	if( pSpriteObj == NULL )
	{
		//char szBuff
		CCLOG( "create spriteobj: alloc memoey failed! %s", szPicFileName );
		return NULL;
	}

	if( !pSpriteObj->create( szPicFileName, rect, ptOffset ) )
	{
		CCLOG( "create spriteobj failed! %s", szPicFileName );
		return NULL;
	}

	//pSpriteObj->autorelease();

	m_mapRenderable[m_uGmaeObjSerial] = pSpriteObj;
	return pSpriteObj;
}
//-------------------------------------------------------------------------
/**
@brief 创建精灵对象
@param szPicFileName [in] 贴图文件名
@param nXPos [in] x位置
@param nYPos [in] y位置
@param rect [in] 贴图区域
@retrun 渲染对象ID
*/
ISprite*	CRenderableManager::createGraySpriteObj( const char* szPicFileName, const CCRect& rect, CCPoint ptOffset )
{
	if( szPicFileName == NULL || szPicFileName[0] == 0 )
	{
		return NULL;
	}

	if( ++m_uGmaeObjSerial == 0 )
	{
		++m_uGmaeObjSerial;
	}

	IRenderable* pRenderable = getRenderable( m_uGmaeObjSerial );
	while( pRenderable != NULL )
	{
		if( ++m_uGmaeObjSerial == 0 )
		{
			++m_uGmaeObjSerial;
		}
		pRenderable = getRenderable( m_uGmaeObjSerial );
	}

	CSpriteObj* pSpriteObj = NULL;
	//RenderableReclaim::iterator iter = m_mapRenderableReclaim[RenderableType_Sprite].find(szPicFileName);
	//if( iter != m_mapRenderableReclaim[RenderableType_Sprite].end() )
	//{
	//	/// 需要重新指定ID
	//	if( !iter->second.empty() )
	//	{
	//		pSpriteObj = (CSpriteObj*)iter->second.front();
	//		if( pSpriteObj != NULL )
	//		{
	//			pSpriteObj->setID(m_uGmaeObjSerial);
	//			m_mapRenderable[m_uGmaeObjSerial] = pSpriteObj;
	//		}

	//		iter->second.pop_front();
	//		return pSpriteObj;
	//	}
	//}

	/// 创建新的对象
	pSpriteObj = new CSpriteObj(m_uGmaeObjSerial);
	if( pSpriteObj == NULL )
	{
		//char szBuff
		CCLOG( "create spriteobj: alloc memoey failed! %s", szPicFileName );
		return NULL;
	}

	if( !pSpriteObj->create( szPicFileName, rect, ptOffset ) )
	{
		CCLOG( "create spriteobj failed! %s", szPicFileName );
		return NULL;
	}

	//pSpriteObj->autorelease();

	m_mapRenderable[m_uGmaeObjSerial] = pSpriteObj;
	return pSpriteObj;
}
//-------------------------------------------------------------------------
/**
@brief 创建精灵对象
@param szPicFileName [in] 贴图文件名
@param nXPos [in] x位置
@param nYPos [in] y位置
@param rect [in] 贴图区域
@retrun 渲染对象ID
*/
IAniSprite* CRenderableManager::createAniSpriteObj( const char* szAniFileName, CCPoint ptOffset )
{
	if( szAniFileName == NULL || szAniFileName[0] == 0 )
	{
		return NULL;
	}

	if( ++m_uGmaeObjSerial == 0 )
	{
		++m_uGmaeObjSerial;
	}

	IRenderable* pRenderable = getRenderable( m_uGmaeObjSerial );
	while( pRenderable != NULL )
	{
		if( ++m_uGmaeObjSerial == 0 )
		{
			++m_uGmaeObjSerial;
		}
		pRenderable = getRenderable( m_uGmaeObjSerial );
	}

	CAniSpriteObj* pAniSpriteObj = NULL;
	//RenderableReclaim::iterator iter = m_mapRenderableReclaim[RenderableType_AniSprite].find(szAniFileName);
	//if( iter != m_mapRenderableReclaim[RenderableType_AniSprite].end() )
	//{
	//	/// 需要重新指定ID
	//	if( !iter->second.empty() )
	//	{
	//		pAniSpriteObj = (CAniSpriteObj*)iter->second.front();
	//		if( pAniSpriteObj != NULL )
	//		{
	//			pAniSpriteObj->setID(m_uGmaeObjSerial);
	//			m_mapRenderable[m_uGmaeObjSerial] = pAniSpriteObj;
	//		}

	//		iter->second.pop_front();
	//		return pAniSpriteObj;
	//	}
	//}

	/// 创建新的对象
	pAniSpriteObj = new CAniSpriteObj(m_uGmaeObjSerial);
	if( pAniSpriteObj == NULL )
	{
		//char szBuff
		CCLOG( "create spriteobj: alloc memoey failed! %s", szAniFileName );
		return NULL;
	}

	if( !pAniSpriteObj->create( szAniFileName, ptOffset ) )
	{
		CCLOG( "create spriteobj failed! %s", szAniFileName );
		return NULL;
	}

	//pAniSpriteObj->autorelease();

	m_mapRenderable[m_uGmaeObjSerial] = pAniSpriteObj;
	return pAniSpriteObj;
}
//-------------------------------------------------------------------------
/**
@brief 创建精灵对象
@param szPicFileName [in] 贴图文件名
@param nXPos [in] x位置
@param nYPos [in] y位置
@param rect [in] 贴图区域
@retrun 渲染对象ID
*/
IEffect* CRenderableManager::createEffectObj( const char* szEffectFileName, CCPoint ptOffset )
{
	if( szEffectFileName == NULL || szEffectFileName[0] == 0 )
	{
		return NULL;
	}

	if( ++m_uGmaeObjSerial == 0 )
	{
		++m_uGmaeObjSerial;
	}

	IRenderable* pRenderable = getRenderable( m_uGmaeObjSerial );
	while( pRenderable != NULL )
	{
		if( ++m_uGmaeObjSerial == 0 )
		{
			++m_uGmaeObjSerial;
		}
		pRenderable = getRenderable( m_uGmaeObjSerial );
	}

	CEffect* pEffect = NULL;
	//RenderableReclaim::iterator iter = m_mapRenderableReclaim[RenderableType_AniSprite].find(szAniFileName);
	//if( iter != m_mapRenderableReclaim[RenderableType_AniSprite].end() )
	//{
	//	/// 需要重新指定ID
	//	if( !iter->second.empty() )
	//	{
	//		pAniSpriteObj = (CAniSpriteObj*)iter->second.front();
	//		if( pAniSpriteObj != NULL )
	//		{
	//			pAniSpriteObj->setID(m_uGmaeObjSerial);
	//			m_mapRenderable[m_uGmaeObjSerial] = pAniSpriteObj;
	//		}

	//		iter->second.pop_front();
	//		return pAniSpriteObj;
	//	}
	//}

	/// 创建新的对象
	pEffect = new CEffect(m_uGmaeObjSerial);
	if( pEffect == NULL )
	{
		//char szBuff
		CCLOG( "create spriteobj: alloc memoey failed! %s", szEffectFileName );
		return NULL;
	}

	if( !pEffect->create( szEffectFileName, ptOffset ) )
	{
		CCLOG( "create spriteobj failed! %s", szEffectFileName );
		return NULL;
	}

	//pAniSpriteObj->autorelease();

	m_mapRenderable[m_uGmaeObjSerial] = pEffect;
	return pEffect;
}
//-------------------------------------------------------------------------
/**
@brief 创建支持DragonBone骨骼动画对象
@param szAniFileName [in] 动画文件
@param ptOffset 偏移
*/
IDragonBoneSprite* CRenderableManager::createDragobBoneSpriteObj( const char* szAniFileName, CCPoint ptOffset )
{
	if( szAniFileName == NULL || szAniFileName[0] == 0 )
	{
		return NULL;
	}

	if( ++m_uGmaeObjSerial == 0 )
	{
		++m_uGmaeObjSerial;
	}

	IRenderable* pRenderable = getRenderable( m_uGmaeObjSerial );
	while( pRenderable != NULL )
	{
		if( ++m_uGmaeObjSerial == 0 )
		{
			++m_uGmaeObjSerial;
		}
		pRenderable = getRenderable( m_uGmaeObjSerial );
	}

	CDragonBoneSpriteObj* pAniSpriteObj = NULL;

	/// 创建新的对象
	pAniSpriteObj = new CDragonBoneSpriteObj(m_uGmaeObjSerial);
	if( pAniSpriteObj == NULL )
	{
		//char szBuff
		CCLOG( "create spriteobj: alloc memoey failed! %s", szAniFileName );
		return NULL;
	}

	if( !pAniSpriteObj->create( szAniFileName, ptOffset ) )
	{
		CCLOG( "create spriteobj failed! %s", szAniFileName );
		return NULL;
	}

	//pAniSpriteObj->autorelease();

	m_mapRenderable[m_uGmaeObjSerial] = pAniSpriteObj;
	return pAniSpriteObj;
}
//-------------------------------------------------------------------------
/**
*@brief 获取渲染对象
*@param uID 对象ID
*/
IRenderable* CRenderableManager::getRenderable( uint32 uID )
{
	RenderableMap::iterator iter = m_mapRenderable.find( uID );
	if( iter != m_mapRenderable.end() )
	{
		return iter->second;
	}

	return NULL;
}
//------------------------------------------------------------------------------
/**
@brief 删除对象
*@param uID 对象ID
*/
void CRenderableManager::removeRenderable( uint32 uID )
{
	RenderableMap::iterator iter = m_mapRenderable.find( uID );
	if( iter != m_mapRenderable.end() )
	{
		if( iter->second )
		{
			iter->second->release();
		}
	}
}

NS_CC_ENGINE_END