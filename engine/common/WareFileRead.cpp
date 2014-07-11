
#include "WareFileRead.h"
//#ifndef WIN32
#include "cocos2d.h"
//#endif

//#ifdef WIN32
//#include <Windows.h>
//#endif

NS_CC_ENGINE_BEGIN

//////////////////////////////////////////////////////////////////////////
/// 二进制和文本模式的区别
/// 1.在windows系统中，文本模式下，文件以"\r\n"代表换行。若以文本模式打开文件，并用fputs等函数写入换行符"\n"时，函数会自动在"\n"前面加上"\r"。即实际写入文件的是"\r\n" 。
///	   fread会自动去除\r\n中的\r
/// 2.在类Unix/Linux系统中文本模式下，文件以"\n"代表换行。所以Linux系统中在文本模式和二进制模式下并无区别。
bool CWareFileRead::open( const char* szFileName )
{
	if( szFileName == NULL || szFileName[0] == 0 )
	{
		return false;
	}

 	std::string strMode("");
	if( m_bTextMode )
	{
		strMode = "r";
	}
	else
	{
		strMode = "rb";
	}
    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(szFileName);
	unsigned long lSize = 0;
	unsigned char* pBuff = CCFileUtils::sharedFileUtils()->getFileData( path.c_str(), strMode.c_str(), &lSize );
	if( pBuff == NULL )
	{
		return false;
	}

	m_bRemoveBuff = true;
	return open( pBuff, lSize );
 
}

bool CWareFileRead::open( const BYTE* pFileBuff, uint32 uSize )
{
	m_pBuff = pFileBuff;
	m_uSize = uSize;
	return true;
}

void CWareFileRead::close()
{
	if( m_pFile )
	{
		fclose( m_pFile );
	}

	if( m_bRemoveBuff && m_pBuff )
	{
		delete [] m_pBuff;
	}

	m_pFile = NULL;
	m_pBuff = NULL;
}

bool CWareFileRead::isEOF()
{
	if( m_pBuff )
	{
		return m_uCurrentPos >= m_uSize;
	}
	else
	{
		return feof( m_pFile ) != 0;
	}
}

uint32 CWareFileRead::seek(  int nOffset, EFilePos eFilePos )
{
	uint32 uRet = 0;

	if( m_pBuff )
	{
		switch( eFilePos )
		{
		case FILE_POS_BEGIN:
			m_uCurrentPos = nOffset;
			break;
		case FILE_POS_CURRENT:
			m_uCurrentPos += nOffset;
			break;
		case FILE_POS_END:
			m_uCurrentPos = m_uSize + nOffset;
			break;
		}

		uRet = m_uCurrentPos;
	}
	else
	{
		switch( eFilePos )
		{
		case FILE_POS_BEGIN:
			fseek(m_pFile,nOffset,SEEK_SET);
			break;
		case FILE_POS_CURRENT:
			fseek(m_pFile,nOffset,SEEK_CUR);
			break;
		case FILE_POS_END:
			fseek(m_pFile,nOffset,SEEK_END);
			break;
		}

		uRet = static_cast<uint32>(ftell(m_pFile));
	}

	return uRet;
}

bool CWareFileRead::read( void* pBuff, int nCount )
{
	if( !pBuff || nCount <= 0 )
		return false;

	if( !m_pBuff )
	{
		//m_In.read( (char*)pBuff, nCount );
		return fread(pBuff,sizeof(unsigned char), nCount,m_pFile) == nCount;
	}
	else
	{
		int nLen = 0;
		if( m_uCurrentPos + nCount >= m_uSize )
		{
			nLen = m_uSize - m_uCurrentPos;
		}
		else
		{
			nLen = nCount;
		}

		memcpy( pBuff, &m_pBuff[m_uCurrentPos], nLen );
		m_uCurrentPos += nLen;
	}

	return true;
}

uint32 CWareFileRead::tell()
{
	if( m_pBuff )
	{
		return m_uCurrentPos; 
	}
	else
	{
		return static_cast<uint32>(ftell(m_pFile));
	}
}

////////////////////////////////////////////////////////////////////////////
//int CWareFileRead::readLineFromBuff( char* pBuff, int nCount )
//{
//	char pChar = 0;
//	int i = 0;
//	for( ; i < nCount && m_uCurrentPos < m_uSize; ++i )
//	{
//		//m_In.read( &pChar, 1 );
//		pChar = m_pBuff[m_uCurrentPos++];
//		if( '\n' == pChar )
//		{
//			pBuff[i] = '\0';
//			return 0;
//		}
//		else if( 0x0d == pChar )
//		{
//			pChar = m_pBuff[m_uCurrentPos++];
//			if( '\n' == pChar )
//			{
//				pBuff[i] = '\0';
//				return 0;
//			}
//			else
//			{
//				pBuff[i] = 0x0d;
//				pBuff[i+1] = pChar;
//			}
//		}
//		else
//		{
//			pBuff[i] = pChar;
//		}
//	}
//
//	if( m_uCurrentPos <= m_uSize )
//	{
//		if( i == 0 )
//		{
//			return -1;
//		}
//		pBuff[i-1] = '\0';
//		return 0;
//	}
//
//	int nRet = 0;
//	while( pChar = m_pBuff[m_uCurrentPos++], ( m_uCurrentPos < m_uSize && '\n' != pChar ) )
//	{
//		if( '\r' == pChar )
//		{
//			pChar = m_pBuff[m_uCurrentPos++];
//			if( '\n' == pChar )
//			{
//				nRet++;
//				seek( -( nRet + 1 ), FILE_POS_CURRENT );
//				return nRet + 1;
//			}
//		}
//
//		nRet++;
//	}
//
//	seek( -( nRet + 1 ), FILE_POS_CURRENT );
//	return nRet + 1;	
//}
/// 只能用于文本模式
int CWareFileRead::readLine( char* pBuff, int nCount )
{
	if( !m_bTextMode )
	{
		nCount = 0;
		return 0;
	}
	//WARE_ASSERT( m_bTextMode, "CWareFileRead::ReadLine 只能用于文本模式" );
	//uint32 nCurPos = Tell();
	char pChar = 0;
	int i = 0;
	for( ; i < nCount && !isEOF(); ++i )
	{
		read( &pChar, 1 );
		if( '\n' == pChar )
		{
			pBuff[i] = '\0';
			return 0;
		}
		else if( 0x0d == pChar )
		{
			read( &pChar, 1 );
			if( '\n' == pChar )
			{
				pBuff[i] = '\0';
				return 0;
			}
			else
			{
				pBuff[i] = 0x0d;
				pBuff[i+1] = pChar;
			}
		}
		else
		{
			pBuff[i] = pChar;
		}
	}

	if( isEOF() )
	{
		if( i == 0 )
		{
			return -1;
		}
		pBuff[i-1] = '\0';
		return 0;
	}

	int nRet = 0;
	while( read( &pChar, 1 ), ( !isEOF() && '\n' != pChar ) )
	{
		if( '\r' == pChar )
		{
			read( &pChar, 1 );
			if( '\n' == pChar )
			{
				nRet++;
				seek( -( nRet + 1 ), FILE_POS_CURRENT );
				return nRet + 1;
			}
		}

		nRet++;
	}

	seek( -( nRet + 1 ), FILE_POS_CURRENT );
	return nRet + 1;
}

uint32 CWareFileRead::getSize()
{
	int nCurrentPos = tell();
	seek( 0, FILE_POS_END );
	uint32 ret = tell();
	seek( nCurrentPos, FILE_POS_BEGIN );
	return ret;
}

NS_CC_ENGINE_END
