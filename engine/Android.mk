LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cocos_engine_static

LOCAL_CFLAGS := -fexceptions

LOCAL_MODULE_FILENAME := libengine

LOCAL_SRC_FILES := audio/AudioSystem.cpp \
	common/ConfigFileManager.cpp \
	common/StringConvert.cpp \
	common/StringUtility.cpp \
	common/WareFileRead.cpp \
	common/WareFileWrite.cpp \
	common/WareIniFile.cpp \
	common/DaemonManager.cpp \
	engine/WareAppDelegate.cpp \
	engine/WareTime.cpp \
	engine/InputManager.cpp \
	render/CMaskSprite.cpp \
	render/AnimationRes.cpp \
	render/AnimationResManager.cpp \
	render/AniSpriteObj.cpp \
	render/CEffect.cpp \
	render/CEffectManager.cpp \
	render/CEffectProp.cpp \
	render/CEffectPropManager.cpp \
	render/CEffectSprite.cpp \
	render/CEffectUnit.cpp \
	render/CEffectUnitProp.cpp \
	render/CParticleSystem.cpp \
	render/CParticleSystemProp.cpp \
	render/CTextureAnimation.cpp \
	render/CTextureAnimationProp.cpp \
	render/DragonBoneAnimation.cpp \
	render/DragonBoneSpriteObj.cpp \
	render/ImageLayer.cpp \
	render/RectangleObj.cpp \
	render/RenderableManager.cpp \
	render/RenderMap.cpp \
	render/RenderObj.cpp \
	render/RenderObjManager.cpp \
	render/RenderObjModel.cpp \
	render/RenderSystem.cpp \
	render/RenderText.cpp \
	render/RenderTextManager.cpp \
	render/SceneLayer.cpp \
	render/SceneManager.cpp \
	render/ShaderManager.cpp \
	render/SpriteFrameSet.cpp \
	render/SpriteObj.cpp \
	render/TerrainBlock.cpp \
	render/TerrainGrid.cpp \
	render/TerrainMask.cpp \
	render/tinystr.cpp \
	render/tinyxml.cpp \
	render/tinyxmlerror.cpp \
	render/tinyxmlparser.cpp \
	render/WareAnimate.cpp \
	render/WareSprite.cpp \
		 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/inc \
						   $(LOCAL_PATH)/inc/common \
						   $(LOCAL_PATH)/inc/common/jsons \
						   $(LOCAL_PATH)/inc/engine \
						   $(LOCAL_PATH)/inc/render \
						   $(LOCAL_PATH)/../Include \
						   $(LOCAL_PATH)/../Common \
						   $(LOCAL_PATH)/../../Include \
						   $(LOCAL_PATH)/../../Common \
						   

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/inc \
					$(LOCAL_PATH)/render \
					$(LOCAL_PATH)/engine \
					$(LOCAL_PATH)/common \
					$(LOCAL_PATH)/../Include \
					$(LOCAL_PATH)/../Common \
				    $(LOCAL_PATH)/../../Include \
				    $(LOCAL_PATH)/../../Common \
				    $(LOCAL_PATH)/../cocos2dx \
                    $(LOCAL_PATH)/../cocos2dx/include \
                    $(LOCAL_PATH)/../cocos2dx/platform \
                    $(LOCAL_PATH)/../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../cocos2dx/kazmath/include \
					$(LOCAL_PATH)/../extensions/CocoStudio/GUI/System

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static 
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static 

include $(BUILD_STATIC_LIBRARY)


$(call import-module,cocos2dx) 
$(call import-module,CocosDenshion/android) 
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,extensions) 
