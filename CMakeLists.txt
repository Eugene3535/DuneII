cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

project(DuneII LANGUAGES C CXX)

set(BUILD_SHARED_LIBS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

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

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)

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
	# src/animation/AnimationManager.cpp
	# src/common/FileProvider.cpp
	# src/common/GraphicsUtils.cpp
	# src/common/ExternalMath.cpp
	# src/effects/blackout/ScreenBlackoutEffect.cpp
	# src/effects/particles/ParticleSystem.cpp
	# ${SCENE_SRC_FILES}
	# src/ui/Cursor.cpp
	# src/game/DuneII.cpp
	src/main.cpp
)

set(DUNEII_HDR_FILES
	# src/animation/AnimationData.hpp
	# src/animation/AnimationManager.hpp
	# src/assets/AssetManager.hpp
	# src/common/Defines.hpp
	# src/common/Enums.hpp
	# src/common/FileProvider.hpp
	# src/common/GraphicsUtils.hpp
	# src/common/ExternalMath.hpp
	# src/ecs/components/Animation.hpp
	# src/ecs/components/Structure.hpp
	# src/effects/blackout/ScreenBlackoutEffect.hpp
	# src/effects/particles/ParticleSystem.hpp
	# ${SCENE_HDR_FILES}
	# src/ui/Cursor.hpp
	# src/game/DuneII.hpp
)

set(_all_files 
	${DUNEII_SRC_FILES} 
#	${DUNEII_HDR_FILES}
)

add_executable(${PROJECT_NAME} ${_all_files})
add_subdirectory(${EXTERNAL_SOURCE_DIR}/glfw glfw)
add_subdirectory(${EXTERNAL_SOURCE_DIR}/glad/core glad)
add_subdirectory(${EXTERNAL_SOURCE_DIR}/glm glm)

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}/src
	${EXTERNAL_SOURCE_DIR}/rapidxml
	${EXTERNAL_SOURCE_DIR}/stb
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	glfw
	glad
	glm
)

target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=$<CONFIG:Debug> GLM_FORCE_INTRINSICS GLM_FORCE_EXPLICIT_CTOR)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(MSVC)
	set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_all_files})

	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res 
	VERBATIM
)