# OpenGL Resources
set(RESOURCE_SRC_FILES
	src/resources/files/FileProvider.cpp
	# src/resources/files/Font.cpp
	src/resources/files/Shader.cpp
	src/resources/files/StbImage.cpp
	src/resources/ogl/buffers/GlBuffer.cpp
	src/resources/ogl/holder/GlResourceManager.cpp
	src/resources/ogl/shaders/ShaderProgram.cpp
	src/resources/ogl/texture/Texture.cpp
	src/resources/ogl/vao/attributes/VertexBufferLayout.cpp
	src/resources/ogl/vao/VertexArrayObject.cpp
)

set(RESOURCE_HDR_FILES
	src/resources/files/FileProvider.hpp
	# src/resources/files/Font.hpp
	src/resources/files/Shader.hpp
	src/resources/files/StbImage.hpp
	src/resources/ogl/buffers/GlBuffer.hpp
	src/resources/ogl/holder/GlResourceManager.hpp
	src/resources/ogl/shaders/ShaderProgram.hpp
	src/resources/ogl/texture/Texture.hpp
	src/resources/ogl/vao/attributes/VertexBufferLayout.hpp
	src/resources/ogl/vao/VertexArrayObject.hpp
)


# Game scenes
set(SCENE_SRC_FILES
	src/game/scenes/Scene.cpp
	src/game/scenes/intro/interactive_elements/Button.cpp
	src/game/scenes/intro/TitleScreen.cpp
	# src/game/scenes/choosing_houses/Destiny.cpp
	# src/game/scenes/mission/Mission.cpp
	# src/game/scenes/mission/tilemap/Tile.cpp
	# src/game/scenes/mission/tilemap/TileMap.cpp
)

set(SCENE_HDR_FILES
	src/game/scenes/Scene.hpp
	src/game/scenes/intro/interactive_elements/Button.hpp
	src/game/scenes/intro/TitleScreen.hpp
	# src/game/scenes/choosing_houses/Destiny.hpp
	# src/game/scenes/mission/Mission.hpp
	# src/game/scenes/mission/tilemap/Tile.hpp
	# src/game/scenes/mission/tilemap/TileMap.hpp
)


# Common
set(DUNEII_SRC_FILES
	src/common/ExternalMath.cpp
	src/graphics/Transform2D.cpp
	src/graphics/OrthogonalCamera.cpp
	src/graphics/sprites/SpriteManager.cpp
	${RESOURCE_SRC_FILES}
	# src/effects/blackout/ScreenBlackoutEffect.cpp
	${SCENE_SRC_FILES}
	src/app/Application.cpp
	src/game/DuneII.cpp
	src/main.cpp
)

set(DUNEII_HDR_FILES
	src/common/Assets.hpp
	src/common/Enums.hpp
	src/common/ExternalMath.hpp
	src/graphics/Meshes.hpp
	src/graphics/Transform2D.hpp
	src/graphics/OrthogonalCamera.hpp
	src/graphics/sprites/SpriteManager.hpp
	${RESOURCE_HDR_FILES}
	# src/effects/blackout/ScreenBlackoutEffect.hpp
	${SCENE_HDR_FILES}
	src/app/Application.hpp
	src/game/DuneII.hpp
	src/game/DuneII.inl
)


set(SHADER_FILES
	${PROJECT_SOURCE_DIR}/src/shaders/sprite.vert
	${PROJECT_SOURCE_DIR}/src/shaders/sprite.frag
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