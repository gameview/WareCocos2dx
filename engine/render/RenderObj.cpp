#include "cocos2d.h"
#include "RenderObj.h"
#include <time.h>
#include "RenderObjManager.h"
#include "IRenderable.h"
#include "IRenderText.h"
#include "IAnimation.h"
#include "RenderSystem.h"
#include "RenderableManager.h"
#include "RenderMap.h"
#include "SceneManager.h"
#include "RenderCommon.h"
#include "StringConvert.h"
#include "StringUtility.h"

NS_CC_ENGINE_BEGIN

CRenderObj::CRenderObj( uint32 uID )
: m_strCurrentAniName("")
, m_strObjFileName("")
{
	m_uID = uID;

	m_pUserData = NULL;
	m_pRootRenderable = NULL;
	m_pBodyRenderable = NULL;
	m_pRenderObjModel = NULL;

	m_bDrag = false;
	m_bSelected = false;
	m_bNeedUpdate = false;
	m_bInScene = false;

	m_ptPos = CCPointMake(0,0);
	m_ptOffset = CCPointMake(0,0);

	m_mapPart.clear();	
	//m_mapLocator.clear();
	m_mapEffect.clear();
	m_mapLinkObj.clear();
}
//------------------------------------------------------------------------------
CRenderObj::~CRenderObj(void)
{
	m_mapPart.clear();
	m_mapEffect.clear();
	m_mapLinkObj.clear();
}
//------------------------------------------------------------------------------
/**
@brief 创建
@param
*/
bool CRenderObj::create( const char* szObjFileName )
{
	if( szObjFileName == NULL || szObjFileName[0] == NULL )
	{
		return false;
	}

	std::string strPathName,strFileName,strExtName;
	StringUtility::parseFileName( szObjFileName, strPathName, strFileName, strExtName );
	if( strExtName != "obj" )
	{
		CCRect rect;
		ISprite* pSprite = CRenderableManager::Instance().createSpriteObj( szObjFileName, rect);
		if( pSprite == NULL )
		{
			return false;
		}

		addChild( pSprite );
		pSprite->release();
		m_strObjFileName = szObjFileName;
		m_pRootRenderable = pSprite;
		return true;
	}

	createPart();
	createEffect();

	m_strObjFileName = szObjFileName;
	
	return true;
}
//------------------------------------------------------------------------------
/**
@brief 销毁
@param
*/
void CRenderObj::onExit()
{
	removeAllChildren();

	if( m_pRenderObjModel )
	{
		m_pRenderObjModel->release();
		m_pRenderObjModel = NULL;
	}

	//detach();

	//m_mapPart.clear();
	//m_mapEffect.clear();
	//m_mapLocator.clear();
	m_mapLinkObj.clear();
}
//-------------------------------------------------------------------------
/**
@brief 更新
@param
*/
void CRenderObj::update( float dt )
{
	ObjPartMap::iterator itPart = m_mapPart.begin();
	for( ; itPart != m_mapPart.end(); ++itPart )
	{
		if( itPart->second )
		{
			itPart->second->update( dt );
		}
	}

	ObjEffectMap::iterator itEffect = m_mapEffect.begin();		/// 特效数据
	for( ; itEffect != m_mapEffect.end(); ++itEffect )
	{
		if( itEffect->second )
		{
			itEffect->second->update( dt );
		}
	}

	LinkObjMap::iterator itLinkObj = m_mapLinkObj.begin();		/// 链接对象
	for( ; itLinkObj != m_mapLinkObj.end(); ++itLinkObj )
	{
		if( itLinkObj->second.pRenderObj )
		{
			itLinkObj->second.pRenderObj->update( dt );
		}
	}
}
//------------------------------------------------------------------------------
/**
@brief 绑定到场景上
@param
*/
void CRenderObj::attach( CCNode* pParentNode, int nZOrder, int nTag )
{
	if( pParentNode == NULL )
	{
		return;
	}

	pParentNode->addChild( this, nZOrder, nTag );
}

/**
@brief 从场景上解除
@param
*/
void CRenderObj::detach(bool bRemove)
{
	CCNode* pParentNode = getParent();
	if( pParentNode == NULL )
	{
		return;
	}

	if( !bRemove )
	{
		/// 增加引用计数，需要外部再调用release
		retain();
	}
	pParentNode->removeChild( this );
}
//-------------------------------------------------------------------------
/**
@brief 释放
@param
*/
void CRenderObj::drop()
{
	release();
}
//------------------------------------------------------------------------------
/**
@brief 设置位置
@param
*/
void CRenderObj::setPos(const CCPoint& ptLoc )
{
	if( m_bInScene )
	{
		CRenderMap* pRenderMap = CSceneManager::Instance().getCurrentRenderMap();
		if( pRenderMap )
		{
			pRenderMap->setObjPos( m_uID, ptLoc );
		}
	}
	else
	{
		setPosition( ptLoc );
		//setZOrder( (int)-ptLoc.y );
	}

	m_ptPos = ptLoc;
}
//------------------------------------------------------------------------------
/**
@brief 获取位置
@param
*/
void CRenderObj::getPos( CCPoint& ptLoc )
{
	ptLoc = m_ptPos;
}
//-------------------------------------------------------------------------
/**
@brief 设置旋转
@param
*/
void CRenderObj::setRotate( float fRotate )
{
	setRotation( fRotate );
}
//-------------------------------------------------------------------------
/**
@brief 获取旋转值
@param
*/
float CRenderObj::getRotate()
{
	return getRotation();
}
//-------------------------------------------------------------------------
/**
@brief 设置缩放
@param
*/
void CRenderObj::setZoom( float fZoom )
{
	setScale( fZoom );
}
//-------------------------------------------------------------------------
/**
@brief 获取缩放
@param
*/
float CRenderObj::getZoom()
{
	return getScale();
}
//-------------------------------------------------------------------------
/**
@brief 给对象加色
@param
*/
void CRenderObj::setColor( const ccColor3B& color )
{
	/// 对象部件
	ObjPartMap::iterator itPart = m_mapPart.begin();
	for( ; itPart != m_mapPart.end(); ++itPart )
	{
		if( itPart->second )
		{
			itPart->second->setColor( color );
		}
	}

	ObjEffectMap::iterator itEffect = m_mapEffect.begin();		/// 特效数据
	for( ; itEffect != m_mapEffect.end(); ++itEffect )
	{
		if( itEffect->second )
		{
			itEffect->second->setColor( color );
		}
	}

	LinkObjMap::iterator itLinkObj = m_mapLinkObj.begin();		/// 链接对象
	for( ; itLinkObj != m_mapLinkObj.end(); ++itLinkObj )
	{
		if( itLinkObj->second.pRenderObj )
		{
			itLinkObj->second.pRenderObj->setColor( color );
		}
	}
}
//------------------------------------------------------------------------------
/**
@brief 设置透明度
@param
*/
void CRenderObj::setOpacity( BYTE byAlpha )
{
	/// 对象部件
	ObjPartMap::iterator itPart = m_mapPart.begin();
	for( ; itPart != m_mapPart.end(); ++itPart )
	{
		if( itPart->second )
		{
			itPart->second->setOpacity( byAlpha );
		}
	}

	ObjEffectMap::iterator itEffect = m_mapEffect.begin();		/// 特效数据
	for( ; itEffect != m_mapEffect.end(); ++itEffect )
	{
		if( itEffect->second )
		{
			itEffect->second->setOpacity( byAlpha );
		}
	}

	LinkObjMap::iterator itLinkObj = m_mapLinkObj.begin();		/// 链接对象
	for( ; itLinkObj != m_mapLinkObj.end(); ++itLinkObj )
	{
		if( itLinkObj->second.pRenderObj )
		{
			itLinkObj->second.pRenderObj->setOpacity( byAlpha );
		}
	}
}
//------------------------------------------------------------------------------
/**
@brief 设置是否允许update
@param
*/
void CRenderObj::setEnableUpdate( bool bEnable )
{
	if( m_bNeedUpdate == bEnable )
	{
		return;
	}

	if( bEnable )
	{
		scheduleUpdate();
	}
	else
	{
		unscheduleUpdate();
	}

	m_bNeedUpdate = bEnable;
}
//------------------------------------------------------------------------------
/**
@brief 设置是否可见
@param
*/
void CRenderObj::visible( bool bVisible )
{
	setVisible( bVisible );
}
//-------------------------------------------------------------------------
/**
@brief 获取该对象的特征串(全局唯一)
@param
*/
void CRenderObj::getFeatureString( std::string& strFeature )
{
	//static char szBuff[1024] = {0};

	////struct cc_timeval now;  // 秒，毫秒
	////CCTime::gettimeofdayCocos2d(&now, NULL);

	//if( m_pRenderable == NULL )
	//{
	//	return;
	//}

	//m_pRenderable->getFeatureString( strFeature );

	strFeature = m_strObjFileName;

	//CCRect rect = m_pRenderable->boundingBox();

	//sprintf( szBuff, "%s_%d_%d_%d_%d", m_strObjFileName.c_str(), rect.getMinX(), rect.getMaxX(), rect.getMinY(), rect.getMaxY() );
}
//-------------------------------------------------------------------------
/**
@brief 碰撞检测
@param
*/
bool CRenderObj::hitTest( const CCPoint& pt )
{
	if( m_pRootRenderable == NULL )
	{
		return false;
	}

	CCRect rect;
	m_pRootRenderable->getBoundingBox( rect );
	rect = CCRectApplyAffineTransform(rect, nodeToParentTransform());
	
	if( rect.containsPoint( pt ) )
	{
		return true;
	}

	LinkObjMap::iterator iter =	m_mapLinkObj.begin();
	for( ; iter != m_mapLinkObj.end(); ++iter )
	{
		if( iter->second.pRenderObj == NULL )
		{
			continue;
		}

		if( iter->second.pRenderObj->hitTest( pt ) )
		{
			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------
/**
@brief 碰撞检测
@param
*/
bool CRenderObj::hitTest( const CCRect& rect )
{
	if( m_pRootRenderable == NULL )
	{
		return false;
	}

	CCRect rc;
	m_pRootRenderable->getBoundingBox( rc );
	rc = CCRectApplyAffineTransform(rc, nodeToParentTransform());
	
	if( rc.intersectsRect(rc) )
	{
		return true;
	}

	LinkObjMap::iterator iter =	m_mapLinkObj.begin();
	for( ; iter != m_mapLinkObj.end(); ++iter )
	{
		if( iter->second.pRenderObj == NULL )
		{
			continue;
		}

		if( iter->second.pRenderObj->hitTest( rect ) )
		{
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------
/**
@brief 设置动画播放速度
@param
*/
void CRenderObj::setAnimationSpeed( float fSpeed )
{
	ObjPartMap::iterator itPart = m_mapPart.begin();
	for( ; itPart != m_mapPart.end(); ++itPart )
	{
		if( itPart->second && ( itPart->second->getType() == RenderableType_AniSprite 
			|| itPart->second->getType() == RenderableType_DragonBone ) )
		{
			IAniSprite* pObj = (IAniSprite*)itPart->second;
			IAnimation* pAni = pObj->getAnimation();
			if( pAni )
			{
				pAni->speed( fSpeed );
			}
		}
	}

	ObjEffectMap::iterator itEffect = m_mapEffect.begin();		/// 特效数据
	for( ; itEffect != m_mapEffect.end(); ++itEffect )
	{
		if( itEffect->second && ( itEffect->second->getType() == RenderableType_AniSprite 
			|| itEffect->second->getType() == RenderableType_DragonBone ) )
		{
			IAniSprite* pObj = (IAniSprite*)itEffect->second;
			IAnimation* pAni = pObj->getAnimation();
			if( pAni )
			{
				pAni->speed( fSpeed );
			}
		}
	}

	LinkObjMap::iterator itLinkObj = m_mapLinkObj.begin();		/// 链接对象
	for( ; itLinkObj != m_mapLinkObj.end(); ++itLinkObj )
	{
		if( itLinkObj->second.pRenderObj )
		{
			itLinkObj->second.pRenderObj->setAnimationSpeed( fSpeed );
		}
	}
}
//------------------------------------------------------------------------------
/**
@brief 设置动画播放回调
@param
*/
void CRenderObj::setAnimationCallback( IAnimationCallback* pAniCallBack, void* pContext )
{
	if( m_pRootRenderable )
	{
		if( m_pRootRenderable && ( m_pRootRenderable->getType() == RenderableType_AniSprite 
			|| m_pRootRenderable->getType() == RenderableType_DragonBone ) )
		{
			IAniSprite* pObj = (IAniSprite*)m_pRootRenderable;
			IAnimation* pAni = pObj->getAnimation();
			if( pAni )
			{
				pAni->setAniCallback( pAniCallBack, pContext );
			}
		}
	}
}
//------------------------------------------------------------------------------
/**
@brief 播放动画 如果该动作处于停止状态，则继续播放
@param[in] szAniName 动作名称
@param[in] fSpeed 播放速度
@param[in] nLoops 循环次数
@param[in] bForceUpdate 是否强制更新动作数据(动作重新播放)
*/
void CRenderObj::playAnimation( const char* szAniName, float fSpeed, int nLoops, bool bForceUpdate )
{
	ObjPartMap::iterator itPart = m_mapPart.begin();
	for( ; itPart != m_mapPart.end(); ++itPart )
	{
		if( itPart->second && ( itPart->second->getType() == RenderableType_AniSprite 
			|| itPart->second->getType() == RenderableType_DragonBone ))
		{
			IAniSprite* pObj = (IAniSprite*)itPart->second;
			IAnimation* pAni = pObj->getAnimation();
			if( pAni )
			{
				//if( pAni->isExistClip(szAniName) )
				//{
				//	itPart->second->setVisible( true );
					pAni->play( szAniName, fSpeed, nLoops, bForceUpdate );
				//}
				//else
				//{
				//	itPart->second->setVisible( false );
				//}
				//if( itPart->first == "Body" )  /// 只给主体设置动画回调
				//{
				//	pAni->setAniCallback( pAniCallBack,  );
				//}
			}
		}
	}

	ObjEffectMap::iterator itEffect = m_mapEffect.begin();		/// 特效数据
	for( ; itEffect != m_mapEffect.end(); ++itEffect )
	{
		if( itEffect->second && ( itEffect->second->getType() == RenderableType_AniSprite 
			|| itEffect->second->getType() == RenderableType_DragonBone ) )
		{
			IAniSprite* pObj = (IAniSprite*)itEffect->second;
			IAnimation* pAni = pObj->getAnimation();
			if( pAni )
			{
				pAni->play( szAniName, fSpeed, nLoops );
			}
		}
	}

	LinkObjMap::iterator itLinkObj = m_mapLinkObj.begin();		/// 链接对象
	for( ; itLinkObj != m_mapLinkObj.end(); ++itLinkObj )
	{
		if( itLinkObj->second.pRenderObj )
		{
			itLinkObj->second.pRenderObj->playAnimation( szAniName, fSpeed, nLoops, bForceUpdate );
		}
	}

	m_strCurrentAniName = szAniName;
}
//------------------------------------------------------------------------------
/**
@brief 播放动画
@param[in] szAniName 动作名称
@param[in] pAniCallBack 动画播放回调
@param[in] nStopFrame 停止的帧号
@param[in] fSpeed 播放速度
*/
void CRenderObj::playAndStop( const char* szAniName, int nStopFrame, float fSpeed )
{
	//m_strCurrentAniName = szAniName;

	std::string strAniName;
	if( szAniName == NULL )
	{
		strAniName = m_strCurrentAniName;
	}
	else
	{
		strAniName = szAniName;
	}

	ObjPartMap::iterator itPart = m_mapPart.begin();
	for( ; itPart != m_mapPart.end(); ++itPart )
	{
		if( itPart->second && itPart->second->getType() == RenderableType_AniSprite )
		{
			IAniSprite* pObj = (IAniSprite*)itPart->second;
			IAnimation* pAni = pObj->getAnimation();
			if( pAni )
			{
				pAni->gotoFrame(nStopFrame);
			}
		}
	}

	ObjEffectMap::iterator itEffect = m_mapEffect.begin();		/// 特效数据
	for( ; itEffect != m_mapEffect.end(); ++itEffect )
	{
		if( itEffect->second && itEffect->second->getType() == RenderableType_AniSprite )
		{
			IAniSprite* pObj = (IAniSprite*)itEffect->second;
			IAnimation* pAni = pObj->getAnimation();
			if( pAni )
			{
				pAni->gotoFrame(nStopFrame);
			}
		}
	}

	LinkObjMap::iterator itLinkObj = m_mapLinkObj.begin();		/// 链接对象
	for( ; itLinkObj != m_mapLinkObj.end(); ++itLinkObj )
	{
		if( itLinkObj->second.pRenderObj )
		{
			itLinkObj->second.pRenderObj->playAndStop(szAniName,nStopFrame,fSpeed);
		}
	}

	m_strCurrentAniName = szAniName;
}
//-------------------------------------------------------------------------
/**
@brief 获取当前帧
@param
*/
int CRenderObj::getCurFrame()
{
	if( m_pRootRenderable == NULL )
	{
		return -1;
	}

	IAniSprite* pObj = (IAniSprite*)m_pRootRenderable;
	IAnimation* pAni = pObj->getAnimation();
	if( pAni )
	{
		return pAni->getCurFrame();
	}
	return -1;
}
//------------------------------------------------------------------------------
/**
@brief 动画暂停(游戏中不会使用,工具中使用)
@param
*/
void CRenderObj::pause()
{
	ObjPartMap::iterator itPart = m_mapPart.begin();
	for( ; itPart != m_mapPart.end(); ++itPart )
	{
		if( itPart->second && ( itPart->second->getType() == RenderableType_AniSprite 
			|| itPart->second->getType() == RenderableType_DragonBone ) )
		{
			IAniSprite* pObj = (IAniSprite*)itPart->second;
			IAnimation* pAni = pObj->getAnimation();
			if( pAni )
			{
				pAni->pause();
			}
		}
	}

	ObjEffectMap::iterator itEffect = m_mapEffect.begin();		/// 特效数据
	for( ; itEffect != m_mapEffect.end(); ++itEffect )
	{
		if( itEffect->second && ( itEffect->second->getType() == RenderableType_AniSprite 
			|| itEffect->second->getType() == RenderableType_DragonBone ) )
		{
			IAniSprite* pObj = (IAniSprite*)itEffect->second;
			IAnimation* pAni = pObj->getAnimation();
			if( pAni )
			{
				pAni->pause();
			}
		}
	}

	LinkObjMap::iterator itLinkObj = m_mapLinkObj.begin();		/// 链接对象
	for( ; itLinkObj != m_mapLinkObj.end(); ++itLinkObj )
	{
		if( itLinkObj->second.pRenderObj )
		{
			itLinkObj->second.pRenderObj->pause();
		}
	}
}
//------------------------------------------------------------------------------
/**
@brief 获取动画列表
@param
*/
void CRenderObj::getAnimations( std::list<std::string>& lstAnimation )
{
	if( m_pRootRenderable == NULL )
	{
		return;
	}

	if( m_pRootRenderable->getType() != RenderableType_AniSprite 
		&& m_pRootRenderable->getType() != RenderableType_DragonBone )
	{
		return;
	}

	IAnimation* pAni = ((IAniSprite*)m_pRootRenderable)->getAnimation();
	if( pAni )
	{
		pAni->getAnimationList( lstAnimation );
	}

	// part的设计本意是作为对象的一部分存在的 所以Root是主对象，动作只在主部分里面查找 
	// 但是新的需要求需要一个对象，部分动作使用序列帧，部分动作使用DragonBone来实现,
	// 因此增加了个Body部件，来作为补充
	if( m_pBodyRenderable == NULL )
	{
		return;
	}

	if( m_pBodyRenderable->getType() != RenderableType_AniSprite 
		&& m_pBodyRenderable->getType() != RenderableType_DragonBone )
	{
		return;
	}
	pAni = ((IAniSprite*)m_pBodyRenderable)->getAnimation();
	if( pAni )
	{
		pAni->getAnimationList( lstAnimation );
	}


}
//------------------------------------------------------------------------------
/**
@brief 获取游戏对象高度
@param[in] uint32 uid
*/
float CRenderObj::getHeight()
{
	if( m_pRenderObjModel == NULL )
	{
		return 150.0f;
	}

	std::string strLocator = "head";
	//CCPoint ptLoc;
	//if( !strLocator.empty() )
	//{
	CRenderObjModel::ObjLocatorMap::iterator itLoc = m_pRenderObjModel->m_mapLocator.find( strLocator );
	if( itLoc != m_pRenderObjModel->m_mapLocator.end() )
	{
		return itLoc->second.ptOffset.y;
	}	
	//}

	return 150.0f; /// 默认高度
}
//-------------------------------------------------------------------------
/**
@brief 获取绑定点的偏移
@param szLocator 绑定名
@param ptOffset 偏移
*/
void CRenderObj::getLocator( const char* szLocator, CCPoint& ptOffset )
{
	if( m_pRenderObjModel == NULL || szLocator == NULL )
	{
		return;
	}

	std::string strLocator = szLocator;
	CRenderObjModel::ObjLocatorMap::iterator itLoc = m_pRenderObjModel->m_mapLocator.find( strLocator );
	if( itLoc != m_pRenderObjModel->m_mapLocator.end() )
	{
		ptOffset = itLoc->second.ptOffset;
	}
}
//------------------------------------------------------------------------------
/**
@brief 挂接对象
@param[in] szLinkobjName link对象名 必须是obj
@param[in] szLocator  绑定点名称
@param[in] nDepth 深度 
@param[in] ptOffset 与宿主对象偏移
*/
unsigned int CRenderObj::addLinkObj( const char* szLinkobjName, const char* szLocator, int nDepth, float fScale, cocos2d::ccColor4B color )
{
	if( m_pRenderObjModel == NULL )
	{
		return 0;
	}

	IRenderSystem* pRenderSys = getRenderSystem();
	if( pRenderSys == NULL )
	{
		return 0;
	}

	LinkObj obj;
	obj.pRenderObj = (CRenderObj*)pRenderSys->createRenderObj( szLinkobjName );
	if( obj.pRenderObj == NULL )
	{
		return 0;
	}

	obj.strLoccator = "";
	CCPoint ptLoc = CCPointZero;
	if( szLocator != NULL )
	{
		obj.strLoccator = szLocator;
		CRenderObjModel::ObjLocatorMap::iterator itLoc = m_pRenderObjModel->m_mapLocator.find( obj.strLoccator );
		if( itLoc != m_pRenderObjModel->m_mapLocator.end() )
		{
			ptLoc = itLoc->second.ptOffset;
		}
	}

	addChild( obj.pRenderObj, nDepth );
	obj.pRenderObj->release();
	obj.pRenderObj->setPosition( ptLoc );
	obj.pRenderObj->setColor( ccc3(color.r,color.g,color.b) );
	obj.pRenderObj->setOpacity( color.a );
	obj.pRenderObj->setScale( fScale );

	m_mapLinkObj[obj.pRenderObj->getID()] = obj;
	return obj.pRenderObj->getID();
}
//------------------------------------------------------------------------------
/**
@brief 挂接对象
@param[in] szLinkobjName link对象名 必须是obj
@param[in] szLocator  绑定点名称
@param[in] nDepth 深度 
@param[in] ptOffset 与宿主对象偏移
*/
unsigned int CRenderObj::addLinkObj( IRenderObj* pRenderObj, const char* szLocator, int nDepth, float fScale, cocos2d::ccColor4B color )
{
	if( m_pRenderObjModel == NULL || pRenderObj == NULL )
	{
		return 0;
	}

	LinkObj obj;
	obj.pRenderObj = (CRenderObj*)pRenderObj;
	if( obj.pRenderObj == NULL )
	{
		return 0;
	}

	obj.strLoccator = "";
	CCPoint ptLoc = CCPointZero;
	if( szLocator != NULL )
	{
		obj.strLoccator = szLocator;
		CRenderObjModel::ObjLocatorMap::iterator itLoc = m_pRenderObjModel->m_mapLocator.find( obj.strLoccator );
		if( itLoc != m_pRenderObjModel->m_mapLocator.end() )
		{
			ptLoc = itLoc->second.ptOffset;
		}
	}

	addChild( obj.pRenderObj, nDepth );
	obj.pRenderObj->release();
	obj.pRenderObj->setPosition( ptLoc );
	obj.pRenderObj->setColor( ccc3(color.r,color.g,color.b) );
	obj.pRenderObj->setOpacity( color.a );
	obj.pRenderObj->setScale( fScale );

	m_mapLinkObj[obj.pRenderObj->getID()] = obj;
	return obj.pRenderObj->getID();
}
//------------------------------------------------------------------------------
/**
@brief 删除挂接对象
@param[in] uid 游戏对象ID
@param[in] szLinkobjName link对象名 可以是 oms oss或者obj
*/
void CRenderObj::removeLinkObj( unsigned int uLinkObjID )
{
	LinkObjMap::iterator iter = m_mapLinkObj.find( uLinkObjID );
	if( iter != m_mapLinkObj.end() )
	{
		if( iter->second.pRenderObj )
		{
			removeChild( iter->second.pRenderObj );
			m_mapLinkObj.erase( iter );
		}
	}
}
//-------------------------------------------------------------------------
/**
@brief 删除挂接对象
@param[in] uid 游戏对象ID
@param[in] szLinkobjName link对象名 可以是 oms oss或者obj
*/
IRenderObj* CRenderObj::removeLinkObj( unsigned int uLinkObjID, bool bRemove )
{
	IRenderObj* pObj = NULL;
	LinkObjMap::iterator iter = m_mapLinkObj.find( uLinkObjID );
	if( iter != m_mapLinkObj.end() )
	{
		if( iter->second.pRenderObj )
		{
			if( !bRemove )
			{
				iter->second.pRenderObj->retain();
			}

			pObj = iter->second.pRenderObj;
			removeChild( iter->second.pRenderObj );
			m_mapLinkObj.erase( iter );
		}
	}

	return pObj;
}
//-------------------------------------------------------------------------
/**
@brief 删除挂接对象
@param[in] uid 游戏对象ID
@param[in] szLinkobjName link对象名 可以是 oms oss或者obj
*/
void CRenderObj::removeAllLinkObj()
{
	LinkObjMap::iterator iter = m_mapLinkObj.begin();
	for( ; iter != m_mapLinkObj.end(); ++iter )
	{
		if( iter->second.pRenderObj )
		{
			removeChild( iter->second.pRenderObj );
		}
	}

	m_mapLinkObj.clear();
}
//-------------------------------------------------------------------------
/**
@brief 获取挂接对象
@param
*/
IRenderObj*	CRenderObj::getLinkObj( unsigned int uLinkObjID )
{
	LinkObjMap::iterator iter = m_mapLinkObj.find( uLinkObjID );
	if( iter != m_mapLinkObj.end() )
	{
		return iter->second.pRenderObj;
	}

	return NULL;
}
//------------------------------------------------------------------------------
/**
@brief 更新部件
@param szPartObjName 部件对象名
@param szPartPlaceName 部件位置名称
@param nDepth 部件层深度
*/
void CRenderObj::changePart( const char* szPartObjName, const char* szPartPlaceName, int nDepth )
{
	if( szPartObjName == NULL || szPartPlaceName == NULL || m_pRenderObjModel == NULL || m_pRenderObjModel == NULL )
	{
		return;
	}

	std::string strPartObjName = szPartObjName;
	std::string strPartName = szPartPlaceName;
	if( strPartName.empty() || strPartObjName.empty() )
	{
		return;
	}

	CRenderObjModel::ObjPartMap::iterator iter = m_pRenderObjModel->m_mapPart.find( szPartPlaceName );
	if( iter == m_pRenderObjModel->m_mapPart.end() )
	{
		m_pRenderObjModel->addPart( szPartPlaceName, szPartObjName, nDepth );
		iter = m_pRenderObjModel->m_mapPart.find( szPartPlaceName );
	}
	else
	{
		if( iter->second.strValue == strPartObjName && iter->second.nDepth == nDepth )
		{
			return;
		}
	}

	float fAniSpeed = 1.0f;
	if( m_pRootRenderable && ( m_pRootRenderable->getType() == RenderableType_AniSprite 
		|| m_pRootRenderable->getType() == RenderableType_DragonBone ) )
	{
		IAniSprite* pObj = (IAniSprite*)m_pRootRenderable;
		IAnimation* pAni = pObj->getAnimation();
		if( pAni )
		{
			fAniSpeed = pAni->getSpeed();
		}
	}

	std::string strPart = iter->second.strValue;
	iter->second.strValue = szPartObjName;
	iter->second.nDepth = nDepth;
	if( !_createPart( iter->second ) )
	{
		iter->second.strValue = strPart;
	}

	// 播放动画
	playAnimation( m_strCurrentAniName.c_str(), fAniSpeed, -1, true );
}
//-------------------------------------------------------------------------
/**
@brief 删除特效
*/
void CRenderObj::removeAllEffect()
{
	ObjEffectMap::iterator iter = m_mapEffect.begin();
	for( ; iter != m_mapEffect.end(); ++iter )
	{
		removeChild( iter->second );
	}

	m_mapEffect.clear();
}
////------------------------------------------------------------------------------
///**
//@brief 添加part属性
//@param
//*/
//void CRenderObj::addPart( const std::string& strName, const std::string& strFile )
//{
//	ObjPart part;
//	part.strName = strName;
//	part.strValue = strFile;
//	m_mapPart[strName] = part;
//}
////------------------------------------------------------------------------------
///**
//@brief 删除节点
//@param
//*/
//void CRenderObj::removePart( const std::string& strName )
//{
//	/// 删除部件
//	ObjPartMap::iterator iter = m_mapPart.find( strName );
//	if( iter != m_mapPart.end() )
//	{
//		if( iter->second.pRenderable )
//		{
//			removeChild( iter->second.pRenderable );
//		}
//	}
//
//	m_mapPart.erase( strName );
//}
////------------------------------------------------------------------------------
///**
//@brief 添加节点
//@param
//*/
//void CRenderObj::addLocator( const std::string& strLocatorName, CCPoint& ptOffset )
//{
//	ObjLocator locator;
//	locator.strName = strLocatorName;
//	locator.ptOffset = ptOffset;
//	m_mapLocator[strLocatorName] = locator;
//}
////------------------------------------------------------------------------------
///**
//@brief 删除节点(适用于编辑器)
//@param
//*/
//void CRenderObj::removeLocator( const std::string& strLocatorName )
//{
//	//ObjLocatorMap::iterator iter = m_mapLocator.find( strLocatorName );
//	//if( iter != m_mapPart.end() )
//	//{
//	//	//if( iter->second.pRenderable )
//	//	//{
//	//	//	removeChild( iter->second.pRenderable );
//	//	//}
//	//}
//
//	m_mapLocator.erase( strLocatorName );
//}
////------------------------------------------------------------------------------
///**
//@brief 添加节点
//@param
//*/
//void CRenderObj::addEffect( const std::string& strEffectName, const std::string& strLocatorName, const std::string& strEffectFileName, CCPoint& ptOffset, ccColor4B& color )
//{
//	ObjEffect effect;
//	effect.strName = strEffectName;
//	effect.ptOffset = ptOffset;
//	effect.strLocator = strLocatorName;
//	effect.strFileName = strEffectFileName;
//	effect.color = color;
//	m_mapEffect[strEffectName] = effect;
//}
//------------------------------------------------------------------------------
/**
@brief 更新部件
@param szPartObjName 部件对象名
@param szPartPlaceName 部件位置名称
*/
void CRenderObj::changeEffect( const std::string& strEffectName, const std::string& strLocatorName, 
	const std::string& strEffectFileName, CCPoint& ptOffset, ccColor4B& color, CCPoint& scale, int nDepth )
{
	if( m_pRenderObjModel == NULL )
	{
		return;
	}

	CRenderObjModel::ObjEffectMap::iterator iter = m_pRenderObjModel->m_mapEffect.find( strEffectName );
	if( iter == m_pRenderObjModel->m_mapEffect.end() )
	{
		m_pRenderObjModel->addEffect( strEffectName, strLocatorName, strEffectFileName, ptOffset, color, scale, nDepth );
		iter = m_pRenderObjModel->m_mapEffect.find( strEffectName );
	}

	ObjEffectMap::iterator itObj = m_mapEffect.find( strEffectName );
	if( itObj != m_mapEffect.end() )
	{
		if( itObj->second)
		{
			removeChild( itObj->second );
			itObj->second = NULL;
		}
	}

	iter->second.strName = strEffectName;
	iter->second.ptOffset = ptOffset;
	iter->second.strLocator = strLocatorName;
	iter->second.strFileName = strEffectFileName;
	iter->second.color = color;
	iter->second.scale = scale;
	iter->second.depth = nDepth;

	if( !_createEffect( iter->second ) )
	{

	}
}
//-------------------------------------------------------------------------
/**
@brief 设置模板数据
@param
*/
void CRenderObj::setObjModel( CRenderObjModel* pObjModel )
{
	if( m_pRenderObjModel )
	{
		m_pRenderObjModel->release();
	}

	m_pRenderObjModel = pObjModel;
	if( m_pRenderObjModel )
	{
		m_pRenderObjModel->retain();
	}
}
//-------------------------------------------------------------------------
/**
@brief 删除节点
@param
*/
void CRenderObj::removePart( const std::string& strName, bool bRemoveData )
{
	if( strName == "Main" )
	{
		if( m_pRootRenderable )
		{
			m_pRootRenderable->getOffset( m_pRenderObjModel->m_ptOffset );
			m_pRootRenderable = NULL;
		}
	}

	ObjPartMap::iterator iter = m_mapPart.find( strName );
	if( iter != m_mapPart.end() )
	{
		if( iter->second )
		{
			removeChild( iter->second );
		}
	}

	if( m_pRenderObjModel && bRemoveData )
	{
		m_pRenderObjModel->removePart( strName );
		m_mapPart.erase( strName );
	}
}
//-------------------------------------------------------------------------
/**
@brief 删除节点
@param
*/
void CRenderObj::removeLocator( const std::string& strLocatorName, bool bRemoveData )
{
	if( m_pRenderObjModel == NULL )
	{
		return;
	}

	/// 特效列表
	std::string strEffectName;
	CRenderObjModel::ObjEffectMap::iterator iter = m_pRenderObjModel->m_mapEffect.begin();
	for( ; iter != m_pRenderObjModel->m_mapEffect.end(); ++iter )
	{
		if( iter->second.strLocator == strLocatorName )
		{
			strEffectName = iter->second.strName;

			ObjEffectMap::iterator itEffect = m_mapEffect.find( strEffectName );
			if( itEffect != m_mapEffect.end() )
			{
				if( itEffect->second )
				{
					itEffect->second->setPosition( iter->second.ptOffset );
				}
			}
		}
	}

	/// 挂接对象
	LinkObjMap::iterator itLink = m_mapLinkObj.begin();
    LinkObjMap::iterator itLink2;
	for( ; itLink != m_mapLinkObj.end(); )
	{
        itLink2 = itLink++;
		if( itLink2->second.strLoccator == strLocatorName )
		{
			removeChild( itLink->second.pRenderObj );
			m_mapLinkObj.erase( itLink2 );
		} 
	}

	if( bRemoveData )
	{
		m_pRenderObjModel->removeLocator( strLocatorName );
	}
}
//-------------------------------------------------------------------------
/**
@brief 删除特效
*/
void CRenderObj::removeEffectByName( const char* szEffectObjName )
{
	removeEffect(szEffectObjName,false);
}
//-------------------------------------------------------------------------
/**
@brief 删除特效(数据)
@param
*/
void CRenderObj::removeEffect( const std::string& strEffectName, bool bRemoveData )
{
	if( m_pRenderObjModel == NULL )
	{
		return;
	}

	/// 特效列表
	ObjEffectMap::iterator itEffect = m_mapEffect.find( strEffectName );
	if( itEffect != m_mapEffect.end() )
	{
		if( itEffect->second )
		{
			removeChild( itEffect->second );
			m_mapEffect.erase( itEffect );
		}
	}

	if( bRemoveData )
	{
		m_pRenderObjModel->removeEffect( strEffectName );
	}
}
//------------------------------------------------------------------------------
/**
@brief 创建part
@param
*/
void CRenderObj::createPart()
{
	if( m_pRenderObjModel == NULL )
	{
		return;
	}

	CRenderObjModel::ObjPartMap::iterator iter = m_pRenderObjModel->m_mapPart.begin();
	for( ; iter != m_pRenderObjModel->m_mapPart.end(); ++iter )
	{
		_createPart( iter->second );
	}
}
//------------------------------------------------------------------------------
/**
@brief 创建特效
@param
*/
void CRenderObj::createEffect()
{
	if( m_pRenderObjModel == NULL )
	{
		return;
	}

	CRenderObjModel::ObjEffectMap::iterator iter = m_pRenderObjModel->m_mapEffect.begin();
	for( ; iter != m_pRenderObjModel->m_mapEffect.end(); ++iter )
	{
		_createEffect( iter->second );
	}
}
//------------------------------------------------------------------------------
/**
@brief 保存到文件
@param
*/
void CRenderObj::saveToFile( const std::string& strObjFileName )
{
	if( m_pRenderObjModel )
	{
		if( !m_ptOffset.equals( CCPointZero ) )
		{
			m_pRenderObjModel->m_ptOffset = getOffset();
		}

		m_pRenderObjModel->saveToFile( strObjFileName );
	}
}
//-------------------------------------------------------------------------
/**
@brief 
@param
*/
void CRenderObj::setOffset( CCPoint& ptOffset )
{
	ObjPartMap::iterator itPart = m_mapPart.begin();
	for( ; itPart != m_mapPart.end(); ++itPart )
	{
		if( itPart->second )
		{
			itPart->second->setOffset( ptOffset );
		}
	}

	//CCLOG( "offset:%f,%f", ptOffset.x, ptOffset.y );
	m_ptOffset = ptOffset;
}
//-------------------------------------------------------------------------
/**
@brief 
@param
*/
CCPoint CRenderObj::getOffset()
{
	if( m_pRootRenderable == NULL )
	{
		return CCPointZero;
	}

	CCPoint ptPos;
	m_pRootRenderable->getOffset( ptPos );
	return ptPos;
}
//-------------------------------------------------------------------------
/**
@brief 添加文字渲染对象
@param
*/
bool CRenderObj::addRenderText( IRenderText* pRenderText, std::string& strLocator, const CCPoint& ptOffset )
{
	if( pRenderText == NULL || m_pRootRenderable == NULL )
	{
		return false;
	}

	CCPoint ptLoc = ptOffset;
	if( strLocator != "" )
	{
		CRenderObjModel::ObjLocatorMap::iterator itLoc = m_pRenderObjModel->m_mapLocator.find( strLocator );
		if( itLoc != m_pRenderObjModel->m_mapLocator.end() )
		{
			ptLoc = ptLoc + itLoc->second.ptOffset;
		}
	}

	pRenderText->setPos( ptLoc );
	pRenderText->attach( m_pRootRenderable, 1000 );

	m_setRenderText.insert( pRenderText ); 

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 删除文字渲染对象
@param
*/
void CRenderObj::removeRenderText( IRenderText* pRenderText )
{
	IRenderSystem* pRenderSys = getRenderSystem();
	if( pRenderSys == NULL )
	{
		return;
	}

	if( pRenderText == NULL )
	{
		return;
	}

	m_setRenderText.erase( pRenderText );
	pRenderSys->removeRenderText( pRenderText );
}
//-------------------------------------------------------------------------
/**
@brief 清理对象(部件，特效) 只保留空对象
@param
*/
void CRenderObj::clear()
{
	ObjPartMap::iterator itPart = m_mapPart.begin(); /// 对象部件
	for( ; itPart != m_mapPart.end(); ++itPart )
	{
		if( itPart->second )
		{
			removeChild( itPart->second );
			itPart->second = NULL;
		}
	}

	m_mapPart.clear();

	ObjEffectMap::iterator itEffect = m_mapEffect.begin(); /// 特效数据
	for( ; itEffect != m_mapEffect.end(); ++itEffect )
	{
		if( itEffect->second )
		{
			removeChild( itEffect->second );
			itEffect->second = NULL;
		}
	}
	m_mapEffect.clear();

	removeAllLinkObj();
}
//------------------------------------------------------------------------------
/**
@brief 添加part
@param
*/
bool CRenderObj::_createPart( CRenderObjModel::ObjPart& part )
{
	IRenderable* pRenderable = NULL;
	if( m_pRenderObjModel == NULL )
	{
		return false;
	}

	ObjPartMap::iterator itObj = m_mapPart.find( part.strName );
	if( itObj != m_mapPart.end() )
	{
		removeChild( itObj->second );
		if( part.strName == "Main" )
		{
			m_pRootRenderable = NULL;
		}

		itObj->second = NULL;
	}

	std::string strPathName,strFileName,strExtName;
	StringUtility::parseFileName( part.strValue, strPathName, strFileName, strExtName );
	StringUtility::makeLower( strExtName ); /// 转换成小写，便于下面的判断代码
	if( strExtName == "oms" )  /// xml的动画文件
	{
		/// 动画对象
		pRenderable = CRenderableManager::Instance().createAniSpriteObj( part.strValue.c_str(), m_pRenderObjModel->m_ptOffset );
	}
	else if( strExtName == "oss" || strExtName == "jpg" || strExtName == "png" ) /// 静态图片 jpg和png添加的类型，应当全部转成oss文件
	{
		CCRect rect;
		pRenderable = CRenderableManager::Instance().createSpriteObj( part.strValue.c_str(), rect, m_pRenderObjModel->m_ptOffset );
	}
	else if( strExtName == "fx" ) /// 特效
	{
		pRenderable = CRenderableManager::Instance().createEffectObj( part.strValue.c_str(), m_pRenderObjModel->m_ptOffset );
	}
	else if( strExtName == "josn" || strExtName == "exportjson" )
	{
		pRenderable = CRenderableManager::Instance().createDragobBoneSpriteObj( part.strValue.c_str(), m_pRenderObjModel->m_ptOffset );
	}
	else if( strExtName == "xml" )
	{
		pRenderable = CRenderableManager::Instance().createDragobBoneSpriteObj( part.strValue.c_str(), m_pRenderObjModel->m_ptOffset );
	}

	if( part.strName == "Main" )
	{
		m_pRootRenderable = pRenderable;
	}

	if( part.strName == "Body" )
	{
		m_pBodyRenderable = pRenderable;
	}

	if( pRenderable != NULL )
	{
		addChild( pRenderable, part.nDepth );
		pRenderable->release();
	}

	m_mapPart[part.strName] = pRenderable;
	
	return true;
}
//------------------------------------------------------------------------------
/**
@brief 添加part
@param
*/
bool CRenderObj::_createEffect( CRenderObjModel::ObjEffect& effect )
{
	IRenderable* pRenderable = NULL;
	std::string strPathName,strFileName,strExtName;
	StringUtility::parseFileName( effect.strFileName, strPathName, strFileName, strExtName );
	if( strExtName == "fx" ) /// 特效
	{
		pRenderable = CRenderableManager::Instance().createEffectObj( effect.strFileName.c_str() );
		if( pRenderable == NULL )
		{
			return false;
		}
	}
	else if( strExtName == "oms" )
	{
		pRenderable = CRenderableManager::Instance().createAniSpriteObj( effect.strFileName.c_str() );
		if( pRenderable == NULL )
		{
			return false;
		}
	}
	else if( strExtName == "oss" )
	{
		CCRect rect;
		pRenderable = CRenderableManager::Instance().createSpriteObj( effect.strFileName.c_str(), rect );
		if( pRenderable == NULL )
		{
			return false;
		}
	}

	CCPoint ptLoc = effect.ptOffset;
	if( effect.strLocator != "" )
	{
		CRenderObjModel::ObjLocatorMap::iterator itLoc = m_pRenderObjModel->m_mapLocator.find( effect.strLocator );
		if( itLoc != m_pRenderObjModel->m_mapLocator.end() )
		{
			ptLoc = ptLoc + itLoc->second.ptOffset;
		}
	}

	addChild( pRenderable, effect.depth );
	pRenderable->release();
	pRenderable->setPosition( ptLoc );
	pRenderable->setColor( ccc3( effect.color.r, effect.color.g, effect.color.b ) );
	pRenderable->setOpacity( effect.color.a );
	pRenderable->setScaleX( effect.scale.x );
	pRenderable->setScaleY( effect.scale.y );

	m_mapEffect[ effect.strName ] = pRenderable;

	return true;
}

NS_CC_ENGINE_END