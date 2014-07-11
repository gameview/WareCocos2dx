#include "HtmlTextLink.h"
#include "CHtmlLabel.h"
NS_CC_BEGIN

CHtmlTextLink::CHtmlTextLink(void)
{
}


CHtmlTextLink::~CHtmlTextLink(void)
{
}
/**
@brief 事件回调 超链接有效
@param
*/
void CHtmlTextLink::onEvent( IHtmlLabelEventHandler* pEnentHandler )
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
void CHtmlTextLink::draw( const CCPoint& ptLoc )
{
	CCLabelTTF* pLabel = CCLabelTTF::create( m_strText.c_str(), m_FontProp.strFontName.c_str(), m_FontProp.nSize );
	if( pLabel == NULL )
	{
		return;
	}

	//CCLabelBMFont* pLabel = CCLabelBMFont::create( m_strText.c_str(), "UI/font/3500ch.fnt" );
	//if( pLabel == NULL )
	//{
	//	return;
	//}

	pLabel->setPosition( ptLoc );
	pLabel->setColor( m_FontProp.color );

	int ypos = ptLoc.y - m_Rect.size.height / 2;

	glLineWidth(1);
	cocos2d::ccColor4B color4 = ccc4( m_FontProp.color.r, m_FontProp.color.g, m_FontProp.color.b, 255 );
	cocos2d::ccDrawColor4B(color4.r,color4.g,color4.b,color4.a);
	cocos2d::ccDrawLine( ccp( ptLoc.x - m_Rect.size.width / 2, ypos), ccp( ptLoc.x + m_Rect.size.width / 2, ypos ) );

	pLabel->visit();

	m_Rect.origin = ccp( ptLoc.x - m_Rect.size.width / 2, ptLoc.y - m_Rect.size.height / 2 );
}
//-------------------------------------------------------------------------
/**
@brief 设置字体属性
@param
*/
void CHtmlTextLink::setFontProp( const std::string strFontName, const ccColor3B& color, int nFontSize )
{
	m_FontProp.strFontName = strFontName;
	m_FontProp.nSize = nFontSize;
	m_FontProp.color = color;
}

NS_CC_END
