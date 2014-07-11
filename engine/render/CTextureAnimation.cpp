#include "CTextureAnimation.h"
#include "CTextureAnimationProp.h"
#include "StringUtility.h"
#include "WareTime.h"
#include <math.h>

NS_CC_ENGINE_BEGIN

CTextureAnimation::CTextureAnimation( IEffectUnitProp* pEffectUnitProp,uint32 uID )
: CEffectUnit( pEffectUnitProp, uID )
, m_pSprite(NULL)
{
}

CTextureAnimation::~CTextureAnimation(void)
{
}

void CTextureAnimation::update( float32 dt )
{
	if( NULL == m_pSprite )
	{
		return;
	}

	CTextureAnimationProp* pProp = (CTextureAnimationProp*)m_pEffectUnitProp;

	uint32 uCurTime = WareTime::getTickCount();
	float32 fElapseTime = static_cast<float32>( uCurTime - m_uStartTime );
	float32 fTotalTime = static_cast<float32>( pProp->m_nTotalTime );
	float32 fPos = fElapseTime / fTotalTime;
	if( fPos == 0.00000f )
	{
		fPos = 0.0f;
	}
	else
	{
		fPos = fPos - floor(fPos);
	}
	

	CCSpriteFrame* pFrame = NULL;
	StringUtility::getMapValue( m_mapTexture, fPos, pFrame );
	if( pFrame != NULL )
	{
		m_pSprite->setDisplayFrame( pFrame );
	}

	CCPoint pos;
	StringUtility::getMapLinearLerpValue( pProp->m_mapOffset, fPos, pos );
	m_pSprite->setPosition( pos );

	float32 fSize = 0.0f;
	StringUtility::getMapLinearLerpValue( pProp->m_mapSize, fPos, fSize );
	m_pSprite->setScale( fSize );

	float32 fAlpha = 0.0f;
	StringUtility::getMapLinearLerpValue( pProp->m_mapAlpha, fPos, fAlpha );
	m_pSprite->setOpacity( static_cast<GLbyte>( fAlpha * 255.0f + 0.5f ) );

	return;
}

bool CTextureAnimation::create()
{
	CTextureAnimationProp* pProp = (CTextureAnimationProp*)m_pEffectUnitProp;
	
	CEffectUnit::create();

	m_pSprite = CCSprite::create();
	m_pSprite->setOpacity( 255 );
	m_pSprite->setContentSize( CCSizeMake(pProp->m_nWidth,pProp->m_nHeight) );

	addChild( m_pSprite );

	m_uStartTime = WareTime::getTickCount();

	return true;
}

void CTextureAnimation::setEffectUnitProp( IEffectUnitProp* pProp )
{
	if( m_pSprite )
	{
		m_pSprite->setTexture( NULL );
	}

	createTexture();
}

NS_CC_ENGINE_END
