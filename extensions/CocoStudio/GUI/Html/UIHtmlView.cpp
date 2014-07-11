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

#include "UIHtmlView.h"
#include "../UIWidgets/UITextField.h"
#include "StringUtility.h"
#include "StringConvert.h"
#include "CCLuaEngine.h"
#include "../UIWidgets/UIImageView.h"

NS_CC_BEGIN

UIHtmlView::UIHtmlView()
{
   // m_WidgetName = WIDGET_HTMLVIEW;
	m_ptOffset = CCPointZero;
	m_ptTouchEndLocation = CCPointZero;
	m_ptTouchStartLocation = CCPointZero;
	m_bTouchMovedEnable = true;
	m_bTouchMove = false;
	m_strWndLuaName = "";
	m_strWindowName = "";

	m_mapExpression.clear();
}

UIHtmlView::~UIHtmlView()
{
    
}

UIHtmlView* UIHtmlView::create()
{
    UIHtmlView* widget = new UIHtmlView();
    if (widget && widget->init())
    {
		widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

void UIHtmlView::onExit()
{
    //ui::Widget::releaseResoures(); 
	//setUpdateEnable(false);

	unscheduleUpdate();

	setTouchEnabled(false);
    m_pCHtmlLabel = NULL;
}

bool UIHtmlView::init()
{
    if (ui::Widget::init())
    {
		m_pCHtmlLabel->setEventHandler(this);
        scheduleUpdate();
        setTouchEnabled(true);
		setAnchorPoint( ccp(0,0) );
        return true;
    }
    return false;
}

void UIHtmlView::initRenderer()
{
    ui::Widget::initRenderer();
    m_pCHtmlLabel = CCHtmlLabel::create();
	//CCNode* pNode = getContainerNode();
	//if( pNode )
	//{
	CCNode::addChild(m_pCHtmlLabel,1000,-1); 
	//}
	
}

void UIHtmlView::setSize(const CCSize &size)
{
	_ignoreSize = false;
	setContentSize(size);
    ui::Widget::setSize(size); 
    m_pCHtmlLabel->setSize(size);
}

void UIHtmlView::handlePressLogic(const CCPoint &touchPoint)
{
	if( !m_bTouchMovedEnable )
	{
		return;
	}

	CCPoint nsp = convertToNodeSpace(touchPoint);
	m_ptTouchStartLocation = nsp;  /// 绝对坐标

	if( m_pCHtmlLabel )
	{
		m_ptOffset = m_pCHtmlLabel->getOffset();
	}
}  

void UIHtmlView::handleMoveLogic( const CCPoint &touchPoint)
{
	if( !m_bTouchMovedEnable )
	{
		return;
	}

	CCPoint nsp = convertToNodeSpace(touchPoint);
	CCPoint ptOffset = nsp - m_ptTouchStartLocation;

	if (ptOffset.y < 0)
	{
		m_eMoveDirection = HTMLVIEW_MOVE_DIR_DOWN; // down
	}
	else if (ptOffset.y > 0)
	{
		m_eMoveDirection = HTMLVIEW_MOVE_DIR_UP; // up
	}

	//CCLOG( "offset: %f", m_ptOffset.y + ptOffset.y );
	
	m_pCHtmlLabel->setOffset(ccp(0,m_ptOffset.y + ptOffset.y));
	m_bTouchMove = true;
}

void UIHtmlView::handleReleaseLogic( const CCPoint &touchPoint)
{
	if( !m_bTouchMovedEnable )
	{
		return;
	}

	CCPoint nsp = convertToNodeSpace(touchPoint);
	m_ptTouchEndLocation = nsp;

	CCPoint ptOffset = nsp - m_ptTouchStartLocation;
	m_ptOffset = m_ptOffset + ptOffset;

	/// 启动回滚
	if( m_pCHtmlLabel )
	{
		m_pCHtmlLabel->startRollback();
	}
}    

bool UIHtmlView::onTouchBegan(CCTouch* touch,CCEvent *unused_event)
{
	bool bRet = ui::Widget::onTouchBegan(touch,unused_event);
	handlePressLogic(touch->getLocation());
	m_ptBengin= touch->getLocation();
	m_bTouchMove = false;
	return bRet;
}

void UIHtmlView::onTouchMoved(CCTouch* touch,CCEvent *unused_event)
{
	ui::Widget::onTouchMoved(touch,unused_event);
	handleMoveLogic(touch->getLocation());
}

void UIHtmlView::onTouchEnded(CCTouch* touch,CCEvent *unused_event)
{
	ui::Widget::onTouchEnded(touch,unused_event);

	CCPoint pt = touch->getLocation();
	handleReleaseLogic(pt); 

	if( m_pCHtmlLabel && (!m_bTouchMove || (m_bTouchMove && (abs( pt.y -m_ptBengin.y )< 30))))
	{
		CCPoint nsp = convertToNodeSpace(pt);
		m_pCHtmlLabel->onEventProcess(nsp);
	}
}

void UIHtmlView::onTouchCancelled(CCTouch* touch,CCEvent *unused_event)
{
	ui::Widget::onTouchCancelled(touch,unused_event);
}

void UIHtmlView::setText(const char * strContent) 
{
	clearString();
	addString(strContent);
}

void UIHtmlView::addString( const char *strContent )
{
	if( strContent == NULL )
	{
		return;
	}

	std::string str = strContent;
	m_pCHtmlLabel->addString(str);
}
void UIHtmlView::clearString()
{
	m_pCHtmlLabel->clearString();
	m_ptOffset = CCPointZero;
	m_pCHtmlLabel->setOffset( m_ptOffset );

	InputMap::iterator iter = m_mapInput.begin();
	for( ; iter != m_mapInput.end(); ++iter )
	{
		if(iter->second.pImageView)
		{
			removeChild( iter->second.pImageView, true );
		}
	}
	m_mapInput.clear();

	ExpressionMap::iterator itExp = m_mapExpression.begin();
	for( ; itExp != m_mapExpression.end(); ++itExp )
	{
		if(itExp->second.pExpression)
		{
			removeChild( itExp->second.pExpression, true );
		}
	}
	m_mapExpression.clear();
}
/**
@brief 设置默认字体名
@param
*/
void UIHtmlView::setFontName( const char* pFontName )
{
	if( m_pCHtmlLabel )
	{
		m_pCHtmlLabel->setFontName( pFontName );
	}
}
//-------------------------------------------------------------------------
/**
@brief 设置字体大小
@param
*/
void UIHtmlView::setFontSize( int nSize )
{
	if( m_pCHtmlLabel )
	{
		m_pCHtmlLabel->setFontSize( nSize );
	}
}
//-------------------------------------------------------------------------
/**
@brief 设置最大容量
@param
*/
void UIHtmlView::setMaxLine( int nMaxLines )
{
	if( m_pCHtmlLabel != NULL )
	{
		m_pCHtmlLabel->setMaxLine( nMaxLines );
	}
}
//-------------------------------------------------------------------------
/**
@brief 滚动到顶部
@param
*/
void UIHtmlView::scrollTop()
{
	if( m_pCHtmlLabel != NULL )
	{
		m_pCHtmlLabel->scrollToTop();
	}
}
//-------------------------------------------------------------------------
/**
@brief 滚动到底部
@param
*/
void UIHtmlView::scrollBottom()
{
	if( m_pCHtmlLabel != NULL )
	{
		m_pCHtmlLabel->scrollToBottom();
	}
}
//-------------------------------------------------------------------------
/// 获取显示内容总的像素宽高
CCSize UIHtmlView::getContentPixelSize()
{
	if( m_pCHtmlLabel != NULL )
	{
		return m_pCHtmlLabel->getPixelSize();
	}

	return CCSizeMake( 0, 0 );
}
/// 是否允许拖动
void UIHtmlView::setTouchMoved( bool bMoved )
{
	m_bTouchMovedEnable = bMoved;
	//setTouchEnabled( bMoved );
}

// 链接事件处理
void UIHtmlView::onLinkEvent( const char* pszCommand, bool bSubmit, bool bClose )
{
	if( pszCommand == NULL )
	{
		return;
	}

	if( m_strLuaName.empty() )
	{
		return;
	}

	std::string strCommand = pszCommand;

	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
	if(pEngine != NULL)
	{
		if( bSubmit )
		{
			if( !m_mapInput.empty() )
			{
				size_t pos = strCommand.find( '?' );
				if( pos == std::string::npos )
				{
					strCommand.append("?");
				}
				else
				{
					strCommand.append("&");
				}
			}

			size_t size = m_mapInput.size();
			size_t nIndex = 0;
			InputMap::iterator iter = m_mapInput.begin();
			for( ; iter != m_mapInput.end(); ++iter )
			{
				/// 获取输入框参数
				//m_mapInput[nElementID] = info;
				nIndex++;
				strCommand.append( iter->second.strName );
				strCommand.append( "=" );
				strCommand.append( iter->second.pTextField->getStringValue() );

				if( nIndex < size )
				{
					strCommand.append("&");
				}
			}
		}

		/// 查看input数据 
		CCLuaValue In[3];
		In[0] = CCLuaValue::stringValue(m_strWindowName);
		In[1] = CCLuaValue::stringValue(strCommand);
		In[2] = CCLuaValue::booleanValue(bClose);
		pEngine->ExecuteLuaFunction( m_strLuaName.c_str(), In, 3 );
	}
}
//-------------------------------------------------------------------------
/**
@brief 元素回调 辑要上层创建的元素
@param strElementName 元素名称 win input exp
@param strAttr 属性串
*/
void UIHtmlView::onElement( const std::string& strElementName, CCSize size, std::map<std::string, std::string>& attrs, int nElementID )
{
	if( strElementName == "win" )
	{
		if( m_strWndLuaName.empty() )
		{
			return;
		}

		CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
		if(pEngine != NULL)
		{
			CCLuaValue In[4];
			std::map<std::string, std::string>::iterator iter = attrs.find( "name" );
			if( iter != attrs.end() )
			{
				In[0] = CCLuaValue::stringValue(iter->second);
			}
			else
			{
				In[0] = CCLuaValue::stringValue("");
			}
			iter = attrs.find( "id" );
			if( iter != attrs.end() )
			{
				int nID = cocos2d::engine::StringConvert::parseInt( iter->second );
				In[1] = CCLuaValue::intValue(nID);
			}
			else
			{
				In[1] = CCLuaValue::intValue(0);
			}

			iter = attrs.find( "alpha" );
			if( iter != attrs.end() )
			{
				int nID = cocos2d::engine::StringConvert::parseInt( iter->second );
				In[2] = CCLuaValue::intValue(nID);
			}
			else
			{
				In[2] = CCLuaValue::intValue(0);
			}

			iter = attrs.find( "close" );
			if( iter != attrs.end() )
			{
				int nID = cocos2d::engine::StringConvert::parseInt( iter->second );
				In[3] = CCLuaValue::booleanValue(nID==1);
			}
			else
			{
				In[3] = CCLuaValue::booleanValue(false);
			}

			pEngine->ExecuteLuaFunction( m_strWndLuaName.c_str() ,In,4);
		}
	}
	else if( strElementName == "input" )
	{
		std::string strName;
		std::map<std::string, std::string>::iterator iter = attrs.find( "name" );
		if( iter != attrs.end() )
		{
			strName = iter->second;
		}

		int nMax = 0;
		iter = attrs.find( "max" );
		if( iter != attrs.end() )
		{
			nMax = cocos2d::engine::StringConvert::parseInt( iter->second );
		}

		bool bOnlyNum = false;
		iter = attrs.find( "num" );
		if( iter != attrs.end() )
		{
			bOnlyNum = cocos2d::engine::StringConvert::parseInt( iter->second ) == 1;
		}

		std::string strDefault;
		iter = attrs.find( "value" );
		if( iter != attrs.end() )
		{
			strDefault = iter->second;
		}

		createInputBox( nElementID, strName.c_str(), size, nMax, bOnlyNum, strDefault );

	}
	else if( strElementName == "exp" )
	{
		SExpression info;

		std::map<std::string, std::string>::iterator iter = attrs.find( "id" );
		if( iter == attrs.end() )
		{
			return;
		}

		static char szExpBuff[512];
		memset( szExpBuff, 0, sizeof(szExpBuff) );
		sprintf( szExpBuff, "Exp/%s.exp", iter->second.c_str() );
		/// 表情动画
		UIExpression* pExpression = UIExpression::create( szExpBuff );
		if( pExpression == NULL )
		{
			return;
		}

		addChild( pExpression );
		pExpression->setVisible( true );

		info.strName = strElementName;
		info.pExpression = pExpression;

		m_mapExpression[nElementID] = info;
	}
}
//-------------------------------------------------------------------------
/**
@brief 数据标签回调
@param
*/
void UIHtmlView::onDataElement( const std::string& strLabelName, const std::string& strValue )
{
	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
	if(pEngine == NULL)
	{
		return;
	}

	if( m_strDataLuaName.empty() )
	{
		return;
	}

	CCLuaValue In[2];
	In[0] = CCLuaValue::stringValue(strLabelName);
	In[1] = CCLuaValue::stringValue(strValue);
	pEngine->ExecuteLuaFunction( m_strDataLuaName.c_str() ,In, 2 );	
}
//-------------------------------------------------------------------------
/**
@brief 开始画外部元素
@param
*/
void UIHtmlView::onBeginDrawElement()
{
	/// 首先
	InputMap::iterator iter = m_mapInput.begin();
	for( ; iter != m_mapInput.end(); ++iter )
	{
		if( iter->second.pImageView == NULL )
		{
			return;
		}

		iter->second.pImageView->setVisible( false );
	}

	ExpressionMap::iterator itExp = m_mapExpression.begin();
	for( ; itExp != m_mapExpression.end(); ++itExp )
	{
		if( itExp->second.pExpression == NULL )
		{
			return;
		}

		itExp->second.pExpression->setVisible( false );
	}
}
//-------------------------------------------------------------------------
/**
@brief 画元素回调
@param
*/
void UIHtmlView::onDrawElement( int nElementID, CCPoint& pos )
{
	InputMap::iterator iter = m_mapInput.find(nElementID);		/// 输入框列表
	if( iter != m_mapInput.end() )
	{
		if( iter->second.pImageView == NULL )
		{
			return;
		}

		CCSize size = m_pCHtmlLabel->getSize();
		CCSize s = iter->second.pTextField->getContentSize();
		if( pos.y <= 0 || pos.y + s.height >= size.height )
		{
			iter->second.pImageView->setVisible( false );
		}
		else
		{
			iter->second.pImageView->setVisible( true );
			iter->second.pImageView->setPosition( pos );
		}
	}

	ExpressionMap::iterator itExp = m_mapExpression.find(nElementID);
	if( itExp != m_mapExpression.end() )
	{
		if( itExp->second.pExpression == NULL )
		{
			return;
		}

		CCSize size = m_pCHtmlLabel->getSize();
		CCSize s = itExp->second.pExpression->getContentSize();
		if( pos.y <= 0 || pos.y + s.height >= size.height )
		{
			itExp->second.pExpression->setVisible( false );
		}
		else
		{
			itExp->second.pExpression->setVisible( true );
			itExp->second.pExpression->setPosition( pos );
		}
	}
}
//-------------------------------------------------------------------------
void UIHtmlView::createInputBox( int nID, const char * lpName, CCSize size, int nMaxSize, bool bOnlyNum, const std::string& strDefault )
{
	if( lpName == NULL )
	{
		return;
	}

	ui::ImageView* pImageView = ui::ImageView::create();
	if( pImageView == NULL )
	{
		return;
	}

	pImageView->loadTexture( "Exp/input.png" );
	addChild( pImageView );
	pImageView->setScale9Enabled( true );
	//pImageView->setScale9Size( size );
	pImageView->setCapInsets(CCRectMake(3, 3, 3, 3));
	pImageView->setPosition( CCPointZero );
	pImageView->setAnchorPoint( CCPointZero );
	pImageView->setVisible( true );

	ui::UITextField* pTextField = ui::UITextField::create();
	if( pTextField == NULL)
	{
		return;
	}

	pTextField->setName(lpName);
	pImageView->addChild( pTextField );
	pTextField->setSize( size );
	pTextField->setPosition( CCPointZero );
	pTextField->setAnchorPoint( CCPointZero );
	pTextField->setVisible( true );
	pTextField->setTouchEnabled( true );
	pTextField->setMaxLength( nMaxSize );
	pTextField->setMaxLengthEnabled( true );
	pTextField->setText( strDefault.c_str() );

	SInputInfo info;
	info.pImageView = pImageView;
	info.pTextField = pTextField;
	info.strName = lpName;

	m_mapInput[nID] = info;
}

void UIHtmlView::setLuaFuncName( const char * lpLuaName ,const char * lpLuaWndCallName, const char* lpLuaDataCallName )
{
	if( lpLuaName!= NULL &&  strlen(lpLuaName) > 0 )
	{
		m_strLuaName = lpLuaName;
	}

	if (lpLuaWndCallName != NULL && strlen(lpLuaWndCallName) > 0)
	{
		m_strWndLuaName = lpLuaWndCallName;
	}

	if( lpLuaDataCallName != NULL && strlen(lpLuaDataCallName) > 0 )
	{
		m_strDataLuaName = lpLuaDataCallName;
	}
}

NS_CC_END