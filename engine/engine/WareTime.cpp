#include "WareTime.h"
#include <math.h>
#include "cocos2d.h"

NS_CC_ENGINE_BEGIN	
	
/**
@brief 获取系统运行时间
@param
*/
uint32 WareTime::getTickCount()
{
	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL); 
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

NS_CC_ENGINE_END