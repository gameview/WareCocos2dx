////*************************************************************************
//	创建日期:	2013-5-30   8:12
//	文件名称:	SceneManager.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	游戏场景管理器
//*************************************************************************
#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include <map>
#include <set>


NS_CC_ENGINE_BEGIN

class CRoot;
class CRenderMap;
struct IRenderMapCallBack;

class CSceneManager
{
	typedef std::set< CRenderMap* >		RenderMapSet;

	CSceneManager(void);
	~CSceneManager(void);

public:

	static CSceneManager& Instance()
	{
		static CSceneManager _inst;
		return _inst;
	}

	/**
	@brief 设置根节点
	@param
	*/
	void			setRoot( CCScene* pScene ) { m_pRoot = pScene; }

	/**
	@brief 
	@param
	*/
	CCScene*		getRoot() { return m_pRoot; }

	/**
	@brief 进入场景
	@param[in] szSceneName 场景数据文件名
	*/
	uint32			enterScene( const char* szSceneConfigName, IRenderMapCallBack* pRenderMapCallBack );

	/**
	@brief 获取当前场景对象
	@param
	*/
	CRenderMap*	getCurrentRenderMap() { return m_pActiveScene; }

	/**
	@brief 更新函数 为资源释放和缓存留下执行机会
	@param
	*/
	void			update( float dt );

private:

	CCScene*		m_pRoot;				/// 根节点
	
	uint32			m_uSceneSeed;			/// 场景ID种子
	CRenderMap*		m_pActiveScene;			/// 当前激活场景
	RenderMapSet	m_setGameSceneReclaim;	/// 场景回收器

};

NS_CC_ENGINE_END

#endif // _MAINSCENE_H_