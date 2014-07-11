#include "ImageLayer.h"

NS_CC_ENGINE_BEGIN

CImageLayer::CImageLayer(void)
{
	//m_uSeed = 0;
	//m_mapTerrainObj.clear();
	m_nLayerID = 0;
	//m_setVisibleObj.clear();
	//m_setRenderObj.clear();
}

CImageLayer::~CImageLayer(void)
{
	//TerrainObjMap::iterator iter = m_mapTerrainObj.begin();
	//for( ; iter != m_mapTerrainObj.end(); ++iter )
	//{
	//	if( iter->second.pSprite )
	//	{
	//		iter->second.pSprite->release();
	//		iter->second.pSprite = NULL;
	//	}
	//}

	//m_mapTerrainObj.clear();
}
//-------------------------------------------------------------------------
//** create one layer */
CImageLayer* CImageLayer::create(std::vector<std::string>& vecImages)
{
	CImageLayer *pRet = new CImageLayer();
	if (pRet && pRet->init(vecImages))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}
//-------------------------------------------------------------------------
/**
@brief ≥ı ºªØ
@param 
*/
bool CImageLayer::init(std::vector<std::string>& vecImages)
{
	std::vector<cocos2d::CCSprite*> vecSprite;
	int nWidth = 0;
	size_t size = vecImages.size();
	if( !vecImages.empty() )
	{
		for( size_t i = 0; i < size; ++i )
		{
			cocos2d::CCSprite* pSprite = cocos2d::CCSprite::create( vecImages[i].c_str() );
			if( pSprite == NULL )
			{
				return false;
			}

			nWidth = pSprite->getContentSize().width;
			m_size.width += nWidth;
			m_size.height += pSprite->getContentSize().height;
			vecSprite.push_back( pSprite );
			addChild(pSprite);
		}
	}

	int k = size / 2;
	int nHalfWidth = nWidth / 2;
	if( size % 2 == 0 )
	{
		int nIndex = 0;
		for( int i = -k; i < k; ++i )
		{
			vecSprite[nIndex++]->setPosition(ccp( (i*2+1)*nHalfWidth,0));
		}
	}
	else
	{
		int nIndex = 0;
		for( int i = -k; i <= k; ++i )
		{
			vecSprite[nIndex++]->setPosition(ccp( i*nWidth,0));
		}		
	}

	return true;
}

NS_CC_ENGINE_END