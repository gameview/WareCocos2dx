#include "CEffectManager.h"
#include "CEffect.h"
#include "CEffectProp.h"
#include "CEffectPropManager.h"

NS_CC_ENGINE_BEGIN

CEffectManager::CEffectManager(void) 
: m_uEffectIDSeed(0)
{
}

CEffectManager::~CEffectManager(void) 
{

}
//-------------------------------------------------------------------------
/**
@brief 创建空特效
@param 
*/
IEffect* CEffectManager::createEffect(IEffectProp* pEffectProp)
{
	if( pEffectProp == NULL )
	{
		CEffectPropManager::Instance().createEmptyEffectProp( &pEffectProp );
	}

	if( NULL == pEffectProp )
	{
		return NULL;
	}

	if( ++m_uEffectIDSeed == 0 )
	{
		++m_uEffectIDSeed;
	}

	CEffect* pEffect = new CEffect(m_uEffectIDSeed);
	if( !pEffect->createEffect( pEffectProp ) )
	{
		return NULL;
	}

	return pEffect;
}
//-------------------------------------------------------------------------
/**
@brief 根据特效文件创建
@param 
*/
IEffect* CEffectManager::createEffectFromFile( const char* szEffectFileName )
{
	if( ++m_uEffectIDSeed == 0 )
	{
		++m_uEffectIDSeed;
	}

	CEffect* pEffect = new CEffect(m_uEffectIDSeed);
	if( !pEffect->create( szEffectFileName ) )
	{
		return NULL;
	}

	return pEffect;
}
////-------------------------------------------------------------------------
///**
//@brief 释放特效
//@param 
//*/
//void CEffectManager::releaseEffect( IEffect* pEffect )
//{
//	if( pEffect )
//	{
//		pEffect->release();
//
//		if( pEffect->isSingleReference() )
//		{
//			pEffect->unscheduleUpdate();
//		}
//	}
//}
//-------------------------------------------------------------------------
/**
@brief 保存特效
@param 
*/
void CEffectManager::saveEffect( const char* szFileName, IEffect* pEffect )
{
	if( pEffect == NULL )
	{
		return;
	}

	if( szFileName == NULL )
	{
		return;
	}

	CEffectPropManager::Instance().saveEffect( szFileName, pEffect );
}

bool CEffectManager::update( uint32 uCurTime )
{
	return true;
}

NS_CC_ENGINE_END
