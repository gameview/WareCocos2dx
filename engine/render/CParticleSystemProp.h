//********************************************************************
//	文件名称:	CParticleSystemProp.h
//	创建日期:	2014-6-5 11:04
//	版权信息: 	www.cocos2dres.com
//	创 建 人:   Rare
//  功能说明:   粒子系统属性
//	修改记录:	
//********************************************************************
#ifndef _CPARTICLESYSTEMPROP_H_
#define _CPARTICLESYSTEMPROP_H_

#include "CEffectUnitProp.h"

NS_CC_ENGINE_BEGIN

/////////////////////////////////////////////////////////////////////////////////
/// 一个粒子系统只包含一个发射器,每一个发射器都有一个自己的粒子影响器
/// 粒子发射器负责发射粒子，粒子影响则会影响粒子发射后的行为
/// 粒子影响器会包含粒子生命周期中的一些属性的变化以及会附加一些像重力这样的影响
///
/////////////////////////////////////////////////////////////////////////////////
class CParticleSystemProp : public CEffectUnitProp
{
	friend class CParticleSystem;

public:
	CParticleSystemProp(void);
	~CParticleSystemProp(void);

	virtual int32			getType()			{ return EFFECTUNIT_TYPE_PARTICLESYS; }

	virtual bool			loadProp( CWareFileRead& File );
	virtual void			saveProp( CWareFileWrite& File );

	virtual const std::string&	getEffectUnitName()	{ return m_strEffectUnitName; }

	virtual uint32			getAdvanceProp( OffsetMap& Offset, NameMap& Texture,SizeMap& Size, ColorMap& Color, AlphaMap& Alpha );

	virtual void			registerProp();

protected:
	std::vector< EffectProp* >	m_vecProp;
	std::string			m_strEffectUnitName;

	// Different modes
	//! Mode A:Gravity + Tangential Accel + Radial Accel
	struct {
		/** Gravity value. Only available in 'Gravity' mode. */
		CCPoint gravity;
		/** speed of each particle. Only available in 'Gravity' mode.  */
		float speed;
		/** speed variance of each particle. Only available in 'Gravity' mode. */
		float speedVar;
		/** tangential acceleration of each particle. Only available in 'Gravity' mode. */
		float tangentialAccel;
		/** tangential acceleration variance of each particle. Only available in 'Gravity' mode. */
		float tangentialAccelVar;
		/** radial acceleration of each particle. Only available in 'Gravity' mode. */
		float radialAccel;
		/** radial acceleration variance of each particle. Only available in 'Gravity' mode. */
		float radialAccelVar;
		/** set the rotation of each particle to its direction Only available in 'Gravity' mode. */
		bool rotationIsDir;
	} modeA;

	//! Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
	struct {
		/** The starting radius of the particles. Only available in 'Radius' mode. */
		float startRadius;
		/** The starting radius variance of the particles. Only available in 'Radius' mode. */
		float startRadiusVar;
		/** The ending radius of the particles. Only available in 'Radius' mode. */
		float endRadius;
		/** The ending radius variance of the particles. Only available in 'Radius' mode. */
		float endRadiusVar;            
		/** Number of degrees to rotate a particle around the source pos per second. Only available in 'Radius' mode. */
		float rotatePerSecond;
		/** Variance in degrees for rotatePerSecond. Only available in 'Radius' mode. */
		float rotatePerSecondVar;
	} modeB;

	/** switch between different kind of emitter modes:
    - kCCParticleModeGravity: uses gravity, speed, radial and tangential acceleration
    - kCCParticleModeRadius: uses radius movement + rotation
    */
	int32				m_nEmitterMode; 
	/** Quantity of particles that are being simulated at the moment */
	uint32				m_uParticleCount;
	/** How many seconds the emitter will run. -1 means 'forever' */
	float32				m_fDuration;
	/** sourcePosition of the emitter */
	CCPoint				m_tSourcePosition;
	/** Position variance of the emitter */
	CCPoint				m_tPosVar;
	/** life, and life variation of each particle */
	float				m_fLife;
	/** life variance of each particle */
	float				m_fLifeVar;
	/** angle and angle variation of each particle */
	float				m_fAngle;
	/** angle variance of each particle */
	float				m_fAngleVar;

	/// particle prop
	/** whether or not the particles are using blend additive.
    If enabled, the following blending function will be used.
    @code
    source blend function = GL_SRC_ALPHA;
    dest blend function = GL_ONE;
    @endcode
    */
    bool				m_bIsBlendAdditive;

	/** start size in pixels of each particle */
    float32				m_fStartSize;
    /** size variance in pixels of each particle */
    float32				m_fStartSizeVar;
    /** end size in pixels of each particle */
    float32				m_fEndSize;
    /** end size variance in pixels of each particle */
    float32				m_fEndSizeVar;
    /** start color of each particle */
    ccColor4F			m_tStartColor;
    /** start color variance of each particle */
	ccColor4F			m_tStartColorVar;
    /** end color and end color variation of each particle */
    ccColor4F			m_tEndColor;
    /** end color variance of each particle */
    ccColor4F			m_tEndColorVar;
    //* initial angle of each particle
    float32				m_fStartSpin;
    //* initial angle of each particle
    float32				m_fStartSpinVar;
    //* initial angle of each particle
    float32				m_fEndSpin;
    //* initial angle of each particle
    float32				m_fEndSpinVar;
    /** emission rate of the particles */
    float32				m_fEmissionRate;
    /** maximum particles of the system */
    uint32				m_uTotalParticles;
    /** conforms to CocosNodeTexture protocol */
    //CCTexture2D*		m_pTexture;
	std::string			m_strTextureName;
    /** conforms to CocosNodeTexture protocol */
    ccBlendFunc			m_tBlendFunc;
    /** does the alpha value modify color */
    bool				m_bOpacityModifyRGB;
	
};

NS_CC_ENGINE_END

#endif // _CPARTICLESYSTEMPROP_H_
