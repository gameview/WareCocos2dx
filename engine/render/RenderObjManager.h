//*************************************************************************
//	创建日期:	2013-5-30   8:11
//	文件名称:	RenderObjManager.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	游戏对象管理器
//*************************************************************************
#ifndef _GAMEOBJMANAGER_H_
#define _GAMEOBJMANAGER_H_

#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include "cocos2dx_engine.h"
#include <map>
#include <list>
#include <string>

NS_CC_ENGINE_BEGIN

class CRenderObj;
class CRenderObjModel;

class CRenderObjManager
{
	typedef std::map< uint32, CRenderObj* >				RenderObjMap;		/// 对象数据
	typedef std::map< std::string, CRenderObjModel* >	RenderObjModelMap;  /// 对象模板数据

public:
	CRenderObjManager(void);
	~CRenderObjManager(void);

	/**
	@brief 创建对象
	@param
	*/
	CRenderObj*		createRenderObj( const char* szObjFileName );

	/**
	@brief 删除对象
	@param
	*/
	void			removeRenderObj( uint32 uID );

	/**
	@brief 获取游戏对象
	@param
	*/
	CRenderObj*		getRenderObj( uint32 uID );

	/**
	@brief 释放没有使用的对象数据
	@param bFlag = true 表示游戏运行中释放obj数据
	*/
	void			releaseObjModel( bool bFlag = true );

private:

	RenderObjMap		m_mapRenderObj;		/// 渲染对象管理器
	RenderObjModelMap	m_mapObjModel;		/// 对象模板数据
	//RenderObjIdleMap	m_mapIdleRenderObj;	/// 空闲渲染对象管理器
	
};

NS_CC_ENGINE_END

#endif // _GAMEOBJMANAGER_H_