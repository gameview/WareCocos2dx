#ifndef _MAINGAMEAPP_H_
#define _MAINGAMEAPP_H_

#include "cocos2d.h"
#include "WareAppDelegate.h"

/**
@brief    The cocos2d Application.
The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class  CMainGameApp : public cocos2d::engine::WareAppDelegate
{
public:
    CMainGameApp();
    virtual ~CMainGameApp();

	/**
	@brief 初始化
	@param
	*/
	virtual bool	init();


	/**
	@brief 场景更新
	@param
	*/
	virtual void	release();

	/**
	@brief 消息回调
	@param uMsgId 消息ID
	@param wParam 参数
	@param lParam 参数
	@return false 表示可以忽略后面调用 true 则表示还要继续执行
	*/
	virtual bool	onMessageCallBack( unsigned long uMsgId, unsigned long wParam, unsigned long lParam, int nTouchID );

	/**
	@brief 
	@param
	*/
	virtual void	updateFrame( float dt );

private:

};

#endif // _MAINGAMEAPP_H_


