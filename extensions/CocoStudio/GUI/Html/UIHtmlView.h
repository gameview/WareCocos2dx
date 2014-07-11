/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __UIHTMLVIEW_H__
#define __UIHTMLVIEW_H__

//#include "../UIWidgets/ScrollWidget/UIDragPanel.h"
#include "../UIWidgets/Compatible/CompatibleClasses.h"
#include "../BaseClasses/UIWidget.h"
#include "CHtmlLabel.h"
#include "../UIWidgets/UITextField.h"
#include "../UIWidgets/UIImageView.h"
#include "UIExpression.h"
#include <map>

NS_CC_BEGIN

enum HTMLVIEW_MOVE_DIR
{
    HTMLVIEW_MOVE_DIR_NONE,
    HTMLVIEW_MOVE_DIR_UP,
    HTMLVIEW_MOVE_DIR_DOWN, 
};

/// 输入控件管理
struct SInputInfo
{
	std::string		strName;
	ui::ImageView*	pImageView;	/// 输入框
	ui::TextField*	pTextField;	/// 输入框
};

/// 输入控件管理
struct SExpression
{
	std::string		strName;
	UIExpression*	pExpression;	/// 表情动画
};

typedef std::map< int, SInputInfo >	InputMap;
typedef std::map< int, SExpression > ExpressionMap;

class UIHtmlView : public ui::Layout, public IHtmlLabelEventHandler
{
public:
    UIHtmlView();
    virtual ~UIHtmlView();
    static UIHtmlView* create();

	virtual void onExit();

	/// 设置显示区域大小
	virtual void setSize(const CCSize &size);

	/**
	@brief 设置Lua回调
	@param lpLuaCallName 超连接回调Lua函数名
	@param lpLuaWndCallName 窗口回调Lua函数名
	@param lpLuaDataCallName 数据标签回调Lua函数名
	*/
	virtual void setLuaFuncName(const char * lpLuaCallName,const char * lpLuaWndCallName, const char* lpLuaDataCallName );

	/// 设置显示内容，仅支持html格式串 会清空之前内容
	virtual void setText(const char * strContent);
	/// 追加显示内容
	virtual void addString( const char *strContent );
	/// 清空显示内容
	virtual void clearString();

	/**
	@brief 设置默认字体名
	@param
	*/
	void setFontName( const char* pFontName );

	/**
	@brief 设置字体大小
	@param
	*/
	void setFontSize( int nSize );

	/**
	@brief 设置最大容量
	@param
	*/
	void		setMaxLine( int nMaxLines );

	/**
	@brief 滚动到顶部
	@param
	*/
	void		scrollTop();

	/**
	@brief 滚动到底部
	@param
	*/
	void		scrollBottom();

	/**
	@brief 设置窗口名
	@param
	*/
	void		setWindowName( const char* pszWindowName ) { if( pszWindowName != NULL ) { m_strWindowName = pszWindowName; } }

	/// 获取显示内容总的像素宽高
	CCSize		getContentPixelSize();
	/// 是否允许拖动
	void		setTouchMoved( bool bMoved );

	// 链接事件处理
	virtual void onLinkEvent( const char* pszCommand, bool bSubmit, bool bClose );

	/**
	@brief 元素回调 辑要上层创建的元素
	@param strElementName 元素名称 win input exp
	@param strAttr 属性串
	*/
	virtual void onElement( const std::string& strElementName, CCSize size, std::map<std::string, std::string>& attrs, int nElementID );

	/**
	@brief 数据标签回调
	@param
	*/
	virtual void onDataElement( const std::string& strLabelName, const std::string& strValue );

	/**
	@brief 开始画外部元素
	@param
	*/
	virtual void onBeginDrawElement();

	/**
	@brief 画元素回调
	@param
	*/
	virtual void onDrawElement( int nElementID, CCPoint& pos );

public:	

	virtual bool onTouchBegan(CCTouch* touch,CCEvent *unused_event);
	virtual void onTouchMoved(CCTouch* touch,CCEvent *unused_event);
	virtual void onTouchEnded(CCTouch* touch,CCEvent *unused_event);
	virtual void onTouchCancelled(CCTouch* touch,CCEvent *unused_event); 

protected:

	virtual bool init();
	virtual void initRenderer();

	// 开始
	void handlePressLogic(const CCPoint &touchPoint);
	// 移动
	void handleMoveLogic(const CCPoint &touchPoint);
	// 结束
	void handleReleaseLogic(const CCPoint &touchPoint);

	/// 添加输入框
	void createInputBox( int nID, const char * lpName, CCSize size, int nMaxSize, bool bOnlyNum, const std::string& strDefault );

protected:
	// 移动方向
	HTMLVIEW_MOVE_DIR	m_eMoveDirection;	
	CCHtmlLabel*		m_pCHtmlLabel;

	bool				m_bTouchMovedEnable;		/// 是否允许拖动

	bool				m_bTouchMove;				/// 是否进行了拖动

	CCPoint				m_ptOffset;
	CCPoint				m_ptBengin;
	CCPoint				m_ptTouchStartLocation;
	CCPoint				m_ptTouchEndLocation;
	std::string			m_strLuaName;
	std::string			m_strWndLuaName;
	std::string			m_strDataLuaName;

	std::string			m_strWindowName;

	InputMap			m_mapInput;			/// 输入框列表
	ExpressionMap		m_mapExpression;	/// 表情动画
};

NS_CC_END

#endif /* defined(__CocoGUI__UIHtmlView__) */
