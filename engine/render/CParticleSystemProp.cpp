#include "CParticleSystemProp.h"

NS_CC_ENGINE_BEGIN

CParticleSystemProp::CParticleSystemProp(void)
{
	m_strEffectUnitName = "粒子系统";
	m_uAdvancePropFlag = EFFECT_ADVANCE_OFFSET | EFFECT_ADVANCE_TEXTURE | EFFECT_ADVANCE_SIZE | EFFECT_ADVANCE_COLOR | EFFECT_ADVANCE_ALPHA;
}

CParticleSystemProp::~CParticleSystemProp(void)
{
}

void CParticleSystemProp::registerProp()
{
	//m_vecProp.clear();

	//m_vecProp.push_back( new EffectProp( "宽度", m_nWidth ) );
	//m_vecProp.push_back( new EffectProp( "高度", m_nHeight ) );
	//m_vecProp.push_back( new EffectProp( "动画序列时长", m_nTotalTime ) );
	//m_vecProp.push_back( new EffectProp( "位置", m_Pos ) );
	//m_vecProp.push_back( new EffectProp( "文件名", m_strEffectUnitName, true ) );


}

bool CParticleSystemProp::loadProp( CWareFileRead& File )
{
	return true;
}

void CParticleSystemProp::saveProp( CWareFileWrite& File )
{

}

uint32 CParticleSystemProp::getAdvanceProp( OffsetMap& Offset, NameMap& Texture,SizeMap& Size, ColorMap& Color, AlphaMap& Alpha )
{
	CEffectUnitProp::getAdvanceProp( Offset, Texture, Size, Color, Alpha );

	return m_uAdvancePropFlag;
}

NS_CC_ENGINE_END
