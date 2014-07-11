//*************************************************************************
//	创建日期:	2013-7-3   18:11
//	文件名称:	AnimationRes.h	
//	版权所有:	Cocos2DRes.com
//	说    明:	动画数据
//*************************************************************************
#ifndef _ANIMATIONRES_H_
#define _ANIMATIONRES_H_

#include "cocos2d.h"
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include "SpriteFrameSet.h"


NS_CC_ENGINE_BEGIN

struct AnimateCallback
{
	float fCallbackTime;		/// 回调时间
	std::string strName;		/// 回调名称
	AnimateCallback()
	{
		//memset( this, 0, sizeof(*this) );
		fCallbackTime = 0.0f;
		strName = "";
	}
};

struct Animate
{
	std::string name;
	float delay;
	bool flipX;
	bool flipY;
	std::vector<std::string> spriteFrames;

	Animate()
	{
		delay = 0.0f;
		flipX = false;
		flipY = false;
		spriteFrames.clear();
	}

	~Animate()
	{
		delay = 0.0f;
		flipX = false;
		flipY = false;
		spriteFrames.clear();
	}
};

class AnimateSaxDelegator : public CCSAXDelegator
{
public:
	enum{
		STATE_NONE,
		STATE_PLIST,
		STATE_ANIMATION,
		STATE_NAME,
		STATE_DELAY,
		STATE_FLIP_X,
		STATE_FLIP_Y,
		STATE_SPRITE_FRAME
	}state;

	void startElement(void *ctx, const char *name, const char **atts) ;
	void endElement(void *ctx, const char *name) ;
	void textHandler(void *ctx, const char *s, int len) ;

	vector<string> plists;//All plist name
	vector<Animate> animates;//All animate data
};

class CAnimationRes : public CCObject
{
public:
	/// 动画信息
	struct SAnimationInfo
	{
		CCAnimation*	pAnimation;
		bool			m_bFlipX;
		bool			m_bFlipY;
		std::vector<AnimateCallback> callbacks;

		SAnimationInfo()
		{
			pAnimation = NULL;
			m_bFlipY = false;
			m_bFlipX = false;
			callbacks.clear();
		}
        ~SAnimationInfo()
		{
			pAnimation = NULL;
			m_bFlipY = false;
			m_bFlipX = false;
			callbacks.clear();
		}
	};

private:
	typedef std::map< std::string, SAnimationInfo >	AnimationMap;		/// 动画列表
	typedef std::map< std::string, bool >			PlistTextureMap;    /// Plist贴图加载标识

public:
	CAnimationRes(void);
	virtual ~CAnimationRes(void);

	virtual void			release();
	 
	/**
	@brief 加载资源
	@param szResName 资源名
	*/
	bool					loadRes( const char* szResName );

	/// 获取资源名称
	const std::string&		getFileName();

	/// 获取动画数据
	SAnimationInfo*			getAnimation( const char* szAnimationName );

	/**
	@brief 
	@param 动画名列表
	*/
	std::set<std::string>&	getAniNameList() { return NameAnimation; }

	/**
	@brief 资源有效性
	@param
	*/
	bool					isVailed() { return m_bVailed ;}

private:
	/**
	@brief 加载动画加高数据
	@param
	*/
	void					loadCallback( const char* szResName );

	/**
	@brief 加载贴图
	@param
	*/
	void					loadTextureAsync( const char* szResName );

	/**
	@brief 加载资源
	@param
	*/
	void					_loadRes( const char* szResName );

	/**
	@brief 图片加载完毕
	@param
	*/
	void					imageLoaded(CCObject* pObj);

private:

	bool					m_bVailed;						/// 资源有效标志

	AnimationMap			m_mapAnimation;					/// 动画列表
	std::string				m_strAniResName;

	//From animate name to CCAnimates
	std::map<std::string,Animate> nameToAnimateMap;
	//From xml path to plist names
	std::map<std::string,std::vector<std::string> > pathToPlistsMap;
	/// 动画列表
	std::set<std::string> NameAnimation;
	/// 贴图加载信息列表
	std::map< std::string, bool >	m_mapTextureLoadInfo;	/// 贴图加载信息

	int			n_index;

	//CCSAXParser			parser;
	AnimateSaxDelegator delegator;
};

NS_CC_ENGINE_END

#endif // _ANIMATIONRES_H_

