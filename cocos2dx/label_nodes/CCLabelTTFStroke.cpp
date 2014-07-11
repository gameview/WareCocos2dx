#include "CCLabelTTFStroke.h"
#include "misc_nodes/CCRenderTexture.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

CCLabelTTFStroke::CCLabelTTFStroke()
	:m_colStroke(ccc3(0,0,0))
	,m_fStrokeSize(1.0f)
	,m_sprite(NULL)
	,m_label(NULL)
{}

CCLabelTTFStroke::~CCLabelTTFStroke()
{
	CC_SAFE_RELEASE_NULL(m_label);
}
//-------------------------------------------------------------------------
CCLabelTTFStroke* CCLabelTTFStroke::create()
{
	CCLabelTTFStroke *pRet = new CCLabelTTFStroke();
	if(pRet && pRet->initWithString())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
bool CCLabelTTFStroke::initWithString()
{
	m_label = CCLabelTTF::create();
	m_label->retain();

	return true;
}
//-------------------------------------------------------------------------
bool CCLabelTTFStroke::initWithString(const char *string, const char *fontName, float fontSize, CCSize dimentions, CCTextAlignment hAlignment,CCVerticalTextAlignment vAlignment)
{
	m_label = CCLabelTTF::create(string, fontName, fontSize, dimentions,hAlignment,vAlignment);
	m_label->retain();

	updateStroke();

	return true;
}
//-------------------------------------------------------------------------
CCLabelTTFStroke* CCLabelTTFStroke::create(const char *string, const char *fontName, float fontSize, CCSize dimentions, CCTextAlignment hAlignment,CCVerticalTextAlignment vAlignment)
{
	CCLabelTTFStroke *pRet = new CCLabelTTFStroke();
	if(pRet && pRet->initWithString(string, fontName, fontSize, dimentions, hAlignment, vAlignment))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
void CCLabelTTFStroke::updateStroke()
{
	if (m_sprite)
	{
		removeChild(m_sprite, true);
	}

	CCSize textureSize = m_label->getContentSize();
	textureSize.width += 2 * m_fStrokeSize;
	textureSize.height += 2 * m_fStrokeSize;
	//call to clear error
	glGetError();
	CCRenderTexture *rt = CCRenderTexture::create(textureSize.width, textureSize.height);
	if(!rt)
	{
		CCLOG("create render texture failed !!!!");
		addChild(m_label);
		return;
	}

	ccColor3B col = m_label->getColor();
	m_label->setColor(m_colStroke);

	ccBlendFunc originalBlend = m_label->getBlendFunc();
	ccBlendFunc func = { GL_SRC_ALPHA, GL_ONE};
	m_label->setBlendFunc(func);

	m_label->setAnchorPoint(ccp(0.5, 0.5));

	CCPoint ptCenter = ccp( textureSize.width * 0.5f, textureSize.height * 0.5f );
	rt->begin();
	for(int i = 0; i < 360; i+=15)
	{
		m_label->setPosition(ccp(
			ptCenter.x + sin(CC_DEGREES_TO_RADIANS(i)) * m_fStrokeSize,
			ptCenter.y + cos(CC_DEGREES_TO_RADIANS(i)) * m_fStrokeSize));
		m_label->visit();
	}

	m_label->setColor(col);
	m_label->setBlendFunc(originalBlend);
	m_label->setPosition(ptCenter);
	m_label->visit();
	rt->end();

	CCTexture2D *texture = rt->getSprite()->getTexture();
	texture->setAliasTexParameters();
	m_sprite = CCSprite::createWithTexture(rt->getSprite()->getTexture());
	setContentSize(m_sprite->getContentSize());
	m_sprite->setAnchorPoint(ccp(0.5, 0.5));
	m_sprite->setPosition(ccp(0, 0));
	((CCSprite *)m_sprite)->setFlipY(true);
	addChild(m_sprite);
}
//-------------------------------------------------------------------------
void CCLabelTTFStroke::setString(const char *label)
{
	if (m_label)
	{
		if(0!=strcmp(label, m_label->getString()))
		{
			m_label->setString(label);
			updateStroke();
		}
	}
	else
	{
		CCLOG("ERROR:CCLabelTTFStroke::setString m_label=NULL");
	}
}
//-------------------------------------------------------------------------
const char*  CCLabelTTFStroke::getString()
{
	if (m_label)
	{
		return m_label->getString();
	}

	CCLOG("ERROR:CCLabelTTFStroke::setString m_label=NULL");
	return NULL;	
}
//-------------------------------------------------------------------------
void CCLabelTTFStroke::setFontSize(int size)
{
	if( m_label )
	{
		m_label->setFontSize(size);
		updateStroke();
	}
}
//-------------------------------------------------------------------------
void CCLabelTTFStroke::setFontName( const char* name )
{
	if( m_label )
	{
		m_label->setFontName(name);
		updateStroke();
	}
}
//-------------------------------------------------------------------------
void CCLabelTTFStroke::setColor(const ccColor3B& color3)
{
	if (m_label)
	{
		ccColor3B col = m_label->getColor();
		if(color3.r!=col.r || color3.g!=col.g || color3.b!=col.b)
		{
			m_label->setColor(color3);
			updateStroke();
		}
	}
	else
	{
		CCLOG("ERROR:CCLabelTTFStroke::setColor m_label=NULL");
	}
}
//-------------------------------------------------------------------------
/**
@brief 设置显示区域
@param
*/
void CCLabelTTFStroke::setDimensions( const CCSize& dimentions )
{
	if (m_label)
	{
		CCSize size = m_label->getDimensions();
		if( !size.equals(dimentions) )
		{
			m_label->setDimensions(dimentions);
			updateStroke();
		}
	}
	else
	{
		CCLOG("ERROR:CCLabelTTFStroke::setDimensions m_label=NULL");
	}	
}
//-------------------------------------------------------------------------
/**
@brief 设置对齐方式
@param
*/
void CCLabelTTFStroke::setHorizontalAlignment( CCTextAlignment alignment )
{
	if (m_label)
	{
		CCTextAlignment al = m_label->getHorizontalAlignment();
		if( alignment != al )
		{
			m_label->setHorizontalAlignment(alignment);
			updateStroke();
		}
	}
	else
	{
		CCLOG("ERROR:CCLabelTTFStroke::setHorizontalAlignment m_label=NULL");
	}
}
//-------------------------------------------------------------------------
/**
@brief 设置对齐方式
@param
*/
void CCLabelTTFStroke::setVerticalAlignment( CCVerticalTextAlignment vAlignment )
{
	if (m_label)
	{
		CCVerticalTextAlignment vl = m_label->getVerticalAlignment();
		if( vAlignment != vl )
		{
			m_label->setVerticalAlignment(vAlignment);
			updateStroke();
		}
	}
	else
	{
		CCLOG("ERROR:CCLabelTTFStroke::setVerticalAlignment m_label=NULL");
	}
}
//-------------------------------------------------------------------------
void CCLabelTTFStroke::setOpacity( unsigned char byAlpha )
{
	if( m_sprite )
	{
		m_sprite->setOpacity( byAlpha );
	}
}

void CCLabelTTFStroke::setFlipX(bool flipX)
{
	if (m_label)
	{
		m_label->setFlipX(flipX);
		updateStroke();
	}
}

void CCLabelTTFStroke::setFlipY(bool flipY)
{
	if (m_label)
	{
		m_label->setFlipY(flipY);
		updateStroke();
	}
}

bool CCLabelTTFStroke::isFlipX()
{
	if (m_label)
	{
		return m_label->isFlipX();
	}
	return false;
}

bool CCLabelTTFStroke::isFlipY()
{
	if (m_label)
	{
		return m_label->isFlipY();
	}
	return false;
}

NS_CC_END