#include "DragonBoneSpriteObj.h"
#include "StringUtility.h"

NS_CC_ENGINE_BEGIN

std::map< std::string, int >	g_mapArmatureRefrence;	/// 骨骼动画引用计数

CDragonBoneSpriteObj::CDragonBoneSpriteObj(uint32 uID)
: m_pArmature(NULL)
{
	m_strPicFileName = "";
}
//-------------------------------------------------------------------------
CDragonBoneSpriteObj::~CDragonBoneSpriteObj(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 创建
@param szPicFileName 资源文件名
@param ptOffset 偏移值
@return 返回创建成功与否标识
*/
bool CDragonBoneSpriteObj::create( const char* szAniFileName, CCPoint ptOffset )
{
	if( NULL != m_pArmature || szAniFileName == NULL )
	{
		return true;
	}

	m_strPicFileName = szAniFileName;
	/// 动画文件名，解析出对象名称
	if( g_mapArmatureRefrence.find( m_strPicFileName ) == g_mapArmatureRefrence.end() )
	{
		g_mapArmatureRefrence[m_strPicFileName] = 1;

		std::string strPathName,strFileName,strExtName;
		StringUtility::parseFileName( m_strPicFileName, strPathName, strFileName, strExtName );
		StringUtility::makeLower( strExtName ); /// 转换成小写，便于下面的判断代码
		if( strExtName == "josn" || strExtName == "exportjson" )
		{
			extension::CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(szAniFileName);
		}
		else if( strExtName == "xml" )
		{
			std::string strPngName, strPlistName;
			strPngName = m_strPicFileName;
			strPlistName = m_strPicFileName;
			StringUtility::stringReplace(strPngName,".xml", ".png" );
			StringUtility::stringReplace(strPlistName,".xml", ".plist" );
			extension::CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo( strPngName.c_str(), strPlistName.c_str(), m_strPicFileName.c_str() );
		}
	}
	else
	{
		g_mapArmatureRefrence[m_strPicFileName]++;
	}

	std::string strPathName, strObjName, strExtName;
	StringUtility::parseFileName( m_strPicFileName, strPathName, strObjName, strExtName );
	m_pArmature = extension::CCArmature::create( strObjName.c_str() );
	if( m_pArmature == NULL )
	{
		return false;
	}

	m_Animate.setAnimationRes(m_pArmature->getAnimation());

	addChild( m_pArmature );

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 更新
@param
*/
void CDragonBoneSpriteObj::update( float dt )
{

}
//-------------------------------------------------------------------------
/**
@brief 
@param
*/
void CDragonBoneSpriteObj::onExit()
{
	if( m_pArmature ) /// m_pArmature 做了autorelease
	{
		if( g_mapArmatureRefrence.find( m_strPicFileName ) != g_mapArmatureRefrence.end() )
		{
			int nRef = g_mapArmatureRefrence[m_strPicFileName];
			if( --nRef == 0 )
			{
				extension::CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(m_strPicFileName.c_str());
				g_mapArmatureRefrence.erase(m_strPicFileName);
			}
			else
			{
				g_mapArmatureRefrence[m_strPicFileName] = nRef;
			}
		}

		removeChild( m_pArmature );
		m_pArmature = NULL;
	}
}
//-------------------------------------------------------------------------
/**
@brief 给对象加色
@param
*/
void CDragonBoneSpriteObj::setColor( const ccColor3B& color )
{
	if( m_pArmature == NULL )
	{
		return;
	}

	m_pArmature->setColor( color );
}
//-------------------------------------------------------------------------
/**
@brief 设置透明度
@param byAlpha 透明度
*/
void CDragonBoneSpriteObj::setOpacity( BYTE byAlpha )
{
	if( m_pArmature == NULL )
	{
		return;
	}

	m_pArmature->setOpacity( byAlpha );
}
//-------------------------------------------------------------------------
/**
@brief 获取该对象的特征串(全局唯一)
@param strFeature 特征串
*/
void CDragonBoneSpriteObj::getFeatureString( std::string& strFeature )
{
	strFeature = m_strPicFileName;
}
//-------------------------------------------------------------------------
/**
@brief 碰撞检测
@param
*/
bool CDragonBoneSpriteObj::hitTest( const CCPoint& pt )
{
	if( m_pArmature == NULL )
	{
		return false;
	}

	CCRect rect = m_pArmature->boundingBox();
	return rect.containsPoint( pt );
}
//-------------------------------------------------------------------------
/**
@brief 碰撞检测
@param
*/
bool CDragonBoneSpriteObj::hitTest( const CCRect& rect )
{
	if( m_pArmature == NULL )
	{
		return false;
	}

	CCRect rc = m_pArmature->boundingBox();
	return rc.intersectsRect( rect );
}
//-------------------------------------------------------------------------
/**
@brief 获取对象包围盒
@param
*/
bool CDragonBoneSpriteObj::getBoundingBox( CCRect& boundBox )
{
	if( m_pArmature )
	{
		boundBox = m_pArmature->boundingBoxReal();
		boundBox = CCRectApplyAffineTransform(boundBox, nodeToParentTransform());
		return true;
	}

	return false; 
}
//-------------------------------------------------------------------------
/**
@brief 获取偏移
@param
*/
void CDragonBoneSpriteObj::setOffset( CCPoint& ptOffet )
{
	if( m_pArmature )
	{
		m_pArmature->setPosition( ptOffet );
	}
}
//-------------------------------------------------------------------------
/**
@brief 获取偏移
@param
*/
void CDragonBoneSpriteObj::getOffset( CCPoint& ptOffet )
{
	if( m_pArmature )
	{
		ptOffet = m_pArmature->getPosition();
	}
}

NS_CC_ENGINE_END