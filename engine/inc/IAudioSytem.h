//*************************************************************************
//	创建日期:	2013-12-20   11:56
//	文件名称:	IAudioSytem.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	音频接口
//*************************************************************************
#ifndef _IAUDIOSYTEM_H__
#define _IAUDIOSYTEM_H__

#include "cocos2d.h"
#include "cocos2dx_types.h"
#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

/// 音频接口
struct IAudioSystem
{
	/// 音频接口
	//////////////////////////////////////////////////////////////////////////
	virtual void		playBackgroundMusic(const char* pszFilePath, bool bLoop) = 0;

    /**
    @brief Stop playing background music
    @param bReleaseData If release the background music data or not.As default value is false
    */
    virtual void		stopBackgroundMusic(bool bReleaseData) = 0;

	    /**
    @brief The volume of the background music max value is 1.0,the min value is 0.0
    */
   virtual float		getBackgroundMusicVolume() = 0;

    /**
    @brief set the volume of background music
    @param volume must be in 0.0~1.0
    */
    virtual void		setBackgroundMusicVolume(float volume) = 0;

	   /**
    @brief The volume of the effects max value is 1.0,the min value is 0.0
    */
    virtual float		getEffectsVolume() = 0;

    /**
    @brief set the volume of sound effecs
    @param volume must be in 0.0~1.0
    */
    virtual void		setEffectsVolume(float volume) = 0;

    // for sound effects
    /**
    @brief Play sound effect
    @param pszFilePath The path of the effect file,or the FileName of T_SoundResInfo
    @bLoop Whether to loop the effect playing, default value is false
    */
    virtual unsigned int playEffect(const char* pszFilePath, bool bLoop) = 0;

    /**
    @brief Resume playing sound effect
    @param nSoundId The return value of function playEffect
    */
    virtual void		resumeEffect(unsigned int nSoundId) = 0;

    /**
    @brief Resume all playing sound effect
    @param nSoundId The return value of function playEffect
    */
    virtual void		resumeAllEffects() = 0;

    /**
    @brief Stop playing sound effect
    @param nSoundId The return value of function playEffect
    */
    virtual void		stopEffect(unsigned int nSoundId) = 0;

    /**
    @brief Stop all playing sound effects
    */
    virtual void		stopAllEffects() = 0;

	/**
	@brief 系统静音
	@param
	*/
	virtual void		muteAllSounds( bool bMute ) = 0;
};

/**
*@brief 获取音频系统
*/
IAudioSystem* getAudioSystem();

NS_CC_ENGINE_END

#endif // _IAUDIOSYTEM_H__
