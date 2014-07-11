/*
** Lua binding: LuaMainAPI
** Generated automatically by tolua++-1.0.92 on 06/24/14 23:02:30.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_LuaMainAPI_open (lua_State* tolua_S);

#include "cocos2dx_types.h"
#include "../GameLuaAPI/GameLuaAPI.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"cocos2d::CCScene");
}

/* function: API_SystemPlatForm */
#ifndef TOLUA_DISABLE_tolua_LuaMainAPI_API_SystemPlatForm00
static int tolua_LuaMainAPI_API_SystemPlatForm00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  API_SystemPlatForm();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'API_SystemPlatForm'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: API_GetRootScene */
#ifndef TOLUA_DISABLE_tolua_LuaMainAPI_API_GetRootScene00
static int tolua_LuaMainAPI_API_GetRootScene00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   cocos2d::CCScene* tolua_ret = (cocos2d::CCScene*)  API_GetRootScene();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"cocos2d::CCScene");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'API_GetRootScene'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: API_SetSearchPath */
#ifndef TOLUA_DISABLE_tolua_LuaMainAPI_API_SetSearchPath00
static int tolua_LuaMainAPI_API_SetSearchPath00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* szSearchPath = ((char*)  tolua_tostring(tolua_S,1,0));
  {
   API_SetSearchPath(szSearchPath);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'API_SetSearchPath'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: API_LoadLuaFile */
#ifndef TOLUA_DISABLE_tolua_LuaMainAPI_API_LoadLuaFile00
static int tolua_LuaMainAPI_API_LoadLuaFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* szLuaFileName = ((char*)  tolua_tostring(tolua_S,1,0));
  {
   API_LoadLuaFile(szLuaFileName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'API_LoadLuaFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_LuaMainAPI_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"API_SystemPlatForm",tolua_LuaMainAPI_API_SystemPlatForm00);
  tolua_function(tolua_S,"API_GetRootScene",tolua_LuaMainAPI_API_GetRootScene00);
  tolua_function(tolua_S,"API_SetSearchPath",tolua_LuaMainAPI_API_SetSearchPath00);
  tolua_function(tolua_S,"API_LoadLuaFile",tolua_LuaMainAPI_API_LoadLuaFile00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_LuaMainAPI (lua_State* tolua_S) {
 return tolua_LuaMainAPI_open(tolua_S);
};
#endif

