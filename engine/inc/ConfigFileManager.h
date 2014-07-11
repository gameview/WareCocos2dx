////*************************************************************************
//	创建日期:	2013-4-29   11:10
//	文件名称:	ConfigFileManager.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	配置文件管理
//*************************************************************************
#ifndef _CONFIGFILEMANAGER_H_
#define _CONFIGFILEMANAGER_H_

#include <map>
#include <vector>
#include "cocos2dx_types.h"
#include "CSVFileCommon.h"
#include "DataContent.h"

NS_CC_ENGINE_BEGIN

/// CSV文件支持的数据类型
enum ECSVDataType
{
	ECSVDataType_NULL = 0,		/// 未定义类型
	ECSVDataType_Int,			/// 整型
	ECSVDataType_Word,			/// 双字节
	ECSVDataType_Byte,			/// 字节
	ECSVDataType_String,		/// 字符串（定长）指定最大长度
	ECSVDataType_Int_Array,		/// 整型数组
	ECSVDataType_Word_Array,	/// 双字节数组
	ECSVDataType_Byte_Array,	/// 字节数组
};

struct SCSVDataItem
{
	std::string		strName;	/// 数据名称
	std::string		strComment; /// 数据注释
	ECSVDataType	eDataType;	/// 数据类型
	bool			bKey;		/// 是否是Key
	int			nLength;	/// 数据长度（针对int,byte,word,string）对数组无效

	SCSVDataItem()
	{
		strName.clear();
		strComment.clear();
		eDataType = ECSVDataType_NULL;
		nLength = 0;
		bKey = false;
	}
};

/// CSV文件定义
struct SCSVFileDef
{
	std::string					strFormat;		/// 格式串
	std::string					strKeyFormat;	/// 关键字格式串
	std::vector<SCSVDataItem>	vecDataItem;	/// 数据类型列表

	SCSVFileDef()
	{
		strFormat.clear();
		strKeyFormat.clear();
		vecDataItem.clear();
	}
};

class CConfigFileManager
{
	typedef std::map< std::string, int >			CSVFileIndexMap;	/// 文件名到列表索引的映射

	typedef std::vector< SDataContent >				DataItemVec;		/// 数据项存储
	typedef std::vector< DataItemVec >				DataSet;			/// 数据集
	typedef std::map< uint32, uint32 >				DataIndexMap;		/// 关键字到数据项的映射
	typedef std::map< std::string, DataSet >		DataSetMap;			/// 文件名到数据集映射
	typedef std::map< std::string, DataIndexMap >	DataSetIndexMap;	/// 文件名到数据索引映射 
	typedef std::map< std::string, SCSVFileDef >	DataParadigmMap;	/// 文件范式映射

private:

	CConfigFileManager(void);
	~CConfigFileManager(void);

public:



	static CConfigFileManager& Instance()
	{
		static CConfigFileManager _inst;
		return _inst;
	}

	/**
	@brief 初始化CSV文件环境配置
	@param strConfigPath 配置文件路径
	@param pCSVFileKeyParadigmList CSV文件范式列表
	@param nCSVFileNumCSV文件数量
	*/
	bool	initCSVContext( const std::string& strConfigPath, SCSVFileParadigm* pCSVFileParadigmList, int nCSVFileNum );

	/**
	@brief 模板函数 获取CSV文件数据项数目
	*/
	int		getDataEntryNum( const std::string& strCSVFileName );
	
	//-------------------------------------------------------------------------
	/**
	@brief 模板函数 获取数据
	@param DataSet	数据集
	@param strConfigPath 文件名
	@param pParam  参数列表
	@param nLen	   参数长度
	*/
	template<typename T>
	bool getDataEntry( T* pDataSet, const std::string& strCSVFilePath, int* pParam, int nLen )
	{
		//DataItemVec DataItem;
		//getDataEntryByParam( DataItem, strCSVFilePath, pParam, nLen );
		return GenerateDataSet( strCSVFilePath, getDataEntryByParam( strCSVFilePath, pParam, nLen ), pDataSet, sizeof(T), GetTypeParadigm(strCSVFilePath) );
	}
	//-------------------------------------------------------------------------
	/**
	@brief 模板函数 获取数据
	@param DataSet	数据集
	@param strConfigPath 文件名
	@param uIndex	获取第几项数据
	*/
	template<typename T>
	bool getDataEntry( T* pDataSet, const std::string& strCSVFilePath, uint32 uIndex )
	{
		//DataItemVec DataItem;
		//DataItemVec* getDataEntryByIndex( DataItem, strCSVFilePath, uIndex );
		return GenerateDataSet( strCSVFilePath, getDataEntryByIndex( strCSVFilePath, uIndex ), pDataSet, sizeof(T), GetTypeParadigm(strCSVFilePath) );
	}

private:
	/**
	@brief 加载单个CSV文件
	@param
	*/
	bool			LoadCSVFile( const std::string& strCSVFileName );

	/**
	@brief 生成key
	@param
	*/
	uint32			GenerateKey( int nIndex, int* pParam, int nLen );

	/**
	@brief 获取关键字范式串
	@param
	*/
	const char*		GetKeyParadigm( const std::string& strCSVFileName );

	/**
	@brief 获取CSV文件数据项 
	@param data 数据容器
	@param nParam 参数数组
	@param nLen	  长度
	*/
	DataItemVec*	getDataEntryByParam( const std::string& strCSVFileName, int* pParam, int nLen );

	/**
	@brief 获取CSV文件数据项 
	@param data 数据容器
	@param strCSVFileName 文件名
	@param uIndex 数据项索引
	@return 数据
	*/
	DataItemVec*	getDataEntryByIndex( const std::string& strCSVFileName, uint32 uIndex );

	/**
	@brief 获取类型范式串
	@param
	*/
	const char*		GetTypeParadigm( const std::string& strCSVFileName );



	/**
	@brief 生成key范式
	@param CSVFileDef CSV文件定义
	@param strTypeParadigm 类型范式
	*/
	bool			GenerateDataSet( const std::string& strCSVFileName, DataItemVec* pDataItem, void* pData, int nLen, const char* szKeyParadigm );

private:

	CSVFileIndexMap		m_mapCSVFileIndex;			/// CSV文件索引

	SCSVFileParadigm*	m_pCSVFileParadigmList;		/// CSV文件范式定义
	int					m_nCSVFileNum;				/// CSV文件数量

	std::string			m_strConfigPath;			/// 配置文件路径

	DataSetMap			m_mapDataSet;				/// 数据集
	DataSetIndexMap		m_mapDataSetIndex;			/// 数据索引
	DataParadigmMap		m_mapDataParadigm;			/// 文件范式
};

NS_CC_ENGINE_END

#endif // _CONFIGFILEMANAGER_H_

