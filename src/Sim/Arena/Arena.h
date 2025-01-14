#pragma once
#include "../../BaseInc.h"
#include "../Car/Car.h"
#include "../Ball/Ball.h"
#include "../BoostPad/BoostPad.h"

#include "../../CollisionMeshFile/CollisionMeshFile.h"
#include "../BoostPad/BoostPadGrid/BoostPadGrid.h"
#include "../SuspensionCollisionGrid/SuspensionCollisionGrid.h"

struct btDiscreteDynamicsWorld;
struct btCollisionConfiguration;
struct btCollisionDispatcher;
struct btDbvtBroadphase;
struct btSequentialImpulseConstraintSolver;

enum class GameMode : byte {
	SOCCAR,

	// No goals, boosts, or arena - cars/ball will fall infinitely, ball is frozen until touched
	THE_VOID, 
	// More coming soon!
};

typedef std::function<void(class Arena* arena, Team scoringTeam, void* userInfo)> GoalScoreEventFn;

// The container for all game simulation
// Stores cars, the ball, all arena collisions, and manages the overall game state
class Arena {
public:

	GameMode gameMode;

	uint32_t _lastCarID = 0;
	vector<Car*> _cars;
	
	Ball* ball;

	vector<BoostPad*> _boostPads;
	BoostPadGrid _boostPadGrid;

	SuspensionCollisionGrid _suspColGrid;

	// Time in seconds each tick (1/tickrate)
	float tickTime; 

	// Returns (1 / tickTime)
	float GetTickRate() {
		return 1 / tickTime;
	}

	// Total ticks this arena instance has been simulated for, never resets
	uint64_t tickCount = 0;

	RSAPI const vector<Car*>& GetCars() { return _cars; }
	RSAPI const vector<BoostPad*>& GetBoostPads() { return _boostPads; }

	RSAPI Car* AddCar(Team team, const CarConfig& config = CAR_CONFIG_OCTANE);

	// Returns false if the car was not found in the cars list
	// NOTE: If the car was removed, the car will be freed and the pointer will be made invalid
	RSAPI bool RemoveCar(Car* car);

	RSAPI Car* GetCarFromID(uint32_t id);

	btDiscreteDynamicsWorld* _bulletWorld;
	struct {
		btCollisionConfiguration* collisionConfig;
		btCollisionDispatcher* collisionDispatcher;
		btDbvtBroadphase* overlappingPairCache;
		btSequentialImpulseConstraintSolver* constraintSolver;
	} _bulletWorldParams;

	vector<struct btRigidBody*> _worldCollisionRBs;
	vector<struct btCollisionShape*> _worldCollisionShapes;

	struct {
		GoalScoreEventFn func = NULL;
		void* userInfo = NULL;
	} _goalScoreCallback;
	RSAPI void SetGoalScoreCallback(GoalScoreEventFn callbackFn, void* userInfo = NULL);

	// NOTE: Arena should be destroyed after use
	RSAPI static Arena* Create(GameMode gameMode, float tickRate = 120);
	
	// Serialize cars, ball, and boostpads to a file
	RSAPI void WriteToFile(std::filesystem::path path);

	// Create a new arena from a file written by Arena.WriteToFile()
	RSAPI static Arena* LoadFromFile(std::filesystem::path path);

	Arena(const Arena& other) = delete; // No copy constructor, use Arena::Clone() instead
	Arena& operator =(const Arena& other) = delete; // No copy operator, use Arena::Clone() instead

	
	Arena(Arena&& other) = delete; // No move constructor
	Arena& operator =(Arena&& other) = delete; // No move operator

	// Get a deep copy of the arena
	RSAPI Arena* Clone(bool copyCallbacks);

	RSAPI static void SerializeCar(DataStreamOut& out, Car* car);
	RSAPI Car* DeserializeNewCar(DataStreamIn& in, Team team);

	// Simulate everything in the arena for a given number of ticks
	RSAPI void Step(int ticksToSimulate = 1);

	RSAPI void ResetToRandomKickoff(int seed = -1);

	// Free all associated memory
	RSAPI ~Arena();

	// NOTE: Passed shape pointer will be freed when arena is deconstructed
	btRigidBody* _AddStaticCollisionShape(btCollisionShape* shape, bool isOwner, btVector3 posBT = btVector3(0,0,0));
	void _SetupArenaCollisionShapes();

	// Static function called by Bullet internally when adding a collision point
	static bool _BulletContactAddedCallback(struct btManifoldPoint& cp, const struct btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const struct btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

	void _BtCallback_OnCarBallCollision(Car* car, Ball* ball, struct btManifoldPoint& manifoldPoint, bool ballIsBodyA);
	void _BtCallback_OnCarCarCollision(Car* car1, Car* car2, struct btManifoldPoint& manifoldPoint);
	void _BtCallback_OnCarWorldCollision(Car* car, struct btCollisionObject* worldObject, struct btManifoldPoint& manifoldPoint);

private:
	
	// Constructor for use by Arena::Create()
	Arena(GameMode gameMode, float tickRate = 120);
};