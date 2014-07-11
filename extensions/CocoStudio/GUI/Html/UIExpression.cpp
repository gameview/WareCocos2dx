#include "UIExpression.h"
#include "WareIniFile.h"
#include "StringUtility.h"
#include "cocos2d.h"
#include <string>

NS_CC_BEGIN

UIExpression::UIExpression(void)
{
	m_nTimer = 0;
	m_nFrameNum = 0;
	m_nFrameDelay = 0;
}

UIExpression::~UIExpression(void)
{
	unscheduleUpdate();
}
//-------------------------------------------------------------------------
/**
@brief 创建
@param
*/
UIExpression* UIExpression::create( const char* szExpName )
{
	UIExpression* widget = new UIExpression();
	if (widget && widget->init( szExpName ))
	{
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return NULL;
}
//-------------------------------------------------------------------------
// 初始化
bool UIExpression::init( const char* szExpName )
{
	if(ui::ImageView::init())
	{
		if( szExpName == NULL )
		{
			return false;
		}

		/// 读取表情资源包数据
		cocos2d::engine::CWareIniFile IniFile;
		if( !IniFile.loadFile( szExpName ) )
		{
			return false;
		}

		std::string strExpName = szExpName;
		std::string strPathName, strFileName, strFileExtName;
		cocos2d::engine::StringUtility::parseFileName( szExpName, strPathName, strFileName, strFileExtName );

		m_nFrameNum = IniFile.getInt( "expression", "frame_num", 0 );

		std::string strImageName, strImageFmt;
		IniFile.getString( "expression", "image_name", strImageName );
		IniFile.getString( "expression", "image_fm", strImageFmt );

		m_nFrameDelay = IniFile.getInt( "expression", "frame_delay", 0 );

		char szFrameBuff[1024];
		for( int i = 0; i < m_nFrameNum; ++i )
		{
			memset( szFrameBuff, 0, sizeof(szFrameBuff) );
			sprintf( szFrameBuff, "Exp/%s/%s%d.%s", strFileName.c_str(), strImageName.c_str(), i, strImageFmt.c_str() );
			m_vecImageName.push_back( szFrameBuff );
		}
		
		scheduleUpdate();
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
/**
@brief 
@param
*/
void UIExpression::update(float dt)
{
	m_nTimer += dt * 1000;
	int nFrame =  m_nTimer / m_nFrameDelay;
	int nCurFrame = nFrame % m_nFrameNum;

	loadTexture( m_vecImageName[nCurFrame].c_str() );
}

NS_CC_END
