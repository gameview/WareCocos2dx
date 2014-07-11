//*************************************************************************
//	创建日期:	2013-7-10   18:10
//	文件名称:	InputManager.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	输入系统实现
//*************************************************************************
#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include <map>
#include <list>

NS_CC_ENGINE_BEGIN

class CInputManager : public CCTargetedTouchDelegate, public CCKeypadDelegate, public CCObject
{
	CInputManager();
	//~CInputManager(void);

public:

	static CInputManager& Instance()
	{
		static CInputManager _inst;
		return _inst;
	}

	/**
	 @brief 设置消息回调处理
	 @param 
	*/
	void			setMessageCallback( IMessageCallBack* pMessageCallback ) { m_pMessageCallback = pMessageCallback; }

	/**
	@brief 设置是否启动触摸
	@param
	*/
	void			setTouchEnabled( bool bTouch );

	/// 触摸开始回调
	virtual bool	ccTouchBegan(CCTouch* touch, CCEvent* event);

	/// 触摸移动回调
	virtual void	ccTouchMoved(CCTouch* touch, CCEvent* event);

	/// 触摸结束回调
	virtual void	ccTouchEnded(CCTouch* touch, CCEvent* event);

	//  The back key clicked
	virtual void	keyBackClicked();

	// The menu key clicked. only available on wophone & android
	virtual void	keyMenuClicked() ;

	/**
	 @brief 更新
	 @param 
	*/
	void			update( float dt );

private:
	IMessageCallBack*	m_pMessageCallback;		/// 消息回调接口
	float				m_fPressTime;			/// 按键按下计时
	bool				m_bPress;				/// 是否按下
	bool				m_bMoved;				/// 是否移动
	CCPoint				m_ptPressPos;			/// 按下位置
	int					m_nTouchID;				/// 按键ID
	bool				m_bLongPress;			/// 是否处于长按状态

};

NS_CC_ENGINE_END

#endif // _INPUTMANAGER_H_
