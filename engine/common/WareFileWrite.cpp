#include "WareFileWrite.h"

#ifdef WIN32
#include <Windows.h>
#endif

NS_CC_ENGINE_BEGIN

bool CWareFileWrite::open( const char* szFileName )
{
#ifdef WIN32
	std::ios_base::open_mode om = std::ios_base::out;
	if( !m_bTextMode )
	{
		om |= std::ios_base::binary;
	}

	if( m_bApp )
	{
		/// 这里使用ate而不是app,是因为app是写次写入都会写在文件的末尾，不能任意seek再写数据
		om |= std::ios::ate;//ios_base::app;
	}

	int nLen = static_cast<int>( strlen( szFileName ) );

	if( nLen > 0 )
	{
		int size = static_cast<int>( MultiByteToWideChar( CP_ACP, 0, szFileName, nLen, 0, 0 ) );
		WCHAR* dest = new WCHAR[size+1];  

		MultiByteToWideChar(CP_ACP, 0,szFileName, nLen, dest, size);

		dest[size] = 0;

		if( dest[0] == 0xFEFF ) // skip Oxfeff
		{
			for(int i = 0; i < size; i ++) 
				dest[i] = dest[i+1]; 
		}

		m_Out.open( dest, om );

		delete[] dest;
	}
#else
	std::ios_base::openmode om = std::ios::out;
	if( !m_bTextMode )
	{
		om |= std::ios::binary;
	}

	if( m_bApp )
	{
		/// 这里使用ate而不是app,是因为app是写次写入都会写在文件的末尾，不能任意seek再写数据
		om |= std::ios::ate;//ios_base::app;
	}

	m_Out.open( szFileName, om );
#endif

	if( m_Out.fail() )
		return false;

	return true;
}

void CWareFileWrite::close()
{
	m_Out.close();
}

void CWareFileWrite::seek( uint32 uOffset, EFilePos eFilePos )
{
	switch( eFilePos )
	{
	case FILE_POS_BEGIN:
		m_Out.seekp( uOffset, std::ios_base::beg );
		break;
	case FILE_POS_CURRENT:
		m_Out.seekp( uOffset, std::ios_base::cur );
		break;
	case FILE_POS_END:
		m_Out.seekp( uOffset, std::ios_base::end );
		break;
	}
}

bool CWareFileWrite::write( const void* pBuff, int nCount )
{
	if( !pBuff || nCount <= 0 )
		return false;

	m_Out.write( static_cast< const char*>(pBuff), nCount );
	m_Out.flush(); /// 保证每次都是直接flush到文件中 最主要是保证log在崩溃时及时写入数据

	return true;
}

void CWareFileWrite::writeLine( const char* pBuff )
{
	//WARE_ASSERT( m_bTextMode, "RareFileWrite::WriteLine 只能用于文本模式" );
	write( pBuff, static_cast<int>( strlen( pBuff ) ) );

#ifdef WIN32
	write( "\r\n", 2 );  /// 写入换行符
#else
	write( "\n", 1 );
#endif

}

uint32 CWareFileWrite::tell()
{
	return static_cast<uint32>( m_Out.tellp() );
}

NS_CC_ENGINE_END