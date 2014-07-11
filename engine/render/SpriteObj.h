////*************************************************************************
//	创建日期:	2013-5-30   8:11
//	文件名称:	SpriteObj.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	游戏对象
//*************************************************************************
#ifndef _SPRITEOBJ_H_
#define _SPRITEOBJ_H_
#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"

#include "IRenderable.h"

NS_CC_ENGINE_BEGIN

class CWareSprite;

class CSpriteObj : public ISprite
{
public:
	CSpriteObj(uint32 uID);
	virtual ~CSpriteObj(void);

	/**
	@brief 创建
	@param szPicFileName 资源文件名
	@param ptOffset 偏移值
	*/
	bool					create( const char* szPicFileName, const CCRect& rect, CCPoint ptOffset = ccp(0,0) );

	/**
	@brief 更新
	@param
	*/
	virtual void			update( float dt );

	/// ISpriteObj接口
	/**
	@brief 
	@param
	*/
	virtual void			onExit();

	/**
	@brief 给对象加色
	@param
	*/
	virtual void			setColor( const ccColor3B& color );

	/**
	@brief 设置透明度
	@param byAlpha 透明度
	*/
	virtual void			setOpacity( BYTE byAlpha );

	/**
	@brief 设置ID
	@param
	*/
	virtual void			setID( uint32 uID ) { m_uID = uID; }

	/**
	@brief 获取ID
	@param
	*/
	virtual uint32			getID() { return m_uID; }

	/**
	@brief 获取该对象的特征串(全局唯一)
	@param
	*/
	virtual void			getFeatureString( std::string& strFeature );

	/**
	@brief 获取贴图文件名
	@param
	*/
	virtual const std::string	getFileName() { return m_strPicFileName; }

	/**
	@brief 碰撞检测
	@param
	*/
	virtual bool			hitTest( const CCPoint& pt );

	/**
	@brief 碰撞检测
	@param
	*/
	virtual bool			hitTest( const CCRect& rect );

	/**
	@brief 
	@param
	*/
	virtual CCSprite*		getSprite() { return (CCSprite*)m_pSprite; }

	/**
	@brief 获取对象包围盒
	@param
	*/
	virtual bool			getBoundingBox( CCRect& boundBox );

	/**
	@brief 获取偏移
	@param
	*/
	virtual void			setOffset( CCPoint& ptOffet );

	/**
	@brief 获取偏移
	@param
	*/
	virtual void			getOffset( CCPoint& ptOffet );

protected:

	/**
	@brief 动作播放回调
	@param
	*/
	void					onActionEnd();

private:

	uint32				m_uID;			/// 对象ID

	CCSprite*			m_pSprite;		/// 精灵对象

	std::string			m_strPicFileName;	/// 贴图文件名
	std::string			m_stFeatureString;	/// 贴图文件名

	//IActionCallBack*	m_pActionCallBack;	/// 动作回调
};

NS_CC_ENGINE_END

#endif // _SPRITEOBJ_H_

