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
				   	../../Classes/CellInfoMenu.cpp \
				   	../../Classes/CellSpinoffMenu.cpp \
				   	../../Classes/CellTasksMenu.cpp \
				   	../../Classes/CellTaskInfoMenu.cpp \
				   	../../Classes/Color.cpp \
				   	../../Classes/Drawable.cpp \
				   	../../Classes/EditorLayer.cpp \
				   	../../Classes/FileUtils.cpp \
				   	../../Classes/GameScene.cpp \
				   	../../Classes/Log.cpp \
				   	../../Classes/LuaBindings.cpp \
				   	../../Classes/LuaInstance.cpp \
				   	../../Classes/MessageManager.cpp \
				   	../../Classes/MessageWidget.cpp \
				   	../../Classes/MainMenuScene.cpp \
				   	../../Classes/MapGuiLayer.cpp \
				   	../../Classes/MapProjector.cpp \
				   	../../Classes/ProgressBar.cpp \
				   	../../Classes/Region.cpp \
				   	../../Classes/RegionInfoLayer.cpp \
				   	../../Classes/SplashScreenScene.cpp \
				   	../../Classes/Task.cpp \
				   	../../Classes/TaskManager.cpp \
				   	../../Classes/Town.cpp \
				   	../../Classes/TownInfoLayer.cpp \
				   	../../Classes/TransitionZoomFade.cpp \
				   	../../Classes/UserMessage.cpp \
				   	../../Classes/Vector2.cpp \
				   	../../Classes/World.cpp \
				   	../../Classes/WorldLoader.cpp \
				   	../../Classes/WorldMapLayer.cpp \
                  	../../dependencies/pugixml-1.4/pugixml.cpp \
                  	\
                  	../../dependencies/luabind-0.9.1/src/class.cpp \
					../../dependencies/luabind-0.9.1/src/class_info.cpp \
					../../dependencies/luabind-0.9.1/src/class_registry.cpp \
					../../dependencies/luabind-0.9.1/src/class_rep.cpp \
					../../dependencies/luabind-0.9.1/src/create_class.cpp \
					../../dependencies/luabind-0.9.1/src/error.cpp \
					../../dependencies/luabind-0.9.1/src/exception_handler.cpp \
					../../dependencies/luabind-0.9.1/src/function.cpp \
					../../dependencies/luabind-0.9.1/src/inheritance.cpp \
					../../dependencies/luabind-0.9.1/src/link_compatibility.cpp \
					../../dependencies/luabind-0.9.1/src/object_rep.cpp \
					../../dependencies/luabind-0.9.1/src/open.cpp \
					../../dependencies/luabind-0.9.1/src/pcall.cpp \
					../../dependencies/luabind-0.9.1/src/scope.cpp \
					../../dependencies/luabind-0.9.1/src/stack_content_by_name.cpp \
					../../dependencies/luabind-0.9.1/src/weak_ref.cpp \
					../../dependencies/luabind-0.9.1/src/wrapper_base.cpp \
					\
					../../dependencies/lua-5.1.5/lapi.c \
					../../dependencies/lua-5.1.5/lauxlib.c \
					../../dependencies/lua-5.1.5/lbaselib.c \
					../../dependencies/lua-5.1.5/lcode.c \
					../../dependencies/lua-5.1.5/ldblib.c \
					../../dependencies/lua-5.1.5/ldebug.c \
					../../dependencies/lua-5.1.5/ldo.c \
					../../dependencies/lua-5.1.5/ldump.c \
					../../dependencies/lua-5.1.5/lfunc.c \
					../../dependencies/lua-5.1.5/lgc.c \
					../../dependencies/lua-5.1.5/linit.c \
					../../dependencies/lua-5.1.5/liolib.c \
					../../dependencies/lua-5.1.5/llex.c \
					../../dependencies/lua-5.1.5/lmathlib.c \
					../../dependencies/lua-5.1.5/lmem.c \
					../../dependencies/lua-5.1.5/loadlib.c \
					../../dependencies/lua-5.1.5/lobject.c \
					../../dependencies/lua-5.1.5/lopcodes.c \
					../../dependencies/lua-5.1.5/loslib.c \
					../../dependencies/lua-5.1.5/lparser.c \
					../../dependencies/lua-5.1.5/lstate.c \
					../../dependencies/lua-5.1.5/lstring.c \
					../../dependencies/lua-5.1.5/lstrlib.c \
					../../dependencies/lua-5.1.5/ltable.c \
					../../dependencies/lua-5.1.5/ltablib.c \
					../../dependencies/lua-5.1.5/ltm.c \
					../../dependencies/lua-5.1.5/lundump.c \
					../../dependencies/lua-5.1.5/lvm.c \
					../../dependencies/lua-5.1.5/lzio.c \
					../../dependencies/lua-5.1.5/print.c
                   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../dependencies/boost-1.55.0 \
					$(LOCAL_PATH)/../../dependencies/lua-5.1.5 \
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
