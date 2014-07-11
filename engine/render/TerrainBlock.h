//*************************************************************************
//	创建日期:	2013-7-12   18:24
//	文件名称:	TerrainBlock.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	地表阻挡
//*************************************************************************
#ifndef _RENDER_TERRAINBLOCK_H_
#define _RENDER_TERRAINBLOCK_H_

#include <map>
#include "cocos2d.h"
#include "cocos2dx_macros.h"
#include "cocos2dx_types.h"
NS_CC_ENGINE_BEGIN

/// 坐标定义
struct tPosition
{
	uint16	x;
	uint16	y;

	tPosition()
	{
		x = 0;
		y = 0;
	}

	bool operator < ( const tPosition& pos ) const
	{
		if( ( x << 16 ) + y < ( pos.x << 16 ) + pos.y )
		{
			return true;
		}

		return false;
	}
};

/// 阻挡信息
struct SBlockInfo
{
	tPosition		pos;			/// 16x 16y
	uint16			nBlockValue;	/// 阻挡标识
//	unsigned int	uidGrid;		/// 场景阻挡网格对象id
//	unsigned int	uid;			/// 场景阻挡Sprite对象id

	SBlockInfo()
	{
		pos.x = 0;
		pos.y = 0;
		nBlockValue = 0;
	//	uid = 0;
	}
};

//class CMetaScene;
class CRenderMap;

class CTerrainBlock
{
	typedef std::map< tPosition, SBlockInfo > BlockMap;
public:
	CTerrainBlock();
	~CTerrainBlock(void);

	/**
	@brief 添加阻挡
	@param
	*/
	void				addBlock( int x, int y, int nBlockValue );

	/**
	@brief 删除阻挡
	@param
	*/
	void				removeBlock( int x, int y, int nBlockValue );

	/**
	@brief 设置地表高度
	@param
	*/
	void				setTerrainHeight( int x, int y, short nHeight );

	/**
	@brief 加载物件
	@param
	*/
	void				loadObjFromFile( std::string& strBlockFileName );

	/**
	@brief 保存物件信息
	@param
	*/
	void				saveToFile( std::string& strBlockFileName );

private:

	//CRenderMap*		m_pRenderMap;			/// 场景
	BlockMap			m_mapBlock;				/// 阻挡列表
	ccColor4B			m_TerrainBlockColor;
};

NS_CC_ENGINE_END

#endif // _RENDER_TERRAINBLOCK_H_

