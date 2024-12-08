cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

project(DuneII LANGUAGES C CXX)

set(EXTERNAL_DIR $ENV{external})
set(BUILD_SHARED_LIBS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

if(WIN32 AND MSVC)
	string(APPEND CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
	string(APPEND CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib")
	string(APPEND CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")
else()
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

set(_duneII_src_files
	src/animation/AnimationManager.cpp
	src/assets/AssetManager.cpp
	src/common/FileProvider.cpp
	src/common/GraphicsUtils.cpp
	src/common/MathUtils.cpp
	src/ecs/SystemManager.cpp
	src/ecs/components/Animation.cpp
	src/ecs/systems/AnimationController.cpp
	src/ecs/systems/CullingController.cpp
	src/ecs/systems/CursorController.cpp
	src/ecs/systems/ViewportController.cpp
	src/ecs/systems/base/System.cpp
	src/effects/ScreenBlackoutEffect.cpp
	src/scenes/base/Scene.cpp
	src/scenes/intro/MainMenu.cpp
	src/scenes/mission/Mission.cpp
	src/scenes/mission/tilemap/TileMap.cpp
	src/scenes/construction_mode/ConstructionMenu.cpp
	src/ui/Cursor.cpp
	src/main.cpp
)

set(_duneII_hdr_files
	src/animation/AnimationData.hpp
	src/animation/AnimationManager.hpp
	src/assets/AssetManager.hpp
	src/common/Defines.hpp
	src/common/Enums.hpp
	src/common/FileProvider.hpp
	src/common/GraphicsUtils.hpp
	src/common/MathUtils.hpp
	src/ecs/SystemManager.hpp
	src/ecs/components/Animation.hpp
	src/ecs/components/Structure.hpp
	src/ecs/systems/AnimationController.hpp
	src/ecs/systems/CullingController.hpp
	src/ecs/systems/CursorController.hpp
	src/ecs/systems/ViewportController.hpp
	src/ecs/systems/base/System.hpp
	src/effects/ScreenBlackoutEffect.hpp
	src/game/Game.hpp
	src/scenes/base/Scene.hpp
	src/scenes/intro/MainMenu.hpp
	src/scenes/mission/Mission.hpp
	src/scenes/mission/tilemap/TileMap.hpp
	src/scenes/construction_mode/ConstructionMenu.hpp
	src/ui/Cursor.hpp
)

set(_all_files 
	${_duneII_src_files} 
	${_duneII_hdr_files}
)

add_executable(${PROJECT_NAME} ${_all_files})
add_subdirectory(${EXTERNAL_DIR}/sfml sfml)
add_subdirectory(${EXTERNAL_DIR}/entt entt)

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}/src
	${EXTERNAL_DIR}/rapidxml
)

target_link_libraries(${PROJECT_NAME}
	sfml-graphics
	sfml-window
	sfml-system
	sfml-audio
	EnTT::EnTT
)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if(MSVC)
	set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_all_files})
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res VERBATIM
)

if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXTERNAL_DIR}/sfml/extlibs/bin/x64/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/openal32.dll VERBATIM
	)

	if(MSVC)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res VERBATIM
		)
	endif()
endif()