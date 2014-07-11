//*************************************************************************
//	创建日期:	2014-5-11   13:59
//	文件名称:	CEffectProp.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	特效属性
//*************************************************************************
#ifndef _CEFFECTPROP_H_
#define _CEFFECTPROP_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include "IEffectProp.h"

NS_CC_ENGINE_BEGIN

class CEffectUnitProp;

//////////////////////////////////////////////////////////////////////////
/// 这里说明下几个概念: CEffectProp ->  CEffect
///						CEffectUnitProp -> CEffectUnit
/// 一个CEffect是由多个CEffectUnit来组成的，他们在各自不同的轨道上
/// CEffectProp保存的就是各个EffectUnit的关系的信息
/// CEffectUnitProp保存各CEffectUnit的信息
//////////////////////////////////////////////////////////////////////////
class CEffectProp : public IEffectProp
{
	friend class CEffect;
public:
	CEffectProp( uint32 uID );
	~CEffectProp();

	/**
	 @brief 释放
	 @param 
	*/
	virtual void					release();

	/// 可以用一个CEffectProp创建很多不同的CEffect,给CEffectProp添加引用计数，但是CEffectUnitProp
	/// 是不会被重用的，他只存在于一个CEffectProp中，所以这里使用CEffectUnitProp来做数据key
	//typedef map< IEffectUnitProp*, SEffectUnitPropTrack* > EffectUnitTrack;
	//////////////////////////////////////////////////////////////////////////
	/// IResource
	virtual int32					getID()									{ return m_uID; }

	///// 载入资源描述信息
	//virtual int32					LoadDesc();
	/// 载入资源数据部分(只有这一部分是可以放到另一个线程去做的)
	virtual int32					loadRes();	
	///// 卸载资源数据部分
	//virtual int32					UnloadRes();
	///// 资源数据部分载入完毕后的回调
	//virtual bool					OnResLoaded();

	virtual bool					saveRes( const char* szFileName );

	//virtual int32					Grab();
	//virtual int32					Drop();

	/**
	 @brief 有效性
	 @param 
	*/
	virtual bool					isValid()								{ return m_bValid; }

	//////////////////////////////////////////////////////////////////////////
	/// IEffectProp for tool
	virtual int32					addEffectUnitProp( int32 nEffectUnitType );
	virtual void					insertEffectUnitProp( int32 nIndex, int32 nEffectUnitType );
	virtual void					setEffectUnitProp( int32 nIndex, int32 nEffectUnitType );
	virtual SEffectUnitPropTrack*	getEffectTrack( int32 nIndex );
	virtual void					removeEffectUnitProp( int32 nIndex );
	virtual const EffectUnitTrack&	getEffectUnitTracks()					{ return m_vecEffectUnitProp; }

	/// 属性设置
	virtual const std::string&		getCustomName()							{ return m_strCustomName; }
	virtual void					setCustomName( const std::string& strName )	{ m_strCustomName = strName; }
	virtual uint32					getEffectTime()							{ return m_uTotalTime; }
	virtual void					setEffectTime( uint32 uTime )			{ m_uTotalTime = uTime; }
	//////////////////////////////////////////////////////////////////////////

	/**
	 @brief 创建对象
	 @param 
	*/
	bool							create( const char* szFileName );

	void							setID( uint32 uID )						{ m_uID = uID; }

	void							setResourceName( const char* szFileName )	{ m_strFileName = szFileName; }
	const std::string&				getResourceName() { return m_strFileName; }

	void							getTrackCount();

	CEffectUnitProp*				createEffectUnitProp( int32 nType );
	void							releaseEffectUnitProp( IEffectUnitProp* pEffectUnitProp );
	
protected:

	uint32							m_uID;
	std::string						m_strFileName;
	bool							m_bValid;

	std::string						m_strCustomName;
	uint32							m_uTotalTime;
	EffectUnitTrack					m_vecEffectUnitProp;
};

NS_CC_ENGINE_END

#endif // _CEFFECTPROP_H_