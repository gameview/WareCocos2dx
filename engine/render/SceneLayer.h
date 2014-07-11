//*************************************************************************
//	创建日期:	2013-7-4   20:33
//	文件名称:	SceneLayer.h	
//	版权所有:	Cocos2DRes.com
//	说    明:	渲染元图形层
//*************************************************************************
#ifndef _SCENELAYER_H_
#define _SCENELAYER_H_

#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include "cocos2d.h"
#include "WareFileWrite.h"
#include "WareFileRead.h"
#include <string>
#include <set>
#include <map>

NS_CC_ENGINE_BEGIN

class CSceneLayer : public CCLayer
{
public:
	CSceneLayer(void);
	virtual ~CSceneLayer(void);

	/** create one layer */
	static CSceneLayer *create(void);

	/**
	@brief 设置层ID
	@param nLayerID 层ID
	*/
	void	setLayerID( int nLayerID ) { m_nLayerID = nLayerID; }

	/**
	@brief 获取层ID
	@param
	*/
	int	getLayerID() { return m_nLayerID; }


private:

	int					m_nLayerID;			/// 层ID

};

NS_CC_ENGINE_END

#endif // _SCENELAYER_H_

