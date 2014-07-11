//********************************************************************
//	文件名称:	CTextureAnimation.h
//	创建日期:	2014-6-5 11:07
//	版权信息: 	www.cocos2dres.com
//	创 建 人:   Rare
//  功能说明:   贴图动画
//	修改记录:	
//********************************************************************
#ifndef _CTEXTUREANIMATION_H_
#define _CTEXTUREANIMATION_H_

#include "cocos2d.h"
#include "CEffectUnit.h"

NS_CC_ENGINE_BEGIN

class CTextureAnimation : public CEffectUnit
{
public:
	CTextureAnimation( IEffectUnitProp* pEffectUnitProp,uint32 uID );
	~CTextureAnimation(void);

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

	/**
	 @brief 设置特效单元属性(工具使用)
	 @param 
	*/
	virtual	void		setEffectUnitProp( IEffectUnitProp* pProp );

private:

	CCSprite*			m_pSprite;
};

NS_CC_ENGINE_END

#endif // _CTEXTUREANIMATION_H_
