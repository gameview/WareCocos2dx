////*************************************************************************
//	创建日期:	2013-5-30   8:10
//	文件名称:	WareAppDelegate.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	引擎框架APP
//*************************************************************************
#ifndef _WAREAPPDELEGATE_H_
#define _WAREAPPDELEGATE_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include "MessageCommon.h"

NS_CC_ENGINE_BEGIN

/**
@brief    The cocos2d Application.
The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class WareAppDelegate : public CCApplication, public IMessageCallBack
{
public:
    WareAppDelegate();
    virtual ~WareAppDelegate();

	/**
	@brief 消息回调
	@param uMsgId 消息ID
	@param wParam
	@param lParam
	@param nTouchID
	@return true 表示可以忽略后面调用 false 则表示还要继续执行
	*/
	virtual bool	onMessageCallBack( unsigned long uMsgId, unsigned long wParam, unsigned long lParam, int nTouchID );

	/**
	@brief 初始化
	@param
	*/
	virtual bool	init();

	/**
	 @brief 帧更新
	 @param 
	*/
	virtual void	updateFrame( float dt );

	/**
	@brief 场景更新
	@param
	*/
	virtual void	release();

	/**
	@brief 是否显示FPS信息
	@param bShow 显示开关
	*/
	void			setShowFPS( bool bShow );

	/**
	@brief 设置设计大小
	*/
	void			setDesignSize( CCSize& size );

	/**
	@brief 获取设计大小
	@param
	*/
	CCSize&			getDesignSize() { return m_DesignSize; }

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    bool			applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    void			applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    void			applicationWillEnterForeground();

protected:

	bool			m_bTouch;			/// 是否开启触摸
	bool			m_bShowFPS;			/// 显示FPS

	CCSize			m_DesignSize;
};

NS_CC_ENGINE_END

#endif //_WAREAPPDELEGATE_H_