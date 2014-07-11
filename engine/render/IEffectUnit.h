//*************************************************************************
//	创建日期:	2014-5-11   13:45
//	文件名称:	IEffectUnit.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	特效单元
//*************************************************************************
#ifndef _IEFFECTUNIT_H_
#define _IEFFECTUNIT_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

struct IEffectUnitProp;

struct IEffectUnit : public CCNode
{
	/**
	 @brief 获取特效单元ID
	 @param 
	*/
	virtual uint32				getID() = 0;

	/**
	 @brief 创建
	 @param 
	*/
	virtual bool				create() = 0;

	/**
	@brief 获取特效单元属性
	@param
	*/
	virtual IEffectUnitProp*	getEffectUnitProp() = 0;

	/**
	@brief 获取特效单元属性
	@param
	*/
	virtual	void				setEffectUnitProp( IEffectUnitProp* pProp ) = 0;
};

NS_CC_ENGINE_END

#endif // _IEFFECTUNIT_H_