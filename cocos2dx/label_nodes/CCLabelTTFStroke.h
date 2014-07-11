//*************************************************************************
//	创建日期:	2013-10-25   14:52
//	文件名称:	CCLabelTTFStroke.h
//  创 建 人:   许小峰	
//	版权所有:	www.cocos2dres.com
//	说    明:	描边字
//*************************************************************************
#ifndef _CCLABELTTFSTROKE_H__
#define _CCLABELTTFSTROKE_H__

#include "sprite_nodes/CCSprite.h"
#include "textures/CCTexture2D.h"
#include "label_nodes/CCLabelTTF.h"

NS_CC_BEGIN

class CC_DLL CCLabelTTFStroke : public CCNode
{
public:
	CCLabelTTFStroke();
	~CCLabelTTFStroke();

public:
	static CCLabelTTFStroke* create();
	static CCLabelTTFStroke* create(const char *string, const char *fontName, float fontSize, CCSize dimentions, CCTextAlignment hAlignment = kCCTextAlignmentCenter, CCVerticalTextAlignment vAlignment = kCCVerticalTextAlignmentTop);
	
	bool initWithString();
	bool initWithString(const char *label, const char *fontName, float fontSize, CCSize dimentions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment);

public:
	/**
	@brief 设置字体颜色
	@param
	*/
	void	setColor(const ccColor3B& color3);
	/**
	@brief 设置显示字串
	@param
	*/
	void	setString(const char *label);

	const char* getString();

	void	setFontSize(int size);

	void	setFontName( const char* name );

	void	setFlipX(bool flipX);

	void	setFlipY(bool flipY);

	bool	isFlipX();

	bool	isFlipY();

	/**
	@brief 设置描边颜色
	@param
	*/
	void	setStrokeColor(ccColor3B col){ m_colStroke = col; updateStroke(); }
	/**
	@brief 设置描边大小
	@param
	*/
	void	setStrokeSize(float StrokeSize){ m_fStrokeSize = StrokeSize; updateStroke();}
	/**
	@brief 设置透明度
	@param
	*/
	void	setOpacity( unsigned char byAlpha );

	/**
	@brief 设置显示区域
	@param
	*/
	void	setDimensions( const CCSize& dimentions );

	/**
	@brief 设置对齐方式
	@param
	*/
	void	setHorizontalAlignment( CCTextAlignment alignment );

	/**
	@brief 设置对齐方式
	@param
	*/
	void	setVerticalAlignment( CCVerticalTextAlignment vAlignment );

protected:
	void updateStroke();

private:
	float          m_fStrokeSize;
	ccColor3B      m_colStroke;
	CCSprite*      m_sprite;
	CCLabelTTF*    m_label;
};

NS_CC_END

#endif // _CCLABELTTFSTROKE_H__
