#include "cocos2d.h"
#include "AnimationRes.h"
#include "StringUtility.h"
#include "tinyxml.h"
#include "StringConvert.h"
#include "AnimationResManager.h"

NS_CC_ENGINE_BEGIN

/////////////////////  AnimateSaxDelegator /////////////////////////////////////////////////////

void AnimateSaxDelegator::startElement( void *ctx, const char *name, const char **atts )
{
	string tag((char*)name);

	if (tag=="plist")
	{
		state=STATE_PLIST;
	} 
	else if (tag=="animation")
	{
		state=STATE_ANIMATION;
		animates.push_back(Animate());
	}
	else if (tag=="name")
	{
		state=STATE_NAME;
	}
	else if (tag=="delay")
	{
		state=STATE_DELAY;
	}
	else if (tag=="spriteFrame")
	{
		state=STATE_SPRITE_FRAME;
	}
	else if (tag=="flipX")
	{
		state=STATE_FLIP_X;
	}
	else if (tag=="flipY")
	{
		state=STATE_FLIP_Y;
	}
	else
	{
		state=STATE_NONE;
	}
}

void AnimateSaxDelegator::endElement( void *ctx, const char *name )
{
	string tag((char*)name);

	if (tag=="plist")
	{
	} 
	else if (tag=="animation")
	{
	}
	else if (tag=="name")
	{
	}
	else if (tag=="delay")
	{
	}
	else if (tag=="spriteFrame")
	{
	}
	else if (tag=="flipX")
	{
	}
	else if (tag=="flipY")
	{
	}
	else
	{
	}

	state = STATE_NONE;
}

void AnimateSaxDelegator::textHandler( void *ctx, const char *ch, int len )
{
	if (state == STATE_NONE)
	{
		return;
	}

	string text((char*)ch,0,len);
	int index;
	float delay;
    if (ch[0] ==0xef)
    {
        int i = 0;
        i++;
    }
	
    switch (state)
	{
	case STATE_PLIST:

		plists.push_back(text);
		break;
	case STATE_ANIMATION:
		break;
	case STATE_NAME:
        {
            
            if (animates.size() == 0 )
            {
                break;
            }
    
            index=animates.size()-1;
            animates[index].name=text;
        }
		break;
	case STATE_DELAY:
        {
         if (animates.size() == 0 )
         {
             break;
         }
		index=animates.size()-1;
		delay=(float)atof(text.c_str());
		animates[index].delay=delay;
        }
		break;
	case STATE_SPRITE_FRAME:
        {
            
        if (animates.size() == 0 )
            {
                break;
            }

            index=animates.size()-1;
            animates[index].spriteFrames.push_back(text);
		}
        break;
	case STATE_FLIP_X:
        {
            
        if (animates.size() == 0 )
            {
                break;
            }

		index=animates.size()-1;
		animates[index].flipX=(text=="true");
		}
        break;
	case STATE_FLIP_Y:
        {
            if (animates.size() == 0 )
            {
                break;
            }

		index=animates.size()-1;
		animates[index].flipY=(text=="true");
    }
            break;
	default:
		break;
	}

}

//////////////////////////////////////////////////////////////////////////

CAnimationRes::CAnimationRes(void)
{
	//m_SpriteFrameSet.init();
	m_bVailed = false;
	m_strAniResName = "";
	m_mapAnimation.clear();
	nameToAnimateMap.clear();
	pathToPlistsMap.clear();
	NameAnimation.clear();
	m_mapTextureLoadInfo.clear();  

	//n_index = 0; 
	//CCLOG("CAnimationRes begin %d m_uReference= %d %x",n_index,m_uReference,this);;
}


CAnimationRes::~CAnimationRes(void)
{
	AnimationMap::iterator iter = m_mapAnimation.begin();
	for( ; iter != m_mapAnimation.end(); ++iter )
	{
		if( iter->second.pAnimation )
		{
			iter->second.pAnimation->release();
			iter->second.pAnimation = NULL;
		}
	}
	//n_index;
//	CCLOG("CAnimationRes end %d m_uReference= %d %x",n_index,m_uReference,this);
	//CAnimationResManager::Instance().releaseAnimation(this);
	m_mapAnimation.clear();
    pathToPlistsMap.clear();
    nameToAnimateMap.clear();
    m_strAniResName.clear();
    m_mapTextureLoadInfo.clear();
	m_strAniResName = "";
}

 
void CAnimationRes::release()
{
	CAnimationResManager::Instance().releaseAnimationRes(this);
	CCObject::release();
}

/**
@brief 加载资源
@param szResName 资源名
*/
bool CAnimationRes::loadRes( const char* szResName )
{
	if( szResName == NULL || szResName[0] == 0 )
	{
		return false;
	}
	n_index ++;
 
	m_strAniResName = szResName;
	//CCLOG("CAnimationRes loadRes %d,m_uReference= %d,file= %s %x",n_index,m_uReference,szResName,this);

#ifndef OBJ_EDITOR
	loadTextureAsync( szResName );
#else
	string strPathName = CCFileUtils::sharedFileUtils()->fullPathForFilename(szResName);

	CCSAXParser parser;
	if (false == parser.init("UTF-8"))
	{
		return false;
	}

	parser.setDelegator(&delegator);
	if( !parser.parse(strPathName.c_str()) )
	{
		/// out log
		return false;
	}

	_loadRes( szResName );
#endif

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 加载资源
@param
*/
void CAnimationRes::_loadRes( const char* szResName )
{
	string strPathName = CCFileUtils::sharedFileUtils()->fullPathForFilename(szResName);
	//n_index --;
	//CCLOG("CAnimationRes _loadRes %d,m_uReference= %d,file= %s %x",n_index,m_uReference,szResName,this);

	//CCSAXParser parser;
	//AnimateSaxDelegator delegator;

	//if (false == parser.init("UTF-8"))
	//{
	//	//TODO
	//	return;
	//}
	//parser.setDelegator(&delegator);
	//if( !parser.parse(strPathName.c_str()) )
	//{
	//	/// out log
	//	return;
	//}

	//load plist
	vector<string> &plists=delegator.plists;
	for (unsigned int i=0;i<plists.size();i++)
	{
		string plistPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(plists[i].c_str(), strPathName.c_str());
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistPath.c_str());
	}
    plists.clear();

	//load animate
	vector<Animate> &animates=delegator.animates;
	CCArray *spriteFramesArray = CCArray::create();

	//set<string> animateNames;
	for (unsigned int i=0;i<animates.size();i++)
	{
		Animate animate=animates[i];
		vector<string> spriteFrames=animate.spriteFrames;

		for (unsigned int j=0;j<spriteFrames.size();j++)
		{
			CCSpriteFrame *spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteFrames[j].c_str());
			if( spriteFrame != NULL )
			{
				spriteFramesArray->addObject(spriteFrame);
			}
		}

		CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(spriteFramesArray,animate.delay);

		SAnimationInfo info;
		info.pAnimation = pAnimation;
		info.m_bFlipX = animate.flipX;
		info.m_bFlipY = animate.flipY;
		m_mapAnimation[animate.name] = info;

		pAnimation->retain();

		spriteFramesArray->removeAllObjects();

		NameAnimation.insert(animate.name);
	}

	//record animate
	for(unsigned int i=0;i<animates.size();i++)
	{
		Animate animate=animates[i];
		nameToAnimateMap[animate.name]=animate;
	}

	//record plist
	pathToPlistsMap[szResName]=plists;

    animates.clear();
	/// 加载回调数据
	loadCallback( szResName );

	m_bVailed = true;
}
//-------------------------------------------------------------------------
/**
@brief 加载贴图
@param
*/
void CAnimationRes::loadTextureAsync( const char* szResName )
{
	string strPathName = CCFileUtils::sharedFileUtils()->fullPathForFilename(szResName);

	CCSAXParser parser;
	//AnimateSaxDelegator delegator;

	if (false == parser.init("UTF-8"))
	{
		return;
	}

	parser.setDelegator(&delegator);
	if( !parser.parse(strPathName.c_str()) )
	{
		/// out log
		return;
	}

	vector<string> &plists=delegator.plists;
	for (unsigned int i=0;i<plists.size();i++)
	{
		std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(plists[i].c_str(),strPathName.c_str());
		CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());
		if( dict == NULL )
		{
			continue;
		}

		string texturePath("");
		CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
		if (metadataDict)
		{
			// try to read  texture file name from meta data
			texturePath = metadataDict->valueForKey("textureFileName")->getCString();
		}

		if (! texturePath.empty())
		{
			// build texture path relative to plist file
			texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(texturePath.c_str(), strPathName.c_str());
		}
		else
		{
			// build texture path by replacing file extension
			texturePath = plists[i].c_str();

			// remove .xxx
			size_t startPos = texturePath.find_last_of("."); 
			texturePath = texturePath.erase(startPos);

			// append .png
			texturePath = texturePath.append(".png");
		}

		m_mapTextureLoadInfo[texturePath] = false;

		/// 添加一个引用计数
		//retain();
		CCTextureCache::sharedTextureCache()->addImageAsync(texturePath.c_str(),this, callfuncO_selector(CAnimationRes::imageLoaded));
		dict->release();
	}
}
//-------------------------------------------------------------------------
/**
@brief 图片加载完毕
@param
*/
void CAnimationRes::imageLoaded(CCObject* pObj)
{
	
	CCTexture2D* pTexture = (CCTexture2D*)pObj;
	if (pTexture)
	{
		const char* pszTextureName = CCTextureCache::sharedTextureCache()->getTextureResName( pTexture );
		if( pszTextureName == NULL )
		{
			return;
		}
		
		std::map< std::string, bool >::iterator iter = m_mapTextureLoadInfo.find( pszTextureName );
		if( iter != m_mapTextureLoadInfo.end() )
		{
			iter->second = true;
		}
	}

	bool bLoad = true;
	std::map< std::string, bool >::iterator iter = m_mapTextureLoadInfo.begin();
	for( ; iter != m_mapTextureLoadInfo.end(); ++iter )
	{
		if( !iter->second )
		{
			bLoad = false;
			break;
		}
	}
    m_mapTextureLoadInfo.clear();

	if( bLoad )
	{
		_loadRes( m_strAniResName.c_str() );
	}

	//release();

	//CAnimationResManager::Instance().releaseAnimationRes( this );

}
//------------------------------------------------------------------------------
/**
@brief 加载动画回调数据
@param
*/
void CAnimationRes::loadCallback( const char* szResName )
{
	/// 暂时不加载回调文件
	return;

	std::string strResName = szResName;
	StringUtility::stringReplace( strResName, ".oms", ".ac" );

	unsigned long lSize = 0;
	unsigned char * pXmlBuff = CCFileUtils::sharedFileUtils()->getFileData( strResName.c_str(), "r", &lSize );
	if( pXmlBuff == NULL )
	{
		return;
	}

	TiXmlDocument xmldoc; 
	TiXmlElement *pRootElement = NULL;

	xmldoc.Parse((char*)pXmlBuff);
	if(xmldoc.Error())
	{
		//CCLOG( "parse obj file failed!" );
		return;
	}

	pRootElement = xmldoc.RootElement();

	const TiXmlElement *pCallbacksElement = NULL;
	const TiXmlElement *pCallback = NULL;

	if (pRootElement != NULL)
	{
		//load object mesh list
		pCallbacksElement = pRootElement->FirstChildElement("callbacks");
		if(pCallbacksElement)
		{
			const TiXmlElement *pEntity = NULL;

			pCallback = pCallbacksElement->FirstChildElement("callback");

			while(pCallback)
			{
				AnimateCallback callback;
				std::string AniName, CallbackName;
				AniName = pCallback->Attribute("ani_name");
				callback.strName = pCallback->Attribute("name");
				callback.fCallbackTime = StringConvert::parseFloat( pCallback->Attribute( "time" ) );

				/// 获取动画数据
				SAnimationInfo* pAniInfo = getAnimation( AniName.c_str() );
				if( pAniInfo )
				{
					pAniInfo->callbacks.push_back( callback );
				}

				pCallback = pCallback->NextSiblingElement("callback");
			}

		}
	}

	delete [] pXmlBuff;
	pXmlBuff = NULL;
}
/// 获取资源名称
const std::string& CAnimationRes::getFileName()
{
	return m_strAniResName;
}
/// 获取动画数据
CAnimationRes::SAnimationInfo* CAnimationRes::getAnimation( const char* szAnimationName )
{
	if (!isVailed())
	{
		return NULL;
	}
	if( m_mapAnimation.empty() || m_mapAnimation.size() == 0)
	{
		return NULL;
	}

	AnimationMap::iterator iter;
	if( szAnimationName == NULL )
	{
		iter = m_mapAnimation.begin();
		return &iter->second;
	}

	iter = m_mapAnimation.find( szAnimationName );
	if( iter != m_mapAnimation.end() )
	{
		return &iter->second;
	}

	return NULL;
}

NS_CC_ENGINE_END