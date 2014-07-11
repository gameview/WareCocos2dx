#include "WareSprite.h"
#include "ShaderManager.h"
#include "RenderSystem.h"

NS_CC_ENGINE_BEGIN

CWareSprite::CWareSprite(void)
: m_bVailed(false)
{
	m_Rect = CCRectZero;
}

CWareSprite::~CWareSprite(void)
{
    CCTextureCache::sharedTextureCache()->removeTexture( this->getTexture() );
}
//------------------------------------------------------------------------------
CWareSprite* CWareSprite::create()
{
	CWareSprite *pSprite = new CWareSprite();
	if (pSprite && pSprite->init())
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}
//-------------------------------------------------------------------------
/** Creates an sprite with an image filename.
The rect used will be the size of the image.
The offset will be (0,0).
*/
CWareSprite* CWareSprite::create(const char *pszFileName)
{
	CWareSprite *pobSprite = new CWareSprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
/** Creates an sprite with an image filename and a rect.
The offset will be (0,0).
*/
CWareSprite* CWareSprite::create(const char *pszFileName, const cocos2d::CCRect& rect)
{
	CWareSprite *pobSprite = new CWareSprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
/**
* Initializes an empty sprite with nothing init.
*/
bool CWareSprite::init(void)
{
	if( CCSprite::init() )
	{
		m_bVailed = true;
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------
bool CWareSprite::initWithFile(const char *pszFilename)
{
	CCAssert(pszFilename != NULL, "Invalid filename for sprite");

	m_bVailed = false;

	//CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszFilename);
	//if (pTexture)
	//{
	//	CCRect rect = CCRectZero;
	//	rect.size = pTexture->getContentSize();
	//	return initWithTexture(pTexture, rect);
	//}

	retain();
	CCTextureCache::sharedTextureCache()->addImageAsync(pszFilename,this, callfuncO_selector(CWareSprite::imageLoaded));
	m_Rect = CCRectZero;
	
	// don't release here.
	// when load texture failed, it's better to get a "transparent" sprite then a crashed program
	// this->release(); 
	return true;
}

bool CWareSprite::initWithFile(const char *pszFilename, const CCRect& rect)
{
	CCAssert(pszFilename != NULL, "");

	m_bVailed = false;

	//CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszFilename);
	//if (pTexture)
	//{
	//	return initWithTexture(pTexture, rect);
	//}

	retain();
	CCTextureCache::sharedTextureCache()->addImageAsync(pszFilename,this, callfuncO_selector(CWareSprite::imageLoaded));
	m_Rect = rect;
	
    m_strPath =pszFilename;

	// don't release here.
	// when load texture failed, it's better to get a "transparent" sprite then a crashed program
	// this->release(); 
	return true;
}
//-------------------------------------------------------------------------
/**
@brief 图片加载完毕
@param
*/
void CWareSprite::imageLoaded(CCObject* pObj)
{
	CCTexture2D* pTexture = (CCTexture2D*)pObj;
	if (pTexture)
	{
		if( m_Rect.equals( CCRectZero ) )
		{
			m_Rect.size = pTexture->getContentSize();
		}

		initWithTexture(pTexture, m_Rect);
	}

	release();
	m_bVailed = true;
}
//-------------------------------------------------------------------------
/// 
void CWareSprite::draw(void)
{
	//ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
	//ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
	if( !m_bVailed )
	{
		return;
	}

	if( g_RenderSystem.m_bGrey )
	{
		CCGLProgram* pProgram = CShaderManager::Instance().getGLProgram( "shaders/grey" );
		if( pProgram )
		{
			setShaderProgram(pProgram);
		}

		getShaderProgram()->use();
	}
	else
	{
		if (getTexture())
		{
			setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
		}
		else
		{
			setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
		}

		getShaderProgram()->use();
	}

	CCSprite::draw();

//	CC_NODE_DRAW_SETUP();
//
//	if (m_pobTexture != NULL)
//	{
//		ccGLBindTexture2D( m_pobTexture->getName() );
//		ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
//	}
//	else
//	{
//		ccGLBindTexture2D(0);
//		ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color );
//	}
//
//#define kQuadSize sizeof(m_sQuad.bl)
//	long offset = (long)&m_sQuad;
//
//	// vertex
//	int diff = offsetof( ccV3F_C4B_T2F, vertices);
//	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
//
//	if (m_pobTexture != NULL)
//	{
//		// texCoods
//		diff = offsetof( ccV3F_C4B_T2F, texCoords);
//		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
//	}
//
//	// color
//	diff = offsetof( ccV3F_C4B_T2F, colors);
//	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	CC_INCREMENT_GL_DRAWS(1);
}

NS_CC_ENGINE_END