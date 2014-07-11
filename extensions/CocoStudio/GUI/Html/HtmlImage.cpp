#include "HtmlImage.h"

NS_CC_BEGIN

CHtmlImage::CHtmlImage()
{
}

CHtmlImage::~CHtmlImage(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 画
@param
*/
void CHtmlImage::draw( const CCPoint& ptLoc )
{
	const char* pszImageName = m_TagAttribute.getStringValue( "src" );
	if( pszImageName == NULL )
	{
		CCLOG( "CHtmlImage::draw no src image" );
		return;
	}

	CCSprite* pSprite = CCSprite::create( pszImageName );
	if( pSprite == NULL )
	{
		return;
	}

	pSprite->setPosition( ptLoc );
	pSprite->visit();

	m_Rect.origin = ccp( ptLoc.x - m_Rect.size.width / 2, ptLoc.y - m_Rect.size.height / 2 );
}

NS_CC_END
