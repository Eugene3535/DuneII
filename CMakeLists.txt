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
	src/game/scenes/Scene.cpp
	# src/game/scenes/intro/interactive_elements/Button.cpp
	src/game/scenes/intro/TitleScreen.cpp
	# src/game/scenes/choosing_houses/Destiny.cpp
	# src/game/scenes/mission/Mission.cpp
	# src/game/scenes/mission/tilemap/Tile.cpp
	# src/game/scenes/mission/tilemap/TileMap.cpp
)

set(SCENE_HDR_FILES
	src/game/scenes/Scene.hpp
	# src/game/scenes/intro/interactive_elements/Button.hpp
	src/game/scenes/intro/TitleScreen.hpp
	# src/game/scenes/choosing_houses/Destiny.hpp
	# src/game/scenes/mission/Mission.hpp
	# src/game/scenes/mission/tilemap/Tile.hpp
	# src/game/scenes/mission/tilemap/TileMap.hpp
)

set(DUNEII_SRC_FILES
	src/common/ExternalMath.cpp
	src/resources/files/FileProvider.cpp
	# src/resources/files/Font.cpp
	src/resources/files/Shader.cpp
	src/resources/files/StbImage.cpp
	# src/effects/blackout/ScreenBlackoutEffect.cpp
	${SCENE_SRC_FILES}
	src/app/Application.cpp
	src/game/DuneII.cpp
	src/main.cpp
)

set(DUNEII_HDR_FILES
	src/common/Enums.hpp
	src/common/ExternalMath.hpp
	src/resources/files/FileProvider.hpp
	# src/resources/files/Font.hpp
	src/resources/files/Shader.hpp
	src/resources/files/StbImage.hpp
	# src/effects/blackout/ScreenBlackoutEffect.hpp
	${SCENE_HDR_FILES}
	src/app/Application.hpp
	src/game/DuneII.hpp
	src/game/DuneII.inl
)

set(_all_files 
	${DUNEII_SRC_FILES} 
	${DUNEII_HDR_FILES}
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