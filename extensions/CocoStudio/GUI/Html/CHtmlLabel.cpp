#include "CHtmlLabel.h"
#include "IHtmlElement.h"
#include "StringUtility.h"
#include "StringConvert.h"

NS_CC_BEGIN
//////////////////////////////////////////////////////////////////////////

CCHtmlLabel::CCHtmlLabel(void)
: m_pSprite(NULL)
, m_pRenderTarget(NULL)
, m_pHandler(NULL)
, m_Size(0.0f,0.0f)
, m_nMargin(PAN_WIDTH)
, m_nLineSpace(LINE_SPACE)
, m_ptOffset(0.0f,0.0f)
, m_bNeedUpdate(false)
, m_bForceUpdateOffset(false)
, m_nMaxLine(500)
{
}

CCHtmlLabel::~CCHtmlLabel(void)
{
	//setTouchEnabled(false);
	clearString();
}
//-------------------------------------------------------------------------
/** creates a CCHtmlLabel with a font name and font size in points
@since v2.0.1
*/
CCHtmlLabel* CCHtmlLabel::create( int nMargin, int nLineSpace )
{
	CCHtmlLabel * pRet = new CCHtmlLabel();
	if (pRet && pRet->init(nMargin,nLineSpace))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

//-------------------------------------------------------------------------
/**
*  Initializes the instance of CCNode
*  @return Whether the initialization was successful.
*/
bool CCHtmlLabel::init( int nMargin, int nLineSpace )
{
	m_nMargin = nMargin;
	m_nLineSpace = nLineSpace;
	scheduleUpdate();

	return true;
}

void CCHtmlLabel::setSize(const CCSize& size)
{ 
	bool bx = (m_Size.width == size.width);
	m_Size = size;
	//if( m_pSprite )
	//{
	//	m_pSprite->setContentSize( m_Size );
	//}
	m_bNeedUpdate = true;
	if (!bx)
	{
		changeWidth(m_Size.width);
	}
	
}
/**
@brief 设置默认字体名
@param
*/
void CCHtmlLabel::setFontName( const char* pFontName )
{
	m_HtmlParse.setFontName( pFontName );
}

/**
@brief 设置字体大小
@param
*/
void CCHtmlLabel::setFontSize( int nSize )
{
	m_HtmlParse.setFontSize( nSize );
}
//-------------------------------------------------------------------------
/**
@brief 
@param
*/
void CCHtmlLabel::update(float delta)
{
	//if( m_vecContent.empty() )
	//{
	//	return;
	//}

	CCSize s = getPixelSize();
	if( s.height > m_Size.height && m_bForceUpdateOffset )
	{
		m_ptOffset.y = s.height - m_Size.height; 
	}

	bool bUpdate = false;

	if( m_bNeedUpdate )
	{
		/// 筛选需要显示的行列表
		m_vecExShowLines.clear();

		int nHeight = 0;
		{
			size_t size = m_vecContent.size();
			for( size_t i = 0; i < size; ++i )
			{
				SContextInfo& info = m_vecContent[i];
				size_t line_size = info.vExLines.size();
				for( size_t l = 0;  l < line_size; ++l )
				{
					if( nHeight > m_ptOffset.y + m_Size.height )
					{
						break;
					}

					if( info.vExLines[l].nHeight == 0 )
					{
						continue;
					}

					int nLineHeight = nHeight + m_nLineSpace;
					nHeight = nLineHeight + info.vExLines[l].nHeight;
					if( nHeight > m_ptOffset.y )
					{
						info.vExLines[l].nYPos = m_Size.height - nLineHeight + m_ptOffset.y;
						m_vecExShowLines.push_back( &info.vExLines[l] );
					}	
				}
			}
		}

		m_bNeedUpdate = false;
		bUpdate = true;
	}

	//size_t size = m_vecExShowLines.size();
	//if( !bUpdate )
	//{
	//	for( size_t i = 0; i < size; ++i )
	//	{
	//		size_t ele_num = m_vecExShowLines[i]->elements.size();
	//		for( size_t j = 0; j < ele_num; ++j )
	//		{
	//			m_vecExShowLines[i]->elements[j].aniAttribute.fUpdateTime += delta;
	//			if( m_vecExShowLines[i]->elements[j].type == CHtmlParse::DrawElement_Ani && m_vecExShowLines[i]->elements[j].aniAttribute.fUpdateTime >= m_vecShowLines[i]->elements[j].aniAttribute.fDelay )
	//			{
	//				bUpdate = true;
	//				m_vecShowLines[i]->elements[j].aniAttribute.nCurFrame++;
	//				if( m_vecShowLines[i]->elements[j].aniAttribute.nCurFrame >= m_vecShowLines[i]->elements[j].aniAttribute.nFrames )
	//				{
	//					m_vecShowLines[i]->elements[j].aniAttribute.nCurFrame = 0;
	//				}

	//				char szFrameName[256];
	//				sprintf( szFrameName, "%s%d.png", m_vecShowLines[i]->elements[j].aniAttribute.strSrc.c_str(), m_vecShowLines[i]->elements[j].aniAttribute.nCurFrame );
	//				m_vecShowLines[i]->elements[j].mText = szFrameName;

	//				m_vecShowLines[i]->elements[j].aniAttribute.fUpdateTime = 0.0f;
	//			}
	//		}
	//	}
	//}

	if( !bUpdate )
	{
		return;
	}

	if( m_pHandler )
	{
		/// 通知外部元素开始绘画
		m_pHandler->onBeginDrawElement();
	}

	m_pRenderTarget = CCRenderTexture::create( m_Size.width, m_Size.height );
	if( m_pRenderTarget == NULL )
	{
		return;
	}

	m_pRenderTarget->begin();

	int nXPox = m_nMargin;
	int nYPos = 0;

	size_t size = m_vecExShowLines.size();
	for( size_t i = 0; i < size; ++i )
	{
		nYPos = m_vecExShowLines[i]->nYPos;
		//CHtmlParse::LineContainer l =  *m_vecShowLines[i];
		drawLine( (int)m_ptOffset.x + nXPox, nYPos, *m_vecExShowLines[i] );
		nXPox = m_nMargin;
	}

	m_pRenderTarget->end();

	CCTexture2D *texture = m_pRenderTarget->getSprite()->getTexture();
	if( texture == NULL )
	{
		return;
	}

	texture->setAliasTexParameters();

	if( m_pSprite )
	{
		removeChild( m_pSprite );
		m_pSprite = NULL;
	}

	if( m_pSprite == NULL )
	{
		CCTexture2D *texture = m_pRenderTarget->getSprite()->getTexture();
		texture->setAliasTexParameters();

		m_pSprite = CCSprite::createWithTexture( texture );
		setContentSize(m_Size);
		m_pSprite->setAnchorPoint(ccp(0,0));
		m_pSprite->setPosition(ccp(0, 0));
		m_pSprite->setFlipY(true);
		addChild(m_pSprite);
	}
	//else
	//{
	//	m_pSprite->setTexture( texture );
	//}
}
//-------------------------------------------------------------------------
/**
@brief 添加字符串，支持html格式 默认往后追加
@param
*/
void CCHtmlLabel::addString( const std::string& strContent )
{
	if( strContent == "" )
	{
		return;
	}

	SContextInfo info;
	info.strContent = strContent;

	// 这里针对<good id=9003 icon=9001 num=1>标签做一个特殊处理
	static char* s_Color[5] = { "#ffffff", "#ffffff", "#ffffff", "#ffffff", "#ffffff" };
	size_t good_begin_pos = 0;
	size_t good_end_pos = 0;
	std::string strGood;
	good_begin_pos = info.strContent.find( "<good " );
	while( good_begin_pos != std::string::npos )
	{
		for( size_t i = good_begin_pos; i < info.strContent.length(); ++i )
		{
			if( strContent.at(i) == '>' )
			{
				std::string strGoodProp = info.strContent.substr( good_end_pos + 5, i - good_begin_pos - 5 );
				strGood = info.strContent.substr( good_end_pos, i - good_begin_pos + 1 );
				std::map<std::string, std::string> attr;
				m_HtmlParse.parseAttribute( strGoodProp, attr );

				unsigned int uGoodID = cocos2d::engine::StringConvert::parseInt( attr["id"] );
				unsigned int uIconID = cocos2d::engine::StringConvert::parseInt( attr["icon"] );
				unsigned int uNum = cocos2d::engine::StringConvert::parseInt( attr["num"] );
				int	nQuality = cocos2d::engine::StringConvert::parseInt( attr["quality"] );
				if( nQuality == 0 )
				{
					nQuality = 1;
				}
				if( nQuality > 5 )
				{
					nQuality = 5;
				}
				std::string strGoodName = attr["name"];

				static char szNew[2048];
				memset( szNew, 0, sizeof(szNew) );
				sprintf( szNew, "<img src='Icon/%d.png'><a href='equip?id=%d'><font color=%s>%s</font></a> X%d", uIconID, uGoodID, s_Color[nQuality-1], strGoodName.c_str(), uNum );

				cocos2d::engine::StringUtility::stringReplace( info.strContent, strGood, szNew );
				break;
			}
		}

		good_begin_pos = info.strContent.find( "<good " );
	}

	m_HtmlParse.parseHtml( info.strContent );
	//m_HtmlParse.generateDrawelementsAndLines( m_Size.width - PAN_WIDTH * 2, info.vLines, m_pHandler );
	m_HtmlParse.generateDrawelementsAndLines( m_Size.width - PAN_WIDTH * 2, info.vExLines, m_pHandler );

	m_vecContent.push_back( info );

	//if( m_nMaxLine)
	int nLine = getLine();
	if( nLine > m_nMaxLine )
	{
		removeLines( nLine - m_nMaxLine );
	}

	m_bNeedUpdate = true;
}
//-------------------------------------------------------------------------
void CCHtmlLabel::clearString()
{
	size_t size = m_vecContent.size();
	for( size_t i = 0; i < size; ++i )
	{
		SContextInfo& info = m_vecContent[i];
		size_t l = info.vExLines.size();
		for( size_t j = 0; j < l; ++j )
		{
			size_t e = info.vExLines[j].elements.size();
			for( size_t en = 0; en < e; ++en )
			{
				if( info.vExLines[j].elements[en] )
				{
					info.vExLines[j].elements[en]->release();
				}
			}
		}
	}

	m_vecContent.clear();
	m_vecExShowLines.clear();
	m_bNeedUpdate = true;
}

//-------------------------------------------------------------------------
/**
@brief 修改宽度
@param
*/
void CCHtmlLabel::changeWidth( float fWidth )
{
	m_Size.width = fWidth;

	size_t size = m_vecContent.size();
	for( size_t i = 0; i < size; ++i )
	{
		SContextInfo& info = m_vecContent[i];
		m_HtmlParse.parseHtml( info.strContent );
		//m_HtmlParse.generateDrawelementsAndLines( m_Size.width - PAN_WIDTH * 2, info.vLines, m_pHandler );
		m_HtmlParse.generateDrawelementsAndLines( m_Size.width - PAN_WIDTH * 2, info.vExLines, NULL );
	}

	m_bNeedUpdate = true;
}
//-------------------------------------------------------------------------
/**
@brief 获取内容的像素大小(包含行间距)
@param
*/
CCSize CCHtmlLabel::getPixelSize()
{
	int nHeight = 0;
	size_t size = m_vecContent.size();
	for( size_t i = 0; i < size; ++i )
	{
		size_t lnum = m_vecContent[i].vExLines.size();
		for( size_t j = 0; j < lnum; ++j )
		{
			nHeight += m_vecContent[i].vExLines[j].nHeight;
			nHeight += m_nLineSpace;
		}
	}

	return CCSize( m_Size.width, nHeight - m_nLineSpace );
}
//-------------------------------------------------------------------------
/**
@brief 设置显示偏移
@param 
*/
void CCHtmlLabel::setOffset( const CCPoint& offset )
{
	m_ptOffset = offset;
	m_bNeedUpdate = true;
}
/**
@brief 滚动到顶部
@param
*/
void CCHtmlLabel::scrollToTop()
{
	setOffset( ccp(0,0) );
}
//-------------------------------------------------------------------------
/**
@brief 滚动到底部
@param
*/
void CCHtmlLabel::scrollToBottom()
{
	float fContentHeight = getPixelSize().height;
	if( fContentHeight < m_Size.height )
	{
		setOffset( ccp(0,0) );
	}
	else
	{
		//( m_ptOffset.y + m_Size.height - m_nMargin > fContentHeight )
		//{
			float fTarget = fContentHeight - m_Size.height + m_nMargin;
		//}

		setOffset( ccp(0,fTarget) );
	}
}
//-------------------------------------------------------------------------
/**
@brief 画一行
@param
*/
void CCHtmlLabel::drawLine( int nXStartPos, int nYStartpos, CHtmlParse::LineContainerEx& line )
{
	if( line.elements.empty() )
	{
		return;
	}

	int nXPos = nXStartPos;
	int nYPos = nYStartpos;

	size_t size = line.elements.size();
	for( size_t i = 0; i < size; ++i )
	{
		nYPos = nYStartpos;
		//CHtmlParse::DrawElement& e = line.elements[i];
		CCSize size = line.elements[i]->getContentSize();
		nXPos += size.width / 2;
		nYPos = nYPos - line.nHeight + size.height / 2;
		//e.rect.origin = ccp( nXPos, nYPos );

		if( line.elements[i] )
		{
			line.elements[i]->draw( ccp(nXPos, nYPos) );
		}

		if( line.elements[i]->getType() == HtmlElement_Exp || line.elements[i]->getType() == HtmlElement_Input )
		{
			if( m_pHandler )
			{
				CCPoint pos = ccp(nXPos - size.width / 2, nYPos - size.height / 2);
				m_pHandler->onDrawElement( line.elements[i]->getID(), pos );
			}
		}

		nXPos += size.width / 2;
	}
}
//-------------------------------------------------------------------------
/**
@brief 事件处理
@param ptLoc 触摸位置
*/
void CCHtmlLabel::onEventProcess( const CCPoint& ptLoc )
{
	if( m_pHandler == NULL )
	{
		return;
	}

	CCPoint ptPos = getPosition();
	ptPos = ptLoc - ptPos;

	//bool bUpdate = false;
	//size_t size = m_vecShowLines.size();
	//for( size_t i = 0; i < size; ++i )
	//{
	//	size_t ele_num = m_vecShowLines[i]->elements.size();
	//	for( size_t j = 0; j < ele_num; ++j )
	//	{
	//		CHtmlParse::DrawElement& e = m_vecShowLines[i]->elements[j];
	//		if( e.type == CHtmlParse::DrawElement_Link )
	//		{
	//			CCRect rect( e.rect.origin.x - e.rect.size.width / 2, e.rect.origin.y - e.rect.size.height / 2, e.rect.size.width, e.rect.size.height );
	//			if( rect.containsPoint( ptPos ) )
	//			{
	//				std::string strAction, strParam;
	//				parseCommand( e.mCommand, strAction, strParam );
	//				m_pHandler->onLinkEvent( strAction.c_str(), strParam.c_str(), true );
	//			}
	//		}
	//	}
	//}

	size_t size = m_vecExShowLines.size();
	for( size_t i = 0; i < size; ++i )
	{
		if( m_vecExShowLines[i] == NULL )
		{
			continue;
		}

		size_t ele_num = m_vecExShowLines[i]->elements.size();
		for( size_t j = 0; j < ele_num; ++j )
		{
			IHtmlElement* pHtmlElement = m_vecExShowLines[i]->elements[j];
			if( pHtmlElement == NULL )
			{
				continue;
			}

			if( pHtmlElement->getType() == HtmlElement_Link || pHtmlElement->getType() == HtmlElement_ImageLink )
			{
				if( pHtmlElement->containsPoint( ptPos ) )
				{
					pHtmlElement->onEvent( m_pHandler );
				}
			}
		}
	}
}
//-------------------------------------------------------------------------
/**
@brief 解析命令串
@param
*/
void CCHtmlLabel::parseCommand( const std::string& strCommand, std::string& strAction, std::string& strParam )
{
	const char* pBuff = strCommand.c_str();
	char szOutBuff[256];
	do 
	{
		memset( szOutBuff, 0, sizeof(szOutBuff) );
		pBuff = cocos2d::engine::StringUtility::parseSubString( pBuff, '?', szOutBuff );
		if( szOutBuff[0] != NULL )
		{
			std::string strOut = szOutBuff;
			cocos2d::engine::StringUtility::trim( strOut );
			std::string strProp = pBuff;
			cocos2d::engine::StringUtility::trim(strProp);
			strAction = strOut;
			strParam = strProp;
			break;
		}
	} while ( pBuff[0] != NULL );
}
//-------------------------------------------------------------------------
/**
@brief 启动回滚(只针对y轴的偏移)
@param
*/
#define ROLLBACK_SPEED  700.0f
void CCHtmlLabel::startRollback()
{
	float fContentHeight = getPixelSize().height;
	float fOffset = 0.0f;
	if( fContentHeight < m_Size.height )
	{
		if( m_ptOffset.y != 0.0f )
		{
			m_fTarget = 0.0f;
		}
		else
		{
			return;
		}
	}
	else
	{
		if( m_ptOffset.y < 0 )
		{
			m_fTarget = 0.0f;
		}
		else if( m_ptOffset.y + m_Size.height - m_nMargin > fContentHeight )
		{
			m_fTarget = fContentHeight - m_Size.height + m_nMargin;
		}
		else
		{
			return;
		}
	}

	fOffset = m_ptOffset.y - m_fTarget;
	/// 启动回滚
	m_fRollbackRemainTime = fabs( fOffset ) / ROLLBACK_SPEED; /// 0.8秒的回滚时间  ROLLBACK_SPEED
	m_fRollbackSpeed = ROLLBACK_SPEED;

	if( fOffset < 0 )
	{
		m_fDir = 1.0f;
	}
	else
	{
		m_fDir = -1.0f;
	}

	schedule( schedule_selector(CCHtmlLabel::rollback) );
}
//-------------------------------------------------------------------------
/**
@brief 回滚函数调用
@param
*/
void CCHtmlLabel::rollback( float dt )
{
	//CCLOG( "CCHtmlLabel::rollback:%f", dt );
	m_fRollbackRemainTime -= dt;
	if( m_fRollbackRemainTime <= 0.0f )
	{
		m_ptOffset.y = m_fTarget;
		setOffset( m_ptOffset );
		unschedule( schedule_selector(CCHtmlLabel::rollback) );
		return;
	}

	//float fSpeed = m_fRollbackRemainTime / ROLLBACK_TIME * m_fRollbackSpeed;
	float fMoveDis = ( m_fRollbackSpeed ) * dt * m_fDir;
	//if( fMoveDis <= 0.0f )
	//{
	//	return;
	//}

	//CCLOG( "CCHtmlLabel::rollback move dis : %f", fMoveDis );

	m_ptOffset.y += fMoveDis;

	setOffset( m_ptOffset );

	//m_fRollbackSpeed = fSpeed;
}
//-------------------------------------------------------------------------
/**
@brief 获取内容总行数
@param
*/
int CCHtmlLabel::getLine()
{
	int nLines = 0;
	size_t size = m_vecContent.size();
	for( size_t i = 0; i < size; ++i )
	{
		nLines += m_vecContent[i].vExLines.size();			
	}

	return nLines;
}
//-------------------------------------------------------------------------
/**
@brief 删除队列前端多少行
@param
*/
void CCHtmlLabel::removeLines( int nLines )
{
	if( m_vecContent.empty() )
	{
		return;
	}

	int nRemoveNum = 0;
	//std::vector<SContextInfo>::iterator iter = m_vecContent.begin();
	//for( ; iter != m_vecContent.end(); )
	//{
	//	nRemoveNum += (*iter).vExLines.size();
	//	if( nRemoveNum < nLines )
	//	{
	//		m_vecContent.erase( iter++ );
	//	}
	//	else
	//	{
	//		++iter;
	//	}
	//}

	size_t remove_index = 0;
	size_t size = m_vecContent.size();
	for( size_t i = 0; i < size; ++i )
	{
		nRemoveNum += m_vecContent[i].vExLines.size();
		if( nRemoveNum >= nLines )
		{
			remove_index = i;
			break;
		}
	}

	m_vecContent.erase( m_vecContent.begin(), m_vecContent.begin() + remove_index + 1 );

	m_bNeedUpdate = true;
}

NS_CC_END