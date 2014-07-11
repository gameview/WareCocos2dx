#include "HtmlParse.h"
#include "StringUtility.h"
#include "StringConvert.h"
#include "HtmlImage.h"
#include "HtmlText.h"
#include "HtmlTextLink.h"
#include "HtmlInput.h"
#include "HtmlExp.h"
#include "HtmlImageLink.h"
#include "HtmlSeparate.h"
#include "CHtmlLabel.h"

NS_CC_BEGIN

void replaceFirstString( std::string& str, const std::string& strSrc, const std::string& strReplace )
{
	size_t pos = str.find( strSrc );
	if( pos == std::string::npos )
	{
		return;
	}

	str.replace( pos, strSrc.length(), strReplace ); // 一次替换  
}

////////////////////////////////////////////////////////////////////////////
/**
@brief 解析html字串
@param
*/
void CHtmlParse::parseHtml( const std::string& string )
{  
	std::string str = string;
	std::vector<std::string> fontStyle;

	std::string tag;

	int il = str.find('<');
	int ir = str.find('>');
	std::string delimeter;
	std::string text;
	int lastPosition = 0;
	m_vecComponent.clear();

	while(il != -1)
	{
		if( il > 0 )
		{
			Component compPrev(str.substr(lastPosition, il ),"",0,fontStyle);
			m_vecComponent.push_back(compPrev);

			str = str.substr( il );

			il = str.find('<');
			ir = str.find('>');
			continue;
		}

		delimeter = str.substr(il, ir - il + 1);
		text = str.substr(il, ir - il );

		int position = str.find(delimeter);
		if(position != -1)
		{
			if(delimeter.find("<p") == 0)
			{
				//paragraph
				replaceFirstString(str,delimeter, " " );
			}
			else
			{
				replaceFirstString(str,delimeter, "" );
			}
			//replace "<" ">"  
		}

		if(text.find("</") == 0)
		{//end of tag
			//1 end of current text 
			tag = text.substr(2);
			if(position != -1)
			{
				for(int i=(int)(m_vecComponent.size()-1);i>=0;i--)
				{
					Component comp = m_vecComponent[i];
					if( "" != tag && "" != comp.mTag &&  comp.mTag == tag )
					{
						Component compPrev(str.substr(lastPosition, position - lastPosition),tag,position,fontStyle, true );
						m_vecComponent.push_back(compPrev);
						break;
					}
				}
			}
			////2 pop current fontStyle;  
			if(fontStyle.size() > 0)
			{
				std::vector<std::string>::iterator itBeg = fontStyle.begin();
				fontStyle.erase( itBeg + fontStyle.size() - 1 );
			}
		}
		else
		{
			////1 end of previous text
			text = text.substr(1);
			cocos2d::engine::StringUtility::trim( text );
			int iSpace = text.find(' '); 
			tag = text.substr( 0, iSpace );

			Component comp( text.substr( iSpace + 1 ),tag,position,fontStyle);

			/// 解析属性
			parseAttribute( comp.mText, comp.attribute );

			m_vecComponent.push_back(comp);

			//2 new fontStyle  
			fontStyle.push_back(text);
		}

		il = str.find('<');
		ir = str.find('>'); 
	}  

	if(lastPosition < (int)str.length())
	{  
		Component comp(str.substr(lastPosition),"",lastPosition,fontStyle);  
		m_vecComponent.push_back(comp);
	}
}
//-------------------------------------------------------------------------
/**
@brief 生成图画元素以及行对象
@param nWidth 行宽度 是减除掉边距后的宽度
*/
void CHtmlParse::generateDrawelementsAndLines( int nWidth, CHtmlParse::ExLines& lines, IHtmlLabelEventHandler* pHandler )
{
	lines.clear();

	FontProp fontProp;
	size_t size = m_vecComponent.size();
	LineContainerEx line;
	std::string strCommand;
	bool bFontUse = false;
	bool bLink = false;
	bool bClose = false;
	int nLineWidth = 0;
	int nHeight = 0;
	static int g_nHtmlElementID = 0;
	for( size_t i = 0; i < size; ++i )
	{
		Component& com = m_vecComponent[i];
		if( com.mTag == "" )  /// darw text
		{
			/// 字体属性设置
			if( bLink )
			{
				if( !bFontUse )
				{
					fontProp.color = ccc3( 0,255,0 );
					fontProp.nSize = m_nFontSize;
					fontProp.strFontName = m_strFontName;
				}
			}
			else
			{
				if( !bFontUse )
				{
					fontProp.color = ccc3( 255,255,255 );
					fontProp.nSize = m_nFontSize;
					fontProp.strFontName = m_strFontName;
				}
			}

			std::string strNewContent = com.mText;
			int nRainWidth = nWidth - nLineWidth;
			/// 以后需要实现更好的方法获取字符像素宽高
			CCSize size = getStringPixelSize( strNewContent, fontProp.strFontName, fontProp.nSize );
			int nNewWidth = size.width;// + line.nWidth;
			while( nNewWidth > 0 && nNewWidth >= nRainWidth - 3 )
			{
				std::string strOut, strRain;
				CCSize s = CCSizeZero;
				partitionStrintByWidth( strNewContent, nRainWidth-3, fontProp.strFontName, fontProp.nSize, s, strOut, strRain );
				if( strRain == "" )
				{
					lines.push_back( line );
					line.elements.clear();
					line.nWidth = 0;
					line.nHeight = 0;
					nLineWidth = 0;

					//strNewContent = strRain;
					size = getStringPixelSize( strNewContent, fontProp.strFontName, fontProp.nSize );
					break;
				}

				//DrawElement e( strOut, s.width, s.height );
				IHtmlElement* pHtmlElement = NULL;
				if( bLink )
				{
					CHtmlTextLink* pHtmlTextLink = new CHtmlTextLink();
					if( pHtmlTextLink )
					{
						pHtmlTextLink->parseAttribute( strCommand );
						pHtmlTextLink->setText( strOut );
						//pHtmlTextLink->setCommand( strCommand );
						pHtmlTextLink->setContentSize( CCSizeMake(s.width,s.height) );
						pHtmlTextLink->setFontProp( fontProp.strFontName, fontProp.color, fontProp.nSize );
						pHtmlElement = pHtmlTextLink;
					}
				}
				else
				{
					//e.type = DrawElement_Text;

					CHtmlText* pHtmlText = new CHtmlText();
					if( pHtmlText )
					{
						pHtmlText->parseAttribute( strOut );
						pHtmlText->setText( strOut );
						//pHtmlText->setCommand( strCommand );
						pHtmlText->setContentSize( CCSizeMake(s.width,s.height) );
						pHtmlText->setFontProp( fontProp.strFontName, fontProp.color, fontProp.nSize );
						pHtmlElement = pHtmlText;
					}
				}

				line.elements.push_back( pHtmlElement );
				if( line.nHeight < s.height )
				{
					line.nHeight = s.height;
				}

				lines.push_back( line );
				line.elements.clear();
				line.nWidth = 0;
				line.nHeight = 0;
				nLineWidth = 0;

				strNewContent = strRain;
				size = getStringPixelSize( strNewContent, fontProp.strFontName, fontProp.nSize );
				nNewWidth = size.width;
				nRainWidth = nWidth;// - nLineWidth;
			}

			IHtmlElement* pHtmlElement = NULL;
			if( bLink )
			{
				CHtmlTextLink* pHtmlTextLink = new CHtmlTextLink();
				if( pHtmlTextLink )
				{
					pHtmlTextLink->parseAttribute( strCommand );
					pHtmlTextLink->setText( strNewContent );
					//pHtmlTextLink->setCommand( strCommand );
					pHtmlTextLink->setContentSize( CCSizeMake(size.width,size.height) );
					pHtmlTextLink->setFontProp( fontProp.strFontName, fontProp.color, fontProp.nSize );
					//pHtmlTextLink->setClose( bClose );
					pHtmlElement = pHtmlTextLink;
				}
			}
			else
			{
				//e.type = DrawElement_Text;

				CHtmlText* pHtmlText = new CHtmlText();
				if( pHtmlText )
				{
					pHtmlText->parseAttribute( strNewContent );
					pHtmlText->setText( strNewContent );
					//pHtmlText->setCommand( strCommand );
					pHtmlText->setContentSize( CCSizeMake(size.width,size.height) );
					pHtmlText->setFontProp( fontProp.strFontName, fontProp.color, fontProp.nSize );
					pHtmlElement = pHtmlText;
				}
			}

			line.elements.push_back( pHtmlElement );
			if( line.nHeight < size.height )
			{
				//line.nWidth = size.width;
				line.nHeight = size.height;
			}

			nLineWidth += size.width;
		}
		else if( com.mTag == "img" )
		{
			/// 图片目前不处理
			std::string strTemp("");
			com.getAttribute( "src", strTemp );
			if( strTemp == "" )
			{
				continue;
			}

			CCSprite* pSprite = CCSprite::create( strTemp.c_str() );
			if( pSprite == NULL )
			{
				continue; /// 是否需要找一个默认图片
			}

			CCSize size = pSprite->getContentSize();
			nLineWidth += size.width;
			if( nLineWidth > nWidth ) /// 如果出现单图片大于行宽的情况则会有问题，外部需要做限制
			{
				/// 直接放入下一行
				lines.push_back( line );
				line.elements.clear();
				line.nWidth = 0;
				line.nHeight = 0;
				nLineWidth = 0;
			}

			IHtmlElement* pHtmlElement = NULL;
			if( bLink )
			{
				CHtmlImageLink* pHtmlImageLink = new CHtmlImageLink();
				if( pHtmlImageLink )
				{
					pHtmlImageLink->setContentSize( size );
					pHtmlImageLink->parseAttribute( strCommand );
					pHtmlImageLink->setText( com.mText ); /// 文件名
					pHtmlElement = pHtmlImageLink;
				}
			}
			else
			{
				CHtmlImage* pHtmlImage = new CHtmlImage();
				if( pHtmlImage )
				{
					pHtmlImage->setContentSize( size );
					pHtmlImage->parseAttribute( com.mText );
					pHtmlElement = pHtmlImage;
				}
			}

			line.elements.push_back( pHtmlElement );
			if( line.nHeight < size.height )
			{
				line.nHeight = size.height;
			}
		}
		else if( com.mTag == "a" )
		{
			if( com.bEnd )
			{
				bLink = false;
				strCommand = "";
				continue;
			}

			bLink = true;
			//std::map< std::string, std::string >::iterator iter = com.attribute.find( "href" );
			strCommand = com.mText;
			//if( iter != com.attribute.end() )
			//{
			//	strCommand = iter->second;
			//}
			//else
			//{
			//	strCommand = "";
			//}

			//iter = com.attribute.find( "close" );
			//if( iter != com.attribute.end() )
			//{
			//	bClose = true;
			//}
		}
		else if( com.mTag == "font" || com.mTag == "text" )
		{
			fontProp.nSize = m_nFontSize;
			fontProp.color = ccc3( 255,255,255);
			if( com.bEnd )
			{
				bFontUse = false;
				continue;
			}
			else
			{
				bFontUse = true;
			}

			std::string strTemp("");
			com.getAttribute( "size", strTemp );
			if( strTemp != "" )
			{
				fontProp.nSize = cocos2d::engine::StringConvert::parseInt( strTemp );
			}

			strTemp = "";
			com.getAttribute( "color", strTemp );
			if( strTemp != "" )
			{
				fontProp.color = cocos2d::engine::StringConvert::parseSharpColor( strTemp );
			}

			strTemp = "";
			com.getAttribute( "face", strTemp );
			if( strTemp != "" )
			{
				fontProp.strFontName = strTemp;
			}
		}
		else if( com.mTag == "ani" )
		{
			//std::string strTemp("");
			//com.getAttribute( "src", strTemp );
			//if( strTemp == "" )
			//{
			//	continue;
			//}

			//char szFrameName[256];
			//sprintf( szFrameName, "%s%d.png", strTemp.c_str(), 0 );
			////lines[i].elements[j].mText = szFrameName;

			//CCSprite* pSprite = CCSprite::create( szFrameName );
			//if( pSprite == NULL )
			//{
			//	continue; /// 是否需要找一个默认图片
			//}

			//CCSize size = pSprite->getContentSize();
			//nLineWidth += size.width;
			//if( nLineWidth > nWidth ) /// 如果出现单图片大于行宽的情况则会有问题，外部需要做限制
			//{
			//	/// 直接放入下一行
			//	lines.push_back( line );
			//	line.elements.clear();
			//	line.nWidth = 0;
			//	line.nHeight = 0;
			//	nLineWidth = 0;
			//}

			//DrawElement e( szFrameName, size.width, size.height );
			//e.type = DrawElement_Ani;

			//e.aniAttribute.strSrc = strTemp;

			//strTemp = "";
			//com.getAttribute( "delay", strTemp );
			//if( strTemp != "" )
			//{
			//	e.aniAttribute.fDelay = cocos2d::engine::StringConvert::parseFloat( strTemp );
			//}

			//strTemp = "";
			//com.getAttribute( "frame", strTemp );
			//if( strTemp != "" )
			//{
			//	e.aniAttribute.nFrames = cocos2d::engine::StringConvert::parseInt( strTemp );
			//}

			//line.elements.push_back( e );
			//if( line.nHeight < size.height )
			//{
			//	line.nHeight = size.height;
			//}
		}
		else if( com.mTag == "win")
		{
			/// 需要回调到上层
			if( pHandler )
			{
				pHandler->onElement( com.mTag, CCSizeZero, com.attribute, 0 );
			}
		}
		else if( com.mTag == "data" )
		{
			if( pHandler )
			{
				std::string strCom, strValue;
				std::map<std::string, std::string>::iterator iter = com.attribute.begin();
				for( ; iter != com.attribute.end(); ++iter )
				{
					/// 获取参数
					if( iter->first == "cmd" )
					{
						strCom = iter->second;
						continue;
					}

					strValue.append( iter->first );
					strValue.append( "=" );
					strValue.append( iter->second );

					strValue.append("&");
				}

				if( strCom.empty() )
				{
					continue;
				}

				if( !strValue.empty() )
				{
					strValue = strValue.substr( 0, strValue.length() - 1 );
				}

				if( pHandler )
				{
					pHandler->onDataElement( strCom, strValue );
				}
			}
		}
		else if( com.mTag == "exp" )
		{
			IHtmlElement* pHtmlElement = NULL;
			CHtmlExp * pHtmlExp = new CHtmlExp(++g_nHtmlElementID);
			if( pHtmlExp == NULL )
			{
				continue;
			}

			if( pHtmlExp )
			{
				pHtmlExp->parseAttribute( com.mText );
				pHtmlExp->setContentSize( CCSizeMake(64,64) ); /// 固定64*64大小
				pHtmlElement = pHtmlExp;
			}

			CCSize size = pHtmlExp->getContentSize();
			nLineWidth += size.width;
			if( nLineWidth > nWidth ) /// 如果出现单图片大于行宽的情况则会有问题，外部需要做限制
			{
				/// 直接放入下一行
				lines.push_back( line );
				line.elements.clear();
				line.nWidth = 0;
				line.nHeight = 0;
				nLineWidth = 0;
			}

			/// 需要回调到上层
			if( pHandler )
			{
				pHandler->onElement( com.mTag, size, com.attribute, g_nHtmlElementID );
			}

			line.elements.push_back( pHtmlElement );
			if( line.nHeight < size.height )
			{
				line.nHeight = size.height;
			}
		}
		else if( com.mTag == "input" )
		{
			IHtmlElement* pHtmlElement = NULL;
			CHtmlInput * pHtmlInput = new CHtmlInput(++g_nHtmlElementID);
			if( pHtmlInput == NULL )
			{
				continue;
			}

			if( pHtmlInput )
			{
				pHtmlInput->parseAttribute( com.mText );
				pHtmlElement = pHtmlInput;
			}

			CCSize size = pHtmlInput->getContentSize();
			nLineWidth += size.width;
			if( nLineWidth > nWidth ) /// 如果出现单图片大于行宽的情况则会有问题，外部需要做限制
			{
				/// 直接放入下一行
				lines.push_back( line );
				line.elements.clear();
				line.nWidth = 0;
				line.nHeight = 0;
				nLineWidth = 0;
			}

			/// 需要回调到上层
			if( pHandler )
			{
				pHandler->onElement( com.mTag, size, com.attribute, g_nHtmlElementID );
			}

			line.elements.push_back( pHtmlElement );
			if( line.nHeight < size.height )
			{
				line.nHeight = size.height;
			}
		}
		else if( com.mTag == "separate" )
		{
			IHtmlElement* pHtmlElement = NULL;
			CHtmlSeparate * pHtmlSep = new CHtmlSeparate();
			if( pHtmlSep == NULL )
			{
				continue;
			}

			/// 分隔符图片
			CCSprite* pSprite = CCSprite::create( SEPRATE_IMAGE );
			if( pSprite == NULL )
			{
				continue; /// 是否需要找一个默认图片
			}

			CCSize size = pSprite->getContentSize();
			if( pHtmlSep )
			{
				pHtmlSep->parseAttribute( com.mText );
				pHtmlSep->setContentSize( CCSizeMake(nWidth,size.height) );
				pHtmlElement = pHtmlSep;
			}

			nLineWidth += size.width;
			if( nLineWidth > nWidth ) /// 如果出现单图片大于行宽的情况则会有问题，外部需要做限制
			{
				/// 直接放入下一行
				lines.push_back( line );
				line.elements.clear();
				line.nWidth = 0;
				line.nHeight = 0;
				nLineWidth = 0;
			}

			line.elements.push_back( pHtmlElement );
			if( line.nHeight < size.height )
			{
				line.nHeight = size.height;
			}	
		}
		else if( com.mTag == "br" )  /// 换行
		{
			lines.push_back( line );
			line.elements.clear();
			line.nWidth = 0;
			line.nHeight = 10;/// 空行高度
			nLineWidth = 0;
		}
	}

	if( !line.elements.empty() )
	{
		lines.push_back( line );
	}
}
//-------------------------------------------------------------------------
/// 获取utf8字符长度
int getUtf8Length( const char* pszutf8 )
{
	if( pszutf8 == NULL )
	{
		return 0;
	}

	for( int i = 8; i > 0; --i )
	{
		unsigned char c = 1 << (i - 1);
		if( ( pszutf8[0] & c ) != c )
		{
			if( i == 8 )
			{
				return 1;
			}
			else
			{
				return 8 - i;
			}
		}
	}

	return 1;
}
//-------------------------------------------------------------------------
/**
@brief 从源字串获取指定宽度的字符串
@param strSrc 必须为utf8编码
*/
void CHtmlParse::partitionStrintByWidth( const std::string& strSrc, int nWidth, const std::string& strFontName, int nFontSize, CCSize& outSize, std::string& strOut, std::string& strRain )
{
	if( strSrc.empty() )
	{
		return;
	}

	size_t size = strSrc.length();
	for( size_t i = 0; i < size; )
	{
		const unsigned char c = strSrc.at(i);
		int len = getUtf8Length((const char*)&c);

		i += len;

		std::string strTemp = strSrc.substr( 0, i );
		outSize = getStringPixelSize( strTemp, strFontName, nFontSize );
		if( outSize.width >= nWidth )
		{
			strOut = strSrc.substr( 0, i - len );
			outSize = getStringPixelSize( strOut, strFontName, nFontSize );
			strRain = strSrc.substr( i - len );
			return;
		}
	}
}
//-------------------------------------------------------------------------
/**
@brief 获取字符串的宽度
@param
*/
CCSize CHtmlParse::getStringPixelSize( const std::string& strSrc, const std::string& strFontName, int nFontSize )
{
	CCLabelTTF* pLabel = CCLabelTTF::create( strSrc.c_str(), strFontName.c_str(), nFontSize );
	if( pLabel == NULL )
	{
		return CCSizeZero;
	}

	return pLabel->getContentSize();
}
//-------------------------------------------------------------------------
/**
@brief 解析属性字符串
@param
*/
void CHtmlParse::parseAttribute( const std::string& strAttr, std::map<std::string, std::string>& attr )
{
	if( strAttr.empty() )
	{
		return;
	}

	attr.clear();

	const char* pBuff = strAttr.c_str();
	char szOutBuff[256];
	do 
	{
		memset( szOutBuff, 0, sizeof(szOutBuff) );
		pBuff = cocos2d::engine::StringUtility::parseSubString( pBuff, ' ', szOutBuff );
		if( szOutBuff[0] != NULL )
		{
			std::string strOut = szOutBuff;
			cocos2d::engine::StringUtility::trim( strOut );
			const char* pPropText = strOut.c_str();
			memset( szOutBuff, 0, sizeof(szOutBuff) );
			pPropText = cocos2d::engine::StringUtility::parseSubString( pPropText, '=', szOutBuff );
			std::string strName = szOutBuff;
			std::string strProp = pPropText;
			cocos2d::engine::StringUtility::trim(strName);
			cocos2d::engine::StringUtility::trim(strProp);

			/// 去掉左右的 ' "
			size_t pos = strProp.find( '\'' );
			if( pos != std::string::npos )
			{
				strProp = strProp.substr( pos + 1, strProp.length() - pos );
			}
			pos = strProp.rfind( '\'');
			if( pos != std::string::npos )
			{
				strProp = strProp.substr( 0, strProp.length() - 1 );
			}

			pos = strProp.find( '\"' );
			if( pos != std::string::npos )
			{
				strProp = strProp.substr( pos + 1, strProp.length() - pos );
			}
			pos = strProp.rfind( '\"');
			if( pos != std::string::npos )
			{
				strProp = strProp.substr( 0, strProp.length() - 1 );
			}

			cocos2d::engine::StringUtility::trim( strProp );

			attr[strName] = strProp;
		}
	} while ( pBuff[0] != NULL );
}

NS_CC_END