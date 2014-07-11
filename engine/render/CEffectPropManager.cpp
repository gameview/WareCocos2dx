#include "CEffectPropManager.h"
#include "CEffectProp.h"
#include "CEffect.h"
#include "StringUtility.h"
#include "WareTime.h"

NS_CC_ENGINE_BEGIN

CEffectPropManager::CEffectPropManager(void) : m_uIDSeed(0)
{
}

CEffectPropManager::~CEffectPropManager(void)
{
}

int32 CEffectPropManager::createEmptyEffectProp( IEffectProp** ppEffectProp, int32 nRecycleLevel )
{
	static int32 s_EffectPropIndex = 0;

	if( ++m_uIDSeed == 0 )
	{
		++m_uIDSeed;
	}

	CEffectProp* pProp = new CEffectProp(m_uIDSeed);
	if( !pProp )
	{
		*ppEffectProp = pProp;
		return 0;
	}

	char szBuff[256];
	s_EffectPropIndex++;
	sprintf( szBuff, "_s_Ware_Cocos_EffectProp_%d_%d", s_EffectPropIndex, rand() );

	SEffectPropInfo sEffectPropInfo;
	sEffectPropInfo.pEffectProp = pProp;
	sEffectPropInfo.uLastUseTime = WareTime::getTickCount();
	sEffectPropInfo.nRecycleLevel = nRecycleLevel;

	pProp->setResourceName( szBuff );

	m_mapEffectProp[szBuff] = sEffectPropInfo;

	*ppEffectProp = pProp;

	return 1;
}

int32 CEffectPropManager::createEffectProp( const char* szEffectName, IEffectProp** ppEffectProp, int32 nRecycleLevel )
{
	if( szEffectName == NULL )
	{
		return 0;
	}

	CEffectProp* pProp = NULL;
	int32 nRet = 1;

	EffectPropMap::iterator iter = m_mapEffectProp.find( szEffectName );
	if( iter == m_mapEffectProp.end() ) /// 不存在
	{
		if( ++m_uIDSeed == 0 )
		{
			++m_uIDSeed;
		}
		pProp = new CEffectProp(m_uIDSeed);

		pProp->setResourceName( szEffectName );
	}
	else
	{
		pProp = (CEffectProp*)iter->second.pEffectProp;
		pProp->retain(); /// 添加引用
	}

	if( !pProp->isValid() )
	{
		pProp->loadRes();
	}

	SEffectPropInfo sEffectPropInfo;
	sEffectPropInfo.pEffectProp = pProp;
	sEffectPropInfo.uLastUseTime = WareTime::getTickCount();
	sEffectPropInfo.nRecycleLevel = nRecycleLevel;

	m_mapEffectProp[szEffectName] = sEffectPropInfo;

	*ppEffectProp = pProp;

	return 1;
}

void CEffectPropManager::releaseEffectProp( IEffectProp* pEffectProp )
{
	if( pEffectProp == NULL )
	{
		return;
	}

	CEffectProp* pEffectPropRef = static_cast<CEffectProp*>( pEffectProp );
	m_mapEffectProp.erase( pEffectPropRef->getResourceName() );
}

bool CEffectPropManager::saveEffect( const char* szEffectFileName, IEffect* pEffect )
{
	if( !pEffect )
		return false;

	IEffectProp* pEffectProp = pEffect->getEffectProp();
	if( !pEffectProp )
		return false;

	CEffectProp* pEffectPropRef = static_cast<CEffectProp*>( pEffectProp );
	return pEffectPropRef->saveRes( szEffectFileName );
}

//int32 CEffectPropManager::ApplyEffect( IEffectProp* pEffectProp )
//{
//	CEffectProp* pEffectPropRef = static_cast<CEffectProp*>( pEffectProp );
//	ApplyRes( pEffectPropRef );
//	return 0;
//}

//void CEffectPropManager::DestroyRes( IResource* pResource )
//{
//	if( !pResource )
//		return;
//
//	CEffectProp* pEffectProp = static_cast<CEffectProp*>( pResource );
//	SAFE_DELETE( pEffectProp );
//}
////-------------------------------------------------------------------------
///**
//@brief 更新
//@param dt 帧间隔时间
//*/
//void CEffectPropManager::update( float32 dt )
//{
//
//}

NS_CC_ENGINE_END
