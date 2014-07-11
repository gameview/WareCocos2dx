#include "cocos2dx_types.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../cocos2dx_support/CCLuaEngine.h"
#include "IRenderSystem.h"

USING_NS_CC;

int	API_SystemPlatForm()
{ 
	return CC_TARGET_PLATFORM;
}

/**
 @brief 获取场景根节点
 @param 
*/
cocos2d::CCScene* API_GetRootScene()
{
	cocos2d::engine::IRenderSystem* pRenderSystem = cocos2d::engine::getRenderSystem();
	if( pRenderSystem == NULL )
	{
		return NULL;
	}

	return pRenderSystem->getRootScene();
}

void API_SetSearchPath( char* szSearchPath )
{
	  CCFileUtils::sharedFileUtils()->setSearchPath(szSearchPath);
}

// 加载LUA脚本
void API_LoadLuaFile( char *szLuaFileName)
{
	cocos2d::CCLog("API_LoadLuaFile = %s",szLuaFileName);

	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine(); 
	std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(szLuaFileName);
	pEngine->executeScriptFile(path.c_str());
}
