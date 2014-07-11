//*************************************************************************
//	创建日期:	2013-7-4   20:33
//	文件名称:	ImageLayer.h	
//	版权所有:	Cocos2DRes.com
//	说    明:	图片层
//*************************************************************************
#ifndef _IMAGELAYER_H_
#define _IMAGELAYER_H_

#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include "cocos2d.h"
#include "WareFileWrite.h"
#include "WareFileRead.h"
#include "SceneLayer.h"
#include <string>
#include <set>
#include <map>

NS_CC_ENGINE_BEGIN

class CImageLayer : public CSceneLayer
{
	//typedef std::map<uint32,STerrainObj>	TerrainObjMap;
public:
	CImageLayer(void);
	~CImageLayer(void);

	/** create one layer */
	static CImageLayer *create(std::vector<std::string>& vecImages);

	/**
	@brief 初始化
	@param 
	*/
	bool	init(std::vector<std::string>& vecImages);

	/**
	@brief 设置层ID
	@param nLayerID 层ID
	*/
	void	setLayerID( int nLayerID ) { m_nLayerID = nLayerID; }

	/**
	@brief 获取层ID
	@param
	*/
	int		getLayerID() { return m_nLayerID; }

	/**
	 @brief 获取大小
	 @param 
	*/
	void	getSize( cocos2d::CCSize& size ) { size = m_size; }

private:

	int					m_nLayerID;			/// 层ID
	cocos2d::CCSize		m_size;				/// 大小

};

NS_CC_ENGINE_END

#endif // _IMAGELAYER_H_

