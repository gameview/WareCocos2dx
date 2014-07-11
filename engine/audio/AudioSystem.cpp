#include "AudioSystem.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

NS_CC_ENGINE_BEGIN

/// 全局的音频系统
IAudioSystem*	g_pAudioSystem = NULL;
CAudioSystem	g_AudioSystem;

/**
*@brief 获取渲染系统
*/
IAudioSystem* getAudioSystem()
{
	return g_pAudioSystem;
}

CAudioSystem::CAudioSystem(void)
	: m_fEffectVolume(1.0f)
	, m_fBackgroundMusicVolume(1.0f)
{
	g_pAudioSystem = this;
}

CAudioSystem::~CAudioSystem(void)
{
	g_pAudioSystem = NULL;
}

/// 音频接口
//////////////////////////////////////////////////////////////////////////
void CAudioSystem::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
	if( pszFilePath == NULL )
	{
		return;
	}
    //setBackgroundMusicVolume(1.0);

	SimpleAudioEngine::sharedEngine()->playBackgroundMusic( pszFilePath, bLoop );
}

/**
@brief Stop playing background music
@param bReleaseData If release the background music data or not.As default value is false
*/
void CAudioSystem::stopBackgroundMusic(bool bReleaseData)
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(bReleaseData);
}

/**
@brief The volume of the background music max value is 1.0,the min value is 0.0
*/
float CAudioSystem::getBackgroundMusicVolume()
{
	return SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
}

/**
@brief set the volume of background music
@param volume must be in 0.0~1.0
*/
void CAudioSystem::setBackgroundMusicVolume(float volume)
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume( volume );
}

/**
@brief The volume of the effects max value is 1.0,the min value is 0.0
*/
float CAudioSystem::getEffectsVolume()
{
	return SimpleAudioEngine::sharedEngine()->getEffectsVolume();
}

/**
@brief set the volume of sound effecs
@param volume must be in 0.0~1.0
*/
void CAudioSystem::setEffectsVolume(float volume)
{
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
}

// for sound effects
/**
@brief Play sound effect
@param pszFilePath The path of the effect file,or the FileName of T_SoundResInfo
@bLoop Whether to loop the effect playing, default value is false
*/
unsigned int CAudioSystem::playEffect(const char* pszFilePath, bool bLoop)
{
	if( pszFilePath == NULL )
	{
		return 0;
	}

	return SimpleAudioEngine::sharedEngine()->playEffect( pszFilePath, bLoop );
}

/**
@brief Resume playing sound effect
@param nSoundId The return value of function playEffect
*/
void CAudioSystem::resumeEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::sharedEngine()->resumeEffect( nSoundId );
}

/**
@brief Resume all playing sound effect
@param nSoundId The return value of function playEffect
*/
void CAudioSystem::resumeAllEffects()
{
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

/**
@brief Stop playing sound effect
@param nSoundId The return value of function playEffect
*/
void CAudioSystem::stopEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::sharedEngine()->stopEffect( nSoundId );
}

/**
@brief Stop all playing sound effects
*/
void CAudioSystem::stopAllEffects()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
}
/**
@brief 系统静音
@param
*/
void CAudioSystem::muteAllSounds( bool bMute )
{
	if( bMute )
	{
		m_fEffectVolume = getEffectsVolume();
		m_fBackgroundMusicVolume = getBackgroundMusicVolume();
		setEffectsVolume( 0.0f );
		setBackgroundMusicVolume( 0.0f );
	}
	else
	{
		setEffectsVolume( m_fEffectVolume );
		setBackgroundMusicVolume( m_fBackgroundMusicVolume );
	}
}

NS_CC_ENGINE_END
