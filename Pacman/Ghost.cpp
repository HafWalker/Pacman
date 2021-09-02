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
	float speed = 30.f;
	int nextTileX = myCurrentTileX + myDesiredMovementX;
	int nextTileY = myCurrentTileY + myDesiredMovementY;

	if (myIsDeadFlag)
		speed = 120.f;

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
				default:
					BehaveWander();
					break;
				}
			}

			myIsDeadFlag = false;
		}
	}

	int tileSize = 22;
	Vector2f destination(myNextTileX * tileSize, myNextTileY * tileSize);
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * speed;

	if (distanceToMove > direction.Length())
	{
		myPosition = destination;
		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
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
	std::cout << "CHASE" << std::endl;
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