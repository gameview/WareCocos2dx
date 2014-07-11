//*************************************************************************
//	创建日期:	2013-4-21   11:53
//	文件名称:	WareSprite.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	扩展Sprite实现
//*************************************************************************
#ifndef _WARESPRITE_H_
#define _WARESPRITE_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"


NS_CC_ENGINE_BEGIN

class CWareSprite : public cocos2d::CCSprite
{
public:
	CWareSprite(void);
	virtual ~CWareSprite(void);

	/// 
	virtual void draw(void);

	 /**
     * Creates an empty sprite without texture. You can call setTexture method subsequently.
     *
     * @return An empty sprite object that is marked as autoreleased.
     */
    static CWareSprite* create();

	/** Creates an sprite with an image filename.
	The rect used will be the size of the image.
	The offset will be (0,0).
	*/
	static CWareSprite* create(const char *pszFileName);

	/** Creates an sprite with an image filename and a rect.
	The offset will be (0,0).
	*/
	static CWareSprite* create(const char *pszFileName, const cocos2d::CCRect& rect);

	/**
    * Initializes an empty sprite with nothing init.
    */
    virtual bool init(void);

	/**
	* Initializes a sprite with an image filename.
	*
	* This method will find pszFilename from local file system, load its content to CCTexture2D,
	* then use CCTexture2D to create a sprite.
	* After initialization, the rect used will be the size of the image. The offset will be (0,0).
	*
	* @param   pszFilename The path to an image file in local file system
	* @return  true if the sprite is initialized properly, false otherwise.
	* @js init
	*/
	virtual bool initWithFile(const char *pszFilename);

	/**
	* Initializes a sprite with an image filename, and a rect.
	*
	* This method will find pszFilename from local file system, load its content to CCTexture2D,
	* then use CCTexture2D to create a sprite.
	* After initialization, the offset will be (0,0).
	*
	* @param   pszFilename The path to an image file in local file system.
	* @param   rect        The rectangle assigned the content area from texture.
	* @return  true if the sprite is initialized properly, false otherwise.
	* @js init
	*/
	virtual bool initWithFile(const char *pszFilename, const CCRect& rect);

	/**
	@brief 图片加载完毕
	@param
	*/
	void		imageLoaded(CCObject* pObj);

private:
    std::string m_strPath;
	bool	m_bVailed;		/// 对象是否有效
	CCRect	m_Rect;			/// 矩形区域
};

#endif // _WARESPRITE_H_

NS_CC_ENGINE_END

