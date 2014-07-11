#include "HtmlText.h"

NS_CC_BEGIN

CHtmlText::CHtmlText(void)
: m_strText("")
{
}

CHtmlText::~CHtmlText(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 画
@param
*/
void CHtmlText::draw( const CCPoint& ptLoc )
{
	CCLabelTTF* pLabel = CCLabelTTF::create( m_strText.c_str(), m_FontProp.strFontName.c_str(), m_FontProp.nSize );
	if( pLabel == NULL )
	{
		return;
	}

//	CCLOG( "Html Text: %s FontSize: %d", m_strText.c_str(), m_FontProp.nSize );

	//CCLabelBMFont* pLabel = CCLabelBMFont::create( m_strText.c_str(), "UI/font/3500ch.fnt" );
	//if( pLabel == NULL )
	//{
	//	return;
	//}

	pLabel->setPosition( ptLoc );
	pLabel->setColor( m_FontProp.color );

	pLabel->visit();

	m_Rect.origin = ccp( ptLoc.x - m_Rect.size.width / 2, ptLoc.y - m_Rect.size.height / 2 );
}
//-------------------------------------------------------------------------
/**
@brief 解析属性
@param
*/
void CHtmlText::setText( const std::string& strAttr )
{
	m_strText = strAttr;
}
//-------------------------------------------------------------------------
/**
@brief 设置字体属性
@param
*/
void CHtmlText::setFontProp( const std::string strFontName, const ccColor3B& color, int nFontSize )
{
	m_FontProp.strFontName = strFontName;
	m_FontProp.nSize = nFontSize;
	m_FontProp.color = color;
	//CCLOG( "Html Text: strFontName %s FontSize: %d", strFontName.c_str(), nFontSize );
}

NS_CC_END
