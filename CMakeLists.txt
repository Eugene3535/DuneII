cmake_minimum_required(VERSION 3.31.10 FATAL_ERROR)

project(DuneII LANGUAGES C CXX)

set(BUILD_SHARED_LIBS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

set(EXTERNAL_SOURCE_DIR $ENV{External})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(CMAKE_BUILD_TYPE)
    string(APPEND CMAKE_RUNTIME_OUTPUT_DIRECTORY "/${CMAKE_BUILD_TYPE}")
	string(APPEND CMAKE_LIBRARY_OUTPUT_DIRECTORY "/${CMAKE_BUILD_TYPE}")
	string(APPEND CMAKE_ARCHIVE_OUTPUT_DIRECTORY "/${CMAKE_BUILD_TYPE}")
endif()

if(MSVC)
	string(APPEND CMAKE_RUNTIME_OUTPUT_DIRECTORY "/bin")
	string(APPEND CMAKE_LIBRARY_OUTPUT_DIRECTORY "/lib")
	string(APPEND CMAKE_ARCHIVE_OUTPUT_DIRECTORY "/lib")
else()
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

if(WIN32)
	if(MSVC)
		list(APPEND CMAKE_PREFIX_PATH "${EXTERNAL_SOURCE_DIR}/install/msvc")
	elseif(MINGW)
		list(APPEND CMAKE_PREFIX_PATH "${EXTERNAL_SOURCE_DIR}/install/mingw")
	endif()
endif()

if(LINUX)
	list(APPEND CMAKE_PREFIX_PATH "${EXTERNAL_SOURCE_DIR}/install/linux")
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(cglm CONFIG REQUIRED)
find_package(entt CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

file(GLOB_RECURSE DUNEII_SRC_FILES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)

file(GLOB SHADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*.vert
	${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*.frag
	${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*.geom
	${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*.comp
	${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*.tess
)

add_executable(${PROJECT_NAME} ${DUNEII_SRC_FILES})

target_sources(${PROJECT_NAME} PRIVATE ${SHADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${DUNEII_SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}/src
	${EXTERNAL_SOURCE_DIR}/rapidxml
	${EXTERNAL_SOURCE_DIR}/stb
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	glfw
	glad::glad
	cglm::cglm
	EnTT::EnTT
	magic_enum::magic_enum
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /GR-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -fno-rtti)# -fno-exceptions (rapidxml throw errors fix)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG> CGLM_USE_ANONYMOUS_STRUCT)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(MSVC)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${DUNEII_ALL_FILES})
	set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_SOURCE_DIR}/res"         "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_SOURCE_DIR}/src/shaders" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res/shaders"
	VERBATIM
)

if(WIN32)
	if(MSVC)
		set(EXTERNAL_BIN_DIR "${EXTERNAL_SOURCE_DIR}/install/msvc/bin")

		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${EXTERNAL_BIN_DIR}/cglm-0$<$<CONFIG:Debug>:d>.dll" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/cglm-0$<$<CONFIG:Debug>:d>.dll"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${EXTERNAL_BIN_DIR}/glad$<$<CONFIG:Debug>:d>.dll"   "$<TARGET_FILE_DIR:${PROJECT_NAME}>/glad$<$<CONFIG:Debug>:d>.dll"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${EXTERNAL_BIN_DIR}/glfw3$<$<CONFIG:Debug>:d>.dll"  "$<TARGET_FILE_DIR:${PROJECT_NAME}>/glfw3$<$<CONFIG:Debug>:d>.dll"
			VERBATIM
		)
	elseif(MINGW)
		set(EXTERNAL_BIN_DIR "${EXTERNAL_SOURCE_DIR}/install/mingw/bin")

		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${EXTERNAL_BIN_DIR}/libcglm-0$<$<CONFIG:Debug>:d>.dll" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libcglm-0$<$<CONFIG:Debug>:d>.dll"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${EXTERNAL_BIN_DIR}/libglad$<$<CONFIG:Debug>:d>.dll"   "$<TARGET_FILE_DIR:${PROJECT_NAME}>/libglad$<$<CONFIG:Debug>:d>.dll"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${EXTERNAL_BIN_DIR}/glfw3$<$<CONFIG:Debug>:d>.dll"     "$<TARGET_FILE_DIR:${PROJECT_NAME}>/glfw3$<$<CONFIG:Debug>:d>.dll"
			VERBATIM
		)
	endif()
endif()