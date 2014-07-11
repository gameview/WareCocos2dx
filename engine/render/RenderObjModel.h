//*************************************************************************
//	创建日期:	2013-8-19   10:59
//	文件名称:	RenderObjModel.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	渲染对象模板
//*************************************************************************
#ifndef _RENDEROBJMODEL_H__
#define _RENDEROBJMODEL_H__

#include "cocos2d.h"
#include "cocos2dx_macros.h"
#include "cocos2dx_types.h"
#include <string>
#include <map>
#include "tinyxml.h"

NS_CC_ENGINE_BEGIN

struct IRenderable;

class CRenderObjModel : public CCObject
{
public:
	CRenderObjModel(void);
	~CRenderObjModel(void);

	/// entity list
	struct ObjPart
	{
		std::string		strName;	/// 部件名
		std::string		strValue;	/// 部件值
		int				nDepth;		/// 部件深度

		ObjPart()
		{
			strName.clear();
			strValue.clear();
			nDepth = 0;
		}
	};

	/// entity list
	struct ObjLocator
	{
		std::string		strName;
		CCPoint			ptOffset;

		ObjLocator()
		{
		}
	};

	/// entity list
	struct ObjEffect
	{
		std::string		strName;
		std::string		strFileName;
		std::string		strLocator;
		CCPoint			ptOffset;
		CCPoint			scale;
		ccColor4B		color;
		int				depth;

		ObjEffect()
		{
			strName.clear();
			strFileName.clear();
			strLocator.clear();
			ptOffset = CCPointZero;
			scale = ccp(1.0,1.0);
			color = ccc4(255,255,255,255);
			depth = 0;
		}
	};

	typedef std::map< std::string, ObjPart >	ObjPartMap;
	typedef std::map< std::string, ObjLocator > ObjLocatorMap;
	typedef std::map< std::string, ObjEffect >	ObjEffectMap;

	std::string		m_strObjName;
	int				m_nVersion;
	CCPoint			m_ptOffset;

	/// 数据定义
	ObjPartMap		m_mapPart;			/// 对象部件
	ObjLocatorMap	m_mapLocator;		/// 绑定点
	ObjEffectMap	m_mapEffect;		/// 特效数据

	/**
	@brief 添加part属性
	@param strName 部件名称
	@param strFile 部件对象名
	@param nDepth  部件深度
	*/
	void			addPart( const std::string& strName, const std::string& strFile, int nDepth );

	/**
	@brief 删除节点
	@param
	*/
	void			removePart( const std::string& strName );

	/**
	@brief 添加节点
	@param
	*/
	void			addLocator( const std::string& strLocatorName, CCPoint& ptOffset );

	/**
	@brief 删除节点
	@param
	*/
	void			removeLocator( const std::string& strLocatorName );

	/**
	@brief 添加节点
	@param
	*/
	void			addEffect( const std::string& strEffectName, const std::string& strLocatorName, const std::string& strEffectFileName, CCPoint& ptOffset, ccColor4B& color, CCPoint& scale, int nDepth );

	/**
	@brief 删除节点
	@param
	*/
	void			removeEffect( const std::string& strEffectName );

	/**
	@brief 解析obj文件
	@param
	*/
	bool			parseObjFile( const char* szObjFileName );

	/**
	@brief 保存到文件
	@param
	*/
	void			saveToFile( const std::string& strObjFileName );

private:

	/**
	@brief 保存到文件
	@param
	*/
	void			saveToFileV10001( const std::string& strObjFileName );

	/**
	@brief 保存到文件
	@param
	*/
	void			saveToFileBinary( const std::string& strObjFileName );

	/**
	@brief 解析obj文件(二进制)
	@param
	*/
	bool			parseObjFileBinary( const std::string& strObjFileName );

	/**
	@brief 
	@param 解析单个文件
	*/
	bool			parseSingleObjFile( TiXmlNode* rootElement, const std::string& strObjName, const std::string& strObjCatName );
};

NS_CC_ENGINE_END

#endif // _RENDEROBJMODEL_H__