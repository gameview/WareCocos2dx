//*************************************************************************
//	创建日期:	2013-4-6   11:47
//	文件名称:	CWareFileRead.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	文件读
//*************************************************************************
#ifndef _CWAREFILEREAD_H_
#define _CWAREFILEREAD_H_

#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include <fstream>

NS_CC_ENGINE_BEGIN

class CWareFileRead
{
public:
	CWareFileRead( bool bTextMode = false ) : m_bTextMode( bTextMode ), m_pBuff(NULL), m_uSize(0), m_uCurrentPos(0),m_pFile(NULL),m_bRemoveBuff(false) {}
	~CWareFileRead(void) {}

	/// 打开磁盘上的文件
	/// szFileName 文件名
	bool	open( const char* szFileName );
	/// 将内存看作是文件
	/// pFileBuff 缓冲区指针
	/// 缓冲区大小
	bool	open( const BYTE* pFileBuff, uint32 uSize );

	void	close();

	int		readLine( char* pBuff, int nCount );

	bool	isEOF();

	uint32	seek( int nOffset, EFilePos eFilePos );
	bool	read( void* pBuff, int nCount );
	uint32	tell();

	uint32	getSize();

	template< class T >
	bool	read( T& data )	{ return read( &data, sizeof(T) ); }

	/// gc++不支持模板特化的写法
//#ifdef WIN32

	/// 模板特化，让适用于string 数据长度是变长的
 
	bool	readStr( std::string& strData )
	{
		int nLen = 0;
		read( nLen );
		if( nLen > 0 )
		{
			if( nLen < 2048 )
			{
				char szBuff[2048];
				if( !read( szBuff, nLen ) )
					return false;

				szBuff[nLen] = 0;
				strData = szBuff;
			}
			else
			{
				char* pBuff = new char[nLen+1];
				if( !read( pBuff, nLen ) )
				{
					delete [] pBuff;
					return false;
				}
				pBuff[nLen] = 0;
				strData = pBuff;
				delete [] pBuff;
			}
		}

		return true;
	}

//#endif

private:
	
	//int	readLineFromBuff( char* pBuff, int nCount );
	//int	readLineFromFile( char* pBuff, int nCount );

private:
	
	bool			m_bTextMode;
	bool			m_bRemoveBuff;
	//std::ifstream	m_In;
	FILE*			m_pFile;
	const BYTE*		m_pBuff;
	uint32			m_uSize;
	uint32			m_uCurrentPos;
};

NS_CC_ENGINE_END

#endif // _CWAREFILEREAD_H_