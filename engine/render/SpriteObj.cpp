#include "SpriteObj.h"
#include "cocos2d.h"
#include <time.h>
//#include "GameObjManager.h"
#include "WareSprite.h"

NS_CC_ENGINE_BEGIN

CSpriteObj::CSpriteObj( uint32 uID )
{
	//m_pParentNode = NULL;
	m_pSprite = NULL;

	m_uID = uID;

	//m_nXPos = 0;
	//m_nYpos = 0;

	m_nTag = 0;
}
//------------------------------------------------------------------------------
CSpriteObj::~CSpriteObj(void)
{
}
//------------------------------------------------------------------------------
/**
@brief 创建
@param
*/
bool CSpriteObj::create( const char* szPicFileName, const CCRect& rect, CCPoint ptOffset )
{
	if( szPicFileName == NULL || szPicFileName[0] == NULL )
	{
		return false;
	}

	if( rect.getMinX() == 0 && rect.getMinY() == 0 && rect.getMaxX() == 0 && rect.getMaxY() == 0 )
	{
		m_pSprite = CWareSprite::create(szPicFileName);
	}
	else
	{
		m_pSprite = CWareSprite::create(szPicFileName,rect);
	}

	if( m_pSprite == NULL )
	{
		return false;
	}

	addChild( m_pSprite );

	m_pSprite->setPosition( ptOffset );

	m_strPicFileName = szPicFileName;

	static char szBuff[1024] = {0};
	sprintf( szBuff, "%s_%d_%d_%d_%d", m_strPicFileName.c_str(), rect.getMinX(), rect.getMaxX(), rect.getMinY(), rect.getMaxY() );
	m_stFeatureString = szBuff;

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 获取偏移
@param
*/
void CSpriteObj::setOffset( CCPoint& ptOffet )
{
	if( m_pSprite )
	{
		m_pSprite->setPosition( ptOffet );
	}
}
//-------------------------------------------------------------------------
/**
@brief 获取偏移
@param
*/
void CSpriteObj::getOffset( CCPoint& ptOffet )
{
	if( m_pSprite )
	{
		ptOffet = m_pSprite->getPosition();
	}
}
//------------------------------------------------------------------------------
/**
@brief 销毁
@param
*/
void CSpriteObj::onExit()
{
	if( m_pSprite )
	{

		/// 删除贴图，释放内存
		CCTextureCache::sharedTextureCache()->removeTexture( m_pSprite->getTexture() );

		removeChild( m_pSprite, true );
		//m_pSprite->release(); /// m_pSprite 是autorelease的资源，所以不需要主动调用release
		m_pSprite = NULL;
	}
}
//-------------------------------------------------------------------------
/**
@brief 更新
@param
*/
void CSpriteObj::update( float dt )
{
}
//-------------------------------------------------------------------------
/**
@brief 给对象加色
@param
*/
void CSpriteObj::setColor( const ccColor3B& color )
{
	if( m_pSprite == NULL )
	{
		return;
	}

	m_pSprite->setColor( color );
}
//------------------------------------------------------------------------------
/**
@brief 设置透明度
@param byAlpha 透明度
*/
void CSpriteObj::setOpacity( BYTE byAlpha )
{
	if( m_pSprite == NULL )
	{
		return;
	}

	m_pSprite->setOpacity( byAlpha );	
}
//-------------------------------------------------------------------------
/**
@brief 获取该对象的特征串(全局唯一)
@param
*/
void CSpriteObj::getFeatureString( std::string& strFeature )
{
	strFeature = m_stFeatureString;
}
//-------------------------------------------------------------------------
/**
@brief 碰撞检测
@param
*/
bool CSpriteObj::hitTest( const CCPoint& pt )
{
	if( m_pSprite == NULL )
	{
		return false;
	}

	CCRect rect = m_pSprite->boundingBox();
	return rect.containsPoint( pt );
}
//-------------------------------------------------------------------------
/**
@brief 碰撞检测
@param
*/
bool CSpriteObj::hitTest( const CCRect& rect )
{
	if( m_pSprite == NULL )
	{
		return false;
	}

	CCRect rc = m_pSprite->boundingBox();
	return rc.intersectsRect(rect);
}
//-------------------------------------------------------------------------
/**
@brief 获取对象包围盒
@param
*/
bool CSpriteObj::getBoundingBox( CCRect& boundBox )
{ 
	if( m_pSprite )
	{
		boundBox = m_pSprite->boundingBox();
		boundBox = CCRectApplyAffineTransform(boundBox, nodeToParentTransform());
		return true;
	}

	return false; 
}

NS_CC_ENGINE_END