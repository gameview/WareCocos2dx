//********************************************************************
//	文件名称:	CEffectPropManager.h
//	创建日期:	2014-6-5 9:49
//	版权信息: 	www.cocos2dres.com
//	创 建 人:   Rare
//  功能说明:   特效属性资源管理器
//	修改记录:	
//********************************************************************
#ifndef _CEFFECTPROPMANAGER_H_
#define _CEFFECTPROPMANAGER_H_

#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include <map>

NS_CC_ENGINE_BEGIN

struct IEffect;
struct IEffectProp;

class CEffectPropManager
{
private:
	CEffectPropManager(void);
	~CEffectPropManager(void);

	/**
	 @brief 特效属性信息
	 @param 
	*/
	struct SEffectPropInfo
	{
		IEffectProp*	pEffectProp;
		uint32			uLastUseTime;
		int32			nRecycleLevel;

		SEffectPropInfo() : pEffectProp(NULL), uLastUseTime(0),nRecycleLevel(0xffffffff) {}
	};

	typedef std::map< std::string, SEffectPropInfo >	EffectPropMap;

public:

	static CEffectPropManager& Instance()
	{
		static CEffectPropManager _inst;
		return _inst;
	}

	/// 此函数只限于工具可用
	/**
	 @brief 创建空特效属性
	 @param[inout] ppEffectProp 特效属性二级指针
	 @param[in] nRecycleLevel 回收等级
	*/
	int32			createEmptyEffectProp( IEffectProp** ppEffectProp, int32 nRecycleLevel = 0xffffffff );

	/**
	 @brief 创建特效
	 @param[in] szEffectName 特效名称
	 @param[inout] ppEffectProp 特效属性二级指针(用于返回)
	 @param[in] nRecycleLevel 回收等级
	*/
	int32			createEffectProp( const char* szEffectName, IEffectProp** ppEffectProp, int32 nRecycleLevel = 0xffffffff );

	/**
	 @brief 释放特效属性
	 @param[in] pEffectProp 特效属性
	*/
	void			releaseEffectProp( IEffectProp* pEffectProp );

	/**
	 @brief 保存特效属性(工具使用)
	 @param[in] szEffectFileName 特效名称
	 @param[in] pEffect 特效
	*/
	bool			saveEffect( const char* szEffectFileName, IEffect* pEffect );

	///**
	// @brief 更新
	// @param dt 帧间隔时间
	//*/
	//void			update( float32 dt );

protected:

	EffectPropMap	m_mapEffectProp;


	uint32			m_uIDSeed;
	///**
	// @brief 释放资源
	// @param 
	//*/
	//virtual void	DestroyRes( IResource* pResouce );

	///**
	// @brief 应用资源
	// @param 
	//*/
	//int32			applyEffect( IEffectProp* pEffectProp );
};

NS_CC_ENGINE_END

#endif // _CEFFECTPROPMANAGER_H_
