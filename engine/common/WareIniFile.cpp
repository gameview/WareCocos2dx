#include "cocos2d.h"
#include "WareIniFile.h"
#include "WareFileRead.h"
#include "StringUtility.h"
#include "platform/CCFileUtils.h"
#include <vector>

NS_CC_ENGINE_BEGIN

CWareIniFile::CWareIniFile(void)
{
	m_mapIniFile.clear();
}
//-------------------------------------------------------------------------
CWareIniFile::~CWareIniFile(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 加载ini文件
@param
*/
bool CWareIniFile::loadFile( const char* szIniFileName )
{
	if( szIniFileName == NULL )
	{
		return false;
	}

	if( szIniFileName[0] == 0 )
	{
		return false;
	}

	//CCLOG( "CWareIniFile::loadFile %s", szIniFileName );
	CWareFileRead File;
	if( !File.open( szIniFileName ) )
	{
		return false;
	}

	uint32 nFileSize = File.getSize();
	//CCLOG( "CWareIniFile::loadFile File Size %d", nFileSize );
	char* pBuff = new char[nFileSize + 3];
	if( pBuff == NULL )
	{
		return false;
	}

	File.read( pBuff, nFileSize );
//#ifdef WIN32
	pBuff[nFileSize] = '\r';
	pBuff[nFileSize+1] = '\n';
	pBuff[nFileSize+2] = 0;
//#else
//	pBuff[nFileSize] = '\n';
//	pBuff[nFileSize+1] = 0;
//#endif

	File.close();

	BYTE* pContent = (BYTE*)pBuff;
	if( pContent[0] == 0xef && pContent[1] == 0xbb && pContent[2] == 0xbf ) /// 则认为文件是utf8编码
	{
		pBuff += 3;
	}

	std::vector<std::string> strList;
//#ifdef WIN32
	char* finder = strstr((char*)pBuff,"\r\n");
//#else
//	char* finder = strstr((char*)pBuff,"\n");
//#endif
	char* oldfinder = (char*)pBuff;
	while(finder!=NULL)
	{
		std::string str;
		str.append(oldfinder,finder);
		strList.push_back(str);

		//CCLOG( "CWareIniFile::loadFile %s", str.c_str() );
//#ifdef WIN32
		oldfinder = finder + 2;
		finder=strstr(oldfinder,"\r\n");
//#else
//		oldfinder = finder + 1;
//		finder=strstr(oldfinder,"\n");
//#endif
	}

	//CCLOG( "CWareIniFile::loadFile IniFile Data %d", strList.size() );

	//write data to m_SecNodes;
	FileMap::iterator sectIter;
	//SecNode *pCurSec = NULL;
	for (int i=0;i<(int)strList.size();i++)
	{
		if(strList[i].empty())		/// 空行直接不处理
		{
			continue;
		}

		if(strList[i].find("//",0) == 0 || strList[i].find("#",0) == 0 ) /// 忽略掉注释
		{			
			continue;
		}

		cocos2d::engine::StringUtility::trim( strList[i] );//skm modified(Trim)
		//CCLOG( "CWareIniFile::loadFile trim" );
		size_t pos = strList[i].find( "[" );
		if( pos != 0 )
		{
			//CCLOG( "CWareIniFile::loadFile *=*" );

			size_t equ_pos =  strList[i].find( "=" );
			if( equ_pos == std::string::npos )
			{
				continue;
			}

			std::string strKey, strValue;
			size_t size = strList[i].find( "=" );
			strKey = strList[i].substr(0,size);
			cocos2d::engine::StringUtility::trim(strKey); 
			strValue = strList[i].substr(size+1,strList[i].length());
			cocos2d::engine::StringUtility::trim(strValue);

			ElementMap& element = sectIter->second;
			//SDataContent sDataContent;
			if( cocos2d::engine::StringUtility::isDigital(strValue) )
			{
				//CCLOG( "CWareIniFile::loadFile int" );
				//sDataContent = ::atoi(strValue.c_str());
				int nNum = 0;
				//CCLOG( "CWareIniFile::loadFile string2int" );
				cocos2d::engine::StringUtility::string2Int( strValue, 10, nNum );
				//CCLOG( "CWareIniFile::loadFile string2int oo" );
                
                SDataContent s(nNum);

				//CCLOG( "CWareIniFile::loadFile string2int map Size %d", element.size() );
				element.insert( make_pair( strKey, s ) );
				//element[strKey] = s;
			}
			else /// 支持一个浮点数
			{
                 SDataContent s(strValue);
				//CCLOG( "CWareIniFile::loadFile string" );
				//element[strKey] = strValue;
				element.insert( make_pair( strKey, s ) );
			}
		}
		else
		{
			//CCLOG( "CWareIniFile::loadFile []" );
			size_t end_pos =  strList[i].rfind( "]" );
			if( end_pos == strList[i].length() - 1 )
			{
				/// 去掉中括号
				std::string strSector = strList[i].substr( 1, strList[i].length() - 1 );
				strSector = strSector.substr(0,strSector.length() - 1 );

				m_mapIniFile[strSector] = ElementMap();
				sectIter = m_mapIniFile.find( strSector );
				//CCLOG( "CWareIniFile::loadFile %s", strSector.c_str() );
			}
		}
	}

	delete[] pContent;
	return true;
}
//-------------------------------------------------------------------------
///**
//@brief 保存到文件
//@param
//*/
//void		SaveToFile();
//-------------------------------------------------------------------------
/**
@brief 
@param
*/
int CWareIniFile::getInt( const std::string& strSectorName, const std::string& strKeyName, int nDeaultValue )
{
	if( strSectorName.empty() || strKeyName.empty() )
	{
		return nDeaultValue;
	}

	FileMap::iterator itSec = m_mapIniFile.find( strSectorName );
	if( itSec == m_mapIniFile.end() )
	{
		return nDeaultValue;
	}

	ElementMap::iterator itElement = itSec->second.find( strKeyName );
	if( itElement == itSec->second.end() )
	{
		return nDeaultValue;
	}

	if( itElement->second.getType() != EDATA_TYPE_INT32 )
	{
		return nDeaultValue;
	}

	return itElement->second.toInt();
}
//-------------------------------------------------------------------------
/**
@brief 获取字符串
@param
*/
bool CWareIniFile::getString( const std::string& strSectorName, const std::string& strKeyName, std::string& strValue )
{
	if( strSectorName.empty() || strKeyName.empty() )
	{
		return false;
	}

	FileMap::iterator itSec = m_mapIniFile.find( strSectorName );
	if( itSec == m_mapIniFile.end() )
	{
		return false;
	}

	ElementMap::iterator itElement = itSec->second.find( strKeyName );
	if( itElement == itSec->second.end() )
	{
		return false;
	}

	if( itElement->second.getType() != EDATA_TYPE_STRING )
	{
		return false;
	}

	const char* pString = itElement->second.toString();
	if( pString == NULL )
	{
		return false;
	}

	strValue = pString;

	return true;
}

NS_CC_ENGINE_END