//*************************************************************************
//	创建日期:	2013-7-17   19:35
//	文件名称:	IRenderInterface.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	渲染层接口
//*************************************************************************
#ifndef _IRENDERSYSTEM_H_
#define _IRENDERSYSTEM_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

struct IRenderMapCallBack;
struct IAnimationCallback;
struct IRenderObj;
struct IRenderText;

struct IRenderSystem
{
	/**
	@brief 渲染系统初始化
	@param sizeDesign 设计大小
	*/
	virtual void		init( CCSize& sizeDesign ) = 0;

	/**
	@brief 获取场景根节点
	@param
	*/
	virtual CCScene*	getRootScene() = 0;

	/// 游戏场景相关操作
	/**
	@brief 进入游戏场景
	@param[in] szScenename 场景配置文件名
	@param[in] pRenderMapCallBack 场景进入和退出回调
	*/
	virtual uint32		enterScene( const char* szScenename, IRenderMapCallBack* pRenderMapCallBack = NULL ) = 0;

	/**
	@brief 是否是遮挡格
	@param
	*/
	virtual bool		isMask( const CCPoint& ptLoc ) = 0;

	/**
	@brief 设置场景视点
	@param[in] dwSceneID 场景运行时ID
	@param[in] ptView 视点位置
	*/
	virtual void		setViewPoint( const CCPoint& ptView ) = 0;

	/**
	@brief 设置场景视点
	@param[in] dwSceneID 场景运行时ID
	@param[in] ptView 视点位置
	*/
	virtual void		getViewPoint( CCPoint& ptView ) = 0;

	/**
	@brief 设置场景缩放
	@param
	*/
	virtual void		setSceneScale( uint32 dwSceneID, float fScale ) = 0;

	/**
	@brief 从屏幕坐标到场景坐标
	@param[in] ptScreenPos 屏幕位置
	@param[out] ptScenePos 场景位置
	*/
	virtual bool		convertScreenToScene( const CCPoint& ptScreenPos, CCPoint& ptScenePos ) = 0;

	/**
	@brief 从场景坐标到屏幕坐标
	@param[in] ptScenePos 场景位置
	@param[out] ptScreenPos 屏幕位置
	*/
	virtual bool		convertSceneToScreen( const CCPoint& ptScenePos, CCPoint& ptScreenPos ) = 0;

	/**
	@brief 获取场景大小(像素)
	@param
	*/
	virtual CCSize		getCurrentSceneSize() = 0;

	/**
	@brief 获取视口大小(Tile坐标)
	@param
	*/
	virtual CCSize		getSceneTileSize() = 0;

	/**
	@brief 获取视口大小(像素)
	@param
	*/
	virtual CCSize		getViewSize() = 0;

	/**
	@brief 设置场景是否灰度图渲染
	@param
	*/
	virtual void		setGrey( bool bGrey ) = 0;

	/**
	@brief 物体碰撞检测 
	@param ptScreenPos 屏幕位置
	@return 碰撞对象
	*/
	virtual IRenderObj*	hitRenderObj( const CCPoint& ptScreenPos ) = 0;

	/**
	@brief 物体碰撞检测 
	@param ptScreenPos 屏幕位置
	@return 碰撞对象
	*/
	virtual void		hitRenderObj( const CCPoint& ptScreenPos, std::list<IRenderObj*>& ObjList ) = 0;


	///////////////////// 游戏对象操作 /////////////////////////////////////////////////////
	/**
	@brief 创建游戏对象
	@param[in] szGameobjName 游戏对象配置文件名
	*/
	virtual IRenderObj*	createRenderObj( const char* szGameObjName ) = 0;

	/**
	@brief 删除没有加入到场景中渲染对
	@param
	*/
	virtual void		removeRenderObj( IRenderObj* pRenderObj ) = 0;

	/**
	@brief 添加到当前场景
	@param[in] uid 游戏对象ID 对象不存在，添加失败
	@param[in] dwSceneID 目标场景ID 如果目标场景不是当前场景，则添加失败
	*/
	virtual bool		addRenderObjToScene( IRenderObj* pRenderObj ) = 0;

	/**
	@brief 从当前场景删除游戏对象
	@param[in] 游戏对象ID
	*/
	virtual void		removeRenderObjFromScene( IRenderObj* pRenderObj ) = 0;

	/**
	@brief 画文字
	@param szFontName 字体名称
	@param nFontSize 字体大小
	*/
	virtual IRenderText* createRenderText( const char* szString, const char* szFontName, int nFontSize, CCSize dimentions = CCSizeZero, bool bBmpFont = false ) = 0;

	/**
	@brief 画文字
	@param szFontName 字体名称
	@param nFontSize 字体大小
	*/
	virtual void		removeRenderText( IRenderText* pRenderText ) = 0;
};

/**
*@brief 获取渲染系统
*/
IRenderSystem* getRenderSystem();

NS_CC_ENGINE_END

#endif // _IRENDERSYSTEM_H_
