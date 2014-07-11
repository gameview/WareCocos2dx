//*************************************************************************
//	创建日期:	2013-4-6   13:20
//	文件名称:	StringUtility.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	字符串处理实用函数
//*************************************************************************
#ifndef _STRINGUTILITY_H_
#define _STRINGUTILITY_H_

#include <vector>
#include <string>
#include <map>
#include <stdlib.h>
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

class StringUtility
{
public:

	/// Hash算法
	static uint32			makeELFHash( const char* szString );

	/// 将字符串转为全小写
	static void				makeLower( std::string& str );

	/// 将字符串转为全大写
	static void				makeUpper( std::string& str );

	/// 去掉左边空白
	static void				leftTrim( std::string& str );
	/// 去掉右边空白
	static void				rightTrim(std::string & str);
	/// 去掉左右两边空白
	static void				trim(std::string & str);
	/// 分解文件路径字串
	static bool				parseFileName( const std::string& strFilePathName, std::string& strPathName, std::string& strFileName, std::string& strFileExtName );
	/// 判断是否是绝对路径 windows系统有效
	static bool				isAbsolutePath( const std::string& strFilePathName );

	/// 安全字符串拷贝
	static char*			sstrcpyn(char* dest, const char* src, size_t destSize);

	/// 字符串比较
	static int				sstrcmp(char* dest, const char* src);
	/// 获取字符长度
	static int				sstrlen( const char* src);
	/// 字符串比较
	static int				sstricmp(char* dest, const char* src);

	/// 从szBuff中读出以 cSeparate结尾的字串 返回的是cSeparate的下一个字符指针
	/// szBuff = "Windows Version_5.0"
	/// if cSeparate = '_'  则szOutBuff = Windows Version
	/// if cSeparate = ' '  则szOutBuff = Windows
	static const char*		parseSubString( const char* szBuff, char cSeparate, char* szOutBuff );

	/// 将指定分隔符的字串分解到vector里面 支持整数int WORD char
	template< typename T >
	static bool				parseString2IntArray( const char* szBuff, char cSeparate, std::vector<T>& vecDataSet )
	{
		if (!szBuff) return false;

		char buf[32];
		int i = 0;
		const char* p = szBuff;
		while (*p)
		{
			if(*p == cSeparate)
			{
				p++;
				buf[i] = 0;
				i = 0;
				vecDataSet.push_back( (T)atoi(buf) );
			}
			else
			{
				buf[i++] = *p++;
			}
		}

		if (i > 0)
		{
			buf[i] = 0;
			vecDataSet.push_back( (T)atoi(buf) );
		}

		return true;
	}

	/// 将指定分隔符的字串分解到vector里面 float
	static bool				parseString2FloatArray( const char* szBuff, char cSeparate, std::vector<float>& vecDataSet );

	/// 将指定分隔符的字串分解到vector里面 string
	static bool				parseString2StringArray( const char* szBuff, char cSeparate, std::vector<std::string>& vecDataSet );

	///  将pSrcStr 所有 cSrc 替换成 cReplace 
	static void				replaceStr( char* pSrcStr, char cSrc, char cReplace );

	/// 替换字符
	/// 替换源串
	/// 替换目的串
	static void				stringReplace( std::string& strInput, const std::string& strFind, const std::string& strReplace );  

	/// 判断一个字串是否是数字串
	static bool				isDigital( const std::string& str );

	/// 判断一个字串是否是数字串(十六进制)
	static bool				isDigitalHex( const std::string& str );

	/// 从字串转换成int， nBase为指数底 比如10进制用10
	static bool				string2Int( const std::string& str, int nBase, int& nNum );

	/// 将源缓冲区与密码缓冲进行异或处理后放到目标缓冲区
	/// 该函数要求目标缓冲的大小必须大于等于源缓冲的大小
	/// 各个缓冲区由使用者分配和释放，该方法不对缓冲区进行管理
	static void				xorBuff( const char* pSrc, int nSrcLen, const char* pKey, int nKeyLen, char* pDest );

	/// XorBuff的加强版，里面带了一个固定串
	static void				xorBuffEx( const char* pSrc, int nSrcLen, char* pDest );

	/// 不支持string
	template< class Type, class File >
	static void loadMapValue( std::map< float32, Type >& Map, File& file )
	{
		Map.clear();

		int32 nSize = 0;
		float32 fPos = 0.0f;
		Type	v; 
		file.read( &nSize, sizeof(int32) );
		for( int32 i = 0; i < nSize; ++i )
		{
			file.read( &fPos, sizeof(float32) );
			file.read( &v, sizeof(Type) );

			Map.insert( std::make_pair( fPos, v ) );
		}
	};

	template< class Type, class File >
	static void saveMapValue( std::map< float32, Type >& Map, File& file )
	{
		size_t size = Map.size();
		file.write( &size, sizeof(int32) );
		typename std::map< float32, Type >::iterator iter = Map.begin();
		for( ; iter != Map.end(); ++iter )
		{
			file.write( &iter->first, sizeof(float32) );
			file.write( &iter->second, sizeof(Type) );
		}
	};

	template< class File >
	static void loadMapString( std::map< float32, std::string >& Map, File& file )
	{
		Map.clear();

		int32 nSize = 0;
		float32 fPos = 0.0f;
		std::string strTemp;
		int32 nLen = 0;
		char szBuff[1024] = {0};
		file.read( &nSize, sizeof(int32) );
		for( int32 i = 0; i < nSize; ++i )
		{
			file.read( &fPos, sizeof(float32) );
			file.read( &nLen, sizeof(int32) );
			file.read( szBuff, nLen );
			szBuff[nLen] = 0; /// 字符结尾
			strTemp = szBuff;
			Map.insert( std::make_pair( fPos, strTemp ) );
		}
	};

	template< class File >
	static void saveMapString( std::map< float32, std::string >& Map, File& file )
	{
		size_t size = Map.size();
		size_t len = 0;

		typename std::map< float32, std::string >::iterator iter = Map.begin();
		for( ; iter != Map.end(); ++iter )
		{
			file.write( &iter->first, sizeof(float32) );
			len = iter->second.size();
			file.write( &len, sizeof(int32) );
			file.write( iter->second.c_str(), len );
		}
	};

	/// 获取指定位置的值 如果指定位置不存在，则取指定位置附近的两个值做线性插值
	/// 有一个线性插值 需要Type支持 + 和 *(float32)的操作
	template< class Type >
	static void getMapLinearLerpValue( std::map< float32, Type >& Map, float32 fPos, Type& v )
	{
		if( Map.size() < 2 )
			return;

		/// Map::upper_bound() 
		/// Returns an iterator to the first element in a map that with a key having a value that is greater than that of a specified key.
		typename std::map< float32, Type >::iterator iter = Map.upper_bound( fPos );
		typename std::map< float32, Type >::iterator itEnd = Map.end();

		if( iter == itEnd )
			return;

		if( iter == Map.begin() )
		{
			v = iter->second;
			return;
		}

		typename std::map< float32, Type >::iterator it = --iter;
		iter++;
		float32 fBeg = it->first;
		float32 fEnd = iter->first;
		float32 fPoint = ( fPos - fBeg ) / ( fEnd - fBeg );

		v = static_cast<Type>( it->second * ( 1.0f - fPoint ) + iter->second * fPoint );

	};

	/// 找一个最接近指定位置的值
	template< class Type >
	static void getMapValue( std::map< float32, Type >& Map, float32 fPos, Type& v )
	{
		if( Map.size() < 2 )
			return;

		typename std::map< float32, Type >::iterator iter = Map.upper_bound( fPos );
		typename std::map< float32, Type >::iterator itEnd = Map.end();

		if( iter == itEnd )
			return;

		if( iter == Map.begin() )
		{
			v = iter->second;
			return;
		}

		typename std::map< float32, Type >::iterator it = --iter;
		iter++;
		float32 fBeg = fPos - it->first;
		float32 fEnd = iter->first - fPos;

		v = fBeg - fEnd > 0.0f ? iter->second : it->second;

	};

	/// 找一个最接近指定位置的节点的Key值
	template< class Type >
	static void getMapKey( std::map< float32, Type >& Map, float32 fPos, float32& fKeyValue )
	{
		if( Map.size() < 2 )
			return;

		typename std::map< float32, Type >::iterator iter = Map.upper_bound( fPos );
		typename std::map< float32, Type >::iterator itEnd = Map.end();

		if( iter == itEnd )
			return;

		if( iter == Map.begin() )
		{
			fKeyValue = iter->first;
			return;
		}

		typename std::map< float32, Type >::iterator it = --iter;
		iter++;
		float32 fBeg = fPos - it->first;
		float32 fEnd = iter->first - fPos;

		fKeyValue = fBeg - fEnd > 0.0f ? iter->first : it->first;

	};
	
};

NS_CC_ENGINE_END

#endif // _STRINGUTILITY_H_

