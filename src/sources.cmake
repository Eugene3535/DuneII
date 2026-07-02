# Files
set(FILE_SRC_FILES
	src/files/FileProvider.hpp
	src/files/FileProvider.cpp
	# src/files/Font.hpp
	# src/files/Font.cpp
	src/files/Shader.hpp
	src/files/Shader.cpp
	src/files/StbImage.hpp
	src/files/StbImage.cpp
)


# Graphics
set(GRAPHICS_SRC_FILES
	src/graphics/texture/Texture2D.hpp
	src/graphics/texture/Texture2D.cpp
	src/graphics/vao/VertexBufferLayout.hpp
	src/graphics/vao/VertexBufferLayout.cpp
	src/graphics/shaders/ShaderProgram.hpp
	src/graphics/shaders/ShaderProgram.cpp
	src/graphics/transform/Transform2D.hpp
	src/graphics/transform/Transform2D.cpp
	src/graphics/camera/OrthoMatrix.hpp
	src/graphics/camera/OrthoMatrix.cpp
	src/graphics/sprites/SpriteManager.hpp
	src/graphics/sprites/SpriteManager.cpp
	src/graphics/geometry/GeometryGenerator.hpp
	src/graphics/geometry/GeometryGenerator.cpp
)


# Scenes
set(SCENE_SRC_FILES
	src/scenes/Scene.hpp
	src/scenes/Scene.cpp
	src/scenes/intro/interactive_elements/Button.hpp
	src/scenes/intro/interactive_elements/Button.cpp
	src/scenes/intro/TitleScreen.hpp
	src/scenes/intro/TitleScreen.cpp
	src/scenes/pick_house/PickHouse.hpp
	src/scenes/pick_house/PickHouse.cpp
	src/scenes/mission/common/Structures.hpp
	src/scenes/mission/common/Units.hpp
	src/scenes/mission/loader/TiledMapLoader.hpp # TODO: move to file folder
	src/scenes/mission/loader/TiledMapLoader.cpp
	src/scenes/mission/tilemap/Tilemap.hpp
	src/scenes/mission/tilemap/Tilemap.cpp
	src/scenes/mission/HUD/construction/ConstructionMenu.hpp
	src/scenes/mission/HUD/construction/ConstructionMenu.cpp
	src/scenes/mission/HUD/previews/EntityPreview.hpp
	src/scenes/mission/HUD/previews/EntityPreview.cpp
	src/scenes/mission/HUD/HeadUpDisplay.hpp
	src/scenes/mission/HUD/HeadUpDisplay.cpp
	src/scenes/mission/Mission.hpp
	src/scenes/mission/Mission.cpp
)


# Common
set(COMMON_SRC_FILES
	src/common/NonCopyable.hpp
	src/common/NonMovable.hpp
	src/common/MemoryAllocator.hpp
	src/common/MemoryAllocator.inl
	src/common/Assets.hpp
	src/common/Enums.hpp
	src/common/ExternalMath.hpp
	src/common/ExternalMath.cpp
	src/common/info/GameInfo.hpp
	src/common/info/GameInfo.cpp
)

# Application
set(DUNEII_SRC_FILES
	${COMMON_SRC_FILES}
	${FILE_SRC_FILES}
	${GRAPHICS_SRC_FILES}
	${SCENE_SRC_FILES}
	src/application/window/MainWindow.hpp
	src/application/window/MainWindow.cpp
	src/application/context/Context.hpp
	src/application/context/Context.cpp
	src/application/game/Game.hpp
	src/application/game/Game.inl
	src/application/game/Game.cpp
	src/main.cpp
)


set(SHADER_FILES
	${PROJECT_SOURCE_DIR}/src/shaders/color_outline.vert
	${PROJECT_SOURCE_DIR}/src/shaders/color_outline.frag
	${PROJECT_SOURCE_DIR}/src/shaders/color_sprite.vert
	${PROJECT_SOURCE_DIR}/src/shaders/color_sprite.frag
	${PROJECT_SOURCE_DIR}/src/shaders/sprite.vert
	${PROJECT_SOURCE_DIR}/src/shaders/sprite.frag
	${PROJECT_SOURCE_DIR}/src/shaders/tilemap.vert
	${PROJECT_SOURCE_DIR}/src/shaders/tilemap.frag
	${PROJECT_SOURCE_DIR}/src/shaders/entity_view.vert
	${PROJECT_SOURCE_DIR}/src/shaders/entity_view.frag
)

source_group("shaders" FILES ${SHADER_FILES})