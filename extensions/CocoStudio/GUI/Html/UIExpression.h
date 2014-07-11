//*************************************************************************
//	创建日期:	2013-10-26   16:22
//	文件名称:	UIExpression.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	表情动画
//*************************************************************************
#ifndef _UIEXPRESSION_H__
#define _UIEXPRESSION_H__

#include "../UIWidgets/UIImageView.h"
#include <map>
#include <vector>

NS_CC_BEGIN

class UIExpression : public ui::ImageView
{
public:
	UIExpression(void);
	~UIExpression(void);

	/**
	@brief 创建
	@param szExpName 表情名称
	*/
	static UIExpression* create( const char* szExpName );

	/**
	@brief 
	@param
	*/
	virtual void	update(float dt);

protected:
	// 初始化
	virtual bool init( const char* szExpName );

private:
	int									m_nTimer;			/// 计时器
	int									m_nFrameNum;		/// 帧数
	int									m_nFrameDelay;		/// 帧间隔

	std::vector<std::string>			m_vecImageName;
};

NS_CC_END

#endif // _UIEXPRESSION_H__