cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(DuneII LANGUAGES C CXX)

set(EXTERNAL_DIR $ENV{External})
set(BUILD_SHARED_LIBS OFF)

set(SRC_FILES
	${PROJECT_SOURCE_DIR}/src/utils/FileProvider.cpp
	${PROJECT_SOURCE_DIR}/src/managers/assets/AssetManager.cpp
	${PROJECT_SOURCE_DIR}/src/managers/buildings/BuildingManager.cpp
	${PROJECT_SOURCE_DIR}/src/animation/Animation.hpp
	${PROJECT_SOURCE_DIR}/src/effects/ScreenBlackoutEffect.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/Scene.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/intro/MainMenu.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/Mission.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/tilemap/TileMap.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/buildings/Building.cpp
	${PROJECT_SOURCE_DIR}/src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
add_subdirectory(${EXTERNAL_DIR}/sfml external/sfml)

target_include_directories(${PROJECT_NAME} PRIVATE
	"${EXTERNAL_DIR}/rapidxml"
	"${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(${PROJECT_NAME}
	sfml-graphics
	sfml-window
	sfml-system
	sfml-audio
)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res             $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
	COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_DIR}/install/sfml/bin/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/openal32.dll)
