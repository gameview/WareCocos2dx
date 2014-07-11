//*************************************************************************
//	创建日期:	2013-7-12   12:47
//	文件名称:	TerrainGrid.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	地表格
//*************************************************************************
#ifndef _TERRAINGRID_H_
#define _TERRAINGRID_H_

#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include "cocos2d.h"
#include "WareFileWrite.h"
#include "WareFileRead.h"
#include <string>
#include <set>
#include <map>

class CTerrainGrid
{
public:
	CTerrainGrid(void);
	~CTerrainGrid(void);

	void addTerrainObj( unsigned int uid ) { m_setTerrainObj.insert( uid ); }

	//void removeTerrainObj( unsigned int uid ) { m_setTerrainObj.erase(uid); }

	std::set<unsigned int>& getTerrainObjs() { return m_setTerrainObj; }

private:

	std::set< unsigned int > m_setTerrainObj;	/// 对象ID列表
};

#endif // _TERRAINGRID_H_

