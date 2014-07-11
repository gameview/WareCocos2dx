#include "AnimationResManager.h"
#include "AnimationRes.h"
#include "cocoa/CCObject.h"
#include "ccMacros.h"

NS_CC_ENGINE_BEGIN

CAnimationResManager::CAnimationResManager(void)
{
}


CAnimationResManager::~CAnimationResManager(void)
{
}

/**
@brief 创建动画资源 如果已经存在该资源，则直接返回资源指针
@param szAnimationResName 动画资源文件名
*/
CAnimationRes* CAnimationResManager::getAnimationRes( const char* szAnimationResName )
{
	if( szAnimationResName == NULL || szAnimationResName[0] == 0 )
	{
		return NULL;
	}

	AnimationResMap::iterator iter = m_mapAnimationRes.find( szAnimationResName );
	if( iter != m_mapAnimationRes.end() )
	{
		iter->second->retain();
		return iter->second;
	}

	CAnimationRes* pAnimationRes = new CAnimationRes();
	if( pAnimationRes == NULL )
	{
		return NULL;
	}

	if( !pAnimationRes->loadRes( szAnimationResName ) )
	{
		return NULL;
	}
	//pAnimationRes->retain();
	m_mapAnimationRes[szAnimationResName] = pAnimationRes;

	return pAnimationRes;
}
//------------------------------------------------------------------------------
/**
@brief 删除动画资源
@param
*/
void CAnimationResManager::releaseAnimationRes( CAnimationRes* pAniRes )
{
	if( pAniRes == NULL )
	{
		return;
	}

	if( pAniRes->isSingleReference() )
	{
		m_mapAnimationRes.erase( pAniRes->getFileName() );	
	}
	//CCLOG("CAnimationResManager::releaseAnimationRes m_uReference= %d %x",pAniRes->retainCount(),pAniRes);
	//pAniRes->release();
}

//------------------------------------------------------------------------------
/**
@brief 释放全部资源
@param
*/
void CAnimationResManager::releaseUnuseResource()
{
	AnimationResMap::iterator iter = m_mapAnimationRes.begin();
	for( ; iter != m_mapAnimationRes.end(); )
	{
		if( iter->second == NULL )
		{
			continue;
		}

		if( iter->second->isSingleReference() )
		{
			iter->second->release();
			m_mapAnimationRes.erase( iter++ );
		}
		else
		{
			++iter;
		}
	}
}
//------------------------------------------------------------------------------
/**
@brief 释放没有使用的资源
@param
*/
void CAnimationResManager::releaseAllResource()
{
	AnimationResMap::iterator iter = m_mapAnimationRes.begin();
	for( ; iter != m_mapAnimationRes.end(); )
	{
		if( iter->second == NULL )
		{
			continue;
		}

		CCAssert( iter->second->isSingleReference(), "reference count must equte 1");

		iter->second->release();
	}

	m_mapAnimationRes.clear();
}

NS_CC_ENGINE_END
