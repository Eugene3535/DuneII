# OpenGL Resources
set(RESOURCE_SRC_FILES
	src/resources/files/FileProvider.cpp
)

set(RESOURCE_HDR_FILES
	src/resources/files/FileProvider.hpp
	src/resources/files/assets/AssetManager.hpp
	src/resources/files/assets/AssetManager.inl
)


# Game scenes
set(SCENE_SRC_FILES
	src/game/scenes/Scene.cpp
	src/game/scenes/intro/interactive_elements/Button.cpp
	src/game/scenes/intro/TitleScreen.cpp
	src/game/scenes/pick_house/PickHouse.cpp
	src/game/scenes/mission/tilemap/Tilemap.cpp
	src/game/scenes/mission/loader/TiledMapLoader.cpp
	# src/game/scenes/mission/HUD/construction/ConstructionMenu.cpp
	# src/game/scenes/mission/HUD/HeadUpDisplay.cpp
	src/game/scenes/mission/Mission.cpp
)

set(SCENE_HDR_FILES
	src/game/scenes/Scene.hpp
	src/game/scenes/intro/interactive_elements/Button.hpp
	src/game/scenes/intro/TitleScreen.hpp
	src/game/scenes/pick_house/PickHouse.hpp
	src/game/scenes/mission/common/Structures.hpp
	# src/game/scenes/mission/common/Units.hpp
	src/game/scenes/mission/tilemap/Tilemap.hpp
	src/game/scenes/mission/loader/TiledMapLoader.hpp
	# src/game/scenes/mission/HUD/construction/ConstructionMenu.hpp
	# src/game/scenes/mission/HUD/HeadUpDisplay.hpp
	src/game/scenes/mission/Mission.hpp
)


# Common
set(DUNEII_SRC_FILES
	# src/common/ExternalMath.cpp
	src/common/info/GameInfo.cpp
	src/graphics/quad/Quad.cpp
	# src/graphics/transform/Transform2D.cpp
	# src/graphics/camera/OrthogonalCamera.cpp
	# src/graphics/sprites/SpriteManager.cpp
	# src/graphics/geometry/GeometryGenerator.cpp
	src/graphics/effects/blackout/ScreenBlackoutEffect.cpp
	src/graphics/effects/particles/ParticleSystem.cpp
	${RESOURCE_SRC_FILES}
	${SCENE_SRC_FILES}
	src/game/DuneII.cpp
	src/main.cpp
)

set(DUNEII_HDR_FILES
	# src/common/Assets.hpp
	# src/common/Enums.hpp
	# src/common/ExternalMath.hpp
	src/common/info/GameInfo.hpp
	src/graphics/quad/Quad.hpp
	# src/graphics/Meshes.hpp
	# src/graphics/transform/Transform2D.hpp
	# src/graphics/camera/OrthogonalCamera.hpp
	# src/graphics/sprites/SpriteManager.hpp
	# src/graphics/geometry/GeometryGenerator.hpp
	src/graphics/effects/blackout/ScreenBlackoutEffect.hpp
	src/graphics/effects/particles/ParticleSystem.hpp
	${RESOURCE_HDR_FILES}
	${SCENE_HDR_FILES}
	src/game/DuneII.hpp
	src/game/DuneII.inl
)


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