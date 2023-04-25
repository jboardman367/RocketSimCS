#include "Sim/Car/Car.h"
#include "Sim/Ball/Ball.h"
#include "Sim/Arena/Arena.h"

#include "Math/Math.h"

// AVAILABLE DEFS FOR ROCKETSIM:
//	RS_MAX_SPEED: Define this to remove certain sanity checks for faster speed
//	RS_DONT_LOG: Define this to disable all logging output
//	RS_NO_SUSPCOLGRID: Disable the suspension-collision grid optimization

struct btBvhTriangleMeshShape;

enum class RocketSimStage : byte {
	UNINITIALIZED,
	INITIALIZING,
	INITIALIZED
};

namespace RocketSim {

	RSAPI void StringInit(const char* collisionMeshesFolder);
	void Init(std::filesystem::path collisionMeshesFolder);
	void AssertInitialized(const char* errorMsgPrefix);

	RSAPI RocketSimStage GetStage();

	const vector<btBvhTriangleMeshShape*>& GetArenaCollisionShapes();

#ifndef RS_NO_SUSPCOLGRID
	const SuspensionCollisionGrid& GetDefaultSuspColGrid();
#endif
}