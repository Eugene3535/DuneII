cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(DuneII LANGUAGES C CXX)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

set(EXTERNAL_DIR $ENV{External})

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

if((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID MATCHES "GNU"))
	add_compile_options(-ffast-math)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options(/fp:fast)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	"${EXTERNAL_DIR}/install/sfml/include"
	"${EXTERNAL_DIR}/rapidxml"
	"${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(${PROJECT_NAME}
	${EXTERNAL_DIR}/install/sfml/lib/sfml-graphics$<$<CONFIG:Debug>:-d>.lib
	${EXTERNAL_DIR}/install/sfml/lib/sfml-window$<$<CONFIG:Debug>:-d>.lib
	${EXTERNAL_DIR}/install/sfml/lib/sfml-system$<$<CONFIG:Debug>:-d>.lib
	${EXTERNAL_DIR}/install/sfml/lib/sfml-audio$<$<CONFIG:Debug>:-d>.lib
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res             $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
	COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_DIR}/install/sfml/bin/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/openal32.dll)

	if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_DIR}/install/sfml/bin/sfml-graphics-$<$<CONFIG:Debug>:d->2.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/sfml-graphics-$<$<CONFIG:Debug>:d->2.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_DIR}/install/sfml/bin/sfml-system-$<$<CONFIG:Debug>:d->2.dll   $<TARGET_FILE_DIR:${PROJECT_NAME}>/sfml-system-$<$<CONFIG:Debug>:d->2.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_DIR}/install/sfml/bin/sfml-window-$<$<CONFIG:Debug>:d->2.dll   $<TARGET_FILE_DIR:${PROJECT_NAME}>/sfml-window-$<$<CONFIG:Debug>:d->2.dll
		COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_DIR}/install/sfml/bin/sfml-audio-$<$<CONFIG:Debug>:d->2.dll    $<TARGET_FILE_DIR:${PROJECT_NAME}>/sfml-audio-$<$<CONFIG:Debug>:d->2.dll
    )
endif(WIN32)
