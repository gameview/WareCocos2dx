//*************************************************************************
//	创建日期:	2014-5-11   14:02
//	文件名称:	EffectProp.h
//	创 建 者:	Rare
//  版本所有:   www.cocos2dres.com
//	说    明:	特效属性
//*************************************************************************
#ifndef _EFFECTPROP_H_
#define _EFFECTPROP_H_

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"
#include <map>
#include <vector>
#include <string>

NS_CC_ENGINE_BEGIN

struct SAdvanceProp;

typedef std::map< float32, cocos2d::CCPoint >	OffsetMap;
typedef std::map< float32, std::string >		NameMap;
typedef std::map< float32, float32 >			SizeMap;
typedef std::map< float32, cocos2d::ccColor4B >	ColorMap;
typedef std::map< float32, float32 >			AlphaMap; /// 混合参数

typedef std::vector< SAdvanceProp >		AdvancePropVec;

/// 高级属性标识
#define		EFFECT_ADVANCE_OFFSET	1
#define		EFFECT_ADVANCE_TEXTURE	2
#define		EFFECT_ADVANCE_SIZE		4
#define		EFFECT_ADVANCE_COLOR	8
#define		EFFECT_ADVANCE_ALPHA	16

class EffectData
{
public:
	enum EPropType
	{
		EFFECT_PROP_TYPE_UNKOWN = 0,  /// 未知类型，视为无效
		EFFECT_PROP_TYPE_INT,
		EFFECT_PROP_TYPE_FLOAT,
		EFFECT_PROP_TYPE_COLOR,
		EFFECT_PROP_TYPE_CVECTOR3,
		EFFECT_PROP_TYPE_CVECTOR2,
		EFFECT_PROP_TYPE_STRING,
		EFFECT_PROP_TYPE_COMMBO,

		EFFECT_PROP_TYPE_COUNT,
	};

	struct CommboData
	{
		const char*	pCommboStr;
		int32			nIndex;
		
		int32		GetIndex()			{ return nIndex; }
		const char*	GetCommboData()		{ return pCommboStr; }	
	};

	EffectData() : eType( EFFECT_PROP_TYPE_UNKOWN ), m_nLength(0), m_pBuff(NULL)
	{
	}

	EffectData( const EffectData& Data ) : m_pBuff(NULL)
	{
		eType = Data.eType;
		_assign( Data.m_pBuff, Data.m_nLength );
	}

	/// *Ptr = *Ptr 这样用法，不调用拷贝构造 而C++默认的赋值操作只是简单的memcpy 
	/// 尚需要验证 stl的一些算法中会出现这样的情况
	EffectData& operator=( const EffectData& Data )
	{
		eType = Data.eType;
		_assign( Data.m_pBuff, Data.m_nLength );
		return *this;
	}

	/// 重载赋值操作符
	inline EffectData& operator = ( const int32 nValue )
	{
		_assign( &nValue, sizeof(int32) );
		eType = EFFECT_PROP_TYPE_INT;
		return *this;
	}

	inline EffectData& operator = ( const float32 fValue )
	{
		_assign( &fValue, sizeof(float32) );
		eType = EFFECT_PROP_TYPE_FLOAT;
		return *this;
	}

	inline EffectData& operator = ( const ccColor4B& cValue )
	{
		_assign( &cValue, sizeof(ccColor4B) );
		eType = EFFECT_PROP_TYPE_COLOR;
		return *this;
	}

	inline EffectData& operator = ( const CCPoint& vValue )
	{
		_assign( &vValue, sizeof(CCPoint) );
		eType = EFFECT_PROP_TYPE_CVECTOR2;
		return *this;
	}

	inline EffectData& operator = ( const char* pValue )
	{
		_assign( pValue, strlen( pValue ) + 1 );
		m_pBuff[m_nLength - 1 ] = 0; /// 添加结尾符
		eType = EFFECT_PROP_TYPE_STRING;
		return *this;
	}

	inline EffectData& operator = ( const std::string& strValue )
	{
		_assign( strValue.c_str(), strValue.length() + 1 );
		m_pBuff[m_nLength - 1 ] = 0; /// 添加结尾符
		eType = EFFECT_PROP_TYPE_STRING;
		return *this;
	}

	inline EffectData& operator = ( const CommboData& Commbo )
	{
		SAFE_DELARR( m_pBuff );

		int32 nIndexLen = sizeof(int32);
		int32 nStrLen = strlen( Commbo.pCommboStr );
		m_nLength = nIndexLen + nStrLen;
		m_pBuff = new BYTE[m_nLength];
		BYTE* pBuff = m_pBuff;
		memcpy( pBuff, &Commbo.nIndex, nIndexLen );
		pBuff += nIndexLen;
		memcpy( pBuff, Commbo.pCommboStr, nStrLen );
		m_pBuff[m_nLength] = 0;

		eType = EFFECT_PROP_TYPE_COMMBO;

		return *this;
	}

	/// 取值
	inline int32 Int()
	{
		//WARE_ASSERT( eType == EFFECT_PROP_TYPE_INT, "属性类型不匹配。" );
		return *(int32*)(m_pBuff);
	}

	inline const char* CStr()
	{
		//WARE_ASSERT( eType == EFFECT_PROP_TYPE_STRING, "属性类型不匹配。" );
		return (char*)m_pBuff;	
	}

	inline const cocos2d::ccColor4B Color()
	{
		//WARE_ASSERT( eType == EFFECT_PROP_TYPE_COLOR, "属性类型不匹配。" );
		return *(cocos2d::ccColor4B*)m_pBuff;	
	}

	inline void	String( std::string& str )
	{
		//WARE_ASSERT( eType == EFFECT_PROP_TYPE_STRING, "属性类型不匹配。" );
		str = (char*)m_pBuff;
	}

	inline void Vector2( cocos2d::CCPoint& v )
	{
		//WARE_ASSERT( eType == EFFECT_PROP_TYPE_CVECTOR2, "属性类型不匹配。" );
		v = *(cocos2d::CCPoint*)(m_pBuff);
	}

	inline float32 Float()
	{
		//WARE_ASSERT( eType == EFFECT_PROP_TYPE_FLOAT, "属性类型不匹配。" );
		return *(float32*)(m_pBuff);
	}

	inline void Commbo( CommboData& Commbo )
	{
		//WARE_ASSERT( eType == EFFECT_PROP_TYPE_COMMBO, "属性类型不匹配。" );
		Commbo.nIndex = *(int32*)(m_pBuff);
		Commbo.pCommboStr = (char*)(m_pBuff+sizeof(int32));
	}

	~EffectData()
	{
		SAFE_DELARR( m_pBuff );
	}

	EPropType		GetType()						{ return eType; }
protected:

	void			_assign( const void* pData, int32 nLength )
	{
		SAFE_DELARR( m_pBuff );
		m_pBuff = new BYTE[nLength];
		memcpy( m_pBuff, pData, nLength );
		m_nLength = nLength;
	}

private:
	EPropType		eType;
	BYTE*			m_pBuff;
	int32			m_nLength;
};

class EffectProp
{
public:
	EffectProp( const EffectProp& Prop )
	{
		m_EffectData = Prop.m_EffectData;
		m_bFile = Prop.m_bFile;
		m_strName = Prop.m_strName;
	}
	
	///
	EffectProp( char* pName, const int32 nValue )
		: m_bFile(false)
	{
		m_EffectData = nValue;
		m_strName = pName;
	}

	EffectProp( char* pName, const float32 fValue )
		: m_bFile(false)
	{
		m_EffectData = fValue;
		m_strName = pName;
	}

	EffectProp( char* pName, const CCPoint& vValue )
		: m_bFile(false)
	{
		m_EffectData = vValue;
		m_strName = pName;
	}

	EffectProp( char* pName, const ccColor4B& cValue )
		: m_bFile(false)
	{
		m_EffectData = cValue;
		m_strName = pName;
	}

	EffectProp( char* pName, const std::string& strValue, bool bFile = false )
		: m_bFile(bFile)
	{
		m_EffectData = strValue;
		m_strName = pName;
	}

	EffectProp( char* pName, const char* pValue, bool bFile = false )
		: m_bFile( bFile )
	{
		m_EffectData = pValue;
		m_strName = pName;
	}

	EffectProp( char* pName, const char* pValue, int32 nIndex )
		: m_bFile(false)
	{
		EffectData::CommboData Commbo;
		Commbo.nIndex = nIndex;
		Commbo.pCommboStr = pValue;
		m_EffectData = Commbo;
		m_strName = pName;
	}

	~EffectProp()
	{
	}

	EffectData			GetData() const					{ return m_EffectData; }
	EffectData&			GetData()						{ return m_EffectData; }
	const std::string&	Name() const					{ return m_strName; }
	bool				IsFile()						{ return m_bFile; }	

private:
	
	EffectData		m_EffectData;
	bool			m_bFile;
	std::string		m_strName;
};

/// 高级属性
struct SAdvanceProp
{
	float32		fPercent;
	EffectData	Prop;

	SAdvanceProp() : fPercent(0.0f) {}

	SAdvanceProp( const SAdvanceProp& v ) : fPercent(v.fPercent),Prop(v.Prop) {}

	SAdvanceProp& operator =( const SAdvanceProp& v )
	{
		fPercent = v.fPercent;
		Prop = v.Prop;
		return *this;
	}
};

NS_CC_ENGINE_END

#endif // _EFFECTPROP_H_