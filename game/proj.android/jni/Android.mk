LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE := game_share

LOCAL_MODULE_FILENAME := libGame

LOCAL_SRC_FILES := game/main.cpp \
                   ../../Classes/MainGameApp.cpp \
			
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../../engine\inc
					
				
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_engine_static cocos_lua_static
        
include $(BUILD_SHARED_LIBRARY)


LOCAL_CFLAGS := -fexceptions -DCOCOS2D_DEBUG=0

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,engine)
$(call import-module,lua/proj.android)
