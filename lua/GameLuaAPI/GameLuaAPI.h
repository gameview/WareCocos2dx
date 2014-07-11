
#ifndef __GAMELUAAPI_H_
#define __GAMELUAAPI_H_

#include "cocos2dx_types.h"
#include "cocos2d.h"

// tolua_begin

/**
 @brief 获取系统平台
 @param 
*/
int					API_SystemPlatForm();

/**
 @brief 获取场景根节点
 @param 
*/
cocos2d::CCScene*	API_GetRootScene();

/**
 @brief 设置资源搜索路径
 @param szSearchPath 资源路径名
*/
void				API_SetSearchPath( char* szSearchPath );

/**
 @brief 加载并执行Lua文件
 @param szLuaFileName lua文件名
*/
void				API_LoadLuaFile(  char * szLuaFileName);

// tolua_end

#endif //_GAMELUAAPI_H_

