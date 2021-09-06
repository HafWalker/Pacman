#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "Avatar.h"
#include <iostream>

Ghost::Ghost(const Vector2f& aPosition, Sprite* entitySprite, GhostBehavior behavior, GhostType type)
	: MovableGameEntity(aPosition, entitySprite),
	myBehavior(behavior)
{
	ghostType = type;
	
	switch (type)
	{
	case Ghost::Cyan:
		std::cout << "Cyan" << std::endl;
		wanderTarget = Vector2f(25.f * tileSize,0.f);
		forcedRespawnTarget = Vector2f(14.f * tileSize, 10.f * tileSize);
		std::cout << wanderTarget.myX << "/" << wanderTarget.myY << std::endl;
		break;
	case Ghost::Orange:
		std::cout << "Orange" << std::endl;
		wanderTarget = Vector2f(0.f, 28.f * tileSize);
		forcedRespawnTarget = Vector2f(11.f * tileSize, 10.f * tileSize);
		std::cout << wanderTarget.myX << "/" << wanderTarget.myY << std::endl;
		break;
	case Ghost::Pink:
		std::cout << "Pink" << std::endl;
		wanderTarget = Vector2f(0.f, 0.f);
		forcedRespawnTarget = Vector2f(11.f * tileSize, 10.f * tileSize);
		std::cout << wanderTarget.myX << "/" << wanderTarget.myY << std::endl;
		break;
	case Ghost::Red:
		std::cout << "Red" << std::endl;
		wanderTarget = Vector2f(25.f * tileSize, 28.f * tileSize);
		forcedRespawnTarget = Vector2f(14.f * tileSize, 10.f * tileSize);
		std::cout << wanderTarget.myX << "/" << wanderTarget.myY << std::endl;
		break;
	default:
		break;
	}

	SwitchGhostSpriteByType(ghostType);

	myIsClaimableFlag = false;
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;
}

Ghost::~Ghost(void)
{
}

void Ghost::Die(World* aWorld)
{
	myPath.clear();
	aWorld->GetPath(myCurrentTileX, myCurrentTileY, 13, 13, myPath);
}

void Ghost::Update(float aTime, World* aWorld, Avatar* avatar)
{
	float speed = 100.f; // 30.f original

	//std::cout << nextTileX << std::endl;
	if (myIsDeadFlag)
		speed = 120.f;

	/*
	if (IsAtDestination())
	{
		if (!myPath.empty())
		{
			PathmapTile* nextTile = myPath.front();
			myPath.pop_front();
			SetNextTile(nextTile->myX, nextTile->myY);
		}
		else if (aWorld->TileIsValid(nextTileX, nextTileY))
		{
			SetNextTile(nextTileX, nextTileY);
		}
		else
		{
			if (myIsClaimableFlag)
				BehaveVulnerable();
			else
			{
				switch (myBehavior)
				{
				case Chase:
					BehaveChase(aWorld, avatar);
					break;
				case Intercept:
				case Fear:
				case Wander:
					//std::cout << "WANDERER" << std::endl;
					//SetNextTileToTarget(aWorld, wanderTarget.myX, wanderTarget.myY);
				default:
					
					break;
				}
			}

			myIsDeadFlag = false;
		}
	}
	*/

	tileSize = 22;
	Vector2f destination(myNextTileX * tileSize, myNextTileY * tileSize);
	currentDirection = destination - myPosition;

	float distanceToMove = aTime * speed;

	if (distanceToMove > currentDirection.Length())
	{
		myPosition = destination;
		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
		
		if (!isInRespawnArea(myCurrentTileX, myCurrentTileY)) {
			SetNextTileToTarget(aWorld, wanderTarget.myX, wanderTarget.myY);
		}
		else {
			SetNextTileToTarget(aWorld, forcedRespawnTarget.myX, forcedRespawnTarget.myY);
		}
	}
	else
	{
		currentDirection.Normalize();
		myPosition += currentDirection * distanceToMove;
	}
}

void Ghost::SetImage(std::string anImage)
{
	sprite->SetFrame(anImage);
}

void Ghost::SwitchGhostSpriteByType(GhostType type)
{
	switch (type)
	{
	case Ghost::Cyan:
		sprite->SetFrame("ghost_32_cyan.png");
		break;
	case Ghost::Orange:
		sprite->SetFrame("ghost_32_orange.png");
		break;
	case Ghost::Pink:
		sprite->SetFrame("ghost_32_pink.png");
		break;
	case Ghost::Red:
		sprite->SetFrame("ghost_32_red.png");
		break;
	default:
		break;
	}
}

void Ghost::BehaveWander()
{
	MovementDirection nextDirection = (MovementDirection)(rand() % DirectionCount);
	switch (nextDirection)
	{
	case Up:
		myDesiredMovementX = 0;
		myDesiredMovementY = 1;
		break;
	case Down:
		myDesiredMovementX = 0;
		myDesiredMovementY = -1;
		break;
	case Left:
		myDesiredMovementX = -1;
		myDesiredMovementY = 0;
		break;
	case Right:
		myDesiredMovementX = 1;
		myDesiredMovementY = 0;
		break;
	default:
		break;
	}
}

void Ghost::BehaveChase(World* aWorld, Avatar* avatar)
{
	//std::cout << "CHASE" << std::endl;
	myPath.clear();
	aWorld->GetPath(myCurrentTileX, myCurrentTileY, avatar->GetCurrentTileX(), avatar->GetCurrentTileY(), myPath);
}

void Ghost::BehaveIntercept(World* aWorld, Avatar* avatar)
{

}

void Ghost::BehaveFear(World* aWorld, Avatar* avatar)
{

}

void Ghost::BehaveVulnerable()
{

}

void Ghost::SetNextTileToTarget(World* aWorld, int targetX, int targetY)
{
	//std::cout << "GHOST CURREN POS = " << "X:" << myCurrentTileX << " - " << "Y:" << myCurrentTileY << std::endl;

	posibleDirections.clear();
	posibleDirections.push_back(Vector2f(0, -1));	// UP
	posibleDirections.push_back(Vector2f(1, 0));	// RIGHT
	posibleDirections.push_back(Vector2f(0, 1));	// DOWN
	posibleDirections.push_back(Vector2f(-1, 0));	// LEFT

	targetPosition.myX = targetX;
	targetPosition.myY = targetY;

	SubstractCurrentMoveDirection();
	RemoveInvalidDirections(aWorld);
	if (posibleDirections.size() > 1) {
		RemoveBiggestVector(aWorld);
	}
	Vector2f nextMoveVector = GetPriorityVector();

	//std::cout << "DESIRED MOVEMENT = " << nextMoveVector.myX << " | " << nextMoveVector.myY << std::endl;
	myDesiredMovementX = nextMoveVector.myX;
	myDesiredMovementY = nextMoveVector.myY;

	int nextTileX = myCurrentTileX + myDesiredMovementX;
	int nextTileY = myCurrentTileY - myDesiredMovementY;

	SetNextTile(nextTileX, nextTileY);

	//std::cout << "NEXT TILE TARGET = " << "X:" << myNextTileX << " - " << "Y:" << myNextTileY << std::endl;
}

void Ghost::SubstractCurrentMoveDirection()
{
	currentDirection.Normalize();
	//std::cout << "CURRENT DIRECTION = " << currentDirection.myX << " | " << currentDirection.myY << std::endl;
	std::list<Vector2f>::const_iterator itr = posibleDirections.cbegin();
	while (itr != posibleDirections.cend())
	{
		if (-itr->myX == currentDirection.myX &&
			itr->myY == currentDirection.myY) {
			itr = posibleDirections.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void Ghost::RemoveInvalidDirections(World* aWorld)
{
	std::list<Vector2f>::const_iterator itr = posibleDirections.cbegin();

	while (itr != posibleDirections.cend())
	{
		int nextTileX = myCurrentTileX + itr->myX;
		int nextTileY = myCurrentTileY - itr->myY;
		//std::cout << aWorld->TileIsValid(nextTileX, nextTileY) << std::endl;
		if (!aWorld->TileIsValid(nextTileX, nextTileY)) {
			itr = posibleDirections.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void Ghost::RemoveBiggestVector(World* aWorld)
{
	//std::cout << "- - - - - - VECTOR REMOVE - - - - - " << std::endl;
	Vector2f vectorToRemove;
	float distanceToTarget;
	bool iterating = true;

	std::list<Vector2f>::const_iterator itr = posibleDirections.cbegin();
	while (iterating) {
		
		bool differentVectorsExist = false;
		float maxDistance = 0.f;

		while (itr != posibleDirections.cend())
		{
			int XDiference = pow(targetPosition.myX - (itr->myX + myPosition.myX), 2);
			int YDiference = pow(targetPosition.myY - (-itr->myY + myPosition.myY), 2);
			int sum = XDiference + YDiference;
			distanceToTarget = sqrt(sum);
			//std::cout << distanceToTarget << std::endl;

			if (distanceToTarget > maxDistance) {
				maxDistance = distanceToTarget;
				differentVectorsExist = true;
				vectorToRemove = Vector2f(itr->myX, itr->myY);
			}
			++itr;
		}

		if (differentVectorsExist) {
			differentVectorsExist = false;
			itr = posibleDirections.cbegin();
			while (itr != posibleDirections.cend())
			{
				if (itr->myX == vectorToRemove.myX &&
					itr->myY == vectorToRemove.myY) {
					itr = posibleDirections.erase(itr);
				}
				else {
					++itr;
				}
			}
		}
		else
		{
			iterating = false;
		}
	}
}

Vector2f Ghost::GetPriorityVector()
{
	bool vectorFound = false;
	Vector2f vectorResult;

	// Priority UP
	for each (Vector2f vectorDirection in posibleDirections)
	{
		if (vectorDirection.myX == 0 && vectorDirection.myY == 1) {
			vectorFound = true;
			vectorResult = vectorDirection;
		}
	}

	// Priority RIGHT
	if (!vectorFound) {
		for each (Vector2f vectorDirection in posibleDirections)
		{
			if (vectorDirection.myX == 1 && vectorDirection.myY == 0) {
				vectorFound = true;
				vectorResult = vectorDirection;
			}
		}
	}

	// Priority DOWN
	if (!vectorFound) {
		for each (Vector2f vectorDirection in posibleDirections)
		{
			if (vectorDirection.myX == 0 && vectorDirection.myY == -1) { // Priority DOWN
				vectorFound = true;
				vectorResult = vectorDirection;
			}
		}
	}

	// Priority LEFT
	if (!vectorFound) {
		for each (Vector2f vectorDirection in posibleDirections)
		{
			if (vectorDirection.myX == -1 && vectorDirection.myY == 0) {
				vectorFound = true;
				vectorResult = vectorDirection;
			}
		}
	}

	return vectorResult;
}

bool Ghost::isInRespawnArea(int currentTileX, int currentTileY)
{
	bool result = false;
	if (currentTileX > 9 &&
		currentTileX < 16 &&
		currentTileY > 10 &&
		currentTileY < 15) 
	{
		result = true;
	}

	return result;
}
