//*************************************************************************
//	创建日期:	2014-5-11   13:42
//	文件名称:	IEffectProp.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	特效属性数据
//*************************************************************************
#ifndef _IEFFECTPROP_H_
#define _IEFFECTPROP_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include <string>

NS_CC_ENGINE_BEGIN

struct IEffectUnitProp;

/// EffectTrack
struct SEffectUnitPropTrack
{
	SEffectUnitPropTrack() : fStartPos(0.0f), fEndPos(1.0f), pEffectUnitProp(NULL) {}

	float32				fStartPos;
	float32				fEndPos;
	IEffectUnitProp*	pEffectUnitProp; 
};

typedef std::vector< SEffectUnitPropTrack* >	EffectUnitTrack;

struct IEffectProp : public CCObject
{
	//////////////////////////////////////////////////////////////////////////
	/// 为Tool准备的函数
	/**
	 @brief 添加特效单元属性
	 @param 
	*/
	virtual int32					addEffectUnitProp( int32 nEffectUnitType ) = 0;
	/**
	 @brief 插入特效单元属性
	 @param 
	*/
	virtual void					insertEffectUnitProp( int32 nIndex, int32 nEffectUnitType ) = 0;

	/// 给已有的轨道设置EffectUnitProp, 如果之前已经存在一个EffectUnitProp,则替换掉以前的
	virtual void					setEffectUnitProp( int32 nIndex, int32 nEffectUnitType ) = 0;
	virtual SEffectUnitPropTrack*	getEffectTrack( int32 nIndex ) = 0;
	virtual void					removeEffectUnitProp( int32 nIndex ) = 0;
	virtual const EffectUnitTrack&	getEffectUnitTracks() = 0;

	/**
	 @brief 获取ID
	 @param 
	*/
	virtual int32					getID() = 0;
	/**
	 @brief 有效性
	 @param 
	*/
	virtual bool					isValid() = 0;

	/// 属性设置
	virtual const std::string&		getCustomName() = 0;
	virtual void					setCustomName( const std::string& strName ) = 0;
	virtual uint32					getEffectTime() = 0;
	virtual void					setEffectTime( uint32 uTime ) = 0;
	//////////////////////////////////////////////////////////////////////////
};

NS_CC_ENGINE_END

#endif // _IEFFECTPROP_H_