//********************************************************************
//	文件名称:	CEffectManager.h
//	创建日期:	2014-6-5 9:45
//	版权信息: 	www.cocos2dres.com
//	创 建 人:   Rare
//  功能说明:   特效管理器
//	修改记录:	
//********************************************************************
#ifndef _CEFFECTMANAGER_H_
#define _CEFFECTMANAGER_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

struct IEffect;
struct IEffectProp;

//class CEffectSprite;
//class IParticleAffector;
//class IResourceManager;

//#define MAX_PARTICLE_COUNT  30000

//struct SParticle
//{
//	SParticle() : pSprite(NULL), nLiftTime(0), pNext(NULL) {}
//
//	CEffectSprite*		pSprite;
//	int32				nLiftTime;
//	IParticleAffector*	pAffector;
//	SParticle*			pNext;
//};
//
///// 粒子影响器
//class IParticleAffector
//{
//public:
//	virtual void update( SParticle* pParticle, uint32 uCurTime ) = 0;
//};

/// 单件
class CEffectManager
{
private:
	CEffectManager(void);
	~CEffectManager(void);

public:
	
	static CEffectManager& getInstance()
	{
		static CEffectManager _instance;
		return _instance;
	}

	//SParticle* 		allocParticle();
	//void			freeParticle( SParticle* pParticel );

	/// 以下方法仅供工具使用
	/**
	 @brief 创建空特效
	 @param 
	*/
	IEffect*		createEffect( IEffectProp* pEffectProp );

	/**
	 @brief 根据特效文件创建
	 @param 
	*/
	IEffect*		createEffectFromFile( const char* szEffectFileName );

	///**
	// @brief 释放特效
	// @param 
	//*/
	//void			releaseEffect( IEffect* pEffect );

	/**
	 @brief 保存特效
	 @param 
	*/
	void			saveEffect( const char* szFileName, IEffect* pEffect );

	bool			update( uint32 uCurTime );

private:

	uint32			m_uEffectIDSeed;
	//int32			m_nCurParticle;
	//SParticle		m_Particle[MAX_PARTICLE_COUNT];
	//SParticle*	m_ParticlePtr[MAX_PARTICLE_COUNT];
};

NS_CC_ENGINE_END

#endif // _CEFFECTMANAGER_H_
