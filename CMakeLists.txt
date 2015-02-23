cmake_minimum_required(VERSION 2.8)

set(APP_NAME TheWe)
project (${APP_NAME})

include(cocos2d/build/BuildHelpers.CMakeLists.txt)

option(USE_CHIPMUNK "Use chipmunk for physics library" ON)
option(USE_BOX2D "Use box2d for physics library" OFF)
option(DEBUG_MODE "Debug or release?" ON)

if(DEBUG_MODE)
	set(CMAKE_BUILD_TYPE DEBUG)
	add_definitions(-D_DEBUG)
else(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif(DEBUG_MODE)

if (MSVC)
	set(CMAKE_C_FLAGS_DEBUG "-DCOCOS2D_DEBUG=1")
	set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")

elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS_DEBUG "-g -Wall -DCOCOS2D_DEBUG=1")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-reorder -Wno-deprecated-declarations -Wno-unused-but-set-variable -Wno-unused-variable -Wno-unused-function")

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

endif()

if(USE_CHIPMUNK)
  message("Using chipmunk ...")
  add_definitions(-DCC_ENABLE_CHIPMUNK_INTEGRATION=1)
  if(UNIX) # assuming linux
	add_definitions(-DLINUX )
  endif()
elseif(USE_BOX2D)
  message("Using box2d ...")
  add_definitions(-DCC_ENABLE_BOX2D_INTEGRATION=1)
  if(UNIX) # assuming linux
	add_definitions(-DLINUX )
  endif()
else(USE_CHIPMUNK)
  message(FATAL_ERROR "Must choose a physics library.")
endif(USE_CHIPMUNK)

# architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	set(ARCH_DIR "64-bit")
else()
	set(ARCH_DIR "32-bit")
endif()

aux_source_directory(./Classes CLASSES_SRC)
aux_source_directory(./dependencies/luabind-0.9.1/src LUABIND_SRC)
aux_source_directory(./dependencies/lua-5.1.5 LUA_SRC)
set(PUGI_SRC dependencies/pugixml-1.4/pugixml.cpp)
set(SQLITE_SRC dependencies/pugixml-1.4/pugixml.cpp)

if( UNIX ) #assume linux
set(GAME_SRC
	proj.linux/main.cpp

	${CLASSES_SRC}

	${LUABIND_SRC}
	${LUA_SRC}
	${PUGI_SRC}
	${SQLITE_SRC}
)
elseif ( WIN32 )
set(GAME_SRC
	proj.win32/main.cpp
	proj.win32/main.h
	proj.win32/resource.h

	${CLASSES_SRC}

	${LUABIND_SRC}
	${LUA_SRC}
	${PUGI_SRC}
	${SQLITE_SRC}
)
endif()

set(COCOS2D_ROOT ${CMAKE_SOURCE_DIR}/cocos2d)
if (WIN32)
include_directories(
	${COCOS2D_ROOT}
	${COCOS2D_ROOT}/cocos
	${COCOS2D_ROOT}/cocos/audio/include
	${COCOS2D_ROOT}/cocos/2d
	${COCOS2D_ROOT}/cocos/2d/renderer
	${COCOS2D_ROOT}/cocos/2d/platform
	${COCOS2D_ROOT}/cocos/2d/platform/desktop
	${COCOS2D_ROOT}/cocos/2d/platform/win32
	${COCOS2D_ROOT}/cocos/base
	${COCOS2D_ROOT}/cocos/deprecated
	${COCOS2D_ROOT}/cocos/physics
	${COCOS2D_ROOT}/cocos/editor-support
	${COCOS2D_ROOT}/cocos/math
	${COCOS2D_ROOT}/extensions
	${COCOS2D_ROOT}/external
	${COCOS2D_ROOT}/external/edtaa3func
	${COCOS2D_ROOT}/external/jpeg/include/win32
	${COCOS2D_ROOT}/external/png/include/win32
	${COCOS2D_ROOT}/external/tiff/include/win32
	${COCOS2D_ROOT}/external/webp/include/win32
	${COCOS2D_ROOT}/external/curl/include/win32
	${COCOS2D_ROOT}/external/tinyxml2
	${COCOS2D_ROOT}/external/unzip
	${COCOS2D_ROOT}/external/chipmunk/include/chipmunk
	${COCOS2D_ROOT}/external/freetype2/include/win32
	${COCOS2D_ROOT}/external/websockets/include/win32
	${COCOS2D_ROOT}/external/spidermonkey/include/win32
	${COCOS2D_ROOT}/external/glfw3/include/win32
	${COCOS2D_ROOT}/external/win32-specific/gles/include/OGLES
	${COCOS2D_ROOT}/external/win32-specific/icon/include
	${COCOS2D_ROOT}/external/win32-specific/zlib/include
	${COCOS2D_ROOT}/external/xxhash
)
link_directories(
	/usr/local/lib
	${COCOS2D_ROOT}/external/png/prebuilt/win32
	${COCOS2D_ROOT}/external/jpeg/prebuilt/win32
	${COCOS2D_ROOT}/external/tiff/prebuilt/win32
	${COCOS2D_ROOT}/external/glfw3/prebuilt/win32
	${COCOS2D_ROOT}/external/webp/prebuilt/win32
	${COCOS2D_ROOT}/external/curl/prebuilt/win32
	${COCOS2D_ROOT}/external/freetype2/prebuilt/win32
	${COCOS2D_ROOT}/external/websockets/prebuilt/win32
	${COCOS2D_ROOT}/external/spidermonkey/prebuilt/win32
	${COCOS2D_ROOT}/external/win32-specific/gles/prebuilt
	${COCOS2D_ROOT}/external/win32-specific/icon/prebuilt
	${COCOS2D_ROOT}/external/win32-specific/zlib/prebuilt
)

elseif(UNIX) #assumed linux
include_directories(
	/usr/local/include/GLFW
	/usr/include/GLFW
	${COCOS2D_ROOT}
	${COCOS2D_ROOT}/cocos
	${COCOS2D_ROOT}/cocos/audio/include
	${COCOS2D_ROOT}/cocos/platform
	${COCOS2D_ROOT}/cocos/platform/desktop
	${COCOS2D_ROOT}/cocos/platform/linux
	${COCOS2D_ROOT}/cocos/editor-support
	${COCOS2D_ROOT}/extensions
	${COCOS2D_ROOT}/external
	${COCOS2D_ROOT}/external/edtaa3func
	${COCOS2D_ROOT}/external/jpeg/include/linux
	${COCOS2D_ROOT}/external/tiff/include/linux
	${COCOS2D_ROOT}/external/webp/include/linux
	${COCOS2D_ROOT}/external/tinyxml2
	${COCOS2D_ROOT}/external/unzip
	${COCOS2D_ROOT}/external/freetype2/include/linux
	${COCOS2D_ROOT}/external/websockets/include/linux
	${COCOS2D_ROOT}/external/spidermonkey/include/linux
	${COCOS2D_ROOT}/external/linux-specific/fmod/include/${ARCH_DIR}
	${COCOS2D_ROOT}/external/xxhash
	${COCOS2D_ROOT}/../dependencies/boost_1_55_0
	${COCOS2D_ROOT}/../dependencies/luabind-0.9.1
	${COCOS2D_ROOT}/../dependencies/lua-5.1.5
	${COCOS2D_ROOT}/../dependencies/sqlite3
)

link_directories(
  /usr/local/lib
  ${COCOS2D_ROOT}/external/jpeg/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/tiff/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/webp/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/freetype2/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/websockets/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/spidermonkey/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/linux-specific/fmod/prebuilt/${ARCH_DIR}
)
endif()

if(USE_CHIPMUNK)

	include_directories(${COCOS2D_ROOT}/external/chipmunk/include/chipmunk)
	# chipmunk library
	add_subdirectory(${COCOS2D_ROOT}/external/chipmunk/src)
endif()

if(USE_BOX2D)
	# box2d library
	add_subdirectory(${COCOS2D_ROOT}/external/Box2D)
endif()

# unzip library
add_subdirectory(${COCOS2D_ROOT}/external/unzip)

# tinyxml2 library
add_subdirectory(${COCOS2D_ROOT}/external/tinyxml2)

# audio
add_subdirectory(${COCOS2D_ROOT}/cocos/audio)

# xxhash library
add_subdirectory(${COCOS2D_ROOT}/external/xxhash)

# cocos2d
add_subdirectory(${COCOS2D_ROOT}/cocos)

# extensions
add_subdirectory(${COCOS2D_ROOT}/extensions)

## Editor Support

# spine
add_subdirectory(${COCOS2D_ROOT}/cocos/editor-support/spine)

# cocosbuilder
add_subdirectory(${COCOS2D_ROOT}/cocos/editor-support/cocosbuilder)

# cocostudio
add_subdirectory(${COCOS2D_ROOT}/cocos/editor-support/cocostudio)

if ( WIN32 )
	# add the executable
	add_executable(${APP_NAME}
	  WIN32
	  ${GAME_SRC}
	)
else()
	# add the executable
	add_executable(${APP_NAME}
	  ${GAME_SRC}
	)
endif()

if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	set(FMOD_LIB "fmodex64")
else()
	set(FMOD_LIB "fmodex")
endif()

target_link_libraries(${APP_NAME}
	# кокосовские библиотеки
	spine
	cocostudio
	cocosbuilder
	audio
	ui # костыльно созданная либа (может перестать работать после смены версии cocos2d)
	extensions
	cocos2d
	# библиотеки для ogl и x11
	glfw3
	Xxf86vm
	Xrandr
	Xcursor
	Xi
	# другие библиотеки
	dl
  )

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES
	 RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}")

if ( WIN32 )
  #also copying dlls to binary directory for the executable to run
  pre_build(${APP_NAME}
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
	COMMAND ${CMAKE_COMMAND} -E copy ${COCOS2D_ROOT}/external/win32-specific/gles/prebuilt/glew32.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
	COMMAND ${CMAKE_COMMAND} -E copy ${COCOS2D_ROOT}/external/win32-specific/zlib/prebuilt/zlib1.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
	)
else()
  pre_build(${APP_NAME}
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
	)

endif()
