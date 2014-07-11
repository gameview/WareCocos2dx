////*************************************************************************
//	创建日期:	2013-4-12   0:05
//	文件名称:	MessageCommon.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	系统消息宏定义
//*************************************************************************
#ifndef _MESSAGECOMMON_H_
#define _MESSAGECOMMON_H_

/// 系统消息宏定义
#define MSG_TOUCH_BEGIN				1		/// 触屏开始(参数是坐标)
#define MSG_TOUCH_MOVE				2		/// 触屏移动(参数是坐标)
#define MSG_TOUCH_END				3		/// 触屏结束(参数是坐标)
#define MSG_TOUCH_LONGPRESS			4		/// 触屏长按(参数是坐标)

#define MSG_KEY_MENU				100		/// 系统菜单键
#define MSG_KEY_BACK				101		/// 系统返回键

#define MSG_USERID					1000	/// 用户自定义消息开始值

struct IMessageCallBack
{
	/**
	@brief 消息回调
	@param uMsgId 消息ID
	@param wParam
	@param lParam
	@param nTouchID
	@return true 表示可以忽略后面调用 false 则表示还要继续执行
	*/
	virtual bool	onMessageCallBack(unsigned long  uMsgId, unsigned long wParam, unsigned long lParam, int nTouchID ) = 0;
};

#endif // _MESSAGECOMMON_H_