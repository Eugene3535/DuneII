cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

set(GPROF_PROFILING OFF)

if(GPROF_PROFILING) # gprof ./DuneII.exe gmon.out -z > analysis.txt ( more info available at https://www.opennet.ru/docs/RUS/gprof/gprof-5.html )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif()

project(DuneII LANGUAGES C CXX)

set(EXTERNAL_DIR $ENV{external})
set(BUILD_SHARED_LIBS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

if(WIN32 AND NOT MINGW)
	string(APPEND CMAKE_RUNTIME_OUTPUT_DIRECTORY "/bin")
	string(APPEND CMAKE_LIBRARY_OUTPUT_DIRECTORY "/lib")
	string(APPEND CMAKE_ARCHIVE_OUTPUT_DIRECTORY "/lib")
endif()

set(_src_dir ${PROJECT_SOURCE_DIR}/src)

set(_manager_src_files
	${_src_dir}/assets/AssetManager.cpp
	${_src_dir}/animation/AnimationManager.cpp
	${_src_dir}/ecs/SystemManager.cpp
)

set(_scenes_src_files
	${_src_dir}/scenes/base/Scene.cpp
	${_src_dir}/scenes/intro/MainMenu.cpp
	${_src_dir}/scenes/mission/Mission.cpp
	${_src_dir}/scenes/mission/tilemap/TileMap.cpp
)

set(_ecs_src_files
	${_src_dir}/ecs/systems/base/System.cpp
	${_src_dir}/ecs/components/Animation.cpp
	${_src_dir}/ecs/systems/AnimationController.cpp
	${_src_dir}/ecs/systems/ViewportController.cpp
	${_src_dir}/ecs/systems/CullingController.cpp
	${_src_dir}/ecs/systems/CursorController.cpp
)

set(_duneII_src_files
	${_src_dir}/common/FileProvider.cpp
	${_src_dir}/common/MathUtils.cpp
	${_src_dir}/effects/ScreenBlackoutEffect.cpp
	${_src_dir}/ui/Cursor.cpp
	${_scenes_src_files}
	${_ecs_src_files}
	${_manager_src_files}
	${_src_dir}/main.cpp
)

add_executable(${PROJECT_NAME} ${_duneII_src_files})
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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)

if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXTERNAL_DIR}/sfml/extlibs/bin/x64/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/openal32.dll
	)
endif()