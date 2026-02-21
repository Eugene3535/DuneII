# OpenGL Resources
set(RESOURCE_SRC_FILES
	src/resources/files/FileProvider.cpp
	# src/resources/files/Font.cpp
	src/resources/files/Shader.cpp
	src/resources/files/StbImage.cpp
	src/resources/gl_interfaces/buffers/GlBuffer.cpp
	src/resources/gl_interfaces/shaders/ShaderProgram.cpp
	src/resources/gl_interfaces/texture/Texture.cpp
	src/resources/gl_interfaces/vao/attributes/VertexBufferLayout.cpp
	src/resources/gl_interfaces/vao/VertexArrayObject.cpp
)

set(RESOURCE_HDR_FILES
	src/resources/files/FileProvider.hpp
	# src/resources/files/Font.hpp
	src/resources/files/Shader.hpp
	src/resources/files/StbImage.hpp
	src/resources/gl_interfaces/buffers/GlBuffer.hpp
	src/resources/gl_interfaces/shaders/ShaderProgram.hpp
	src/resources/gl_interfaces/texture/Texture.hpp
	src/resources/gl_interfaces/vao/attributes/VertexBufferLayout.hpp
	src/resources/gl_interfaces/vao/VertexArrayObject.hpp
)


# Game scenes
set(SCENE_SRC_FILES
	src/game/scenes/Scene.cpp
	src/game/scenes/intro/interactive_elements/Button.cpp
	src/game/scenes/intro/TitleScreen.cpp
	src/game/scenes/pick_house/PickHouse.cpp
	src/game/scenes/mission/tilemap/Tilemap.cpp
	src/game/scenes/mission/loader/TiledMapLoader.cpp
	src/game/scenes/mission/HUD/construction/ConstructionMenu.cpp
	src/game/scenes/mission/HUD/HeadUpDisplay.cpp
	src/game/scenes/mission/Mission.cpp
)

set(SCENE_HDR_FILES
	src/game/scenes/Scene.hpp
	src/game/scenes/intro/interactive_elements/Button.hpp
	src/game/scenes/intro/TitleScreen.hpp
	src/game/scenes/pick_house/PickHouse.hpp
	src/game/scenes/mission/common/Structures.hpp
	src/game/scenes/mission/common/Units.hpp
	src/game/scenes/mission/tilemap/Tilemap.hpp
	src/game/scenes/mission/loader/TiledMapLoader.hpp
	src/game/scenes/mission/HUD/construction/ConstructionMenu.hpp
	src/game/scenes/mission/HUD/HeadUpDisplay.hpp
	src/game/scenes/mission/Mission.hpp
)


# Common
set(DUNEII_SRC_FILES
	src/common/ExternalMath.cpp
	src/common/info/GameInfo.cpp
	src/graphics/transform/Transform2D.cpp
	src/graphics/camera/OrthogonalCamera.cpp
	src/graphics/sprites/SpriteManager.cpp
	src/graphics/geometry/GeometryGenerator.cpp
	${RESOURCE_SRC_FILES}
	# src/effects/blackout/ScreenBlackoutEffect.cpp
	${SCENE_SRC_FILES}
	src/app/Game.cpp
	src/game/Engine.cpp
	src/main.cpp
)

set(DUNEII_HDR_FILES
	src/common/Assets.hpp
	src/common/Enums.hpp
	src/common/ExternalMath.hpp
	src/common/info/GameInfo.hpp
	src/graphics/Meshes.hpp
	src/graphics/transform/Transform2D.hpp
	src/graphics/camera/OrthogonalCamera.hpp
	src/graphics/sprites/SpriteManager.hpp
	src/graphics/geometry/GeometryGenerator.hpp
	${RESOURCE_HDR_FILES}
	# src/effects/blackout/ScreenBlackoutEffect.hpp
	${SCENE_HDR_FILES}
	src/app/Game.hpp
	src/game/Engine.hpp
	src/game/Engine.inl
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
)

source_group("shaders" FILES ${SHADER_FILES})


set(DUNEII_ALL_FILES 
	${DUNEII_SRC_FILES} 
	${DUNEII_HDR_FILES}
)


unset(RESOURCE_SRC_FILES)
unset(RESOURCE_HDR_FILES)
unset(SCENE_SRC_FILES)
unset(SCENE_HDR_FILES)
unset(DUNEII_SRC_FILES)
unset(DUNEII_HDR_FILES)