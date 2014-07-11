//********************************************************************
//	文件名称:	CEffectUnit.h
//	创建日期:	2014-6-4 17:39
//	版权信息: 	www.cocos2dres.com
//	创 建 人:   Rare
//  功能说明:   特效单元
//	修改记录:	
//********************************************************************
#ifndef _CEFFECTUNIT_H_
#define _CEFFECTUNIT_H_

#include "IEffectUnit.h"

NS_CC_ENGINE_BEGIN

struct IEffectUnitProp;

class CEffectUnit : public IEffectUnit
{
public:

	CEffectUnit( IEffectUnitProp* pEffectUnitProp, uint32 uID );
	~CEffectUnit();

	typedef std::map< float32, CCSpriteFrame* >  TextureMap;

	/**
	 @brief 创建
	 @param 
	*/
	virtual bool				create();

	/**
	 @brief 释放
	 @param 
	*/
	virtual void				release();

	/**
	 @brief 获取特效单元ID
	 @param 
	*/
	virtual uint32				getID()			{ return m_uID; }

	virtual IEffectUnitProp*	getEffectUnitProp();

	virtual	void				setEffectUnitProp( IEffectUnitProp* pProp );

	void						setStartTime( uint32 uStartTime )	{ m_uStartTime = uStartTime; }

	int32						getType();
	void						createTexture();
	void						releaseAllTexture();
	
protected:

	IEffectUnitProp*			m_pEffectUnitProp;		/// 特效单元属性
	TextureMap					m_mapTexture;			/// 贴图列表

	uint32						m_uStartTime;			/// 开始时间
	int32						m_nEffectUnitType;		/// 特效单元类型

	uint32						m_uID;					/// 特效单元ID
};

NS_CC_ENGINE_END

#endif // _CEFFECTUNIT_H_