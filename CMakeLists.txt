cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

project(DuneII LANGUAGES C CXX)

set(BUILD_SHARED_LIBS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(EXTERNAL_SOURCE_DIR $ENV{External})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

if(MSVC)
	string(APPEND CMAKE_RUNTIME_OUTPUT_DIRECTORY "/bin")
	string(APPEND CMAKE_LIBRARY_OUTPUT_DIRECTORY "/lib")
	string(APPEND CMAKE_ARCHIVE_OUTPUT_DIRECTORY "/lib")
else()
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)


set(SCENE_SRC_FILES
	src/scenes/Scene.cpp
	src/scenes/intro/interactive_elements/Button.cpp
	src/scenes/intro/TitleScreen.cpp
	src/scenes/choosing_houses/Destiny.cpp
	src/scenes/mission/Mission.cpp
	src/scenes/mission/tilemap/Tile.cpp
	src/scenes/mission/tilemap/TileMap.cpp
)

set(SCENE_HDR_FILES
	src/scenes/Scene.hpp
	src/scenes/intro/interactive_elements/Button.hpp
	src/scenes/intro/TitleScreen.hpp
	src/scenes/choosing_houses/Destiny.hpp
	src/scenes/mission/Mission.hpp
	src/scenes/mission/tilemap/Tile.hpp
	src/scenes/mission/tilemap/TileMap.hpp
)

set(DUNEII_SRC_FILES
	src/animation/AnimationManager.cpp
	src/common/FileProvider.cpp
	src/common/GraphicsUtils.cpp
	src/common/ExternalMath.cpp
	src/effects/blackout/ScreenBlackoutEffect.cpp
	src/effects/particles/ParticleSystem.cpp
	${SCENE_SRC_FILES}
	src/ui/Cursor.cpp
	src/game/DuneII.cpp
	src/main.cpp
)

set(DUNEII_HDR_FILES
	src/animation/AnimationData.hpp
	src/animation/AnimationManager.hpp
	src/assets/AssetManager.hpp
	src/common/Defines.hpp
	src/common/Enums.hpp
	src/common/FileProvider.hpp
	src/common/GraphicsUtils.hpp
	src/common/ExternalMath.hpp
	src/ecs/components/Animation.hpp
	src/ecs/components/Structure.hpp
	src/effects/blackout/ScreenBlackoutEffect.hpp
	src/effects/particles/ParticleSystem.hpp
	${SCENE_HDR_FILES}
	src/ui/Cursor.hpp
	src/game/DuneII.hpp
)

set(_all_files 
	${DUNEII_SRC_FILES} 
	${DUNEII_HDR_FILES}
)

add_executable(${PROJECT_NAME} ${_all_files})
add_subdirectory(${EXTERNAL_SOURCE_DIR}/sfml sfml)
add_subdirectory(${EXTERNAL_SOURCE_DIR}/entt entt)

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}/src
	${EXTERNAL_SOURCE_DIR}/rapidxml
)

target_link_libraries(${PROJECT_NAME}
	sfml-graphics
	sfml-window
	sfml-system
	sfml-audio
	EnTT::EnTT
)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(MSVC)
	set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_all_files})

#   Working directory fix 
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res 
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/sfml/bin/$<CONFIG> $<TARGET_FILE_DIR:${PROJECT_NAME}>
		VERBATIM
	)
endif()

if(UNIX OR MINGW)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res 
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/sfml/bin $<TARGET_FILE_DIR:${PROJECT_NAME}>
		VERBATIM
	)
endif()