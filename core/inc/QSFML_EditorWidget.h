#pragma once

/// USER_SECTION_START 1

/// USER_SECTION_END

#include "QSFML_EditorWidget_info.h"

/// USER_SECTION_START 2
#define _USE_MATH_DEFINES
#include <math.h>

#include "Scene/Scene.h"

#include "components/base/Component.h"
#include "components/base/Drawable.h"
#include "components/base/SfEventHandle.h"
#include "components/base/AbstractPressEvent.h"
#include "components/Physics/Collider.h"
#include "components/drawable/Text.h"
#include "components/drawable/LinePainter.h"
#include "components/drawable/PathPainter.h"
#include "components/drawable/PointPainter.h"
#include "components/drawable/VectorPainter.h"
#include "components/drawable/VectorFieldPainter.h"
#include "components/drawable/PixelPainter.h"
#include "components/drawable/RectPainter.h"
#include "components/drawable/Image.h"
#include "components/drawable/Shape.h"
#include "components/Button.h"
#include "components/KeyPressEvent.h"
#include "components/MouseFollower.h"
#include "components/MousePressEvent.h"
#include "components/TextureAnimation.h"


#include "objects/base/GameObject.h"
#include "objects/DefaultEditor.h"
#include "objects/BackgroundGrid.h"
#include "objects/CameraController.h"
#include "objects/VisibleCamera.h"
#include "objects/LineChart.h"
#include "objects/RuntimeInfo.h"
#include "objects/CameraWindow.h"


#include "utilities/AABB.h"
#include "utilities/Ray.h"
#include "utilities/PathFinder.h"
#include "utilities/Updatable.h"
#include "utilities/VectorOperations.h"
#include "utilities/RandomEngine.h"
#include "utilities/Stats.h"
#include "utilities/ObjectQuadTree.h"
#include "utilities/Origin.h"
#include "utilities/PerlinNoise.h"
#include "utilities/Color.h"
#include "utilities/LifetimeChecker.h"
#include "utilities/CameraRecorder.h"
#include "utilities/TileMap/Chunk.h"
#include "utilities/TileMap/ChunkData.h"
#include "utilities/TileMap/ChunkManager.h"

#include "assets/TextureManager.h"
#include "assets/TextureMap.h"

#if FAST_NOISE_LITE_LIBRARY_AVAILABLE == 1
	#include "FastNoiseLite.h"
#endif

#if IMGUI_SFML_LIBRARY_AVAILABLE == 1
	#include "imgui.h"
	#if IMPLOT_LIBRARY_AVAILABLE == 1
		#include "implot.h"
	#endif
#endif
/// USER_SECTION_END