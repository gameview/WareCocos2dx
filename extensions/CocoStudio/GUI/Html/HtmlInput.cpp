#include "HtmlInput.h"

NS_CC_BEGIN

CHtmlInput::CHtmlInput( int nID )
{
	m_nID = nID;
}

CHtmlInput::~CHtmlInput(void)
{
}

/**
@brief 解析属性
@param
*/
void CHtmlInput::parseAttribute( const std::string& strAttr )
{
	IHtmlElement::parseAttribute( strAttr );

	m_Rect.size = CCSizeMake( m_TagAttribute.getIntValue("width"), m_TagAttribute.getIntValue("height") == 0 ? 30 : m_TagAttribute.getIntValue("height")  );
}

NS_CC_END
