#include "CEffectUnitProp.h"
#include "StringUtility.h"

NS_CC_ENGINE_BEGIN

bool CEffectUnitProp::loadProp( CWareFileRead& File )
{
	StringUtility::loadMapValue( m_mapOffset, File );
	StringUtility::loadMapString( m_mapTextureFileName, File );
	StringUtility::loadMapValue( m_mapSize, File );
	StringUtility::loadMapValue( m_mapColor, File );
	StringUtility::loadMapValue( m_mapAlpha, File );

	return true;
}

void CEffectUnitProp::saveProp( CWareFileWrite& File )
{
	StringUtility::saveMapValue( m_mapOffset, File );
	StringUtility::saveMapString( m_mapTextureFileName, File );
	StringUtility::saveMapValue( m_mapSize, File );
	StringUtility::saveMapValue( m_mapColor, File );
	StringUtility::saveMapValue( m_mapAlpha, File );
}

uint32 CEffectUnitProp::getAdvanceProp( OffsetMap& Offset, NameMap& Texture,SizeMap& Size, ColorMap& Color, AlphaMap& Alpha )
{
	Offset = m_mapOffset;
	Texture = m_mapTextureFileName;
	Size = m_mapSize;
	Color = m_mapColor;
	Alpha = m_mapAlpha;

	return 0;	
}

void CEffectUnitProp::setAdvanceProp( OffsetMap& Offset, NameMap& Texture,SizeMap& Size, ColorMap& Color, AlphaMap& Alpha )
{
	m_mapOffset = Offset;
	m_mapSize = Size;
	m_mapColor = Color;
	m_mapAlpha = Alpha;
	m_mapTextureFileName = Texture;	
}

NS_CC_ENGINE_END