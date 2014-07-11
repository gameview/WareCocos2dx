#include "CEffectUnit.h"
#include "IEffectUnitProp.h"
#include "CEffectUnitProp.h"
#include "cocos2d.h"

NS_CC_ENGINE_BEGIN

CEffectUnit::CEffectUnit( IEffectUnitProp* pEffectUnitProp, uint32 uID )
: m_pEffectUnitProp(pEffectUnitProp)
, m_uStartTime(0)
, m_nEffectUnitType(0)
, m_uID(uID)
{
}

CEffectUnit::~CEffectUnit()
{
	//releaseAllTexture();
}

bool CEffectUnit::create()
{
	createTexture();
	return true;
}

/**
@brief ÊÍ·Å
@param 
*/
void CEffectUnit::release()
{
	releaseAllTexture();

	delete this;
}

IEffectUnitProp* CEffectUnit::getEffectUnitProp()
{
	return m_pEffectUnitProp;
}

void CEffectUnit::setEffectUnitProp( IEffectUnitProp* pProp )
{	
}

//////////////////////////////////////////////////////////////////////////
int32 CEffectUnit::getType()
{
	return m_nEffectUnitType;
}

void CEffectUnit::createTexture()
{
	releaseAllTexture();

	CEffectUnitProp* pProp = (CEffectUnitProp*)m_pEffectUnitProp;
	m_nEffectUnitType = pProp->getType();

	CCTexture2D* pTex = NULL;
	NameMap& TexMap = pProp->m_mapTextureFileName;
	NameMap::iterator iterName = TexMap.begin();
	for( ; iterName != TexMap.end(); ++iterName )
	{
		pTex = CCTextureCache::sharedTextureCache()->addImage(iterName->second.c_str());
		if( !pTex )
		{
			continue;
		}

		CCSpriteFrame* pFrame = new CCSpriteFrame();
		pFrame->initWithTexture(pTex, CCRect(0,0,pTex->getPixelsWide(),pTex->getPixelsHigh()) );

		m_mapTexture.insert( std::make_pair( iterName->first, pFrame ) );
	}	
}

void CEffectUnit::releaseAllTexture()
{
	TextureMap::iterator iter = m_mapTexture.begin();
	for( ; iter != m_mapTexture.end(); ++iter )
	{
		if( iter->second )
		{
			iter->second->release();
		}
	}

	m_mapTexture.clear();
}

NS_CC_ENGINE_END