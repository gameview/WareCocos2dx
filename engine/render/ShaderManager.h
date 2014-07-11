//*************************************************************************
//	创建日期:	2013-9-5   17:41
//	文件名称:	ShaderManager.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	Shader管理器
//*************************************************************************
#ifndef _SHADERMANAGER_H__
#define _SHADERMANAGER_H__

#include "cocos2dx_macros.h"
#include "shaders/CCGLProgram.h"
#include <vector>
#include <string>

NS_CC_ENGINE_BEGIN

class CShaderManager
{
private:
	CShaderManager(void);
	~CShaderManager(void);

public:

	static CShaderManager& Instance()
	{
		static CShaderManager _inst;
		return _inst;
	}

	/**
	@brief pszGLProgramName shader文件名
	@param
	*/
	CCGLProgram*	getGLProgram( const char* pszGLProgramName );

	/**
	@brief 
	@param
	*/
	void			reloadAllGLProgram();

private:

	/**
	@brief pszGLProgramName shader文件名
	@param
	*/
	void			loadGLProgram( CCGLProgram* pProgram, const char* pszGLProgramName );


private:

	std::vector<std::string>	m_vecGLPrograme;		/// Shader管理器
};

NS_CC_ENGINE_END

#endif // _SHADERMANAGER_H__

