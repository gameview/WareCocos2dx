//#include "StdAfx.h"
#include "RectangleObj.h"

NS_CC_ENGINE_BEGIN

CRectangleObj::CRectangleObj(void)
{
	m_nWidth = 0;
	m_nHeight = 0;

	m_bFillColor = false;

	m_Color = m_FillColor = cocos2d::ccc4( 255,255,255,255 );

	m_pDrawNode = NULL;
}


CRectangleObj::~CRectangleObj(void)
{
}
//------------------------------------------------------------------------------
/** create one CRectangleObj */
CRectangleObj* CRectangleObj::create(void)
{
	CRectangleObj *pRet = new CRectangleObj();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}
//------------------------------------------------------------------------------
/**
@brief 设置矩形宽高
@param
*/
void CRectangleObj::setRect( int nWidth, int nHeight )
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}
//------------------------------------------------------------------------------
/**
@brief 重载draw方法
@param
*/
void CRectangleObj::draw()
{
	//if( m_bFillColor )
	//{
	//	if( m_pDrawNode == NULL )
	//	{
	//		m_pDrawNode = cocos2d::CCDrawNode::create();
	//		addChild(m_pDrawNode, 0);
	//	}

	//	if( m_pDrawNode == NULL )
	//	{
	//		return;
	//	}

	//	cocos2d::CCRect rect( 0,
	//		0, m_nWidth, m_nHeight );

	//	int nViewLeft = rect.getMinX();
	//	int nViewRight = rect.getMaxX();
	//	int nViewTop = rect.getMaxY();
	//	int nViewBottom = rect.getMinY();

	//	BYTE bOpacity = getOpacity();

	//	cocos2d::ccColor4B color4 = m_Color;
	//	color4.r *= bOpacity/255.0f;
	//	color4.g *= bOpacity/255.0f;
	//	color4.b *= bOpacity/255.0f;
	//	color4.a = bOpacity;

	//	cocos2d::ccColor4B colorFill = m_FillColor;
	//	colorFill.r *= bOpacity/255.0f;
	//	colorFill.g *= bOpacity/255.0f;
	//	colorFill.b *= bOpacity/255.0f;
	//	colorFill.a = bOpacity;

	//	cocos2d::CCPoint points[] = { ccp(nViewLeft,nViewBottom), ccp(nViewRight,nViewBottom), ccp(nViewRight,nViewTop), ccp(nViewLeft,nViewTop) };
	//	m_pDrawNode->drawPolygon(points, sizeof(points)/sizeof(points[0]), cocos2d::ccc4f(colorFill.r/255.0f,colorFill.g/255.0f,colorFill.b/255.0f,colorFill.a/255.0f), 1, cocos2d::ccc4f(color4.r/255.0f,color4.g/255.0f,color4.b/255.0f,color4.a/255.0f));
	//}
	//else
	{
		//if( m_pDrawNode )
		//{
		//	removeChild( m_pDrawNode, true );
		//	m_pDrawNode = NULL;
		//}

		cocos2d::CCRect rect( 0,
			0, m_nWidth, m_nHeight );

		int nViewLeft = rect.getMinX();
		int nViewRight = rect.getMaxX();
		int nViewTop = rect.getMaxY();
		int nViewBottom = rect.getMinY();

		CHECK_GL_ERROR_DEBUG();

		BYTE bOpacity = getOpacity();

		cocos2d::ccColor4B color4 = m_Color;
		color4.r *= bOpacity/255.0f;
		color4.g *= bOpacity/255.0f;
		color4.b *= bOpacity/255.0f;
		color4.a = bOpacity;

		cocos2d::ccDrawColor4B(color4.r,color4.g,color4.b,color4.a);

		glLineWidth(1);
		cocos2d::CCPoint vertices[] = { ccp(nViewLeft,nViewBottom), ccp(nViewRight,nViewBottom), ccp(nViewRight,nViewTop), ccp(nViewLeft,nViewTop), ccp(nViewLeft,nViewBottom) };
		cocos2d::ccDrawPoly( vertices, 5, false);
	}
}

NS_CC_ENGINE_END
