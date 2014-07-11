//*************************************************************************
//	创建日期:	2013-7-3   18:18
//	文件名称:	AnimationResManager.h	
//	版权所有:	Cocos2DRes.com
//	说    明:	动画资源管理
//*************************************************************************
#ifndef _ANIMATIONRESMANAGER_H_
#define _ANIMATIONRESMANAGER_H_
#include "cocos2d.h"
#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include <string>
#include <map>


NS_CC_ENGINE_BEGIN

class CAnimationRes;

class CAnimationResManager
{
private:
	CAnimationResManager(void);
	virtual ~CAnimationResManager(void);

	typedef std::map< std::string, CAnimationRes* > AnimationResMap;

public:

	static CAnimationResManager& Instance()
	{
		static CAnimationResManager _inst;
		return _inst;
	}

	/**
	@brief 创建动画资源 如果已经存在该资源，则直接返回资源指针
	@param szAnimationResName 动画资源文件名
	*/
	CAnimationRes*	getAnimationRes( const char* szAnimationResName );

	/**
	@brief 删除动画资源
	@param
	*/
	void			releaseAnimationRes( CAnimationRes* pAniRes );

	/**
	@brief 释放全部资源
	@param
	*/
	void			releaseAllResource();

	/**
	@brief 释放没有使用的资源
	@param
	*/
	void			releaseUnuseResource();

private:

	AnimationResMap	m_mapAnimationRes;		/// 动画资源列表

};

NS_CC_ENGINE_END

#endif // _ANIMATIONRESMANAGER_H_

