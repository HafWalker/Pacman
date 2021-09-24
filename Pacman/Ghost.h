#ifndef GHOST_H
#define GHOST_H

#include <list>
#include <ctime>

#include "MovableGameEntity.h"

class World;
class PathmapTile;
class Avatar;

class Ghost : public MovableGameEntity
{
public:
	enum GhostBehavior
	{
		Wander,
		Chase,
		Dead,
		Fear,
	};

	enum GhostType
	{
		Cyan,
		Orange,
		Pink,
		Red,
	};

	GhostType ghostType;

	int tileSize = 22;
	Vector2f wanderTarget;
	Vector2f forcedRespawnTarget;
	Vector2f currentDirection;
	Vector2f targetPosition;

	double clockDuration;

	Ghost(const Vector2f& aPosition, Sprite* entitySprite, GhostBehavior behavior, GhostType type);
	~Ghost(void);

	void Update(float aTime, World* aWorld, Avatar* avatar);

	bool myIsClaimableFlag;
	bool myIsDeadFlag;

	void SetImage(std::string anImage);

	void Die(World* aWorld);

protected:
	void SwitchGhostSpriteByType(GhostType type);
	void SwitchGhostSpriteByBehaviour(GhostBehavior behaviour);

	void BehaveWander(World* aWorld);
	void BehaveChase(World* aWorld, Avatar* avatar);
	void BehaveIntercept(World* aWorld, Avatar* avatar);
	void BehaveFear(World* aWorld, Avatar* avatar);
	void BehaveVulnerable();

	GhostBehavior myBehavior;
	int myDesiredMovementX;
	int myDesiredMovementY;

	std::list<PathmapTile*> myPath;

	void SetNextTileToTarget(World* aWorld, int targetX, int targetY);
	void PickRandomTileAsTarget(World* aWorld);

private:
	bool triggedInverseDirection = false;

	void InverseSubstractCurrentMoveDirection();

	void SubstractCurrentMoveDirection();
	void RemoveInvalidDirections(World* aWorld);
	void RemoveBiggestVector(World* aWorld);
	Vector2f GetPriorityVector();
	Vector2f GetRandomVectorFromAvailables();

	bool isInRespawnArea(int currentTileX, int currentTileY);

	std::clock_t timer;
	bool TriggedTimer = false;
	bool CheckTimer(double duration);
};

#endif // GHOST_H