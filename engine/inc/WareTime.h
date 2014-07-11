//*************************************************************************
//	创建日期:	2013-6-2   11:55
//	文件名称:	WareTime.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	时间方法集
//*************************************************************************
#ifndef _WARETIME_H_
#define _WARETIME_H_

#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

class WareTime
{
public:

	/**
	@brief 获取系统运行时间
	@param
	*/
	static uint32		getTickCount();
};

NS_CC_ENGINE_END

#endif // _WARETIME_H_

