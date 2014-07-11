////*************************************************************************
//	创建日期:	2013-4-6   11:42
//	文件名称:	CWareFileWrite.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	文件读
//*************************************************************************
#ifndef _CWAREFILEWRITE_H_
#define _CWAREFILEWRITE_H_

#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include <fstream>
//using namespace std;

NS_CC_ENGINE_BEGIN

class CWareFileWrite
{
public:
	CWareFileWrite( bool bTextMode = false, bool bApp = false ) : m_bTextMode(bTextMode), m_bApp(bApp) {}
	~CWareFileWrite(void) {}

	bool	open( const char* szFileName );
	void	close();

	bool	isEOF()	{ return m_Out.eof(); }

	void	seek( uint32 uOffset, EFilePos eFilePos );
	bool	write( const void* pBuff, int nCount );
	void	writeLine( const char* pBuff );
	uint32	tell(); 

	/// gc++不支持模板特化的写法 
	template< class T > 
	bool	write( T& data ) { return write( (void*)&data, sizeof(T) ); }

	template< class T > 
	bool	write( const T& data ) { return write( (void*)&data, sizeof(T) ); }

	/// 模板特化，让适用于string 数据长度是变长的
	//template<>
	bool	writeStr( std::string& strData )
	{
		int len = static_cast<int>( strData.length() );
		write( len );
		if( !strData.empty() )
		{
			return write( (void*)strData.data(), len );
		}

		return true;
	}

	bool	writeStr( const std::string& strData )
	{
		int len = static_cast<int>( strData.length() );
		write( len );
		if( !strData.empty() )
		{
			return write( (void*)strData.data(), len );
		}

		return true;
	} 
	 
private:
	bool			m_bTextMode;
	bool			m_bApp;

	std::ofstream	m_Out;
};

NS_CC_ENGINE_END

#endif // _CWAREFILEWRITE_H_
