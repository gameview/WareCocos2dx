//*************************************************************************
//	创建日期:	2013-8-20   16:35
//	文件名称:	RenderTextManager.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	文字渲染管理
//*************************************************************************
#ifndef _RENDERTEXTMANAGER_H__
#define _RENDERTEXTMANAGER_H__

#include <map>
#include "cocos2d.h"
#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

struct IRenderText;

class CRenderTextManager
{
	typedef std::map< unsigned int, IRenderText* >	RenderTextMap;
public:
	CRenderTextManager(void);
	virtual ~CRenderTextManager(void);

	/**
	@brief 画文字
	@param szFontName 字体名称
	@param nFontSize 字体大小
	*/
	IRenderText* createRenderText( const char* szString, const char* szFontName, int nFontSize, CCSize dimentions, bool bBmpFont = false );

	/**
	@brief 画文字
	@param szFontName 字体名称
	@param nFontSize 字体大小
	*/
	void		removeRenderText( IRenderText* pRenderText );

private:

	RenderTextMap	m_mapRenderText;
};

NS_CC_ENGINE_END

#endif // _RENDERTEXTMANAGER_H__

