//********************************************************************
//	文件名称:	CEffect.h
//	创建日期:	2014-6-4 14:14
//	版权信息: 	www.cocos2dres.com
//	创 建 人:   Rare
//  功能说明:   特效实现
//	修改记录:	
//********************************************************************
#ifndef _CEFFECT_H_
#define _CEFFECT_H_

#include "IRenderable.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

struct IEffectProp;
struct IEffectUnitProp;
struct IEffectUnit;
class CEffectUnit;

struct SEffectTrack
{
	SEffectTrack() : pEffectUnit(NULL), fStartPos(0.0f), fEndPos(1.0f), bActive(false) {}

	IEffectUnit*	pEffectUnit;
	float32			fStartPos;
	float32			fEndPos;
	bool			bActive;
};

class CEffect : public IEffect
{
public:
	CEffect( uint32 uID );
	~CEffect(void);

	typedef std::vector< SEffectTrack* > EffectUnitVec;

	/**
	@brief 创建
	@param szPicFileName 资源文件名
	@param ptOffset 偏移值
	@return 返回创建成功与否标识
	*/
	virtual bool		create( const char* szEffectFileName, CCPoint ptOffset = ccp(0,0) );

	/**
	 @brief 释放
	 @param 
	*/
	virtual void		release();

	/**
	 @brief 更新
	 @param 
	*/
	virtual void		update(float dt);

	/**
	 @brief 获取特效属性
	 @param 
	*/
	virtual IEffectProp* getEffectProp();


		/**
	@brief 
	@param
	*/
	virtual void		onExit();

	/**
	@brief 给对象加色
	@param
	*/
	virtual void		setColor( const ccColor3B& color );

	/**
	@brief 设置ID
	@param
	*/
	virtual void		setID( uint32 uID ) { m_uID = uID; }

	/**
	@brief 获取ID
	@param
	*/
	virtual uint32		getID() { return m_uID; }

	/**
	@brief 设置透明度
	@param byAlpha 透明度
	*/
	virtual void		setOpacity( BYTE byAlpha );

	/**
	@brief 获取该对象的特征串(全局唯一)
	@param strFeature 特征串
	*/
	virtual void		getFeatureString( std::string& strFeature );

	/**
	@brief 获取贴图文件名
	@param
	*/
	virtual const std::string getFileName() { return m_strEffectName; }

	/**
	@brief 碰撞检测
	@param
	*/
	virtual bool		hitTest( const CCPoint& pt );

	/**
	@brief 碰撞检测
	@param
	*/
	virtual bool		hitTest( const CCRect& rect );

	/**
	@brief 获取对象包围盒
	@param
	*/
	virtual bool		getBoundingBox( CCRect& boundBox );

	/**
	@brief 获取偏移
	@param
	*/
	virtual void		setOffset( CCPoint& ptOffet );

	/**
	@brief 获取偏移
	@param
	*/
	virtual void		getOffset( CCPoint& ptOffet );

	/**
	 @brief 创建特效
	 @param pEffectProp 特效属性
	*/
	bool				createEffect( IEffectProp* pEffectProp );

	/**
	 @brief 创建特效单元
	 @param[in] pEffectUnitProp 特效单元属性
	*/
	CEffectUnit*		createEffectUnit( IEffectUnitProp* pEffectUnitProp );

	/**
	 @brief 释放特效单元
	 @param[in] pEffectUnit 特效单元
	*/
	void				releaseEffectUnit( IEffectUnit* pEffectUnit );

private:

	std::string			m_strEffectName;
	uint32				m_uID;						/// 特效ID
	uint32				m_uEffectUnitID;			/// 特效单元ID

	IEffectProp*		m_pEffectProp;				/// 特效属性

	EffectUnitVec		m_vecEffectUnitTrack;		/// 特效轨道数据

	uint32				m_uStartTime;				/// 特效时间
};

NS_CC_ENGINE_END

#endif // _CEFFECT_H_