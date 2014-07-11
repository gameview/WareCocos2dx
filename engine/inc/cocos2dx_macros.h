#ifndef _COCOS2DX_MACROS_H_
#define _COCOS2DX_MACROS_H_

#pragma warning(disable : 4251) 

#include "MessageCommon.h"
#include "FileCommon.h"
#include "RenderCommon.h"

/// 命名空间定义
#define NS_CC_ENGINE_BEGIN              namespace cocos2d{ namespace engine {
#define NS_CC_ENGINE_END                }}
#define USING_NS_CC_ENGINE              using namespace cocos2d::engine

///// 常用工具宏定义
#ifndef SAFE_DELETE
#define SAFE_DELETE( x )	if( (x) ) { delete (x); (x) = NULL; }
#endif

#ifndef SAFE_DELARR
#define SAFE_DELARR( x )	if( (x) ) { delete [] (x); (x) = NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( x )	if( (x) ) { (x)->release(); (x) = NULL; }
#endif

/* 将四个char合并到一个uint中
 */
#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((uint32)(BYTE)(ch0) | ((uint32)(BYTE)(ch1) << 8) |   \
                ((uint32)(BYTE)(ch2) << 16) | ((uint32)(BYTE)(ch3) << 24 ))
#endif

#ifndef DeleteVecObj
#define DeleteVecObj(v) \
size_t size = v.size();\
for( size_t i = 0; i < size; ++i )\
{\
	delete v[i];\
}\
v.clear();
#endif // !DeleteVecObj

//size_t size = m_vecProp.size();
//for( size_t i = 0; i < size; ++i )
//{
//	delete m_vecProp[i];
//}

//m_vecProp.clear();

#endif // _COCOS2DX_MACROS_H_

