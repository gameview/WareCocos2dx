////*************************************************************************
//	创建日期:	2013-4-12   1:12
//	文件名称:	RenderCommon.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	渲染系统参数定义
//*************************************************************************
#ifndef _RENDERCOMMON_H_
#define _RENDERCOMMON_H_

#include <memory.h>
#include <string.h>
#include "cocos2dx_types.h"

//#define		VIEW_CULL_WIDTH			800		/// 裁剪视口宽度
//#define		VIEW_CULL_HEIGHT		500		/// 裁剪视口高度

#define TERRAIN_GRID			10000	/// 地表网格
#define TERRAIN_BLOCK			10001	/// 地表阻挡网格
#define TERRAIN_TILE_GRID		10002	/// 地表Tile格为了画阻挡
#define TERRAIN_MASK			10003	/// 地表遮挡网格
#define TERRAIN_BACKGROUND_LAYER	-2		/// 背景层
#define TERRAIN_EMBE_LAYER			-1		/// 修饰层ID

#define TERRAIN_CHUNK_LAYER		1		/// 地表块层ID
#define TERRAIN_OBJ_LAYER		1000	/// 地表对象层
#define TERRAIN_OBJ_LAYER_End	8000	/// 地表对象层
/// 地形宫格宽高
#define TERRAIN_GRID_WIDTH		512
#define TERRAIN_GRID_HEIGHT		512

#define TERRAIN_TILE_WIDTH		16		/// Tile(阻挡)格宽
#define TERRAIN_TILE_HEIGHT		16		/// Tile(阻挡)格高
enum eTerrainType	/// 地表类型
{
	eTerrainType_Tile = 0,	// tile拼接地图
	eTerrainType_Brush,		// 笔刷制作地图
};

/// 高位字节行走禁止 第2个字节是飞行禁止 低2位是高度信息
/// 0100行走阻挡 0200 飞行阻挡 低字节存储高度
enum eTerrainBlock
{
	eTerrainBlock_Walk_Forbid = 0x0001,	/// 禁止行走
	eTerrainBlock_Fly_Forbid = 0x0002,	/// 禁止飞行
	eTerrainBlock_Mask_Forbid = 0x0004,	/// 遮挡数据
};

/// 渲染对象类型
enum RenderableType
{
	RenderableType_Sprite = 0,	/// 精灵对象(没有动画)
	RenderableType_AniSprite,	/// 动画对象
	RenderableType_DragonBone,  /// DragonBone对象
	RenderableType_Effect,		/// 特效对象(没有精灵)
	
	RenderableType_Count,
};

/// 层深度定义
enum EGameLayerDepth
{
	/// 背景层
	EGameLayerDepth_BackGround = 10,				/// 背景层

	/// 游戏层
	EGameLayerDepth_BackGround_Ground = 50,			/// 地表层
	EGameLayerDepth_BackGround_Ground_Shadow = 55,	/// 地表阴影
	EGameLayerDepth_BackGround_Object_Shadow = 60,	/// 物件阴影
	EGameLayerDepth_BackGround_Object = 65,			/// 物件层
	EGameLayerDepth_BackGround_Object_Mask = 70,	/// 物件遮罩层

	/// UI层
	EGameLayerDepth_Overlay			= 100,			/// 场景覆盖层
	EGameLayerDepth_GUI				= 200,			/// GUI层

	EGameLayerDepth_GUI_OVER		= 500,			/// GUI覆盖层

	EGameLayerDepth_TOPMASTE		= 1000,			/// 置顶层
};

/// 字体对齐定义
enum ETextVerticalAlignment
{
	/// 垂直对齐
	ETextAlignmentVerticalTop,
	ETextAlignmentVerticalCenter,
	ETextAlignmentVerticalBottom,
};

enum ETextHorizontalAlignment
{
	//! Horizontal text alignment type 水平对齐
	ETextAlignmentHorizontalLeft,
	ETextAlignmentHorizontalCenter,
	ETextAlignmentHorizontalRight,
};

namespace cocos2d
{
	namespace engine
	{
		/**
		@brief 动画播放回调信息
		*/
		struct SAnimationCallbackInfo
		{
			char	szActName[32];	///	动作名称
			char	szName[32];		///	回调名称
			float	fFrameTime;		/// 帧回调时间

			SAnimationCallbackInfo()
			{
				memset( this, 0, sizeof(*this) );
			}
		};

		/// 动画播放架设接口
		struct IAnimationCallback
		{
			/**
			@brief 
			@param
			*/
			virtual void	onAnimaitonCallback( const SAnimationCallbackInfo& info, void* pContext ) = 0;
		};

		///// 游戏场景回调
		//struct IRenderMapCallBack
		//{
		//	/**
		//	@brief 场景进入
		//	@param
		//	*/
		//	virtual void	onEnter( uint32 dwSceneID ) {}
		//
		//	/**
		//	@brief 场景退出
		//	@param
		//	*/
		//	virtual void	onExit( uint32 dwSceneID ) {}
		//};
	}
}


#endif // _RENDERCOMMON_H_