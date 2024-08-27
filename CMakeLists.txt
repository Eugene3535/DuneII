cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(DuneII LANGUAGES C CXX)

set(EXTERNAL_DIR $ENV{External})
set(BUILD_SHARED_LIBS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(SRC_FILE_DIR ${PROJECT_SOURCE_DIR}/src)

set(SRC_FILES
	${SRC_FILE_DIR}/common/FileProvider.cpp
	${SRC_FILE_DIR}/common/MathUtils.cpp
	${SRC_FILE_DIR}/managers/assets/AssetManager.cpp
	${SRC_FILE_DIR}/effects/ScreenBlackoutEffect.cpp
	${SRC_FILE_DIR}/scenes/Scene.cpp
	${SRC_FILE_DIR}/scenes/intro/MainMenu.cpp
	${SRC_FILE_DIR}/scenes/mission/Mission.cpp
	${SRC_FILE_DIR}/scenes/mission/animated_tile/AnimatedTile.cpp
	${SRC_FILE_DIR}/scenes/mission/tilemap/TileMap.cpp
	${SRC_FILE_DIR}/scenes/mission/buildings/Building.cpp
	${SRC_FILE_DIR}/scenes/mission/buildings/Builder.cpp
	${SRC_FILE_DIR}/main.cpp
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
	COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_DIR}/install/sfml/bin/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/openal32.dll
)
