cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(DuneII LANGUAGES C CXX)

set(EXTERNAL_DIR $ENV{external})
set(BUILD_SHARED_LIBS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

if(WIN32 AND NOT MINGW)
	string(APPEND CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
	string(APPEND CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib")
	string(APPEND CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")
endif()

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

set(_duneII_src_files
	${SRC_DIR}/main.cpp
	${SRC_DIR}/common/FileProvider.cpp
	${SRC_DIR}/common/MathUtils.cpp
	${SRC_DIR}/effects/ScreenBlackoutEffect.cpp
	${SRC_DIR}/assets/AssetManager.cpp
	${SRC_DIR}/ecs/common/EntityContainer.cpp
	${SRC_DIR}/ecs/common/EntitySet.cpp
	${SRC_DIR}/ecs/EntityManager.cpp
	${SRC_DIR}/ecs/systems/base/System.cpp
	${SRC_DIR}/ecs/systems/MoveSystem.cpp
	${SRC_DIR}/ecs/SystemManager.cpp
	${SRC_DIR}/ecs/components/Sprite.cpp
	${SRC_DIR}/ecs/components/Bounds.cpp
	${SRC_DIR}/scenes/Scene.cpp
	${SRC_DIR}/scenes/intro/MainMenu.cpp
	${SRC_DIR}/scenes/mission/Mission.cpp
	${SRC_DIR}/scenes/mission/buildings/Builder.cpp
	${SRC_DIR}/scenes/mission/buildings/Building.cpp
	${SRC_DIR}/scenes/mission/tilemap/TileMap.cpp
)

add_executable(${PROJECT_NAME} ${_duneII_src_files})
add_subdirectory(${EXTERNAL_DIR}/sfml sfml)

if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}/src
	${EXTERNAL_DIR}/rapidxml
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
	COMMAND ${CMAKE_COMMAND} -E copy_directory    ${CMAKE_SOURCE_DIR}/res                       $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXTERNAL_DIR}/install/sfml/bin/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/openal32.dll
)
