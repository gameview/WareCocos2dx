#include "RenderObjModel.h"
#include "StringConvert.h"
#include "StringUtility.h"
#include "WareFileWrite.h"
#include "WareFileRead.h"

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((uint32)(BYTE)(ch0) | ((uint32)(BYTE)(ch1) << 8) |   \
	((uint32)(BYTE)(ch2) << 16) | ((uint32)(BYTE)(ch3) << 24 ))
#endif

#define OBJ_FILE_VERSION  20002

NS_CC_ENGINE_BEGIN

CRenderObjModel::CRenderObjModel(void)
{
	m_strObjName = "";
	m_nVersion = 0;
	m_ptOffset = CCPointZero;


	m_mapPart.clear();			/// 对象部件
	m_mapLocator.clear();		/// 绑定点
	m_mapEffect.clear();		/// 特效数据
}


CRenderObjModel::~CRenderObjModel(void)
{
}

//------------------------------------------------------------------------------
/**
@brief 添加part属性
@param strName 部件名称
@param strFile 部件对象名
@param nDepth  部件深度
*/
void CRenderObjModel::addPart( const std::string& strName, const std::string& strFile , int nDepth )
{
	ObjPart part;
	part.strName = strName;
	part.strValue = strFile;
	part.nDepth = nDepth;
	m_mapPart[strName] = part;
}
//------------------------------------------------------------------------------
/**
@brief 删除节点
@param
*/
void CRenderObjModel::removePart( const std::string& strName )
{
	/// 删除部件
	//ObjPartMap::iterator iter = m_mapPart.find( strName );
	//if( iter != m_mapPart.end() )
	//{
	//	if( iter->second.pRenderable )
	//	{
	//		//removeChild( iter->second.pRenderable );
	//	}
	//}

	m_mapPart.erase( strName );
}
//------------------------------------------------------------------------------
/**
@brief 添加节点
@param
*/
void CRenderObjModel::addLocator( const std::string& strLocatorName, CCPoint& ptOffset )
{
	ObjLocator locator;
	locator.strName = strLocatorName;
	locator.ptOffset = ptOffset;
	m_mapLocator[strLocatorName] = locator;
}
//------------------------------------------------------------------------------
/**
@brief 删除节点(适用于编辑器)
@param
*/
void CRenderObjModel::removeLocator( const std::string& strLocatorName )
{
	//ObjLocatorMap::iterator iter = m_mapLocator.find( strLocatorName );
	//if( iter != m_mapPart.end() )
	//{
	//	//if( iter->second.pRenderable )
	//	//{
	//	//	removeChild( iter->second.pRenderable );
	//	//}
	//}

	m_mapLocator.erase( strLocatorName );
}
//------------------------------------------------------------------------------
/**
@brief 添加节点
@param
*/
void CRenderObjModel::addEffect( const std::string& strEffectName, const std::string& strLocatorName, const std::string& strEffectFileName, CCPoint& ptOffset, ccColor4B& color, CCPoint& scale,int nDepth )
{
	ObjEffect effect;
	effect.strName = strEffectName;
	effect.ptOffset = ptOffset;
	effect.strLocator = strLocatorName;
	effect.strFileName = strEffectFileName;
	effect.color = color;
	effect.scale = scale;
	effect.depth = nDepth;
	m_mapEffect[strEffectName] = effect;
}
//------------------------------------------------------------------------------
/**
@brief 删除节点
@param
*/
void CRenderObjModel::removeEffect( const std::string& strEffectName )
{
	//ObjEffectMap::iterator iter = m_mapEffect.find( strEffectName );
	//if( iter != m_mapEffect.end() )
	//{
	//	if( iter->second.pRenderable )
	//	{
	//		//removeChild( iter->second.pRenderable );
	//	}
	//}

	m_mapEffect.erase( strEffectName );
}
//------------------------------------------------------------------------------
/**
@brief 解析obj文件
@param
*/
bool CRenderObjModel::parseObjFile( const char* szObjFileName )
{
	if( szObjFileName == NULL || szObjFileName[0] == 0 )
	{
		return false;
	}

	unsigned long lSize = 0;
	unsigned char * pXmlBuff = CCFileUtils::sharedFileUtils()->getFileData( szObjFileName, "r", &lSize );
	if( pXmlBuff == NULL )
	{
		return false;
	}

	TiXmlDocument xmldoc; 
	TiXmlElement *pRootElement = NULL;

	xmldoc.Parse((char*)pXmlBuff,NULL,TIXML_ENCODING_UTF8);
	if(xmldoc.Error())
	{
		//CCLOG( "parse obj file failed: %s", xmldoc.ErrorDesc() );
		m_strObjName = szObjFileName;
		return parseObjFileBinary( szObjFileName );
	}

	m_strObjName = szObjFileName;

	// 获取xml文件的root element，取得它的value，用于判断当前解析的是合并后的文件还是合并前的文件
	pRootElement = xmldoc.RootElement();

	if (pRootElement != NULL)
	{
		// 如果是合并后的文件，就逐个解析文件中的object
		//	for( TiXmlNode* child = pRootElement->FirstChild(); child; child = child->NextSibling() )
		//	{
		//		parseSingleObjFile(child, "", szObjFileName );
		//	}
		//}
		//else
		//{
		parseSingleObjFile(pRootElement, szObjFileName, szObjFileName );
	}

	delete [] pXmlBuff;
	pXmlBuff = NULL;

	return true;
}
//------------------------------------------------------------------------------
/**
@brief 
@param 解析单个文件
*/
bool CRenderObjModel::parseSingleObjFile( TiXmlNode* rootElement, const std::string& strObjName, const std::string& strObjCatName )
{
	const char* pTempValue = NULL;

	const TiXmlElement *pObjectElement = rootElement->ToElement();

	if (!pObjectElement)
	{
		CCLOG( "Object Element is NULL : parseSingleObjFile %s ", strObjName.c_str() );
		return false;
	}

	const TiXmlElement *pAttribElement = NULL;
	const TiXmlElement *pEntityListElement = NULL;
	const TiXmlElement *pMaterialListElement = NULL;
	const TiXmlElement *pEffectElement = NULL;
	const TiXmlElement *pShadowUncastableElement = NULL;
	const TiXmlElement *pLocatorsElement = NULL;
	const TiXmlElement *pAnimationSkeletonElement = NULL;
	const TiXmlElement *pFabricColliderElement = NULL;

	// 如果object为空字符串，说明这个obj的定义是在合并后的文件中
	std::string realObjectName, strOffset,strVersion;
	//int nVersion = 0;

	if (strObjName.empty())
	{
		realObjectName = pObjectElement->Attribute("name");
	}
	else
	{
		realObjectName = strObjName;
	}

	const char* szOffset = pObjectElement->Attribute( "anchor" );
	if( szOffset != NULL )
	{
		strOffset = szOffset;
		m_ptOffset = StringConvert::parseDotPoint( strOffset );
	}

	const char* szVersion = pObjectElement->Attribute( "version" );
	if( szVersion != NULL )
	{
		strVersion = szVersion;
		m_nVersion = StringConvert::parseInt( strVersion );
	}

	//load object mesh list
	pEntityListElement = pObjectElement->FirstChildElement("PartList");
	if(pEntityListElement)
	{
		const TiXmlElement *pEntity = NULL;

		pEntity = pEntityListElement->FirstChildElement("Part");

		while(pEntity)
		{
			std::string name, fileName;

			pTempValue = pEntity->Attribute("name");

			if(pTempValue) 
			{
				name = pTempValue;
			}
			else 
			{
				name = "";
			}

			pTempValue = pEntity->Attribute("file");

			if(pTempValue) 
			{
				fileName = pTempValue;
			}
			else 
			{
				fileName = "";
			}

			//StringUtility::makeLower( fileName );

			addPart(name, fileName,0);

			pEntity = pEntity->NextSiblingElement("Part");
		}
	}

	//load locator info list
	pLocatorsElement = pObjectElement->FirstChildElement("Locators");

	if(pLocatorsElement)
	{
		const TiXmlElement *pLocator = NULL;

		pLocator = pLocatorsElement->FirstChildElement("Locator");

		while(pLocator)
		{
			std::string LocatorName;
			CCPoint ptOffset;

			ObjLocator locator;

			bool transferable = false;
			bool translateFirst = false;

			pTempValue = pLocator->Attribute("name");

			if(pTempValue) 
			{
				locator.strName = pTempValue;
			}
			else
			{
				pLocator = pLocator->NextSiblingElement("Locator");
				continue;
			}

			if( m_nVersion == 0 )
			{
				locator.ptOffset.x = StringConvert::parseFloat( pLocator->Attribute("x") );
				locator.ptOffset.y = StringConvert::parseFloat( pLocator->Attribute("y") );
			}
			else
			{
				locator.ptOffset = StringConvert::parseDotPoint( pLocator->Attribute("offset") );
			}

			m_mapLocator[locator.strName] = locator;

			pLocator = pLocator->NextSiblingElement("Locator");
		}

	}

	//load object effect list
	pEffectElement = pObjectElement->FirstChildElement("Effects");
	if(pEffectElement)
	{
		const TiXmlElement *pEffect = NULL;

		pEffect = pEffectElement->FirstChildElement("Effect");

		while(pEffect)
		{
			ObjEffect effect;

			pTempValue = pEffect->Attribute("name");

			if(pTempValue) 
			{
				effect.strName = pTempValue;
			}
			else 
			{
				effect.strName = "";
			}

			pTempValue = pEffect->Attribute("effect");

			if(pTempValue) 
			{
				effect.strFileName = pTempValue;
			}
			else 
			{
				effect.strFileName = "";
			}

			pTempValue = pEffect->Attribute("locator");

			if(pTempValue) 
			{
				effect.strLocator = pTempValue;
			}
			else 
			{
				effect.strLocator = "";
			}

			pTempValue = pEffect->Attribute("color");

			if (pTempValue)
			{
				effect.color = StringConvert::parseColor(pTempValue);
			}

			pTempValue = pEffect->Attribute( "offset" );
			if( pTempValue )
			{
				effect.ptOffset = StringConvert::parseDotPoint( pTempValue );
			}

			pTempValue = pEffect->Attribute( "scale" );
			if( pTempValue )
			{
				effect.scale = StringConvert::parseDotPoint( pTempValue );
			}

			pTempValue = pEffect->Attribute( "depth" );
			if( pTempValue )
			{
				effect.depth = StringConvert::parseInt( pTempValue );
			}
			else
			{
				effect.depth = 0;
			}

			m_mapEffect[effect.strName] = effect;

			pEffect = pEffect->NextSiblingElement("Effect");
		}
	}

	return true;
}
//-------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string NumTabs(int count)
{
	std::string tmp = "";
	for(int i=0;i<count;i++)
	{
		tmp+="\t";
	}

	return tmp;

}
//------------------------------------------------------------------------------
/**
@brief 保存到文件
@param
*/
void CRenderObjModel::saveToFile( const std::string& strObjFileName )
{
	m_nVersion = 20001;
	if( m_nVersion == 10001 )
	{
		saveToFileV10001( strObjFileName );
	}
	else
	{
		saveToFileBinary( strObjFileName );
	}
}
//-------------------------------------------------------------------------
/**
@brief 解析obj文件(二进制)
@param
*/
bool CRenderObjModel::parseObjFileBinary( const std::string& strObjFileName )
{
	m_strObjName = strObjFileName;

	//m_nVersion = 20001;
	std::string strObjName = strObjFileName;
	if( strObjName.empty() )
	{
		strObjName = m_strObjName;
	}

	std::string strPath, strFileName, strExt;
	cocos2d::engine::StringUtility::parseFileName( strObjName, strPath, strFileName, strExt );

	std::string strObj = strFileName;
	strObj.append( "." );
	strObj.append( strExt );

	unsigned long lSize = 0;
	unsigned char* pBuff = CCFileUtils::sharedFileUtils()->getFileData( strObjFileName.c_str(), "rb", &lSize );
	cocos2d::engine::CWareFileRead File;
	if( !File.open( pBuff, lSize ) )
	{
		CCLOG( "%s 不存在!", strObjFileName.c_str() );
		return false;
	}

	int nFlag = 0;
	File.read( nFlag );
	if( nFlag != MAKEFOURCC( 'O', 'B','J', 0 ) )
	{
		CCLOG( "%s obj文件格式不对!", strObjFileName.c_str() );
		return false;
	}

	File.read( m_nVersion );

	File.read( m_ptOffset.x );
	File.read( m_ptOffset.y );

	size_t size = 0;//m_mapPart.size();
	File.read( size );
	//ObjPartMap::iterator itPart = m_mapPart.begin();
	m_mapPart.clear();
	for( size_t i = 0; i < size; ++i )
	{
		ObjPart sPart;
		File.readStr( sPart.strName );
		File.readStr( sPart.strValue );
		if( m_nVersion > 20001 )
		{
			File.read( sPart.nDepth );
		}
		m_mapPart[sPart.strName] = sPart;
	}
	
	size = 0;
	File.read( size );
	//ObjLocatorMap::iterator itLocator = m_mapLocator.begin();		/// 特效数据
	for( size_t i = 0; i < size; ++i )
	{
		ObjLocator sObjLocator;
		File.readStr( sObjLocator.strName );
		File.read( sObjLocator.ptOffset.x );
		File.read( sObjLocator.ptOffset.y );
		m_mapLocator[sObjLocator.strName] = sObjLocator;
	}

	size = 0;
	File.read( size );
	//ObjEffectMap::iterator itEffect = m_mapEffect.begin();		/// 特效数据
	for( size_t i = 0; i < size; ++i )
	{
		ObjEffect sEffect;
		File.readStr( sEffect.strName );
		File.readStr( sEffect.strFileName );
		File.readStr( sEffect.strLocator );
		File.read( sEffect.ptOffset.x );
		File.read( sEffect.ptOffset.y );
		File.read( sEffect.scale.x );
		File.read( sEffect.scale.y );
		File.read( sEffect.depth );
		File.read( sEffect.color.r );
		File.read( sEffect.color.g );
		File.read( sEffect.color.b );
		File.read( sEffect.color.a );

		m_mapEffect[sEffect.strName] = sEffect;
	}

	File.close();

	delete [] pBuff;
	return true;
}
//-------------------------------------------------------------------------
/**
@brief 保存到文件
@param
*/
void CRenderObjModel::saveToFileBinary( const std::string& strObjFileName )
{
	int nVersion = OBJ_FILE_VERSION;
	std::string strObjName = strObjFileName;
	if( strObjName.empty() )
	{
		strObjName = m_strObjName;
	}

	std::string strPath, strFileName, strExt;
	cocos2d::engine::StringUtility::parseFileName( strObjName, strPath, strFileName, strExt );

	std::string strObj = strFileName;
	strObj.append( "." );
	strObj.append( strExt );

	cocos2d::engine::CWareFileWrite File;
	if( !File.open( strObjName.c_str() ) )
	{
		return;
	}

	int nFlag = MAKEFOURCC( 'O', 'B','J', 0 );
	File.write( nFlag );

	File.write( nVersion );

	File.write( m_ptOffset.x );
	File.write( m_ptOffset.y );

	size_t size = m_mapPart.size();
	File.write( size );
	ObjPartMap::iterator itPart = m_mapPart.begin();
	for( ; itPart != m_mapPart.end(); ++itPart )
	{
		File.writeStr( itPart->second.strName );
		File.writeStr( itPart->second.strValue );
		if( nVersion > 20001 )
		{
			File.write( itPart->second.nDepth );
		}
	}
	
	size = m_mapLocator.size();
	File.write( size );
	ObjLocatorMap::iterator itLocator = m_mapLocator.begin();		/// 特效数据
	for( ; itLocator != m_mapLocator.end(); ++itLocator )
	{
		File.writeStr( itLocator->second.strName );
		File.write( itLocator->second.ptOffset.x );
		File.write( itLocator->second.ptOffset.y );
	}

	size = m_mapEffect.size();
	File.write( size );
	ObjEffectMap::iterator itEffect = m_mapEffect.begin();		/// 特效数据
	for( ; itEffect != m_mapEffect.end(); ++itEffect )
	{
		File.writeStr( itEffect->second.strName );
		File.writeStr( itEffect->second.strFileName );
		File.writeStr( itEffect->second.strLocator );
		File.write( itEffect->second.ptOffset.x );
		File.write( itEffect->second.ptOffset.y );
		File.write( itEffect->second.scale.x );
		File.write( itEffect->second.scale.y );
		File.write( itEffect->second.depth );
		File.write( itEffect->second.color.r );
		File.write( itEffect->second.color.g );
		File.write( itEffect->second.color.b );
		File.write( itEffect->second.color.a );
	}

	File.close();
}
//-------------------------------------------------------------------------
/**
@brief 保存到文件
@param
*/
void CRenderObjModel::saveToFileV10001( const std::string& strObjFileName )
{
	int nVersion = 10001;
	std::string strObjName = strObjFileName;
	if( strObjName.empty() )
	{
		strObjName = m_strObjName;
	}

	std::string strPath, strFileName, strExt;
	cocos2d::engine::StringUtility::parseFileName( strObjName, strPath, strFileName, strExt );

	std::string strObj = strFileName;
	strObj.append( "." );
	strObj.append( strExt );

	FILE* fp = fopen( strObjName.c_str(), "w" );
	if(!fp)
	{
		return;
	}

	char c = 0xef;
	fwrite( &c, 1,1, fp );
	c = 0xbb;
	fwrite( &c, 1,1, fp );
	c = 0xbf;
	fwrite( &c, 1,1, fp );

	fprintf( fp,"<?xml version=\"1.0\" encoding=\"utf-8\" ?>\r\n" );

	fprintf(fp, "%s<Object name=\"%s\" anchor=\"%f,%f\" version=\"%d\">\r\n", NumTabs(0).c_str(), strObj.c_str(), m_ptOffset.x, m_ptOffset.y, nVersion );

	fprintf(fp, "%s<PartList>\n", NumTabs(1).c_str(), strObj.c_str() );
	ObjPartMap::iterator itPart = m_mapPart.begin();
	for( ; itPart != m_mapPart.end(); ++itPart )
	{
		fprintf(fp, "%s<Part name=\"%s\" file=\"%s\" />\r\n", NumTabs(2).c_str(), itPart->second.strName.c_str(), itPart->second.strValue.c_str() );
	}
	fprintf(fp, "%s</PartList>\r\n", NumTabs(1).c_str(), strObj.c_str() );


	fprintf(fp, "%s<Locators>\r\n", NumTabs(1).c_str(), strObj.c_str() );
	ObjLocatorMap::iterator itLocator = m_mapLocator.begin();		/// 特效数据
	for( ; itLocator != m_mapLocator.end(); ++itLocator )
	{
		fprintf(fp, "%s<Locator name=\"%s\" offset=\"%f,%f\" />\r\n", NumTabs(2).c_str(),
			itLocator->second.strName.c_str(), itLocator->second.ptOffset.x, itLocator->second.ptOffset.y );
	}
	fprintf(fp, "%s</Locators>\r\n", NumTabs(1).c_str(), strObj.c_str() );

	fprintf(fp, "%s<Effects>\r\n", NumTabs(1).c_str(), strObj.c_str() );
	ObjEffectMap::iterator itEffect = m_mapEffect.begin();		/// 特效数据
	for( ; itEffect != m_mapEffect.end(); ++itEffect )
	{
		fprintf(fp, "%s<Effect name =\"%s\" effect=\"%s\" locator =\"%s\" offset=\"%f,%f\" scale=\"%f,%f\" depth=\"%d\" color =\"%d,%d,%d,%d\" />\r\n", NumTabs(2).c_str(),
			itEffect->second.strName.c_str(), itEffect->second.strFileName.c_str(), itEffect->second.strLocator.c_str(), itEffect->second.ptOffset.x,
			itEffect->second.ptOffset.y, itEffect->second.scale.x, itEffect->second.scale.y, itEffect->second.depth,
			itEffect->second.color.r, itEffect->second.color.g, itEffect->second.color.b, itEffect->second.color.a );
	}
	fprintf(fp, "%s</Effects>\r\n", NumTabs(1).c_str(), strObj.c_str() );

	fprintf(fp, "%s</Object>\r\n", NumTabs(0).c_str(), strObj.c_str() );

	fclose( fp );
}

NS_CC_ENGINE_END
