//*************************************************************************
//	创建日期:	2013-4-6   23:36
//	文件名称:	WareIniFile.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	ini文件读取 
//				目前只支持整型和字符串 以后会支持浮点型 不支持行中注释
//*************************************************************************
#ifndef _WAREINIFILE_H_
#define _WAREINIFILE_H_

#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include "DataContent.h"
#include <map>

NS_CC_ENGINE_BEGIN

class CWareIniFile
{
	typedef std::map< std::string, SDataContent > ElementMap;	/// 数据元素
	typedef std::map< std::string, ElementMap >	FileMap;		/// Ini文件
public:
	CWareIniFile(void);
	~CWareIniFile(void);

	/**
	@brief 加载ini文件
	@param
	*/
	bool		loadFile( const char* szIniFileName );

	///**
	//@brief 保存到文件
	//@param
	//*/
	//void		SaveToFile();

	/**
	@brief 
	@param
	*/
	int		getInt( const std::string& strSectorName, const std::string& strKeyName, int nDeaultValue );

	/**
	@brief 获取字符串
	@param
	*/
	bool		getString( const std::string& strSectorName, const std::string& strKeyName, std::string& strValue );

private:

	FileMap		m_mapIniFile;

};

NS_CC_ENGINE_END

#endif // _WAREINIFILE_H_

