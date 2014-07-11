//*************************************************************************
//	创建日期:	2013-12-20   12:06
//	文件名称:	AudioSystem.h
//  创 建 人:   Rare	
//	版权所有:	www.cocos2dres.com
//	说    明:	音频系统实现
//*************************************************************************
#ifndef _AUDIOSYSTEM_H__
#define _AUDIOSYSTEM_H__

#include "IAudioSytem.h"

NS_CC_ENGINE_BEGIN

class CAudioSystem : public IAudioSystem
{
public:
	CAudioSystem(void);
	~CAudioSystem(void);

	/// 音频接口
	//////////////////////////////////////////////////////////////////////////
	virtual void		playBackgroundMusic(const char* pszFilePath, bool bLoop);

	/**
	@brief Stop playing background music
	@param bReleaseData If release the background music data or not.As default value is false
	*/
	virtual void		stopBackgroundMusic(bool bReleaseData);

	/**
	@brief The volume of the background music max value is 1.0,the min value is 0.0
	*/
	virtual float		getBackgroundMusicVolume();

	/**
	@brief set the volume of background music
	@param volume must be in 0.0~1.0
	*/
	virtual void		setBackgroundMusicVolume(float volume);

	/**
	@brief The volume of the effects max value is 1.0,the min value is 0.0
	*/
	virtual float		getEffectsVolume();

	/**
	@brief set the volume of sound effecs
	@param volume must be in 0.0~1.0
	*/
	virtual void		setEffectsVolume(float volume);

	// for sound effects
	/**
	@brief Play sound effect
	@param pszFilePath The path of the effect file,or the FileName of T_SoundResInfo
	@bLoop Whether to loop the effect playing, default value is false
	*/
	virtual unsigned int playEffect(const char* pszFilePath, bool bLoop);

	/**
	@brief Resume playing sound effect
	@param nSoundId The return value of function playEffect
	*/
	virtual void		resumeEffect(unsigned int nSoundId);

	/**
	@brief Resume all playing sound effect
	@param nSoundId The return value of function playEffect
	*/
	virtual void		resumeAllEffects();

	/**
	@brief Stop playing sound effect
	@param nSoundId The return value of function playEffect
	*/
	virtual void		stopEffect(unsigned int nSoundId);

	/**
	@brief Stop all playing sound effects
	*/
	virtual void		stopAllEffects();

	/**
	@brief 系统静音
	@param
	*/
	virtual void		muteAllSounds( bool bMute );

private:

	float				m_fEffectVolume;			/// 特效音量
	float				m_fBackgroundMusicVolume;	/// 背景音乐音量

};

extern CAudioSystem g_AudioSystem;

NS_CC_ENGINE_END

#endif // _AUDIOSYSTEM_H__

