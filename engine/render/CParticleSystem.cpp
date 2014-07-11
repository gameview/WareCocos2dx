#include "CParticleSystem.h"
#include "CParticleSystemProp.h"

NS_CC_ENGINE_BEGIN

CParticleSystem::CParticleSystem(IEffectUnitProp* pEffectUnitProp,uint32 uID)
: CEffectUnit(pEffectUnitProp,uID)
{
}

CParticleSystem::~CParticleSystem(void)
{
}


bool CParticleSystem::create()
{
	return true;
}


void CParticleSystem::update( float32 dt )
{
	CParticleSystemProp* pProp = (CParticleSystemProp*)m_pEffectUnitProp;
	//// 只是发射器的更新(就是产生粒子) 粒子的更新(由影响器决定)放到EffectManager中去
	//pProp->modeA.

	return;
}

NS_CC_ENGINE_END
