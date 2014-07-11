#ifndef _COCOS2DX_TYPES_H_
#define _COCOS2DX_TYPES_H_
 
#include "ccTypes.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

typedef unsigned char       byte;
typedef unsigned char       BYTE;

typedef char				int8;
typedef short				int16;
typedef int                 int32;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned long       uint32;

typedef float				float32;
typedef double				float64;

#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <hash_map>

typedef __int64             longlong;

#else

#include <stdlib.h>
#include <string.h>

#include <ext/hash_map>
#define stdext __gnu_cxx

typedef long long           longlong;

#endif

#define MAX_PATH			260

#endif // _COCOS2DX_TYPES_H_