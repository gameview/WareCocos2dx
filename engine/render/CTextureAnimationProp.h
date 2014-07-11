//********************************************************************
//	文件名称:	CTextureAnimationProp.h
//	创建日期:	2014-6-5 11:29
//	版权信息: 	www.cocos2dres.com
//	创 建 人:   Rare
//  功能说明:   贴图动画属性
//	修改记录:	
//********************************************************************
#ifndef _CTEXTUREANIMATIONPROP_H_
#define _CTEXTUREANIMATIONPROP_H_

#include "CEffectUnitProp.h"

NS_CC_ENGINE_BEGIN

class CTextureAnimationProp : public CEffectUnitProp
{
	friend class CTextureAnimation;
public:
	CTextureAnimationProp();   /// 构造函数中调用虚函数是有问题的
	~CTextureAnimationProp(void);

	virtual int32			getType()			{ return EFFECTUNIT_TYPE_TEXTUREANI; }

	virtual bool			loadProp( CWareFileRead& File );
	virtual void			saveProp( CWareFileWrite& File );

	virtual int32			setProp( std::vector< EffectProp* >& PropList );
	virtual void			getProp( std::vector< EffectProp* >& PropList );

	virtual const std::string&	getEffectUnitName()	{ return m_strEffectUnitName; }

	virtual uint32			getAdvanceProp( OffsetMap& Offset, NameMap& Texture,SizeMap& Size, ColorMap& Color, AlphaMap& Alpha );

	virtual void			registerProp();

protected:

	std::vector< EffectProp* >	m_vecProp;
	std::string				m_strEffectUnitName;

	int32					m_nWidth;		/// 宽度
	int32					m_nHeight;		/// 高度
	int32					m_nTotalTime;	/// 动画序列的总时间
	int32					m_nRenderMode;
	CCPoint					m_Pos;			/// 位置
};

NS_CC_ENGINE_END

#endif // _CTEXTUREANIMATIONPROP_H_