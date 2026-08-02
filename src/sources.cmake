# Files
set(FILE_SRC_FILES
	src/files/FileProvider.cpp
	src/files/FileProvider.hpp
	# src/files/Font.cpp
	# src/files/Font.hpp
	src/files/Shader.cpp
	src/files/Shader.hpp
	src/files/StbImage.cpp
	src/files/StbImage.hpp
	src/files/TiledMapLoader.cpp
	src/files/TiledMapLoader.hpp
)


# Graphics
set(GRAPHICS_SRC_FILES
	src/graphics/texture/Texture2D.cpp
	src/graphics/texture/Texture2D.hpp
	src/graphics/vao/VertexBufferLayout.cpp
	src/graphics/vao/VertexBufferLayout.hpp
	src/graphics/transform/Transform2D.cpp
	src/graphics/transform/Transform2D.hpp
	src/graphics/camera/OrthoMatrix.cpp
	src/graphics/camera/OrthoMatrix.hpp
	src/graphics/resources/GlResourceManager.cpp
	src/graphics/resources/GlResourceManager.hpp
	src/graphics/sprites/SpriteManager.cpp
	src/graphics/sprites/SpriteManager.hpp
	src/graphics/geometry/GeometryGenerator.cpp
	src/graphics/geometry/GeometryGenerator.hpp
)


# Scenes
set(SCENE_SRC_FILES
	src/scenes/Scene.cpp
	src/scenes/Scene.hpp
	src/scenes/SceneManager.cpp
	src/scenes/SceneManager.hpp
	src/scenes/intro/interactive_elements/Button.cpp
	src/scenes/intro/interactive_elements/Button.hpp
	src/scenes/intro/TitleScreen.cpp
	src/scenes/intro/TitleScreen.hpp
	src/scenes/pick_house/PickHouse.cpp
	src/scenes/pick_house/PickHouse.hpp
	src/scenes/mission/data/Structures.hpp
	src/scenes/mission/data/Units.hpp
	src/scenes/mission/tilemap/TileMap.cpp
	src/scenes/mission/tilemap/TileMap.hpp
	src/scenes/mission/HUD/construction/ConstructionMenu.cpp
	src/scenes/mission/HUD/construction/ConstructionMenu.hpp
	src/scenes/mission/HUD/previews/EntityPreview.cpp
	src/scenes/mission/HUD/previews/EntityPreview.hpp
	src/scenes/mission/HUD/HeadUpDisplay.cpp
	src/scenes/mission/HUD/HeadUpDisplay.hpp
	src/scenes/mission/Mission.cpp
	src/scenes/mission/Mission.hpp
)


# Common
set(COMMON_SRC_FILES
	src/common/Assets.hpp
	src/common/Enums.hpp
	src/common/math/ExternalMath.cpp
	src/common/math/ExternalMath.hpp
	src/common/info/GameInfo.cpp
	src/common/info/GameInfo.hpp
	src/common/allocator/MemoryAllocator.hpp
	src/common/allocator/MemoryAllocator.inl
	src/common/action/Action.cpp
	src/common/action/Action.hpp
	src/common/action/ActionData.hpp
	src/common/action/ActionManager.cpp
	src/common/action/ActionManager.hpp
)

# Application
set(DUNEII_SRC_FILES
	${COMMON_SRC_FILES}
	${FILE_SRC_FILES}
	${GRAPHICS_SRC_FILES}
	${SCENE_SRC_FILES}
	src/app/window/WindowData.hpp
	src/app/window/MainWindow.cpp
	src/app/window/MainWindow.hpp
	src/app/context/Context.cpp
	src/app/context/Context.hpp
	src/app/loop/MainLoop.cpp
	src/app/loop/MainLoop.hpp
	src/app/game/Game.hpp
	src/main.cpp
)


set(SHADER_FILES
	${PROJECT_SOURCE_DIR}/src/shaders/color_outline.vert
	${PROJECT_SOURCE_DIR}/src/shaders/color_outline.frag
	${PROJECT_SOURCE_DIR}/src/shaders/color_sprite.frag
	${PROJECT_SOURCE_DIR}/src/shaders/selection.vert
	${PROJECT_SOURCE_DIR}/src/shaders/selection.frag
	${PROJECT_SOURCE_DIR}/src/shaders/sprite.vert
	${PROJECT_SOURCE_DIR}/src/shaders/sprite.frag
	${PROJECT_SOURCE_DIR}/src/shaders/entity_view.frag
)

source_group("shaders" FILES ${SHADER_FILES})