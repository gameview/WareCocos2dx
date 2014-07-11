#include "StringConvert.h"
#include "StringUtility.h"

NS_CC_ENGINE_BEGIN
/** Converts a string to a float. 
@returns
0.0 if the value could not be parsed, otherwise the float version of the string.
*/
float StringConvert::parseFloat(const std::string& val)
{	
	return (float)::atof(val.c_str());
}
//------------------------------------------------------------------------------
/** Converts a string to a whole number. 
@returns
0.0 if the value could not be parsed, otherwise the numeric version of the string.
*/
int StringConvert::parseInt(const std::string& val)
{
	return atoi(val.c_str());
}
/**
@brief convert s string to color
@param
*/
ccColor4B StringConvert::parseColor(const std::string& val)
{
	char szDataItemName[256];
	const char* pBuff = val.c_str();
	int nIndex = 0;
	ccColor4B color;
	do 
	{
		
		memset( szDataItemName, 0, sizeof(szDataItemName) );
		pBuff = StringUtility::parseSubString( pBuff, ',', szDataItemName );
		if( szDataItemName[0] != 0 )
		{
			switch( nIndex )
			{
			case 0: { color.r = parseInt( szDataItemName ); ++nIndex; break; }
			case 1: { color.g = parseInt( szDataItemName ); ++nIndex; break; }
			case 2: { color.b = parseInt( szDataItemName ); ++nIndex; break; }
			case 3: { color.a = parseInt( szDataItemName ); ++nIndex; break; }
			default:
				break;
			}
			
		}
	}while( pBuff[0] != NULL );

	return color;
}
//------------------------------------------------------------------------------
/**
@brief convert a string to point
@param
*/
cocos2d::CCPoint StringConvert::parsePoint( const std::string& val )
{
	char szDataItemName[256];
	const char* pBuff = val.c_str();
	int nIndex = 0;
	CCPoint ptLoc;
	do 
	{
		memset( szDataItemName, 0, sizeof(szDataItemName) );
		pBuff = StringUtility::parseSubString( pBuff, ';', szDataItemName );
		if( szDataItemName[0] != 0 )
		{
			switch( nIndex )
			{
			case 0: { ptLoc.x = parseFloat( szDataItemName ); ++nIndex; break; }
			case 1: { ptLoc.y = parseFloat( szDataItemName ); ++nIndex; break; }
			default:
				break;
			}

		}
	}while( pBuff[0] != NULL );

	return ptLoc;
}
//-------------------------------------------------------------------------
/**
@brief convert a string to point 以点为分隔符
@param 
*/
cocos2d::CCPoint StringConvert::parseDotPoint( const std::string& val )
{
	char szDataItemName[256];
	const char* pBuff = val.c_str();
	int nIndex = 0;
	CCPoint ptLoc;
	do 
	{
		memset( szDataItemName, 0, sizeof(szDataItemName) );
		pBuff = StringUtility::parseSubString( pBuff, ',', szDataItemName );
		if( szDataItemName[0] != 0 )
		{
			switch( nIndex )
			{
			case 0: { ptLoc.x = parseFloat( szDataItemName ); ++nIndex; break; }
			case 1: { ptLoc.y = parseFloat( szDataItemName ); ++nIndex; break; }
			default:
				break;
			}

		}
	}while( pBuff[0] != NULL );

	return ptLoc;
}
//-------------------------------------------------------------------------
/**
@brief convert color with #
@param
*/
cocos2d::ccColor3B StringConvert::parseSharpColor( const std::string& val )
{
	std::string strVal = val;
	StringUtility::trim( strVal );
	StringUtility::makeLower( strVal );
	cocos2d::ccColor3B cc = ccc3( 255,255,255 );
	size_t pos = val.find( '#' );
	if( pos == std::string::npos )
	{
		return cc;
	}
	else
	{
		strVal = strVal.substr( pos + 1 );
	}

	unsigned char r,g, b;
	r = g = b = 0;
	int len = strVal.length();
	int nIndex = 0;  /// 0->b 1->g 2->r
	int nNum[2]; /// 两位一个值
	int nUserChar = 0;
	while( len > 0 )
	{
		memset( nNum, 0, sizeof(nNum) );

		std::string strTemp;
		if( len < 2 )
		{
			strTemp = strVal.substr( len - 1, 1 );
			char c = strTemp.at(0);
			if( c >= '0' && c <= '9' )
			{
				nNum[0] = c - '0';
			}
			else
			{
				if( c >= 'a' && c <= 'f' )
				{
					nNum[0] = c - 'a' + 10;
				}
				else
				{
					nNum[0] = 0;
				}
			}
			
			nUserChar = 1;
		}
		else
		{
			strTemp = strVal.substr( len - 2, 2 );
			for( int i = 0; i < 2; ++i )
			{
				char c = strTemp.at( 1 - i );
				if( c >= '0' && c <= '9' )
				{
					nNum[i] = c - '0';
				}
				else
				{
					if( c >= 'a' && c <= 'f' )
					{
						nNum[i] = c - 'a' + 10;
					}
					else
					{
						nNum[i] = 0;
					}
				}
			}

			nUserChar = 2;
		}

		switch( nIndex )
		{
		case 0:
			{
				b = nNum[1] * 16 + nNum[0];
				break;
			}
		case 1:
			{
				g = nNum[1] * 16 + nNum[0];
				break;
			}
		case 2:
			{
				r = nNum[1] * 16 + nNum[0];
				break;
			}
		}

		strVal = strVal.substr( 0, len - nUserChar );
		len = strVal.length();
		++nIndex;
	}

	cc = ccc3( r, g, b );
	return cc;
}

NS_CC_ENGINE_END