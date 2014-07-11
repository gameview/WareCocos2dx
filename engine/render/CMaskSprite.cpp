#include "CMaskSprite.h"
#include "ShaderManager.h"
#include "textures/CCTextureCache.h"


NS_CC_BEGIN

CMaskSprite::CMaskSprite(void)
: m_pmaskTexture(NULL)
, m_bUpdate(true)
, m_pSprite(NULL)
, m_pMaskSprite(NULL)
//, m_pSrcSprite(NULL)
{
}

CMaskSprite::~CMaskSprite(void)
{
	if( m_pMaskSprite )
	{
		CCTextureCache::sharedTextureCache()->removeTexture( m_pMaskSprite->getTexture() );
		CC_SAFE_RELEASE_NULL(m_pMaskSprite);
	}

	SpriteList::iterator iter = m_lstSprite.begin();
	for( ; iter != m_lstSprite.end(); ++iter )
	{
		if( (*iter).m_pSprite == NULL )
		{
			continue;
		}
		/// 删除贴图，释放内存
		CCTextureCache::sharedTextureCache()->removeTexture( (*iter).m_pSprite->getTexture() );
		CC_SAFE_RELEASE_NULL((*iter).m_pSprite);
	}
	m_lstSprite.clear();
	CC_SAFE_RELEASE_NULL(m_pmaskTexture);
}
//-------------------------------------------------------------------------
/**
* Creates an empty sprite without texture. You can call setTexture method subsequently.
*
* @return An empty sprite object that is marked as autoreleased.
*/
CMaskSprite* CMaskSprite::create( const char *pszMaskFileName )
{
	CMaskSprite *pSprite = new CMaskSprite();
	if (pSprite && pSprite->initMask( pszMaskFileName ) )
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}
//-------------------------------------------------------------------------
/** Initializes an sprite with a texture and a rect in points, optionally rotated.
The offset will be (0,0).
IMPORTANT: This is the designated initializer.
*/
bool CMaskSprite::initMask(const char *pszMaskFileName)
{
	if( pszMaskFileName == NULL || pszMaskFileName == NULL )
	{
		return false;
	}

	std::string strMaskImage = pszMaskFileName;
	if( m_pMaskSprite == NULL )
	{
		m_pMaskSprite = CCSprite::create( strMaskImage.c_str() );
		if( m_pMaskSprite == NULL )
		{
			CCLOG( "CMaskSprite::initMask create mask failed!", strMaskImage.c_str() );
			return false;
		}

		m_pMaskSprite->retain();
		m_maskSize = m_pMaskSprite->getContentSize();
	}

	m_strMaskImage = pszMaskFileName;
	scheduleUpdate();

	return true;
}
//-------------------------------------------------------------------------
bool CMaskSprite::addSprite( const char *pszSpriteName, const CCPoint& offset )
{
	std::string strImage = pszSpriteName;

	CCSprite* pSprite = CCSprite::create( strImage.c_str() );
	if( pSprite == NULL )
	{
		CCLOG( "CMaskSprite::initMask create src failed! %s", strImage.c_str() );
		return false;
	}

	pSprite->retain();

	m_lstSprite.push_back( SSpriteInfo( pSprite, offset ) );

	return true;
}
//-------------------------------------------------------------------------
void CMaskSprite::update(float delta)
{
	if( m_pMaskSprite == NULL )
	{
		return;
	}

	if( m_bUpdate )
	{
		CCRenderTexture* pRenderTarget = CCRenderTexture::create( m_maskSize.width, m_maskSize.height );
		if( pRenderTarget )
		{
			
			m_pMaskSprite->setAnchorPoint(ccp(0, 0));

			//pSrc->setPosition(ccp(m_srcSize.width/2, m_srcSize.height/2));
			
			//CCLOG( "Src offset: %f,%f", m_ptOffset.x, m_ptOffset.y );
			//pMask->setPosition(ccp(m_maskSize.width/2,m_maskSize.height/2));
			m_pMaskSprite->setPosition(ccp(0,0));

			ccBlendFunc func1 = {GL_ONE, GL_ZERO};
			ccBlendFunc func2 = {GL_DST_ALPHA, GL_ZERO};

			m_pMaskSprite->setBlendFunc(func1);
			

			pRenderTarget->begin();

			m_pMaskSprite->visit();

			SpriteList::iterator itSprite = m_lstSprite.begin();
			for( ; itSprite != m_lstSprite.end(); ++itSprite )
			{
				if( (*itSprite).m_pSprite == NULL )
				{
					continue;
				}
				
				CCSprite* pSprite = (*itSprite).m_pSprite;

				pSprite->setAnchorPoint(ccp(0, 0));
				pSprite->setPosition(m_ptOffset +m_maskSize/2 + (*itSprite).m_ptOffset);
				pSprite->setBlendFunc(func2);
				pSprite->visit();
			}

			////设置点的大小  
			PointMap::iterator iter = m_mapPoint.begin();
			for( ; iter != m_mapPoint.end(); ++iter )
			{
				cocos2d::ccPointSize(iter->second.nPointSize);  
				////设置后面要进行绘制时所用的色彩  
				cocos2d::ccDrawColor4B(iter->second.color.r, iter->second.color.g, iter->second.color.b, iter->second.color.a );  
				////绘制一个点  
				////ccDrawPoint( ccp(s.width / 2, s.height / 2) );  
				cocos2d::ccDrawPoint( iter->second.pos );
			}

			pRenderTarget->end();

			CCTexture2D *texture = pRenderTarget->getSprite()->getTexture();
			texture->setAliasTexParameters();

			if( m_pSprite == NULL )
			{
				m_pSprite = CCSprite::createWithTexture( texture );
				//setContentSize(m_Size);
				m_pSprite->setAnchorPoint(ccp(0, 0));
				m_pSprite->setPosition(ccp(0, 0));
				m_pSprite->setFlipY(true);
				addChild(m_pSprite);
			}
			else
			{
				m_pSprite->setTexture( texture );
			}
		}

		m_bUpdate = false;
	}
}
//-------------------------------------------------------------------------
/**
@brief 设置更新
@param
*/
void CMaskSprite::setUpdate()
{
	m_bUpdate = true;
}
//-------------------------------------------------------------------------
/// shader
//void CMaskSprite::draw(void)
//{
//	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
//
//	CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");
//
//	CCGLProgram* pProgram = cocos2d::engine::CShaderManager::Instance().getGLProgram( "shaders/mask" );
//	if( pProgram )
//	{
//		if( getShaderProgram() != pProgram )
//		{
//			setShaderProgram(pProgram);
//			getShaderProgram()->use();
//
//			_maskLocation = glGetUniformLocation( pProgram->getProgram(), "u_mask" );        
//			_offsetLocation = glGetUniformLocation( pProgram->getProgram(), "v_offset" );
//			_sizeLocation = glGetUniformLocation( pProgram->getProgram(), "v_size" );
//		}
//
//		CCSize size = CCDirector::sharedDirector()->getVisibleSize();
//		glUniform1i(_maskLocation, 1);
//		glUniform2f(_offsetLocation, m_ptOffset.x, m_ptOffset.y); //3
//		glUniform2f(_sizeLocation, size.width, size.height );
//	}
//	else
//	{
//		if (getTexture())
//		{
//			setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
//		}
//		else
//		{
//			setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
//		}
//
//		getShaderProgram()->use();
//	}
//
//
//	CC_NODE_DRAW_SETUP();
//
//	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
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
//	if( m_pmaskTexture )
//	{
//		ccGLBindTexture2DN( 1, m_pmaskTexture->getName() );
//	}
//
//	//glUniform1i(_maskLocation, 1);
//	//glUniform2f(_offsetLocation, offsetWithPosition.x, offsetWithPosition.y); //3
//
//#define kQuadSize sizeof(m_sQuad.bl)
//#ifdef EMSCRIPTEN
//	long offset = 0;
//	setGLBufferData(&m_sQuad, 4 * kQuadSize, 0);
//#else
//	long offset = (long)&m_sQuad;
//#endif // EMSCRIPTEN
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
//
//
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//	CHECK_GL_ERROR_DEBUG();
//
//
//#if CC_SPRITE_DEBUG_DRAW == 1
//	// draw bounding box
//	CCPoint vertices[4]={
//		ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
//		ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
//		ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
//		ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
//	};
//	ccDrawPoly(vertices, 4, true);
//#elif CC_SPRITE_DEBUG_DRAW == 2
//	// draw texture box
//	CCSize s = this->getTextureRect().size;
//	CCPoint offsetPix = this->getOffsetPosition();
//	CCPoint vertices[4] = {
//		ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
//		ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
//	};
//	ccDrawPoly(vertices, 4, true);
//#endif // CC_SPRITE_DEBUG_DRAW
//
//	CC_INCREMENT_GL_DRAWS(1);
//
//	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
//
//	glActiveTexture(GL_TEXTURE0);   /// 重新激活0层贴图
//}
//-------------------------------------------------------------------------
void CMaskSprite::setOffset( const CCPoint& ptOffset )
{
	m_ptOffset = ptOffset;
	m_bUpdate = true;
}
//-------------------------------------------------------------------------
/**
@brief 添加点
@param
*/
int CMaskSprite::addPoint( int nPointSize, const cocos2d::ccColor4B color, const CCPoint& pos )
{
	static int s_n_Point_ID = 0;
	if( ++s_n_Point_ID == 0 )
	{
		++s_n_Point_ID;
	}
	m_mapPoint[s_n_Point_ID] = SPointInfo( nPointSize, color, pos );
	m_bUpdate = true;
	return s_n_Point_ID;
}
//-------------------------------------------------------------------------
/**
@brief 删除点 
@param
*/
void CMaskSprite::removePoint( int nPointID )
{
	m_mapPoint.erase( nPointID );
	m_bUpdate = true;
}
//-------------------------------------------------------------------------
/**
@brief 修改点位置
@param
*/
void CMaskSprite::changePointPos( int nPointID, cocos2d::CCPoint& pos )
{
	PointMap::iterator iter = m_mapPoint.find( nPointID );
	if( iter != m_mapPoint.end() )
	{
		iter->second.pos = pos;
		m_bUpdate = true;
	}
}
//-------------------------------------------------------------------------
/**
@brief 获取指定点位置
@param
*/
void CMaskSprite::getPointPos( int nPointID, CCPoint& pos )
{
	PointMap::iterator iter = m_mapPoint.find( nPointID );
	if( iter != m_mapPoint.end() )
	{
		pos = iter->second.pos;
	}
}
//-------------------------------------------------------------------------
/**
@brief 清除点
@param
*/
void CMaskSprite::clearPoint()
{
	m_mapPoint.clear();
}


NS_CC_END