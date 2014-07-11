////*************************************************************************
//	创建日期:	2013-5-30   8:11
//	文件名称:	RenderObj.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	游戏对象
//*************************************************************************
#ifndef _RENDEROBJ_H_
#define _RENDEROBJ_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
//#include "WareAnimate.h"
#include "IRenderable.h"
#include "IRenderObj.h"
#include "RenderObjModel.h"
#include <map>
#include <set>

NS_CC_ENGINE_BEGIN

class CWareSprite;
struct IRenderText;

class CRenderObj : public CCNode, public IRenderObj
{
public:

	typedef std::map< std::string, IRenderable* >	ObjPartMap;
	//typedef std::map< std::string, IRenderable* > ObjLocatorMap;
	typedef std::map< std::string, IRenderable* >	ObjEffectMap;

	struct LinkObj
	{
		std::string	strLoccator;
		CRenderObj*	pRenderObj;
	};

	/// LinkObj
	typedef std::map< uint32, LinkObj >		LinkObjMap;

	/// RenderText
	typedef std::set< IRenderText* >		RenderTextSet;

public:
	CRenderObj(uint32 uID);
	virtual ~CRenderObj(void);

	/**
	@brief 更新
	@param
	*/
	void			update( float dt );

	/**
	@brief 创建
	@param
	*/
	bool			create( const char* szObjFileName );

	/**
	@brief 释放
	@param
	*/
	void			onExit();

	/**
	@brief 设置ID
	@param
	*/
	void			setID( uint32 uID ) { m_uID = uID; }

	/**
	@brief 设置用户数据
	@param
	*/
	virtual void	setUserData( void* pData ) { m_pUserData = pData; }

	/**
	@brief 获取用户数据
	@param
	*/
	virtual void*	getUserData() { return m_pUserData; }

	/**
	@brief 绑定到场景上
	@param
	*/
	virtual void	attach( CCNode* pParentNode, int nZOrder, int nTag = 0 );

	/**
	@brief 从场景上解除
	@param
	*/
	virtual void	detach(bool bRemove = true );

	/**
	@brief 释放
	@param
	*/
	virtual void	drop();

	/**
	@brief 设置位置
	@param
	*/
	virtual void	setPos( const CCPoint& ptLoc );

	/**
	@brief 获取位置
	@param
	*/
	virtual void	getPos( CCPoint& ptLoc );

	/**
	@brief 设置旋转
	@param
	*/
	virtual void	setRotate( float fRotate );

	/**
	@brief 获取旋转值
	@param
	*/
	virtual float	getRotate();

	/**
	@brief 设置缩放
	@param
	*/
	virtual void	setZoom( float fZoom );

	/**
	@brief 获取缩放
	@param
	*/
	virtual float	getZoom();

	/**
	@brief 获取ID
	@param
	*/
	virtual uint32	getID() { return m_uID; }

	/**
	@brief 给对象加色
	@param
	*/
	virtual void	setColor( const ccColor3B& color );

	/**
	@brief 设置透明度
	@param
	*/
	virtual void	setOpacity( BYTE byAlpha );

	/**
	@brief 设置是否允许update
	@param
	*/
	virtual void	setEnableUpdate( bool bEnable );

	/**
	@brief 设置是否可见
	@param
	*/
	virtual void	visible( bool bVisible );

	/**
	@brief 获取该对象的特征串(全局唯一)
	@param
	*/
	virtual void	getFeatureString( std::string& strFeature );

	/**
	@brief 获取游戏对象配置文件
	@param
	*/
	virtual const std::string& getFileName() { return m_strObjFileName; }

	/**
	@brief 碰撞检测
	@param
	*/
	virtual bool	hitTest( const CCPoint& pt );

	/**
	@brief 碰撞检测
	@param
	*/
	virtual bool	hitTest( const CCRect& rect );

	/**
	@brief 设置能否被选中
	@param
	*/
	virtual void	setEnableSelect( bool bEnable ) { m_bSelected = bEnable; }

	/**
	@brief 设置动画播放速度
	@param
	*/
	virtual void	setAnimationSpeed( float fSpeed );

	/**
	@brief 设置动画播放回调
	@param
	*/
	virtual void	setAnimationCallback( IAnimationCallback* pAniCallBack, void* pContext );

	/**
	@brief 播放动画 如果该动作处于停止状态，则继续播放
	@param[in] szAniName 动作名称
	@param[in] fSpeed 播放速度
	@param[in] nLoops 循环次数
	@param[in] bForceUpdate 是否强制更新动作数据(动作重新播放)
	*/
	virtual void	playAnimation( const char* szAniName, float fSpeed = 1.0f, int nLoops = -1, bool bForceUpdate = false );

	/**
	@brief 播放动画
	@param[in] szAniName 动作名称
	@param[in] pAniCallBack 动画播放回调
	@param[in] nStopFrame 停止的帧号
	@param[in] fSpeed 播放速度
	*/
	virtual void	playAndStop( const char* szAniName, int nStopFrame, float fSpeed = 1.0f );

	/**
	@brief 获取当前帧
	@param
	*/
	virtual int		getCurFrame();

	/**
	@brief 动画暂停(游戏中不会使用,工具中使用)
	@param
	*/
	virtual void	pause();

	/**
	@brief 获取动画列表
	@param
	*/
	virtual void	getAnimations( std::list<std::string>& lstAnimation );

	/**
	@brief 获取游戏对象高度
	@param[in] uint32 uid
	*/
	virtual float	getHeight();

	/**
	@brief 获取绑定点的偏移
	@param szLocator 绑定名
	@param ptOffset 偏移
	*/
	virtual void	getLocator( const char* szLocator, CCPoint& ptOffset );

	/**
	@brief 挂接对象
	@param[in] szLinkobjName link对象名 必须是obj
	@param[in] szLocator  绑定点名称
	@param[in] nDepth 深度 
	@param[in] ptOffset 与宿主对象偏移
	*/
	virtual unsigned int addLinkObj( const char* szLinkobjName, const char* szLocator, int nDepth, float fScale = 1.0f, cocos2d::ccColor4B color = ccc4(255,255,255,255) );

	/**
	@brief 挂接对象
	@param[in] szLinkobjName link对象名 必须是obj
	@param[in] szLocator  绑定点名称
	@param[in] nDepth 深度 
	@param[in] ptOffset 与宿主对象偏移
	*/
	virtual unsigned int addLinkObj( IRenderObj* pRenderObj, const char* szLocator, int nDepth, float fScale = 1.0f, cocos2d::ccColor4B color = ccc4(255,255,255,255) );

	/**
	@brief 删除挂接对象
	@param[in] uLinkObjID 游戏对象ID
	*/
	virtual void	removeLinkObj( unsigned int uLinkObjID );

	/**
	@brief 删除挂接对象
	@param[in] uid 游戏对象ID
	@param[in] szLinkobjName link对象名 可以是 oms oss或者obj
	*/
	virtual IRenderObj* removeLinkObj( unsigned int uLinkObjID, bool bRemove );

	/**
	@brief 删除挂接对象
	*/
	virtual void	removeAllLinkObj();

	/**
	@brief 获取挂接对象
	@param
	*/
	virtual IRenderObj*	getLinkObj( unsigned int uLinkObjID );

	/**
	@brief 更新部件
	@param szPartObjName 部件对象名
	@param szPartPlaceName 部件位置名称
	*/
	virtual void	changePart( const char* szPartObjName, const char* szPartPlaceName, int nDepth );

	/**
	@brief 删除特效
	*/
	virtual void	removeEffectByName( const char* szEffectObjName );

	/**
	@brief 删除特效
	*/
	virtual void	removeAllEffect();

	/**
	@brief 
	@param
	*/
	virtual void	setOffset( CCPoint& ptOffset );

	/**
	@brief 
	@param
	*/
	virtual CCPoint	getOffset();

	/**
	@brief 添加文字渲染对象
	@param
	*/
	virtual bool	addRenderText( IRenderText* pRenderText, std::string& strLocator, const CCPoint& ptOffset );

	/**
	@brief 删除文字渲染对象
	@param
	*/
	virtual void	removeRenderText( IRenderText* pRenderText );

	/**
	@brief 清理对象(部件，特效) 只保留空对象
	@param
	*/
	virtual void	clear();

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief 设置是否在场景
	@param
	*/
	void			setInScene( bool bInScene ) { m_bInScene = bInScene; }

	/**
	@brief 是否在场景中
	@param
	*/
	bool			isInScene() { return m_bInScene; }

	/**
	@brief 
	@param
	*/
	bool			isSelected() { return m_bSelected; }

	/**
	@brief 更新部件
	@param szPartObjName 部件对象名
	@param szPartPlaceName 部件位置名称
	*/
	void			changeEffect( const std::string& strEffectName, const std::string& strLocatorName, const std::string& strEffectFileName, CCPoint& ptOffset, ccColor4B& color, CCPoint& scale, int nDepth );

	/**
	@brief 设置模板数据
	@param
	*/
	void				setObjModel( CRenderObjModel* pObjModel );

	CRenderObjModel*	getObjMode() { return m_pRenderObjModel; }

	/**
	@brief 获取部件数据
	@param
	*/
	CRenderObjModel::ObjPartMap&	getParts() { CCAssert( m_pRenderObjModel != NULL, "对象模板数据不能为空" ); return m_pRenderObjModel->m_mapPart; }

	/**
	@brief 获取绑定点数据
	@param
	*/
	CRenderObjModel::ObjLocatorMap&	getLocators() { CCAssert( m_pRenderObjModel != NULL, "对象模板数据不能为空" );  return m_pRenderObjModel->m_mapLocator; }

	/**
	@brief 获取特效数据
	@param
	*/
	CRenderObjModel::ObjEffectMap&	getEffects() { CCAssert( m_pRenderObjModel != NULL, "对象模板数据不能为空" );  return m_pRenderObjModel->m_mapEffect; }

	/**
	@brief 删除节点
	@param
	*/
	void			removePart( const std::string& strName, bool bRemoveData = true );

	/**
	@brief 删除节点
	@param
	*/
	void			removeLocator( const std::string& strLocatorName, bool bRemoveData = true );

	/**
	@brief 删除节点
	@param
	*/
	void			removeEffect( const std::string& strEffectName, bool bRemoveData = true );

	/**
	@brief 保存到文件
	@param
	*/
	void			saveToFile( const std::string& strObjFileName );

protected:

	/**
	@brief 创建part
	@param
	*/
	void			createPart();

	/**
	@brief 创建特效
	@param
	*/
	void			createEffect();

	/**
	@brief 添加part
	@param
	*/
	bool			_createPart( CRenderObjModel::ObjPart& part );

	/**
	@brief 添加part
	@param
	*/
	bool			_createEffect( CRenderObjModel::ObjEffect& effect );

private:

	uint32				m_uID;				/// 对象ID

	void*				m_pUserData;		/// 用户数据指针

	IRenderable*		m_pRootRenderable;		/// 主渲染对象
	IRenderable*		m_pBodyRenderable;		/// 身体渲染对象

	std::string			m_strObjFileName;	/// 贴图文件名

	bool				m_bNeedUpdate;		/// 是否需要更新

	bool				m_bDrag;			/// 是否被抓起

	bool				m_bSelected;		/// 能否被选中

	bool				m_bInScene;			/// 是否在场景标识

	CCPoint				m_ptPos;			/// 记录位置
	CCPoint				m_ptOffset;			/// 偏移位置，用来设置锚点

	std::string			m_strCurrentAniName;/// 当前动作名

	CRenderObjModel*	m_pRenderObjModel;	/// 对象模板

	ObjPartMap			m_mapPart;			/// 对象部件
	//ObjLocatorMap		m_mapLocator;		/// 绑定点
	ObjEffectMap		m_mapEffect;		/// 特效数据
	LinkObjMap			m_mapLinkObj;		/// 链接对象
	RenderTextSet		m_setRenderText;	/// 文字渲染对象
};

NS_CC_ENGINE_END

#endif // _RENDEROBJ_H_

