#include "RenderText.h"
#include "IRenderSystem.h"

NS_CC_ENGINE_BEGIN

CRenderText::CRenderText( unsigned int uid )
: m_pLabel(NULL)
, m_pLabelBmp(NULL)
, m_uID(uid)
, m_hAlignment(ETextAlignmentHorizontalLeft)
, m_vAlignment(ETextAlignmentVerticalCenter)
, m_pParent(NULL)
, m_pPos(CCPointZero)
, m_byAlpha(255)
{
	m_color = ccc3(255,255,255);
}

CRenderText::~CRenderText(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 创建
@param szFontName 字体名称
@param nFontSize 字体大小
*/
void CRenderText::create( const char* szString, const char* szFontName, int nFontSize, CCSize dimentions )
{
	if( szString == NULL )
	{
		return;
	}

	if( szFontName == NULL )
	{
		return;
	}

	m_pLabel = CCLabelTTF::create( szString, szFontName, (float)nFontSize, dimentions, (CCTextAlignment)m_hAlignment, (CCVerticalTextAlignment)m_vAlignment );

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if( m_pLabel )
	{
	/*	ccFontDefinition strokeShaodwTextDef;
		strokeShaodwTextDef.m_fontSize = nFontSize;
		strokeShaodwTextDef.m_fontName = szFontName;

		strokeShaodwTextDef.m_stroke.m_strokeEnabled = true;
		strokeShaodwTextDef.m_stroke.m_strokeColor   = ccBLACK;
		strokeShaodwTextDef.m_stroke.m_strokeSize    = 2.0f;

		strokeShaodwTextDef.m_shadow.m_shadowEnabled = false;
		strokeShaodwTextDef.m_shadow.m_shadowOffset  = CCSizeMake(2, -2);
		strokeShaodwTextDef.m_shadow.m_shadowOpacity = 1.0;
		strokeShaodwTextDef.m_shadow.m_shadowBlur    = 1.0;
		strokeShaodwTextDef.m_fontFillColor   =  ccc3( 255, 255, 255 );

	m_pLabel->setTextDefinition( &strokeShaodwTextDef );
*/
        
        m_pLabel->enableStroke(ccc3(10,0,10),0.5f);
        m_pLabel->enableShadow(CCSizeMake(2, -2), 255, 2);

	}
#endif
	//if( m_pLabel )
	//{
	//	m_pLabel->setStrokeSize( 1.0f );
	//}
}
//-------------------------------------------------------------------------
/**
@brief 创建
@param szFontName 字体名称
@param nFontSize 字体大小
*/
void CRenderText::createByBmpFont( const char* szString, const char* szFontName, int nFontSize, CCSize dimentions )
{
	if( szString == NULL )
	{
		return;
	}

	if( szFontName == NULL )
	{
		return;
	}

	m_pLabelBmp = CCLabelBMFont::create( szString, szFontName, kCCLabelAutomaticWidth, kCCTextAlignmentCenter );
}
//-------------------------------------------------------------------------
/**
@brief 关闭
@param
*/
void CRenderText::release()
{
	detach();

	/// m_pLabel是auto_release对象，不需要手动释放
	//if( m_pLabel )
	//{
	//	m_pLabel->release();
	//	m_pLabel = NULL;
	//}
	m_pLabel = NULL;
	
	delete this;
}
//-------------------------------------------------------------------------
/**
@brief 绑定到场景上
@param
*/
void CRenderText::attach( CCNode* pParentNode, int nZOrder, int nTag )
{
	if( pParentNode == NULL )
	{
		return;
	}

	if( m_pParent == pParentNode )
	{
		return;
	}

	if( m_pParent )
	{
		if( m_pLabel )
		{
			m_pLabel->retain();
			m_pParent->removeChild( m_pLabel );
		}

		if( m_pLabelBmp )
		{
			m_pLabelBmp->retain();
			m_pParent->removeChild( m_pLabelBmp );
		}
	}

	if( m_pLabel )
	{
		pParentNode->addChild( m_pLabel, nZOrder, nTag );
	}
	
	if( m_pLabelBmp )
	{
		pParentNode->addChild( m_pLabelBmp, nZOrder, nTag );
	}

	m_pParent = pParentNode;
}
//-------------------------------------------------------------------------
/**
@brief 从场景上解除
@param
*/
void CRenderText::detach()
{
	if( m_pParent )
	{
		m_pParent->removeChild( m_pLabel );
		if (m_pLabelBmp != NULL)
		{
			m_pParent->removeChild( m_pLabelBmp );
		}

		m_pParent = NULL;
		m_pLabel = NULL;
		m_pLabelBmp = NULL;
	}
}
//-------------------------------------------------------------------------
/**
@brief 设置位置
@param ptLoc 位置
*/
void CRenderText::setPos( const CCPoint& ptLoc )
{
	if( m_pLabel )
	{
		m_pLabel->setPosition( ptLoc );
	}

	if( m_pLabelBmp )
	{
		m_pLabelBmp->setPosition( ptLoc );
	}
}

/**
@brief 设置字串
@param
*/
void CRenderText::setString( const char* szText )
{
	if( szText == NULL )
	{
		return;
	}

	if( m_strContent == szText )
	{
		return;
	}

	m_strContent = szText;

	if( m_pLabel )
	{
		m_pLabel->setString( szText );
	}

	if( m_pLabelBmp )
	{
		m_pLabelBmp->setString( szText );
	}
}
//-------------------------------------------------------------------------
/**
@brief 设置字体
@param
*/
void CRenderText::setFont( const char* szFont )
{

}
//-------------------------------------------------------------------------
/**
@brief 给对象加色
@param
*/
void CRenderText::setColor( const ccColor3B& color )
{
	if( m_pLabel )
	{
		m_pLabel->setColor( color );
	}

	if( m_pLabelBmp )
	{
		m_pLabelBmp->setColor( color );
	}
}
//-------------------------------------------------------------------------
/**
@brief 设置透明度
@param
*/
void CRenderText::setOpacity( BYTE byAlpha )
{
	if( m_pLabel )
	{
		m_pLabel->setOpacity( (GLubyte)byAlpha );
	}

	if( m_pLabelBmp )
	{
		m_pLabelBmp->setOpacity( (GLubyte)byAlpha );
	}
}
//-------------------------------------------------------------------------
/**
@brief 设置显示区域
@param
*/
void CRenderText::setDimensions( const CCSize& dimentions )
{
	if( m_Dimentions.equals( dimentions ) )
	{
		return;
	}

	m_Dimentions = dimentions;
	if( m_pLabel )
	{
		m_pLabel->setDimensions( m_Dimentions );
	}

	//if( m_pLabelBmp )
	//{
	//	m_pLabelBmp->setDimensions( m_Dimentions );
	//}
}
//-------------------------------------------------------------------------
/**
@brief 设置对齐方式
@param
*/
void CRenderText::setHorizontalAlignment( ETextHorizontalAlignment hAlignment )
{
	if( m_hAlignment == hAlignment )
	{
		return;
	}

	if( m_pLabel )
	{
		m_hAlignment = hAlignment;
		m_pLabel->setHorizontalAlignment( (CCTextAlignment)m_hAlignment );
	}	
}

/**
@brief 设置对齐方式
@param
*/
void CRenderText::setVerticalAlignment( ETextVerticalAlignment vAlignment )
{
	if( m_vAlignment == vAlignment )
	{
		return;
	}

	if( m_pLabel )
	{
		m_vAlignment = vAlignment;
		m_pLabel->setVerticalAlignment( (CCVerticalTextAlignment)m_vAlignment );
	}
}

NS_CC_ENGINE_END