cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(DuneII LANGUAGES C CXX)

set(EXTERNAL_DIR $ENV{external})
set(BUILD_SHARED_LIBS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(_src_files
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/common/FileProvider.cpp
	${PROJECT_SOURCE_DIR}/src/common/MathUtils.cpp
	${PROJECT_SOURCE_DIR}/src/effects/ScreenBlackoutEffect.cpp
	${PROJECT_SOURCE_DIR}/src/managers/assets/AssetManager.cpp
	${PROJECT_SOURCE_DIR}/src/managers/ecs/systems/base/System.cpp
	${PROJECT_SOURCE_DIR}/src/managers/ecs/systems/MoveSystem.cpp
	${PROJECT_SOURCE_DIR}/src/managers/ecs/SystemManager.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/Scene.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/intro/MainMenu.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/Mission.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/buildings/Builder.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/buildings/Building.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/tilemap/TileMap.cpp
)

add_executable(${PROJECT_NAME} ${_src_files})
add_subdirectory(${EXTERNAL_DIR}/sfml sfml)
add_subdirectory(${EXTERNAL_DIR}/entt entt)

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
	EnTT::EnTT
)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory    ${CMAKE_SOURCE_DIR}/res                       $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXTERNAL_DIR}/install/sfml/bin/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/openal32.dll
)
