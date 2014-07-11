#include "RenderTextManager.h"
#include "IRenderSystem.h"
#include "RenderText.h"

NS_CC_ENGINE_BEGIN

CRenderTextManager::CRenderTextManager(void)
{
}


CRenderTextManager::~CRenderTextManager(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 画文字
@param szFontName 字体名称
@param nFontSize 字体大小
*/
IRenderText* CRenderTextManager::createRenderText( const char* szString, const char* szFontName, int nFontSize, CCSize dimentions, bool bBmpFont )
{
	static unsigned int s_rendertext_id = 0;
	if( ++s_rendertext_id == 0 )
	{
		++s_rendertext_id;
	}

	CRenderText* pRenderText = new CRenderText( s_rendertext_id );
	if( pRenderText == NULL )
	{
		return NULL;
	}

	if( bBmpFont )
	{
		pRenderText->createByBmpFont( szString, szFontName, nFontSize, dimentions );
	}
	else
	{
		pRenderText->create( szString, szFontName, nFontSize, dimentions );
	}
	
	m_mapRenderText[s_rendertext_id ] = (IRenderText*)pRenderText;

	return pRenderText;
}
//-------------------------------------------------------------------------
/**
@brief 画文字
@param szFontName 字体名称
@param nFontSize 字体大小
*/
void CRenderTextManager::removeRenderText( IRenderText* pRenderText )
{
	if( pRenderText == NULL )
	{
		return;
	}

	unsigned int uid = ((CRenderText*)pRenderText)->getID();
	m_mapRenderText.erase( uid );
	
	/// 释放对象
	pRenderText->release();
}

NS_CC_ENGINE_END
