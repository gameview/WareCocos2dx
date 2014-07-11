//*************************************************************************
//	创建日期:	2013-7-12   18:24
//	文件名称:	TerrainMask.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	地表阻挡
//*************************************************************************
#ifndef _RENDER_TERRAINMASK_H_
#define _RENDER_TERRAINMASK_H_

#include <map>
#include "cocos2d.h"
#include "cocos2dx_macros.h"
#include "cocos2dx_types.h"
#include "TerrainBlock.h"

NS_CC_ENGINE_BEGIN

//class CMetaScene;
class CRenderMap;

class CTerrainMask
{
	typedef std::map< tPosition, SBlockInfo > MaskMap;
public:
	CTerrainMask();
	~CTerrainMask(void);

	/**
	@brief 添加阻挡
	@param
	*/
	void				addMask( int x, int y, int nMaskValue );

	/**
	@brief 加载物件
	@param
	*/
	void				loadObjFromFile( std::string& strMaskFileName );

	/**
	@brief 是否是遮挡位置
	@param
	*/
	bool				isMask( const CCPoint& ptLoc );

private:

	//CRenderMap*		m_pRenderMap;			/// 场景
	MaskMap				m_mapMask;				/// 阻挡列表
	ccColor4B			m_TerrainBlockColor;
};

NS_CC_ENGINE_END

#endif // _RENDER_TERRAINMASK_H_

