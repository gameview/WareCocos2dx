#include "HtmlSeparate.h"

NS_CC_BEGIN

CHtmlSeparate::CHtmlSeparate()
{
}

CHtmlSeparate::~CHtmlSeparate(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 画
@param
*/
void CHtmlSeparate::draw( const CCPoint& ptLoc )
{
	/// 分隔符图片
	CCSprite* pSprite = CCSprite::create( SEPRATE_IMAGE );
	if( pSprite == NULL )
	{
		return;
	}

	CCSize size = pSprite->getContentSize();
	pSprite->setScaleX( m_Rect.size.width / size.width );

	pSprite->setPosition( ptLoc );
	pSprite->visit();

	m_Rect.origin = ccp( ptLoc.x - m_Rect.size.width / 2, ptLoc.y - m_Rect.size.height / 2 );
}

NS_CC_END
