//*************************************************************************
//	创建日期:	2014-5-11   13:55
//	文件名称:	IEffectUnitProp.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	特效单元属性
//*************************************************************************
#ifndef _IEFFECTUNITPROP_H_
#define _IEFFECTUNITPROP_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include "WareFileRead.h"
#include "WareFileWrite.h"
#include "EffectProp.h"

NS_CC_ENGINE_BEGIN

enum EEffectUnitType
{
	EFFECTUNIT_TYPE_UNKOWN = 0,
	EFFECTUNIT_TYPE_TEXTUREANI,
	EFFECTUNIT_TYPE_PARTICLESYS,

	EFFECTUNIT_TYPE_COUNT,
};

struct IEffectUnitProp : public CCObject
{
	/**
	@brief 获取特效单元类型
	@param
	*/
	virtual int32				getType() = 0;
	/**
	@brief 加载特效单元
	@param
	*/
	virtual bool				loadProp( CWareFileRead& File ) = 0;
	/**
	@brief 保存特效单元数据
	@param
	*/
	virtual void				saveProp( CWareFileWrite& File ) = 0;

	/// 以下方法 是开放给工具使用
	/**
	@brief 获取特效单元名称
	@param
	*/
	virtual const std::string&	getEffectUnitName() = 0;
	/**
	@brief 获取特效单元属性
	@param
	*/
	virtual void				getProp( std::vector< EffectProp* >& PropList ) = 0;
	/**
	@brief 获取特效单元属性
	@param PropList 属性列表
	@return 设置了多少个属性
	*/
	virtual int32				setProp( std::vector< EffectProp* >& PropList ) = 0;
	/**
	@brief 获取高级属性
	@param
	*/
	virtual uint32				getAdvanceProp( OffsetMap& Offset, NameMap& Texture,SizeMap& Size, ColorMap& Color, AlphaMap& Alpha ) = 0;
	/**
	@brief 设置高级属性
	@param
	*/
	virtual void				setAdvanceProp( OffsetMap& Offset, NameMap& Texture,SizeMap& Size, ColorMap& Color, AlphaMap& Alpha ) = 0;
};

NS_CC_ENGINE_END

#endif // _IEFFECTUNITPROP_H_