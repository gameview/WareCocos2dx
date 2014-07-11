#include "ConfigFileManager.h"
#include "StringUtility.h"
#include "WareFileRead.h"

NS_CC_ENGINE_BEGIN

#define CSV_DATA_LINE_MAX			2048		/// CSV数据一行长度

//-------------------------------------------------------------------------
/**
@brief 生成key范式
@param CSVFileDef CSV文件定义
@param strKeyParadigm 关键字范式
*/
bool GenerateKeyParadigm( SCSVFileDef& CSVFileDef, std::string& strKeyParadigm )
{
	size_t size = CSVFileDef.vecDataItem.size();
	for( size_t i = 0; i < size; ++i )
	{
		if( CSVFileDef.vecDataItem[i].bKey )
		{
			switch( CSVFileDef.vecDataItem[i].eDataType )
			{
			case ECSVDataType_Int:
				{
					strKeyParadigm.append( "4" );
					break;
				}
			case ECSVDataType_Word:
				{
					strKeyParadigm.append( "2" );
					break;
				}
			case ECSVDataType_Byte:
				{
					strKeyParadigm.append( "1" );
					break;
				}
			}
		}
		else
		{
			strKeyParadigm.append( "0" );
		}
	}

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 生成key范式
@param CSVFileDef CSV文件定义
@param strTypeParadigm 类型范式
*/
bool GenerateTypeParadigm( SCSVFileDef& CSVFileDef, std::string& strTypeParadigm )
{
	size_t size = CSVFileDef.vecDataItem.size();
	for( size_t i = 0; i < size; ++i )
	{
		switch( CSVFileDef.vecDataItem[i].eDataType )
		{
		case ECSVDataType_Int:			/// 整型
			{
				strTypeParadigm.append("i");
				break;
			}
		case ECSVDataType_Word:			/// 双字节
			{
				strTypeParadigm.append("w");
				break;
			}
		case ECSVDataType_Byte:			/// 字节
			{
				strTypeParadigm.append("b");
				break;
			}
		case ECSVDataType_String:		/// 字符串（定长）指定最大长度
			{
				strTypeParadigm.append("s");
				break;
			}
		case ECSVDataType_Int_Array:		/// 整型数组
			{
				strTypeParadigm.append("I");
				break;
			}
		case ECSVDataType_Word_Array:	/// 双字节数组
			{
				strTypeParadigm.append("W");
				break;
			}
		case ECSVDataType_Byte_Array:	/// 字节数组
			{
				strTypeParadigm.append("B");
				break;
			}
		}
	}

	return true;
}
/// 定义一个数据链
CConfigFileManager::CConfigFileManager(void)
: m_pCSVFileParadigmList(NULL)
, m_nCSVFileNum(0)
{
}

CConfigFileManager::~CConfigFileManager(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 初始化CSV文件环境配置
@param strConfigPath 配置文件路径
@param pCSVFileKeyParadigmList CSV文件Key范式列表
@param nCSVFileNumCSV文件数量
*/
bool CConfigFileManager::initCSVContext( const std::string& strConfigPath, SCSVFileParadigm* pCSVFileParadigmList, int nCSVFileNum )
{
	if( pCSVFileParadigmList == NULL || nCSVFileNum == 0 )
	{
		return false;
	}

	m_pCSVFileParadigmList = pCSVFileParadigmList;
	m_nCSVFileNum = nCSVFileNum;
	m_strConfigPath = strConfigPath;

	for( int i = 0; i < nCSVFileNum; ++i )
	{
		m_mapCSVFileIndex[pCSVFileParadigmList[i].szFileName] = i;
	}

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 获取CSV文件数据项 
@param strCSVFileName 文件名
@param pParam 参数列表
@param nLen	  参数数量
*/
CConfigFileManager::DataItemVec* CConfigFileManager::getDataEntryByParam( const std::string& strCSVFileName, int* pParam, int nLen )
{
	DataSetIndexMap::iterator iter = m_mapDataSetIndex.find( strCSVFileName );
	if( iter == m_mapDataSetIndex.end() )
	{
		return NULL;
	}

	int nFileIndex = -1;
	CSVFileIndexMap::iterator itFileIndex = m_mapCSVFileIndex.find( strCSVFileName );
	if( itFileIndex == m_mapCSVFileIndex.end() )
	{
		return NULL;
	}

	nFileIndex = itFileIndex->second;
	uint32 uKey = GenerateKey( nFileIndex, pParam, nLen );

	DataIndexMap::iterator itDataIndex = iter->second.find( uKey );
	if( itDataIndex == iter->second.end() )
	{
		return NULL;
	}

	return getDataEntryByIndex( strCSVFileName, itDataIndex->second );
}
//-------------------------------------------------------------------------
/**
@brief 获取CSV文件数据项 
@param strCSVFileName 文件名
@param uIndex 文件数据索引
@return 数据
*/
CConfigFileManager::DataItemVec* CConfigFileManager::getDataEntryByIndex( const std::string& strCSVFileName, uint32 uIndex )
{
	DataSetMap::iterator iter = m_mapDataSet.find( strCSVFileName );
	if( iter == m_mapDataSet.end() )
	{
		return NULL;
	}

	if( uIndex >= static_cast<uint32>(iter->second.size()) )
	{
		return NULL;
	}

	return &iter->second[uIndex];
}
//-------------------------------------------------------------------------
/**
@brief 模板函数 获取数据
@param DataSet	数据集
@param strConfigPath 文件名
@param uIndex	获取第几项数据
*/
int CConfigFileManager::getDataEntryNum( const std::string& strCSVFileName )
{
	DataSetMap::iterator iter = m_mapDataSet.find( strCSVFileName );
	if( iter == m_mapDataSet.end() )
	{
		/// 加载CSV文件 如果加载失败，则返回0
		if( !LoadCSVFile(strCSVFileName) )
		{
			return 0;
		}

		iter = m_mapDataSet.find( strCSVFileName );
	}

	return static_cast<int>(iter->second.size());
}
//-------------------------------------------------------------------------
/**
@brief 获取类型范式串
@param
*/
const char*	CConfigFileManager::GetKeyParadigm( const std::string& strCSVFileName )
{
	int nFileIndex = -1;
	CSVFileIndexMap::iterator iter = m_mapCSVFileIndex.find( strCSVFileName );
	if( iter == m_mapCSVFileIndex.end() )
	{
		return false;
	}

	return m_pCSVFileParadigmList[iter->second].szKeyParadigm;
}
//-------------------------------------------------------------------------
/**
@brief 获取类型范式串
@param
*/
const char*	CConfigFileManager::GetTypeParadigm( const std::string& strCSVFileName )
{
	int nFileIndex = -1;
	CSVFileIndexMap::iterator iter = m_mapCSVFileIndex.find( strCSVFileName );
	if( iter == m_mapCSVFileIndex.end() )
	{
		return false;
	}

	return m_pCSVFileParadigmList[iter->second].szDataItemType;
}
//-------------------------------------------------------------------------
/**
@brief 生成key范式
@param CSVFileDef CSV文件定义
@param strTypeParadigm 类型范式
*/
bool CConfigFileManager::GenerateDataSet( const std::string& strCSVFileName, DataItemVec* pDataItem, void* pData, int nLen, const char* szKeyParadigm )
{
	if( szKeyParadigm == NULL || pData == NULL || pDataItem == NULL )
	{
		return false;
	}

	if( (*pDataItem).empty() )
	{
		return false;
	}

	DataParadigmMap::iterator itParadigm = m_mapDataParadigm.find( strCSVFileName );
	if( itParadigm == m_mapDataParadigm.end() )
	{
		return false;
	}

	SCSVFileDef& sCSVFileDef = itParadigm->second;

	char* pDataBuff = (char*)pData;
	int nOffset = 0;
	size_t size = (*pDataItem).size();
	for( size_t i = 0; i < size; ++i )
	{
		switch( szKeyParadigm[i] )
		{
		case 'i'://ECSVDataType_Int:			/// 整型
			{
				int nValue = (*pDataItem)[i].toInt();
				nOffset += sizeof(int);
				if( nOffset > nLen )
				{
					return false;
				}
				memcpy( pDataBuff, &nValue, sizeof(int) );
				pDataBuff += sizeof(int);

				break;
			}
		case 'w'://ECSVDataType_Word:			/// 双字节
			{
				uint16 nValue = (uint16)(*pDataItem)[i].toInt();
				nOffset += sizeof(uint16);
				if( nOffset > nLen )
				{
					return false;
				}
				memcpy( pDataBuff, &nValue, sizeof(uint16) );
				pDataBuff += sizeof(uint16);
				break;
			}
		case 'b'://ECSVDataType_Byte:			/// 字节
			{
				char nValue = (char)(*pDataItem)[i].toInt();
				nOffset += sizeof(char);
				if( nOffset > nLen )
				{
					return false;
				}
				memcpy( pDataBuff, &nValue, sizeof(char) );
				pDataBuff += sizeof(char);
				break;
			}
		case 's'://ECSVDataType_String:		/// 字符串（定长）指定最大长度
			{
				const char* pValue = (*pDataItem)[i].toString();
				nOffset += sCSVFileDef.vecDataItem[i].nLength;
				if( nOffset > nLen )
				{
					return false;
				}
				memcpy( pDataBuff, pValue, (*pDataItem)[i].length() );
				pDataBuff += sCSVFileDef.vecDataItem[i].nLength;
				break;
			}
		case 'I'://ECSVDataType_Int_Array:		/// 整型数组
			{
				IntArray* piA = (IntArray*)pDataBuff;
				int nNumLen = (*pDataItem)[i].length();
				int* pDataArray = (int*)(*pDataItem)[i].toData();
				int nNum = nNumLen / sizeof(int);
				for( int i = 0; i < nNum; ++i )
				{
					piA->m_vecInt.push_back( pDataArray[i] );
				}
				nOffset += sizeof(IntArray);
				if( nOffset > nLen )
				{
					return false;
				}
				pDataBuff += sizeof(IntArray);
				break;
			}
		case 'W'://ECSVDataType_Word_Array:	/// 双字节数组
			{
				WordArray* piA = (WordArray*)pDataBuff;
				int nNumLen = (*pDataItem)[i].length();
				int* pDataArray = (int*)(*pDataItem)[i].toData();
				int nNum = nNumLen / sizeof(int);
				for( int i = 0; i < nNum; ++i )
				{
					piA->m_vecInt.push_back( pDataArray[i] );
				}
				nOffset += sizeof(WordArray);
				if( nOffset > nLen )
				{
					return false;
				}
				pDataBuff += sizeof(WordArray);
				break;
			}
		case 'B'://ECSVDataType_Byte_Array:	/// 字节数组
			{
				ByteArray* piA = (ByteArray*)pDataBuff;
				int nNumLen = (*pDataItem)[i].length();
				int* pDataArray = (int*)(*pDataItem)[i].toData();
				int nNum = nNumLen / sizeof(int);
				for( int i = 0; i < nNum; ++i )
				{
					piA->m_vecInt.push_back( pDataArray[i] );
				}
				nOffset += sizeof(ByteArray);
				if( nOffset > nLen )
				{
					return false;
				}
				pDataBuff += sizeof(ByteArray);
				break;
			}
		}
	}

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 加载单个CSV文件
@param
*/
bool CConfigFileManager::LoadCSVFile( const std::string& strCSVFileName )
{
	if( m_pCSVFileParadigmList == NULL )
	{
		return false;
	}

	if( strCSVFileName.empty() )
	{
		return false;
	}

	/// 已经存在该文件
	if( m_mapDataSet.find( strCSVFileName ) != m_mapDataSet.end() || m_mapDataSetIndex.find(strCSVFileName) != m_mapDataSetIndex.end() )
	{
		return true;
	}

	CWareFileRead File(true);
	if( !File.open( strCSVFileName.c_str() ) )
	{
		return false;
	}

	char szBuff[CSV_DATA_LINE_MAX];
	SCSVFileDef sCSVFileDef;

	/// 读第一行 数据名称
	int nIndex = 0;
	memset( szBuff, 0, sizeof(szBuff) );
	if( File.readLine( szBuff, sizeof(szBuff) ) == 0 )
	{
		/// 分析数据
		char szDataItemName[256];
		const char* pBuff = szBuff;
		do 
		{
			SCSVDataItem sCSVDataItem;
			memset( szDataItemName, 0, sizeof(szDataItemName) );
			pBuff = StringUtility::parseSubString( pBuff, ',', szDataItemName );
			if( szDataItemName[0] != 0 )
			{
				sCSVDataItem.strName = szDataItemName;
				sCSVFileDef.vecDataItem.push_back(sCSVDataItem);
				++nIndex;
			}
			else
			{
				++nIndex;
				printf("%s 第 %d 列数据名称没有定义\n", strCSVFileName.c_str(), nIndex );
			}
		} while ( pBuff[0] != 0 );
	}

	/// 读第二行 注释
	nIndex = 0;
	memset( szBuff, 0, sizeof(szBuff) );
	if( File.readLine( szBuff, sizeof(szBuff) ) == 0 )
	{
		/// 分析数据
		char szDataIteComment[256];
		const char* pBuff = szBuff;
		do 
		{
			memset( szDataIteComment, 0, sizeof(szDataIteComment) );
			pBuff = StringUtility::parseSubString( pBuff, ',', szDataIteComment );
			if( szDataIteComment[0] != 0 )
			{
				sCSVFileDef.vecDataItem[nIndex++].strComment = szDataIteComment;
			}
			else
			{
				++nIndex;
				printf( "%s 第 %d 列 没有数据注释\n", strCSVFileName.c_str(), nIndex );
			}
		} while ( pBuff[0] != 0 );
	}

	/// 读第三行 数据类型
	nIndex = 0;
	memset( szBuff, 0, sizeof(szBuff) );
	if( File.readLine( szBuff, sizeof(szBuff) ) == 0 )
	{
		/// 分析数据
		char szDataType[256];
		const char* pBuff = szBuff;
		std::string strDataType;
		do 
		{
			memset( szDataType, 0, sizeof(szDataType) );
			pBuff = StringUtility::parseSubString( pBuff, ',', szDataType );
			if( szDataType[0] != 0 )
			{
				strDataType = szDataType;
				if( strDataType == "int" )
				{
					sCSVFileDef.vecDataItem[nIndex++].eDataType = ECSVDataType_Int;
				}
				else if( strDataType == "byte" )
				{
					sCSVFileDef.vecDataItem[nIndex++].eDataType = ECSVDataType_Byte;
				}
				else if( strDataType == "word" )
				{
					sCSVFileDef.vecDataItem[nIndex++].eDataType = ECSVDataType_Word;
				}
				else if( strDataType == "string" )
				{
					sCSVFileDef.vecDataItem[nIndex++].eDataType = ECSVDataType_String;
				}
				else if( strDataType == "byte[]" )
				{
					sCSVFileDef.vecDataItem[nIndex++].eDataType = ECSVDataType_Byte_Array;
				}
				else if( strDataType == "int[]" )
				{
					sCSVFileDef.vecDataItem[nIndex++].eDataType = ECSVDataType_Int_Array;
				}
				else if( strDataType == "word[]" )
				{
					sCSVFileDef.vecDataItem[nIndex++].eDataType = ECSVDataType_Word_Array;
				}
			}
			else
			{
				++nIndex;
				printf( "%s 第 %d 列 数据类型没有定义\n", strCSVFileName.c_str(), nIndex );
			}
		} while ( pBuff[0] != 0 );
	}

	/// 读第四行 匹配Key
	nIndex = 0;
	memset( szBuff, 0, sizeof(szBuff) );
	if( File.readLine( szBuff, sizeof(szBuff) ) == 0 )
	{
		/// 分析数据
		char szDataDesc[256];
		const char* pBuff = szBuff;
		std::string strDataDesc;
		int nKeyLen = 0;
		do 
		{
			memset( szDataDesc, 0, sizeof(szDataDesc) );
			pBuff = StringUtility::parseSubString( pBuff, ',', szDataDesc );
			strDataDesc = szDataDesc;
			if( strDataDesc == "key" )
			{
				if( sCSVFileDef.vecDataItem[nIndex].eDataType == ECSVDataType_Byte
					|| sCSVFileDef.vecDataItem[nIndex].eDataType == ECSVDataType_Word
					|| sCSVFileDef.vecDataItem[nIndex].eDataType == ECSVDataType_Int )
				{
					switch( sCSVFileDef.vecDataItem[nIndex].eDataType )
					{
					case ECSVDataType_Int:
						{
							nKeyLen += sizeof(int);
							break;
						}
					case ECSVDataType_Word:
						{
							nKeyLen += sizeof(uint16);
							break;
						}
					case ECSVDataType_Byte:
						{
							nKeyLen += sizeof(char);
							break;
						}
					}

					sCSVFileDef.vecDataItem[nIndex++].bKey = true;
				}
				else
				{
					printf( "%s 非byte,word,int类型作key,严重错误！\n", strCSVFileName.c_str() );
				}
			}
			else
			{
				int nLength = 0;
				StringUtility::string2Int( szDataDesc, 10, nLength );
				if( sCSVFileDef.vecDataItem[nIndex].eDataType == ECSVDataType_String )
				{
					sCSVFileDef.vecDataItem[nIndex++].nLength = nLength;
				}
			}
		} while ( pBuff[0] != 0 );

		if( nKeyLen > 32 )
		{
			printf( "%s key长度超过32位，严重错误！\n", strCSVFileName.c_str() );
		}
	}
	
	/// 保存文件范式
	m_mapDataParadigm.insert( std::make_pair( strCSVFileName, sCSVFileDef ) );

	/// 添加文件数据集
	m_mapDataSet.insert( std::make_pair(strCSVFileName, DataSet() ) );
	m_mapDataSetIndex.insert( std::make_pair(strCSVFileName, DataIndexMap() ) );

	DataSetMap::iterator itDataSet = m_mapDataSet.find(strCSVFileName);
	DataSetIndexMap::iterator itDataIndex = m_mapDataSetIndex.find(strCSVFileName);

	std::string strKeyParadigm;
	std::string strTypeParadigm;
	GenerateKeyParadigm( sCSVFileDef, strKeyParadigm );
	GenerateTypeParadigm( sCSVFileDef, strTypeParadigm );

	int nFileIndex = -1;
	CSVFileIndexMap::iterator iter = m_mapCSVFileIndex.find( strCSVFileName );
	if( iter == m_mapCSVFileIndex.end() )
	{
		return false;
	}

	nFileIndex = iter->second;

	if( strKeyParadigm != m_pCSVFileParadigmList[nFileIndex].szKeyParadigm || strTypeParadigm != m_pCSVFileParadigmList[nFileIndex].szDataItemType )
	{
		return false;
	}

	/// 根据类型添加数据
	int nDataIndex = 0;
	memset( szBuff, 0, sizeof(szBuff) );
	while( File.readLine( szBuff, sizeof(szBuff) ) == 0 )
	{
		if( szBuff == NULL )
		{
			continue;
		}

		if( szBuff[0] == 0 )
		{
			continue;
		}

		nIndex = 0;
		/// 分析数据
		char szDataItem[256];
		const char* pBuff = szBuff;
		std::string strDataItem;
		DataItemVec DataItem;

		do 
		{
			memset( szDataItem, 0, sizeof(szDataItem) );
			pBuff = StringUtility::parseSubString( pBuff, ',', szDataItem );

			if( szDataItem[0] != 0 )
			{
				strDataItem = szDataItem;

				switch( sCSVFileDef.vecDataItem[nIndex].eDataType )
				{
				case ECSVDataType_Int:
					{
						int nValue = 0;
						StringUtility::string2Int(strDataItem,10,nValue);
						DataItem.push_back( SDataContent(nValue) );
						break;
					}
				case ECSVDataType_Word:
					{
						int nValue = 0;
						//uint16 nValue = 0;
						StringUtility::string2Int(strDataItem,10,nValue);
						//nValue = nNum;
						DataItem.push_back( SDataContent(nValue) );
						break;
					}
				case ECSVDataType_Byte:
					{
						int nValue = 0;
						//char nValue = 0;
						StringUtility::string2Int(strDataItem,10,nValue);
						//nValue = nNum;
						DataItem.push_back( SDataContent(nValue) );
						break;
					}
				case ECSVDataType_String:
					{
						DataItem.push_back( SDataContent(strDataItem) );
						break;
					}
				case ECSVDataType_Byte_Array:
					{
						std::vector<char> vecInt;
						StringUtility::parseString2IntArray<char>( strDataItem.c_str(), '+', vecInt );
						DataItem.push_back( SDataContent( &vecInt[0], sizeof(char) * vecInt.size() ) );
						break;
					}
				case ECSVDataType_Int_Array:
					{
						std::vector<int> vecInt;
						StringUtility::parseString2IntArray<int>( strDataItem.c_str(), '+', vecInt );
						DataItem.push_back( SDataContent( &vecInt[0], sizeof(int) * vecInt.size() ) );
						break;
					}
				case ECSVDataType_Word_Array:
					{
						std::vector<uint16> vecInt;
						StringUtility::parseString2IntArray<uint16>( strDataItem.c_str(), '+', vecInt );
						DataItem.push_back( SDataContent( &vecInt[0], sizeof(uint16) * vecInt.size() ) );
						break;
					}
				}
			}
			else
			{
				printf( "%s 第 %d 列 数据类型没有定义\n", strCSVFileName.c_str(), nIndex + 1 );
			}

			++nIndex;
		} while ( pBuff[0] != 0 );

		/// 生成key
		uint32 uKey = 0;
		size_t size = DataItem.size();
		std::vector<int> vecParam;
		//vecParam.resize( 4, 0 );
		int nKeyIndex = 0;
		for( size_t i = 0; i < size; ++i )
		{
			switch( m_pCSVFileParadigmList[nFileIndex].szKeyParadigm[i] )
			{
			case '4':
				{
					vecParam.push_back(DataItem[i].toInt());
					++nKeyIndex;
					break;
				}
			case '2':
				{
					vecParam.push_back(DataItem[i].toInt());
					++nKeyIndex;
					break;
				}
			case '1':
				{
					vecParam.push_back(DataItem[i].toInt());
					++nKeyIndex;
					break;
				}
			}
		}

		uKey = GenerateKey( nFileIndex, &vecParam[0], static_cast<int>( vecParam.size() ) );
		itDataSet->second.push_back( DataItem );
		itDataIndex->second.insert( std::make_pair( uKey, nDataIndex ) );
		memset( szBuff, 0, sizeof(szBuff) );

		++nDataIndex;
	}

	File.close();

	return true;
}
//-------------------------------------------------------------------------
/**
@brief 生成key
@param
*/
uint32 CConfigFileManager::GenerateKey( int nIndex, int* pParam, int nLen )
{
	if( m_pCSVFileParadigmList == NULL || pParam == NULL )
	{
		return 0;
	}

	std::vector<int> vecParam;
	vecParam.resize( 4, 0 );
	int nKeyPos = 0;
	int nKeyIndex = 0;
	size_t size = nLen;
	uint32 uKey = 0;
	for( size_t i = 0; i < size; ++i )
	{
		switch( m_pCSVFileParadigmList[nIndex].szKeyParadigm[i] )
		{
		case '4':
			{
				return pParam[nKeyIndex++];
			}
		case '2':
			{
				int nKeyParam  = pParam[nKeyIndex++];
				nKeyParam <<= nKeyPos;
				uKey |= nKeyParam;

				nKeyPos += 16;
				break;
			}
		case '1':
			{
				int nKeyParam  = pParam[nKeyIndex++];
				nKeyParam <<= nKeyPos;
				uKey |= nKeyParam;

				nKeyPos += 8;
				break;
			}
		}
	}

	return uKey;
}

NS_CC_ENGINE_END
