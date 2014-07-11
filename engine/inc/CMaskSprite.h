//*************************************************************************
//	创建日期:	2013-9-6   15:10
//	文件名称:	CMaskSprite.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	遮罩精灵(用于不规则图形渲染, 比如小地图，技能按钮等)
//*************************************************************************
#ifndef _CMASKSPRITE_H__
#define _CMASKSPRITE_H__

#include "cocos2d.h"
#include <string>
#include <map>
#include <list>
#include <vector>

NS_CC_BEGIN

class CMaskSprite : public CCNode
{
	struct SPointInfo
	{
		int			nPointSize;
		ccColor4B	color;
		CCPoint		pos;

		SPointInfo( int size, const ccColor4B& c, const CCPoint& p ) : nPointSize(size), color(c), pos(p)
		{
		}

		SPointInfo() : nPointSize(3), color(ccc4(255,255,255,255)), pos(ccp(0,0))
		{
		}
	};

	struct SSpriteInfo
	{
		CCPoint		m_ptOffset;
		CCSprite*	m_pSprite;

		SSpriteInfo( CCSprite* pSprite, const CCPoint& p ) : m_pSprite(pSprite), m_ptOffset(p)
		{
		}

		SSpriteInfo() : m_pSprite(NULL)
		{
		}
	};

	typedef std::map< int, SPointInfo > PointMap;
	typedef std::list< SSpriteInfo >	SpriteList;

public:
	CMaskSprite(void);
	virtual ~CMaskSprite(void);

	/** Initializes an sprite with a texture and a rect in points, optionally rotated.
	The offset will be (0,0).
	IMPORTANT: This is the designated initializer.
	*/
	bool initMask(const char *pszMaskFileName);

	bool addSprite( const char *pszSpriteName, const CCPoint& offset );
	//virtual void draw(void);

	virtual void update(float delta);

	void	setOffset( const CCPoint& ptOffset );

	CCPoint	getOffset()	{ return m_ptOffset; }

	/// 遮罩大小
	CCSize	getMaskSize() { return m_maskSize; }
	/// 源大小
	//CCSize	getSrcSize()  { return m_srcSize; }

	/**
	@brief 添加点
	@param
	*/
	int					addPoint( int nPointSize, const ccColor4B color, const CCPoint& pos );

	/**
	@brief 删除点 
	@param
	*/
	void				removePoint( int nPointID );

	/**
	@brief 修改点位置
	@param
	*/
	void				changePointPos( int nPointID, cocos2d::CCPoint& pos );

	/**
	@brief 获取指定点位置
	@param
	*/
	void				getPointPos( int nPointID, CCPoint& pos );

	/**
	@brief 清除点
	@param
	*/
	void				clearPoint();

	/**
	@brief 设置更新
	@param
	*/
	void				setUpdate();

	/**
	* Creates an empty sprite without texture. You can call setTexture method subsequently.
	*
	* @return An empty sprite object that is marked as autoreleased.
	*/
	static CMaskSprite* create( const char *pszMaskFileName );

private:

	CCTexture2D*	m_pmaskTexture;

	CCPoint			m_ptOffset;		/// 偏移

	CCSize			m_maskSize;		/// 遮罩大小
	//CCSize			m_srcSize;		/// 源图片大小

	CCSprite*		m_pSprite;		/// 图片精灵对象

	CCSprite*		m_pMaskSprite;	/// 遮罩层
	//CCSprite*		m_pSrcSprite;	/// 源图像

	bool			m_bUpdate;		/// 是否需要更新

	std::string		m_strSrcImage;
	std::string		m_strMaskImage;

	SpriteList		m_lstSprite;	/// 精灵列表
	PointMap		m_mapPoint;
};

NS_CC_END

#endif // _CMASKSPRITE_H__