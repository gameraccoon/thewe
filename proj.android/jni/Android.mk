LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_CPP_FEATURES += exceptions

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
				   	../../Classes/Application.cpp \
				   	../../Classes/ArbitraryHull.cpp \
				   	../../Classes/Cell.cpp \
				   	../../Classes/CellGameInterface.cpp \
				   	../../Classes/Color.cpp \
				   	../../Classes/Drawable.cpp \
				   	../../Classes/EditorLayer.cpp \
				   	../../Classes/FileUtils.cpp \
				   	../../Classes/GameScene.cpp \
				   	../../Classes/Log.cpp \
				   	../../Classes/MainMenuScene.cpp \
				   	../../Classes/MapGuiLayer.cpp \
				   	../../Classes/MapProjector.cpp \
				   	../../Classes/PlayersProfiles.cpp \
				   	../../Classes/Region.cpp \
				   	../../Classes/RegionInfoLayer.cpp \
				   	../../Classes/SplashScreenScene.cpp \
				   	../../Classes/Task.cpp \
				   	../../Classes/TaskManager.cpp \
				   	../../Classes/Town.cpp \
				   	../../Classes/TownInfoLayer.cpp \
				   	../../Classes/Vector2.cpp \
				   	../../Classes/World.cpp \
				   	../../Classes/WorldLoader.cpp \
				   	../../Classes/WorldMapLayer.cpp \
                  	../../dependencies/pugixml-1.4/pugixml.cpp
                   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../dependencies/boost_1_56_0 \
					$(LOCAL_PATH)/../../dependencies/lua-5.1 \
					$(LOCAL_PATH)/../../dependencies/luabind-0.9.1 \
					$(LOCAL_PATH)/../../dependencies/pugixml-1.4 \
					$(LOCAL_PATH)/../../cocos2d/extensions

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
$(call import-module,extensions)
