#include "StringUtility.h"
#include <math.h>

NS_CC_ENGINE_BEGIN

/// Hash算法
uint32 StringUtility::makeELFHash( const char* szString )
{
	int nHash = 0;
	int nX    = 0;

	int nLen = static_cast<int>( strlen( szString ) );
	for(int i = 0; i < nLen; ++i )
	{
		nHash = (nHash << 4) + szString[i];
		if((nX = (int)(nHash & 0xF0000000L)) != 0)
		{
			nHash ^= (nX >> 24);
			nHash &= ~nX;
		}
	}

	return (nHash & 0x7FFFFFFF);
}
//-------------------------------------------------------------------------
void StringUtility::makeLower( std::string& str )
{
	char* pStr = new char[str.length()+1];
	size_t size = str.length();
	for( size_t i = 0; i < size; ++i )
	{
		char cc = str.at( i );
		if( cc >= 'A' && cc <= 'Z' )
		{
			cc += 'a' - 'A';
		}

		pStr[i] = cc;
	}

	pStr[size] = 0;
	str = pStr;
	delete [] pStr;
}
//-------------------------------------------------------------------------
void StringUtility::makeUpper( std::string& str )
{
	char* pStr = new char[str.length()+1];
	size_t size = str.length();
	for( size_t i = 0; i < size; ++i )
	{
		char cc = str.at( i );
		if( cc >= 'a' && cc <= 'z' )
		{
			cc -= 'a' - 'A';
		}

		pStr[i] = cc;
	}

	pStr[size] = 0;
	str = pStr;
	delete [] pStr;
}
//-------------------------------------------------------------------------
/// 去掉左边空白
void StringUtility::leftTrim( std::string& str )
{
	if (str.empty())
	{
		return;
	}
	
	std::basic_string<char>::iterator it = str.begin();
	size_t nPos = 0;
	while( it != str.end() )
	{
		if( (*it) != ' ' )
		{
			break;
		}

		++it;
		++nPos;
	}

	if( str.length() == nPos )
	{
		str.clear();
	}
	else
	{
		str = str.substr( nPos, str.length() - nPos );
	}
}
//-------------------------------------------------------------------------
/// 去掉右边空白
void StringUtility::rightTrim(std::string& str)
{
	if (str.empty())
	{
		return;
	}

	std::basic_string<char>::reverse_iterator it = str.rbegin();
	size_t nLenght = str.size();
	for( ;it != str.rend(); ++it )
	{
		if( (*it) != ' ' )
		{
			break;
		}

		--nLenght;
	}

	if( nLenght == 0 )
	{
		str.clear();
	}
	else
	{
		str = str.substr( 0, nLenght );
	}
}
//-------------------------------------------------------------------------
/// 去掉左右两边空白
void StringUtility::trim(std::string& str)
{
	leftTrim( str );
	rightTrim( str );
}
//------------------------------------------------------------------------------
/// 分解文件路径字串
bool StringUtility::parseFileName( const std::string& strFilePathName, std::string& strPathName, std::string& strFileName, std::string& strFileExtName )
{
	if( strFilePathName.empty() )
	{
		return false;
	}

	strPathName.clear();
	strFileName.clear();
	strFileExtName.clear();

	std::size_t point_pos = strFilePathName.rfind( '.' ); 

	std::size_t pos = strFilePathName.rfind('\\');
	if( pos == std::string::npos )
	{
		pos = strFilePathName.rfind('/');
		if( pos == std::string::npos )
		{
			if( point_pos == std::string::npos )
			{
				return false;
			}
		}
	}

	if( point_pos == std::string::npos )
	{
		strPathName = strFilePathName;
	}
	else
	{
		/// 以斜杠结尾
		if( pos == strFilePathName.length() - 1 )
		{
			strPathName = strFilePathName.substr( 0, pos );
		}
		else
		{
			if( pos != std::string::npos )
			{
				strPathName = strFilePathName.substr( 0, pos );
			}
			
			strFileName = strFilePathName.substr( pos+1, point_pos - pos - 1 );
			strFileExtName = strFilePathName.substr( point_pos + 1, strFilePathName.length() - point_pos - 1 );
		}
	}
	
	return true;
}
//-------------------------------------------------------------------------
/// 判断是否是绝对路径
bool StringUtility::isAbsolutePath( const std::string& strFilePathName )
{
	if( strFilePathName.empty() )
	{
		return false;
	}

	std::string strTemp = strFilePathName;
	stringReplace( strTemp, "\\", "/" );

	return strFilePathName.find( ":/") == 1;
}
//-------------------------------------------------------------------------
/// 拷贝字符串
char* StringUtility::sstrcpyn(char* dest, const char* src, size_t destSize)
{
#ifdef WIN32
	return lstrcpynA(dest, src, destSize);
#else
	/// 保证不会越界
	memcpy( dest, src, destSize );
	dest[destSize-1] = 0;
	return dest;
#endif
}
//-------------------------------------------------------------------------
/// 获取字符长度
int StringUtility::sstrlen( const char* src)
{
#ifdef WIN32
	return lstrlenA(src);
#else
	return strlen(src);
#endif
}
//-------------------------------------------------------------------------
/// 字符串比较
int StringUtility::sstricmp(char* dest, const char* src)
{
#ifdef WIN32
	return stricmp(dest, src);
#else
	return strcasecmp(dest, src);
#endif
}
//-------------------------------------------------------------------------
/// 字符串比较
int StringUtility::sstrcmp(char* dest, const char* src)
{
#ifdef WIN32
	return lstrcmpA(dest, src);
#else
	return strcmp(dest, src);
#endif
}
//-------------------------------------------------------------------------
/// 从szBuff中读出以 cSeparate结尾的字串 返回的是cSeparate的下一个字符指针
/// szBuff = "Windows Version_5.0"
/// if cSeparate = '_'  则szOutBuff = Windows Version
/// if cSeparate = ' '  则szOutBuff = Windows
const char* StringUtility::parseSubString( const char* szBuff, char cSeparate, char* szOutBuff )
{
	if( !szBuff || !szOutBuff )
		return NULL;

	const char* pBuff = szBuff;
	while( *pBuff )
	{
		if( *pBuff == cSeparate )
		{
			++pBuff;
			break;
		}

		*(szOutBuff++) = *(pBuff++);
	}

	*szOutBuff = 0;

	return pBuff;
}
//-------------------------------------------------------------------------
/// 将指定分隔符的字串分解到vector里面 float
bool StringUtility::parseString2FloatArray( const char* szBuff, char cSeparate, std::vector<float>& vecDataSet )
{
	if (!szBuff) return false;

	char buf[32];
	int i = 0;
	int j = 0;
	const char* p = szBuff;
	while (*p)
	{
		if (*p == cSeparate)
		{
			p++;
			buf[i] = 0;
			i = 0;
			vecDataSet.push_back( (float)atof(buf) );
		}
		else
		{
			buf[i++] = *p++;
		}
	}

	if (i > 0)
	{
		buf[i] = 0;
		vecDataSet.push_back( (float)atof(buf) );
	}

	return true;
}
//-------------------------------------------------------------------------
/// 将指定分隔符的字串分解到vector里面 string
bool StringUtility::parseString2StringArray( const char* szBuff, char cSeparate, std::vector<std::string>& vecDataSet )
{
	if (!szBuff) return false;

	char buf[32];
	int i = 0;
	int j = 0;
	const char* p = szBuff;
	while (*p)
	{
		if (*p == cSeparate)
		{
			p++;
			buf[i] = 0;
			i = 0;
			vecDataSet.push_back( buf );
		}
		else
		{
			buf[i++] = *p++;
		}
	}

	if (i > 0)
	{
		buf[i] = 0;
		vecDataSet.push_back( buf );
	}

	return true;
}
//-------------------------------------------------------------------------
/// 替换字符
void StringUtility::replaceStr( char* pSrcStr, char cSrc, char cReplace )
{
	while( *pSrcStr )
	{
		if( *pSrcStr == cSrc )
		{
			*pSrcStr = cReplace;
		}

		++pSrcStr;
	}
}
//-------------------------------------------------------------------------
void StringUtility::stringReplace( std::string& strInput, const std::string& strFind, const std::string& strReplace )     
{    
	size_t pos = 0;   
	while( std::string::npos != ( pos = strInput.find( strFind, pos ) ) )   
	{   
		strInput.replace( pos, strFind.length(), strReplace ); // 一次替换   
		pos = pos + strReplace.length();                       // 防止循环替换!!   
	}     
} 
//-------------------------------------------------------------------------
bool StringUtility::isDigitalHex( const std::string& str )
{
	int nLen = str.length();
	int nIndex = 0;
	while( nIndex < nLen )
	{
		char ch = str.at( nIndex++ );
		if( ch < '0' || ch > '9' )
		{
			if( ch < 'a' || ch > 'f' )
			{
				if( ch < 'A' || ch > 'F' )
				{
					return false;
				}
			}
		}
	}

	return true;	
}
//-------------------------------------------------------------------------
bool StringUtility::isDigital( const std::string& str )
{
	int nLen = str.length();
	int nIndex = 0;
	while( nIndex < nLen )
	{
		char ch = str.at( nIndex++ );
		if( ch < '0' || ch > '9' )
		{
			return false;
		}
	}

	return true;	
}
//-------------------------------------------------------------------------
bool StringUtility::string2Int( const std::string& str, int nBase, int& nNum )
{
	nNum = 0;
	int nLen = str.length();
	int nIndex = 0;
	while( nIndex < nLen )
	{
		char ch = str.at( nIndex++ );
		if( ch < '0' || ch > '9' )
		{
			if( ch < 'a' || ch > 'f' )
			{
				if( ch < 'A' || ch > 'F' )
				{
					return false;
				}
				else
				{
					nNum += ( ch - 'A' + 10 ) * static_cast<int>( pow(static_cast<float>( nBase ), static_cast<float>( nLen - nIndex ) ) );
				}
			}
			else
			{
				nNum += ( ch - 'a' + 10 ) * static_cast<int>( pow( static_cast<float>( nBase ), static_cast<float>( nLen - nIndex ) ) );
			}
		}
		else
		{
			nNum += ( ch - '0' ) * static_cast<int>( pow( static_cast<float>( nBase ), static_cast<float>( nLen - nIndex ) ) );
		}
	}

	return true;
}
//-------------------------------------------------------------------------
void StringUtility::xorBuff( const char* pSrc, int nSrcLen, const char* pKey, int nKeyLen, char* pDest )
{
	if( !pSrc || !pKey || !pDest || nSrcLen < 0 || nKeyLen < 0 )
		return;

	for( int i = 0; i < nSrcLen; ++i )
	{
		int nKeyIndex = i % nKeyLen;
		pDest[i] = pSrc[i] ^ pKey[nKeyIndex];
	}
}
//-------------------------------------------------------------------------
#define XOR_FIX_KEY 16
void StringUtility::xorBuffEx( const char* pSrc, int nSrcLen, char* pDest )
{
	if( !pSrc || !pDest || nSrcLen < 0 )
		return;

	/// 如果修改了长度，一定要修改后面的字串
	static char szBuff[XOR_FIX_KEY] = "NC&*@!()*SADL(?";

	for( int i = 0; i < nSrcLen; ++i )
	{
		int nKeyIndex = i % XOR_FIX_KEY;
		pDest[i] = pSrc[i] ^ szBuff[nKeyIndex];
	}
}

NS_CC_ENGINE_END
