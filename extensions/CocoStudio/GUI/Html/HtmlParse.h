//*************************************************************************
//	创建日期:	2013-9-12   21:06
//	文件名称:	HtmlParse.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	html解析
//*************************************************************************
#ifndef _HTMLPARSE_H__
#define _HTMLPARSE_H__

#include "cocos2d.h"
#include <string>
#include <map>
#include <list>
#include <vector>

#define FONT_DEFAULT_SIZE  24

/// 边距
#define PAN_WIDTH   8
	/// 行间距
#define LINE_SPACE   4

NS_CC_BEGIN

struct IHtmlLabelEventHandler;
struct IHtmlElement;

/// html解析工具
class CHtmlParse
{
public:

	CHtmlParse() 
	{
		m_strFontName = "黑体";
		m_nFontSize =FONT_DEFAULT_SIZE;
	}

	/// html元素组件
	struct Component
	{
		std::string mTag;	/// 节点标识
		std::string mText;	/// 字符串
		int mPosition;
		bool bEnd;			/// 是否结束标识
		std::vector<std::string> mFontStyle;
		std::map< std::string, std::string > attribute;

		Component(const std::string& pText,std::string pTag, int pPosition, std::vector<std::string>& pFontStyle, bool end = false )
		{
			mText = pText;
			mTag = pTag;  
			mPosition = pPosition;
			//attribute.clear();
			bEnd = end;
			mFontStyle = pFontStyle;
			attribute.clear();
		}

		/// 获取属性
		void getAttribute( const std::string& strAttriName, std::string& strValue )
		{
			std::map< std::string, std::string >::iterator iter = attribute.find( strAttriName );
			if( iter != attribute.end() )
			{
				strValue = iter->second;
			}
		}
	};

	/// 字体属性
	struct FontProp
	{
		int			nSize;
		std::string strFontName; /// 默认 Arial
		ccColor3B	color;

		FontProp()
		{
			nSize = FONT_DEFAULT_SIZE;
			strFontName = "黑体";
			color = ccc3( 255,255,255 );
		}
	};

	/// 动画属性
	struct AniProp
	{
		float		fDelay;			/// 帧间隔
		int			nFrames;		/// 帧数
		int			nCurFrame;		/// 当前帧
		float		fUpdateTime;	/// 更新时间
		std::string strSrc;			/// 文件路径名
		AniProp() : strSrc("")
		{
			nCurFrame = 0;
			fDelay = 0.1f;
			nFrames = 1;
			fUpdateTime = 0.0f;
		}
	};

	/// 图画元素类型
	enum EDrawElementType
	{
		DrawElement_Text = 0,	/// 文字
		DrawElement_Link,		/// 文字链接
		DrawElement_Image,		/// 图片
		DrawElement_ImageLink,	/// 图片链接
		DrawElement_Ani,		/// 图片带动画
		DrawElement_Input,		/// 输入框
		DrawElement_Exp,		/// 表情动画
	};

	/// 图画元素
	struct DrawElement
	{
		EDrawElementType type;		/// 图画类别
		std::string mText;			/// DrawElement_Text/DrawElement_Link 显示的字符 DrawElement_Image/DrawElement_Ani 图片路径
		cocos2d::CCRect	rect;		/// 元素区域 位置和宽高
		FontProp	fontAttribute;	/// 字体属性
		AniProp		aniAttribute;	/// 动画属性
		int			nFrame;			/// DrawElement_Ani 图片帧数
		std::string mCommand;		/// 命令字符串 

		DrawElement( std::string pText, int pw, int ph )
		{
			type = DrawElement_Text;
			mText = pText;  
			rect.size.width = pw;  
			rect.size.height = ph;
			nFrame = 0;
		}  
	};

	/// 行容器
	struct LineContainer
	{
		int nWidth;		/// 行宽
		int nHeight;	/// 行高
		int nYPos;		/// y位置
		std::vector< DrawElement >  elements;	/// 元素列表

		LineContainer()
		{
			nWidth = 0;
			nHeight = 0;
			nYPos = 0;
			elements.clear();
		}
	};

	typedef std::vector<LineContainer>	Lines;

	/// 行容器
	struct LineContainerEx
	{
		int nWidth;		/// 行宽
		int nHeight;	/// 行高
		int nYPos;		/// y位置
		std::vector< IHtmlElement* >  elements;	/// 元素列表

		LineContainerEx()
		{
			nWidth = 0;
			nHeight = 0;
			nYPos = 0;
			elements.clear();
		}
	};

	typedef std::vector<LineContainerEx>	ExLines;

public:

	/**
	@brief 解析html字串
	@param
	*/
	void			parseHtml( const std::string& string );

	/**
	@brief 生成图画元素以及行对象
	@param nWidth 指定宽度
	@param lines 行列表
	@param pHandler 事件回调句柄
	*/
	//void			generateDrawelementsAndLines( int nWidth, CHtmlParse::Lines& lines, IHtmlLabelEventHandler* pHandler );

	/**
	@brief 生成图画元素以及行对象
	@param nWidth 指定宽度
	@param lines 行列表
	@param pHandler 事件回调句柄
	*/
	void			generateDrawelementsAndLines( int nWidth, CHtmlParse::ExLines& lines, IHtmlLabelEventHandler* pHandler );

	/**
	@brief 解析属性字符串
	@param
	*/
	void			parseAttribute( const std::string& strAttr, std::map<std::string, std::string>& attr );

	/**
	@brief 设置默认字体名
	@param
	*/
	void			setFontName( const char* pFontName ) { if( pFontName != NULL ) { m_strFontName = pFontName; } }

	/**
	@brief 设置字体大小
	@param
	*/
	void			setFontSize( int nSize ) { m_nFontSize = nSize; }

private:
	
	/**
	@brief 从源字串获取指定宽度的字符串
	@param
	*/
	void			partitionStrintByWidth( const std::string& strSrc, int nWidth, const std::string& strFontName, int nFontSize, CCSize& outSize, std::string& strOut, std::string& strRain );

	/**
	@brief 获取字符串的大小
	@param
	*/
	CCSize			getStringPixelSize( const std::string& strSrc, const std::string& strFontName, int nFontSize );



private:

	std::vector<Component>		m_vecComponent;	/// 组件元素
	//Lines						m_vecLines;		/// 全部行元素

	std::string		m_strFontName;		/// 默认字体名
	int				m_nFontSize;		/// 默认字体大小
};

NS_CC_END

#endif // _HTMLPARSE_H__
