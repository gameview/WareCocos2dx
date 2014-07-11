#include "CEffect.h"
#include "CEffectProp.h"
#include "CEffectUnit.h"
#include "CTextureAnimation.h"
#include "CParticleSystem.h"
#include "IEffectUnitProp.h"
#include "CEffectPropManager.h"
#include "WareTime.h"

NS_CC_ENGINE_BEGIN

CEffect::CEffect(uint32 uID)
: m_pEffectProp(NULL)
, m_uID(uID)
, m_uEffectUnitID(0)
, m_uStartTime(0)
, m_strEffectName("")
{
}

CEffect::~CEffect(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 释放
@param 
*/
void CEffect::release()
{
	if( isSingleReference() )
	{
		if( m_pEffectProp )
			m_pEffectProp->release();

		size_t size = m_vecEffectUnitTrack.size();
		for( size_t i = 0; i < size; ++i )
		{
			if( m_vecEffectUnitTrack[i] )
				releaseEffectUnit( m_vecEffectUnitTrack[i]->pEffectUnit );
			SAFE_DELETE( m_vecEffectUnitTrack[i] );
		}
	}

	CCObject::release();
}
//-------------------------------------------------------------------------
/**
@brief 创建
@param szPicFileName 资源文件名
@param ptOffset 偏移值
@return 返回创建成功与否标识
*/
bool CEffect::create( const char* szEffectFileName, CCPoint ptOffset )
{
	if( szEffectFileName == NULL )
	{
		return false;
	}

	if( szEffectFileName )
	{
		m_strEffectName = szEffectFileName;
	}

	if( !CEffectPropManager::Instance().createEffectProp( szEffectFileName, &m_pEffectProp ) )
	{
		CCLOG( "create effect %s failed!", szEffectFileName );
		return false;
	}

	scheduleUpdate();

	return true;
}
//-------------------------------------------------------------------------
void CEffect::update( float dt )
{
	CEffectProp* pProp = static_cast<CEffectProp*>( m_pEffectProp );
	if( !pProp )
	{
		return;
	}

	uint32 uCurTime = WareTime::getTickCount();
	if( m_uStartTime == 0 )
	{
		m_uStartTime = uCurTime;
	}

	float32 fElapseTime = static_cast<float32>( uCurTime - m_uStartTime );
	float32 fTotalTime = static_cast<float32>( pProp->m_uTotalTime );
	float32 fPos = fElapseTime / fTotalTime;
	if( fPos == 0.00000f )
	{
		fPos = 0.0f;
	}
	else
	{
		fPos = fPos - floor( fPos );
	}

	CEffectUnit* pEffectUnit = NULL;
	size_t size = m_vecEffectUnitTrack.size();
	for( size_t i = 0; i < size; ++i )
	{
		if( m_vecEffectUnitTrack[i]->pEffectUnit )
		{
			pEffectUnit = static_cast<CEffectUnit*>( m_vecEffectUnitTrack[i]->pEffectUnit );

			if( fPos >= m_vecEffectUnitTrack[i]->fStartPos && fPos <= m_vecEffectUnitTrack[i]->fEndPos )
			{
				pEffectUnit = static_cast<CEffectUnit*>( m_vecEffectUnitTrack[i]->pEffectUnit );
				if( !m_vecEffectUnitTrack[i]->bActive )
				{
					pEffectUnit->setStartTime( uCurTime );

					m_vecEffectUnitTrack[i]->bActive = true;

					/// 轨道顺序是层次关系
					addChild( pEffectUnit, i, pEffectUnit->getID() );
				}

				pEffectUnit->update( dt );
			}
			else
			{
				pEffectUnit->setStartTime( 0 );
				m_vecEffectUnitTrack[i]->bActive = false;

				//if( pEffectUnit->getType() != EFFECTUNIT_TYPE_PARTICLESYS )
				//{
				removeChildByTag( pEffectUnit->getID() );
				//}
			}
		}
	}

	return;
}

IEffectProp* CEffect::getEffectProp()
{
	return m_pEffectProp;
}
//-------------------------------------------------------------------------
/**
@brief 
@param
*/
void CEffect::onExit()
{
	size_t size = m_vecEffectUnitTrack.size();
	for( size_t i = 0; i < size; ++i )
	{
		if( m_vecEffectUnitTrack[i]->bActive )
		{
			removeChild( m_vecEffectUnitTrack[i]->pEffectUnit );
		}
	}
}
//-------------------------------------------------------------------------
/**
@brief 给对象加色
@param
*/
void CEffect::setColor( const ccColor3B& color )
{
	//size_t size = m_vecEffectUnitTrack.size();
	//for( size_t i = 0; i < size; ++i )
	//{
	//	m_vecEffectUnitTrack[i]->pEffectUnit->setColor( color );
	//}
}
//-------------------------------------------------------------------------
/**
@brief 设置透明度
@param byAlpha 透明度
*/
void CEffect::setOpacity( BYTE byAlpha )
{
	//size_t size = m_vecEffectUnitTrack.size();
	//for( size_t i = 0; i < size; ++i )
	//{
	//	m_vecEffectUnitTrack[i]->pEffectUnit->setOpacity( byAlpha );
	//}
}
//-------------------------------------------------------------------------
/**
@brief 获取该对象的特征串(全局唯一)
@param strFeature 特征串
*/
void CEffect::getFeatureString( std::string& strFeature )
{
	strFeature = m_strEffectName;
}
//-------------------------------------------------------------------------
/**
@brief 碰撞检测
@param
*/
bool CEffect::hitTest( const CCPoint& pt )
{
	size_t size = m_vecEffectUnitTrack.size();
	for( size_t i = 0; i < size; ++i )
	{
		CCRect rect = m_vecEffectUnitTrack[i]->pEffectUnit->boundingBox();
		return rect.containsPoint( pt );
	}

	return false;
}
//-------------------------------------------------------------------------
/**
@brief 碰撞检测
@param
*/
bool CEffect::hitTest( const CCRect& rect )
{
	size_t size = m_vecEffectUnitTrack.size();
	for( size_t i = 0; i < size; ++i )
	{
		CCRect rect = m_vecEffectUnitTrack[i]->pEffectUnit->boundingBox();
		return rect.intersectsRect( rect );
	}

	return false;
}
//-------------------------------------------------------------------------
/**
@brief 获取对象包围盒
@param
*/
bool CEffect::getBoundingBox( CCRect& boundBox )
{
	boundBox = boundingBoxReal();
	boundBox = CCRectApplyAffineTransform(boundBox, nodeToParentTransform());

	return true; 
}
//-------------------------------------------------------------------------
/**
@brief 获取偏移
@param
*/
void CEffect::setOffset( CCPoint& ptOffet )
{
	setPosition( ptOffet );
}
//-------------------------------------------------------------------------
/**
@brief 获取偏移
@param
*/
void CEffect::getOffset( CCPoint& ptOffet )
{
	ptOffet = getPosition();
}


//////////////////////////////////////////////////////////////////////////
bool CEffect::createEffect( IEffectProp* pEffectProp )
{
	if( !pEffectProp )
	{
		return false;
	}

	if( m_pEffectProp )
	{
		m_pEffectProp->release();
	}

	m_pEffectProp = pEffectProp;
	pEffectProp->retain();

	///
	CEffectProp* pProp = static_cast<CEffectProp*>( pEffectProp );
	const EffectUnitTrack& Track = pProp->getEffectUnitTracks();

	size_t size = Track.size();
	SEffectTrack* pTrack = NULL;
	for( size_t i = 0; i < size; ++i )
	{
		pTrack = new SEffectTrack();
		pTrack->fStartPos = Track[i]->fStartPos;
		pTrack->fEndPos = Track[i]->fEndPos;
		
		CEffectUnit* pEffectUnit = NULL;
		if( Track[i]->pEffectUnitProp )
		{
			pEffectUnit = createEffectUnit( Track[i]->pEffectUnitProp );
			pEffectUnit->create();
		}

		pTrack->pEffectUnit = pEffectUnit;

		m_vecEffectUnitTrack.push_back( pTrack );
	}

	scheduleUpdate();

	return true;
}

CEffectUnit* CEffect::createEffectUnit( IEffectUnitProp* pEffectUnitProp )
{
	int32 nType = pEffectUnitProp->getType();

	if( ++m_uEffectUnitID == 0 )
	{
		++m_uEffectUnitID;
	}
	if( nType == EFFECTUNIT_TYPE_TEXTUREANI )
	{
		return new CTextureAnimation( pEffectUnitProp,m_uEffectUnitID );
	}
	else if( nType == EFFECTUNIT_TYPE_PARTICLESYS )
	{
		return new CParticleSystem( pEffectUnitProp,m_uEffectUnitID );
	}

	return NULL;
}

void CEffect::releaseEffectUnit( IEffectUnit* pEffectUnit )
{
	if( !pEffectUnit )
		return;

	CEffectUnit* pEffectUnitRef = static_cast<CEffectUnit*>(pEffectUnit);
	int32 nType = pEffectUnitRef->getType();
	
	if( nType == EFFECTUNIT_TYPE_TEXTUREANI )
	{
		CTextureAnimation* pTextureAnim = static_cast<CTextureAnimation*>(pEffectUnit);
		SAFE_DELETE( pTextureAnim );
	}
	else if( nType == EFFECTUNIT_TYPE_PARTICLESYS )
	{
		CParticleSystem* pParticleSystem = static_cast<CParticleSystem*>(pEffectUnit);
		SAFE_DELETE( pParticleSystem );
	}
}

NS_CC_ENGINE_END