//*************************************************************************
//	创建日期:	2013-7-17   20:35
//	文件名称:	RenderInterface.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	渲染接口实现
//*************************************************************************
#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_

#include "IRenderSystem.h"
#include "RenderObjManager.h"
#include "RenderTextManager.h"

NS_CC_ENGINE_BEGIN

struct IRenderObj;

class CRenderSystem : public IRenderSystem
{
public:
	CRenderSystem(void);
	~CRenderSystem(void);

	/**
	@brief 渲染系统初始化
	@param sizeDesign 设计大小
	*/
	virtual void		init( CCSize& sizeDesign );

	/**
	@brief 获取场景根节点
	@param
	*/
	virtual CCScene*	getRootScene();

	/// 游戏场景相关操作
	/**
	@brief 进入游戏场景
	@param[in] szScenename 场景配置文件名
	@param[in] pRenderMapCallBack 场景进入和退出回调
	*/
	virtual uint32		enterScene( const char* szScenename, IRenderMapCallBack* pRenderMapCallBack = NULL );

	/**
	@brief 是否是遮挡格
	@param
	*/
	virtual bool		isMask( const CCPoint& ptLoc );

	/**
	@brief 设置场景视点
	@param[in] ptView 视点位置
	*/
	virtual void		setViewPoint( const CCPoint& ptView );

	/**
	@brief 获取场景视点(场景坐标)
	@param[in] ptView 视点位置
	*/
	virtual void		getViewPoint( CCPoint& ptView );

	/**
	@brief 设置场景缩放
	@param
	*/
	virtual void		setSceneScale( uint32 dwSceneID, float fScale );

	/**
	@brief 从屏幕坐标到场景坐标
	@param[in] ptScreenPos 屏幕位置
	@param[out] ptScenePos 场景位置
	*/
	virtual bool		convertScreenToScene( const CCPoint& ptScreenPos, CCPoint& ptScenePos );

	/**
	@brief 从场景坐标到屏幕坐标
	@param[in] ptScenePos 场景位置
	@param[out] ptScreenPos 屏幕位置
	*/
	virtual bool		convertSceneToScreen( const CCPoint& ptScenePos, CCPoint& ptScreenPos );

	/**
	@brief 获取视口大小(像素)
	@param
	*/
	virtual CCSize		getCurrentSceneSize();

	/**
	@brief 获取视口大小(像素)
	@param
	*/
	virtual CCSize		getSceneTileSize();

	/**
	@brief 获取视口大小(像素)
	@param
	*/
	virtual CCSize		getViewSize();

	/**
	@brief 设置场景是否灰度图渲染
	@param
	*/
	virtual void		setGrey( bool bGrey );

	/**
	@brief 物体碰撞检测 
	@param ptScreenPos 屏幕位置
	@return 碰撞对象
	*/
	virtual IRenderObj*	hitRenderObj( const CCPoint& ptScreenPos );

	/**
	@brief 物体碰撞检测 
	@param ptScreenPos 屏幕位置
	@return 碰撞对象
	*/
	virtual void		hitRenderObj( const CCPoint& ptScreenPos, std::list<IRenderObj*>& ObjList );

	///////////////////// 游戏对象操作 /////////////////////////////////////////////////////
	/**
	@brief 创建游戏对象
	@param[in] szGameobjName 游戏对象配置文件名
	*/
	virtual IRenderObj*	createRenderObj( const char* szGameobjName );

	/**
	@brief 删除没有加入到场景中渲染对
	@param
	*/
	virtual void		removeRenderObj( IRenderObj* pRenderObj );

	/**
	@brief 添加到当前场景
	@param[in] uid 游戏对象ID 对象不存在，添加失败
	@param[in] dwSceneID 目标场景ID 如果目标场景不是当前场景，则添加失败
	*/
	virtual bool		addRenderObjToScene( IRenderObj* pRenderObj );

	/**
	@brief 从当前场景中删除游戏对象
	@param[in] 游戏对象ID
	*/
	virtual void		removeRenderObjFromScene( IRenderObj* pRenderObj );

	/**
	@brief 画文字
	@param szFontName 字体名称
	@param nFontSize 字体大小
	*/
	virtual IRenderText* createRenderText( const char* szString, const char* szFontName, int nFontSize, CCSize dimentions = CCSizeZero, bool bBmpFont = false );

	/**
	@brief 画文字
	@param szFontName 字体名称
	@param nFontSize 字体大小
	*/
	virtual void		removeRenderText( IRenderText* pRenderText );

public:

	static bool			m_bGrey;		/// 场景是否灰度图渲染

private:

	CRenderObjManager	m_RenderObjManager;
	CRenderTextManager	m_RenderTextManager;
};

extern CRenderSystem g_RenderSystem;

NS_CC_ENGINE_END

#endif // _RENDERSYSTEM_H_

