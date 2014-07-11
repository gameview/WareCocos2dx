#include "AniSpriteObj.h"
#include "cocos2d.h"
#include <time.h>
#include "AnimationResManager.h"
#include "WareSprite.h"
#include "RenderCommon.h"

NS_CC_ENGINE_BEGIN

CAniSpriteObj::CAniSpriteObj( uint32 uID )
{
	//m_pParentNode = NULL;
	m_pSprite = NULL;

	m_uID = uID;

	//m_nXPos = 0;
	//m_nYpos = 0;

	//m_nTag = 0;

	//m_pActionCallBack = NULL;
}
//------------------------------------------------------------------------------
CAniSpriteObj::~CAniSpriteObj(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 
@param
*/
void CAniSpriteObj::onExit()
{
	if( m_pSprite ) /// m_pSprite 做了autorelease
	{
		removeChild( m_pSprite );
		m_pSprite = NULL;
	}
}
//------------------------------------------------------------------------------
/**
@brief 创建
@param
*/
bool CAniSpriteObj::create( const char* szAniFileName, CCPoint ptOffset )
{
	if( szAniFileName == NULL || szAniFileName[0] == NULL )
	{
		return false;
	}

	m_pSprite = CWareSprite::create();

	if( m_pSprite == NULL )
	{
		return false;
	}

	CAnimationRes* pAniRes = CAnimationResManager::Instance().getAnimationRes( szAniFileName );
	m_Animate.setAnimationRes( pAniRes );
	m_Animate.setTarget( m_pSprite );

	addChild( m_pSprite );
	m_pSprite->setPosition( ptOffset );

	m_strPicFileName = szAniFileName;

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 获取偏移
@param
*/
void CAniSpriteObj::setOffset( CCPoint& ptOffet )
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
void CAniSpriteObj::getOffset( CCPoint& ptOffet )
{
	if( m_pSprite )
	{
		ptOffet = m_pSprite->getPosition();
	}
}
////------------------------------------------------------------------------------
///**
//@brief 销毁
//@param
//*/
//void CAniSpriteObj::destroy()
//{
//	//if( m_pParentNode != NULL )
//	//{
//	//	m_pParentNode->removeChild( m_pSprite, true );
//	//}
//
//	m_Animate.reset();
//
//	if( m_pSprite )
//	{
//		m_pSprite->release();
//	}
//}
//-------------------------------------------------------------------------
/**
@brief 更新
@param
*/
void CAniSpriteObj::update( float dt )
{
	m_Animate.update( dt );
}
//------------------------------------------------------------------------------
/**
@brief 给对象加色
@param
*/
void CAniSpriteObj::setColor( const ccColor3B& color )
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
void CAniSpriteObj::setOpacity( BYTE byAlpha )
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
void CAniSpriteObj::getFeatureString( std::string& strFeature )
{
	//static char szBuff[1024] = {0};

	//struct cc_timeval now;  // 秒，毫秒
	//CCTime::gettimeofdayCocos2d(&now, NULL);

	//CCRect rect = m_pSprite->boundingBox();

	strFeature = m_strPicFileName;

	//sprintf( szBuff, "%s_%d_%d_%d_%d", m_strPicFileName.c_str(), rect.getMinX(), rect.getMaxX(), rect.getMinY(), rect.getMaxY() );
}
//-------------------------------------------------------------------------
/**
@brief 碰撞检测
@param
*/
bool CAniSpriteObj::hitTest( const CCPoint& pt )
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
bool CAniSpriteObj::hitTest( const CCRect& rect )
{
	if( m_pSprite == NULL )
	{
		return false;
	}

	CCRect rc = m_pSprite->boundingBoxReal();
	return rc.intersectsRect(rect);
}
//-------------------------------------------------------------------------
/**
@brief 获取对象包围盒
@param
*/
bool CAniSpriteObj::getBoundingBox( CCRect& boundBox )
{ 
	if( m_pSprite )
	{
		boundBox = m_pSprite->boundingBoxReal();
		boundBox = CCRectApplyAffineTransform(boundBox, nodeToParentTransform());
		return true;
	}

	return false; 
}

NS_CC_ENGINE_END