cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(DuneII LANGUAGES C CXX)

set(EXTERNAL_DIR $ENV{External})
set(BUILD_SHARED_LIBS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(GLOB_RECURSE DUNE_II_SOURCE_FILES RELATIVE 
	"${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

if(MSVC)
	foreach(source IN LISTS DUNE_II_SOURCE_FILES)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "/" "\\" source_path_msvc "${source_path}")
		source_group("${source_path_msvc}" FILES "${source}")
	endforeach()
endif()

add_executable(${PROJECT_NAME} ${DUNE_II_SOURCE_FILES})
add_subdirectory(${EXTERNAL_DIR}/sfml sfml)

if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

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
