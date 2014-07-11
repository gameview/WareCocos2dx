//#include "StdAfx.h"
#include "SceneLayer.h"
//#include "SceneDef.h"
#include "RectangleObj.h"

NS_CC_ENGINE_BEGIN

CSceneLayer::CSceneLayer(void)
{
	m_nLayerID = 0;
}

CSceneLayer::~CSceneLayer(void)
{
}
//------------------------------------------------------------------------------
CSceneLayer* CSceneLayer::create()
{
	CSceneLayer *pRet = new CSceneLayer();
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

NS_CC_ENGINE_END