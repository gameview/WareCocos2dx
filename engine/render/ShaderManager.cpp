#include "ShaderManager.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccShaders.h"
#include "CCFileUtils.h"
NS_CC_ENGINE_BEGIN

CShaderManager::CShaderManager(void)
{
}


CShaderManager::~CShaderManager(void)
{
}

/**
@brief pszGLProgramName shader文件名
@param
*/
CCGLProgram* CShaderManager::getGLProgram( const char* pszGLProgramName )
{
	if( pszGLProgramName == NULL )
	{
		return NULL;
	}

	CCGLProgram* pProgram = CCShaderCache::sharedShaderCache()->programForKey( pszGLProgramName );
	if( pProgram )
	{
		return pProgram;
	}

	pProgram = new CCGLProgram();
	if( pProgram == NULL )
	{
		return NULL;
	}

	loadGLProgram( pProgram, pszGLProgramName );
	CCShaderCache::sharedShaderCache()->addProgram( pProgram, pszGLProgramName );
	m_vecGLPrograme.push_back( pszGLProgramName );

	pProgram->release();
	CHECK_GL_ERROR_DEBUG();

	return pProgram;
}
//-------------------------------------------------------------------------
/**
@brief pszGLProgramName shader文件名
@param
*/
void CShaderManager::loadGLProgram( CCGLProgram* pProgram, const char* pszGLProgramName )
{
	if( pProgram == NULL )
	{
		return;
	}

	char szBuff[256];
	sprintf( szBuff, "%s.v", pszGLProgramName );
	unsigned long lSize = 0;
	GLchar* pszVertSource = (GLchar*)CCFileUtils::sharedFileUtils()->getFileData( szBuff, "rt", &lSize );
	const GLchar* pszVert = NULL;
	if( pszVertSource == NULL )
	{
		pszVert = ccPositionTextureColor_vert;
	}
	else
	{
		pszVertSource[lSize] = 0;
		pszVert = pszVertSource;
	}

	sprintf( szBuff, "%s.f", pszGLProgramName );
	lSize = 0;
	GLchar* pszfragSource = (GLchar*)CCFileUtils::sharedFileUtils()->getFileData( szBuff, "rt", &lSize );
	const GLchar* pszFrag = NULL;
	if( pszfragSource == NULL )
	{
		pszFrag = ccPositionTextureColor_frag;
	}
	else
	{
		pszfragSource[lSize] = 0;
		pszFrag = pszfragSource;
	}

	if( !pProgram->initWithVertexShaderByteArray(pszVert, pszFrag) )
	{
		CC_SAFE_DELETE_ARRAY(pszVertSource);
		CC_SAFE_DELETE_ARRAY(pszfragSource);
		CCLOG( "CShaderManager::getGLProgram failed!" );
		return;
	}

	pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	CHECK_GL_ERROR_DEBUG();

	pProgram->link();
	CHECK_GL_ERROR_DEBUG();

	pProgram->updateUniforms();
	CHECK_GL_ERROR_DEBUG();
	CC_SAFE_DELETE_ARRAY(pszVertSource);
	CC_SAFE_DELETE_ARRAY(pszfragSource);
}
//-------------------------------------------------------------------------
/**
@brief 
@param
*/
void CShaderManager::reloadAllGLProgram()
{
	CCShaderCache::sharedShaderCache()->reloadDefaultShaders();

	size_t size = m_vecGLPrograme.size();
	for( size_t i = 0; i < size; ++i )
	{
		CCGLProgram * pProgram = CCShaderCache::sharedShaderCache()->programForKey(m_vecGLPrograme[i].c_str());
		if( pProgram )
		{
			pProgram->reset();
			loadGLProgram( pProgram, m_vecGLPrograme[i].c_str() );
		}
	}
}

NS_CC_ENGINE_END
