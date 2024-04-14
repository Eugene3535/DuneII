cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(DuneII LANGUAGES C CXX)

set(EXTERNAL_DIR $ENV{External})

set(SRC_FILES
	${PROJECT_SOURCE_DIR}/src/utils/FileProvider.cpp
	${PROJECT_SOURCE_DIR}/src/loaders/Assets.cpp
	${PROJECT_SOURCE_DIR}/src/effects/ScreenBlackoutEffect.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/Scene.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/intro/MainMenu.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/Mission.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/utils/TileMap.cpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/utils/TileAnimator.cpp
	${PROJECT_SOURCE_DIR}/src/main.cpp
)

set(HDR_FILES
	${PROJECT_SOURCE_DIR}/src/utils/FileProvider.hpp
	${PROJECT_SOURCE_DIR}/src/loaders/Assets.hpp
	${PROJECT_SOURCE_DIR}/src/scenes/Scene.hpp
	${PROJECT_SOURCE_DIR}/src/scenes/intro/MainMenu.hpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/Mission.hpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/utils/TileMap.hpp
	${PROJECT_SOURCE_DIR}/src/scenes/mission/utils/TileAnimator.hpp
	${PROJECT_SOURCE_DIR}/src/Game/Game.hpp
)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

if((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID MATCHES "GNU"))
	add_compile_options(-ffast-math)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options(/fp:fast)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

	set(VISUALIZER_ALL_FILES 
		${VISUALIZER_SRC_FILES} 
		${VISUALIZER_HDR_FILES})

	foreach(source IN LISTS VISUALIZER_ALL_FILES)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "/" "\\" source_path_msvc "${source_path}")
		source_group("${source_path_msvc}" FILES "${source}")
	endforeach()

endif()

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES})

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
