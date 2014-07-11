//********************************************************************
//	文件名称:	CParticleSystem.h
//	创建日期:	2014-6-5 10:59
//	版权信息: 	www.cocos2dres.com
//	创 建 人:   Rare
//  功能说明:   粒子系统
//	修改记录:	
//********************************************************************
#ifndef _CPARTICLESYSTEM_H_
#define _CPARTICLESYSTEM_H_

#include "cocos2dx_macros.h"
#include "CEffectUnit.h"

NS_CC_ENGINE_BEGIN

class CParticleSystem : public CEffectUnit
{
public:
	CParticleSystem( IEffectUnitProp* pEffectUnitProp,uint32 uID );
	~CParticleSystem(void);
	
	/**
	 @brief 创建
	 @param 
	*/
	virtual bool		create();

	/**
	@brief 更新
	@param dt 帧间隔时间
	*/
	virtual void		update( float32 dt );

};


NS_CC_ENGINE_END

#endif // _CPARTICLESYSTEM_H_
