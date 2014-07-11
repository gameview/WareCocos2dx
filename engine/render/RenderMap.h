////*************************************************************************
//	创建日期:	2013-4-11   21:39
//	文件名称:	RenderMap.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	场景声明
//*************************************************************************
#ifndef _RenderMap_H_
#define _RenderMap_H_

#include "cocos2d.h"
#include "cocos2dx_engine.h"
#include "TerrainBlock.h"
#include "TerrainMask.h"
#include "TerrainGrid.h"
#include <map>
#include <list>

USING_NS_CC;

NS_CC_ENGINE_BEGIN


class CRenderMap;
class CRenderObj;
class CSceneLayer;
class CImageLayer;
struct IRenderObj;

/// 地表块
struct STerrainChunk
{
	int nXIndex;
	int nYIndex;
	std::string strTexture;
	CCRect	rect;

	STerrainChunk()
	{
		nXIndex = 0;
		nYIndex = 0;
		strTexture.clear();
	}
};

// 物件数据
struct STerrainObj
{
	uint32				uid;		/// 对象id
	int					nLayerID;	/// 对象层ID
	bool				bRectObj;	/// 矩形对象标识
	ccColor4B			fillColor;	/// 填充颜色
	bool				bFillColor;	/// 是否需要填充
	ccColor4B			color;		/// 对象颜色
	std::string			strObjName;	/// 物件名称
	CCRect				rect;		/// 图片区域
	CCRect				boundBox;	/// 图片区域
	CCPoint				ptPos;		/// 物件位置
	float				fScale;		/// 缩放比例
	float				fAngle;		/// 旋转角度
	int					nTag;		/// 标识符
	BYTE				byAlpha;	/// 透明度
	uint32				dwTime;		/// 不在场景起始时间
	CRenderObj*			pRenderObj;	/// 渲染对象

	STerrainObj( bool bSoildRect = false )
	{
		uid = 0;
		strObjName = "";
		ptPos.x = 0;
		ptPos.y = 0;
		//pSprite = NULL;
		bRectObj = bSoildRect;
		nTag = 0;
		fScale = 1.0f;
		fAngle = 0.0f;
		nLayerID = 0;
		byAlpha = 255;
		bFillColor = false;
		pRenderObj = NULL;
		dwTime = 0;
		color.a = 255;
		color.r = 255;
		color.g = 255;
		color.b = 255;
	}
};

// 图片层数据定义
struct SImageLayer
{
	int nImageLayerID;		/// 层ID
	std::vector<std::string> m_vecImageList;	/// 层图片列表

	SImageLayer()
	{
		nImageLayerID = 0;
		m_vecImageList.clear();
	}
};

class CRenderMap : public CCNode
{
	typedef std::set< uint32 >				RenderObjSet;		/// 游戏对象列表
	typedef std::map< int, CRenderObj* >	RenderObjTagMap;	/// 标识符到对象的映射
	typedef std::map< int, CSceneLayer* >	SceneLayerMap;		/// 场景层列表
	typedef std::vector<STerrainChunk>		TerrainChunks;		/// 地表块数据
	typedef std::map<int,CTerrainGrid*>		TerrainGridMap;		/// 地表宫格
	typedef std::map<uint32,STerrainObj>	TerrainObjMap;		/// 地表物件集
	typedef std::map<uint32,uint32>			RenderObjToSceneMap;/// 渲染对象ID->scene
	typedef std::map<int,SImageLayer>		ImageLayerMap;		/// 图片层数据

public:
	CRenderMap( uint32 uid );
	virtual ~CRenderMap(void);

	/**
	@brief 场景更新
	@param
	*/
	virtual void	update(float dt);

	/**
	@brief 进入场景
	@param
	*/
	virtual void	onEnter();

	/**
	@brief 退出
	@param
	*/
	virtual void	onExit();

	/**
	@brief 获取场景ID
	@param
	*/
	uint32			getID() { return m_uID; }

	/**
	@brief 设置场景回调接口
	@param
	*/
	void			setSceneCallBack( IRenderMapCallBack* pRenderMapCallBack ) { m_pRenderMapCallBack = pRenderMapCallBack; }

	/**
	@brief 从场景文件创建
	@param
	*/
	bool			initWithFile( const char* szSceneName );

	/**
	@brief 添加动态渲染对象
	@param
	*/
	uint32			addRenderObj( int nLayerID, CRenderObj* pRenderObj );

	/**
	@brief 场景树上摘除对象
	@param
	*/
	void			removeRenderObj( uint32 uid );

	/**
	@brief 获取渲染对象
	@param
	*/
	CRenderObj*		getRenderObj( uint32 uid );

	/**
	@brief 设置对象位置
	@param uid 对象ID 用来更新对象位置
	*/
	void			setObjPos( uint32 uid, const CCPoint& ptLoc );

	/**
	@brief 是否开启Update
	@param
	*/
	void			setUpdateEnable( bool bEnable );

	/**
	@brief 转换屏幕坐标到场景坐标
	@param
	*/
	void			convertScreenToScene( const CCPoint& ptScreenPos, const CCSize& size, CCPoint& ptScenePos );

	/**
	@brief 转换屏幕坐标到场景坐标
	@param
	*/
	void			convertSceneToScreen( const CCPoint& ptScenePos, const CCSize& size, CCPoint& ptScreenPos );

	/**
	@brief 设置视点
	@param
	*/
	void			setViewPoint( const CCPoint& pt );

	/**
	@brief 获取视点
	@param
	*/
	CCPoint&		getViewPoint() { return m_ptView; }

	/**
	@brief 获取场景宽高
	@param
	*/
	void			getSize( CCSize& size );

	/**
	@brief 获取tile的宽高
	@param
	*/
	void			getTileSize( CCSize& size );

	/**
	@brief 设置缩放比例
	@param
	*/
	void			setRenderScale(float fScale);

	/**
	@brief 碰撞检测（物件）
	@param
	*/
	CRenderObj*		hitTestObj( const CCPoint& pt );

	/**
	@brief 碰撞检测（物件）
	@param
	*/
	void			hitTestObj( const CCPoint& pt, std::list<IRenderObj*>& ObjList );

	/**
	@brief 是否是遮挡层
	@param
	*/
	bool			isMask( const CCPoint& pt );

	/**
	@brief 获取文件名
	@param
	*/
	std::string&	getFileName() { return m_strSceneName; }


protected:

	/**
	@brief 获取场景层
	@param
	*/
	CSceneLayer*	getSceneLayer( int id );

	/**
	@brief 设置场景数据(宽高,地表图块宽高)
	@param
	*/
	void			setTerrainInfo( int nTerrainWidth, int nTerrainHeight, int nTerrainChunkWidth, int nTerrainChunkHeight );

	/**
	@brief 设置地表块信息
	@param
	*/
	void			setTerrainChunk( int nXIndex, int nYIndex, const std::string& strTextureName, const cocos2d::CCRect rc );


	/**
	@brief 添加物件对象
	@param
	*/
	uint32			addTerrainObj( int nLayerID, const std::string& strTerrainObjName, CCRect& rc, CCPoint& ptPos, bool bCullObj = true );

	/**
	@brief 添加矩形对象
	@param
	*/
	uint32			addRectangleObj( int nLayerID, CCRect& rc, ccColor4B& borderColor, int nTag = 0, ccColor4B fillColor = ccc4(255,255,255,255), bool bFillColor = false );

	/**
	 @brief 添加图片层
	 @param nLayerID 层ID
	 @param vecImageFileName jpg文件列表
	*/
	void			addImageLayer( int nLayerID, std::vector<std::string>& vecImageFileName );

	/**
	@brief 设置矩形对象颜色
	@param
	*/
	void			setTerrainGridObjColor( ccColor4B& color );

	/**
	@brief 添加阻挡
	@param x,y  tile格坐标
	@param nBlockValue 阻挡值
	*/
	void			addBlock( int x, int y, int nBlockValue );

	/**
	@brief 删除阻挡
	@param
	*/
	void			removeBlock( int x, int y, int nBlockValue );

	/**
	@brief 删除对象
	@param
	*/
	void			removeObj( uint32 uid );

	/**
	@brief 筛选可见对象 地表对象
	@param
	*/
	void			cullVisibleObject();

	/**
	@brief 更新可视集 地表对象
	@param
	*/
	void			updateVisibleObject();

	/**
	@brief 读取场景文件
	@param
	*/
	bool			readSceneFileVer( CWareFileRead& File, int nVersion );

	/**
	 @brief 读取地表块数据
	 @param 
	*/
	void			readTerrainChunck( CWareFileRead& File );

	
	/**
	 @brief 读取地表对象
	 @param 
	*/
	void			readTerrainObject( CWareFileRead& File );

	/**
	 @brief 读取视差数据
	 @param 
	*/
	void			readParallax( CWareFileRead& File );

	/**
	@brief 加载物件
	@param
	*/
	void			loadObjFromFile( int nLayerID, CWareFileRead& File );


private:

	uint32				m_uID;						/// 场景ID

	int					m_nTerrainChunkWidth;		/// 地表块宽度
	int					m_nTerrainChunkHeight;		/// 地表块高度
	int					m_nTerrainWidth;			/// 地表宽度
	int					m_nTerrainHeight;			/// 地表高度

	int					m_nTerrainGridWidth;		/// 地表宫格宽
	int					m_nTerrainGridHeight;		/// 地表宫格高

	int					m_nTerrainTileWidth;		/// 地表Tile格宽
	int					m_nTerrainTileHeight;		/// 地表Tile格高

	bool				m_bParallax;				/// 是否开启视差
	float32				m_fParallexFactor[2];		/// 视差因子 EmebLayer和背景层
	TerrainChunks		m_vecTerrainChunk;			/// 地表块集
	std::vector<int>	m_vecVisibleTerrainChunk;	/// 可见地表集

	TerrainObjMap		m_mapTerrainObj;			/// 地表物件

	TerrainGridMap		m_mapTerrainGrid;			/// 地表宫格
	ImageLayerMap		m_mapImageLayer;			/// 图片层数据

	std::set<uint32>	m_setRenderObj;				/// 渲染对象集
	std::set<uint32>	m_setVisibleObj;			/// 可见地表物件集

	SceneLayerMap		m_mapSceneLayer;			/// 场景层(地表和物件)

	uint32				m_uSeed;					/// 地表物件随机种子

	CCPoint				m_ptView;					/// 地图视点

	float				m_fScale;					/// 缩放比例

	std::string			m_strSceneName;				/// 场景文件名(带路径)

	bool				m_bUpdateEnable;			/// 是否允许帧更新

	bool				m_bUpdateTerrainObj;		/// 是否需要更新地表物件

	RenderObjTagMap		m_mapRenderObjTag;			/// 游戏对象到tag映射
	RenderObjSet		m_setRenderObject;			/// 游戏对象列表 /// 还需要分静态对象和交互对象
	RenderObjToSceneMap	m_mapRenderObjToScene;		/// 渲染对象到场景对象映射
	
	CTerrainBlock		m_TerrainBlock;				/// 地表阻挡数据
	CTerrainMask		m_TerrainMask;				/// 地表遮挡数据

	IRenderMapCallBack* m_pRenderMapCallBack;		/// 场景回调接口
};

NS_CC_ENGINE_END

#endif // _RenderMap_H_
