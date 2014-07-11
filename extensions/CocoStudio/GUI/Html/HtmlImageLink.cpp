#include "HtmlImageLink.h"
#include "CHtmlLabel.h"

NS_CC_BEGIN

CHtmlImageLink::CHtmlImageLink(void)
{
}


CHtmlImageLink::~CHtmlImageLink(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 事件回调 超链接有效
@param
*/
void CHtmlImageLink::onEvent( IHtmlLabelEventHandler* pEnentHandler )
{
	if( pEnentHandler )
	{
		pEnentHandler->onLinkEvent( m_TagAttribute.getStringValue( "href" ), true, m_TagAttribute.getBoolValue("close") );
	}
}
//-------------------------------------------------------------------------
/**
@brief 画
@param
*/
void CHtmlImageLink::draw( const CCPoint& ptLoc )
{
	//const char* pszImageName = m_TagAttribute.getStringValue( "src" );
	//if( pszImageName == NULL )
	//{
	//	CCLOG( "CHtmlImage::draw no src image" );
	//	return;
	//}

	CCSprite* pSprite = CCSprite::create( m_strText.c_str() );
	if( pSprite == NULL )
	{
		return;
	}

	pSprite->setPosition( ptLoc );
	pSprite->visit();

	m_Rect.origin = ccp( ptLoc.x - m_Rect.size.width / 2, ptLoc.y - m_Rect.size.height / 2 );
}

NS_CC_END