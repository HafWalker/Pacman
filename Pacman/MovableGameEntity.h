#ifndef MOVABLEGAMEENTITY_H
#define MOVABLEGAMEENTITY_H

#include "GameEntity.h"
#include "Vector2f.h"

class MovableGameEntity : public GameEntity
{
public:
	enum MovementDirection: int
	{
		Up = 0,
		Down = 1,
		Left = 2,
		Right = 3,
		DirectionCount = 4
	};

	Vector2f nextMoveDirection;
	std::list<Vector2f> posibleDirections;

	MovableGameEntity(const Vector2f& aPosition, Sprite* entitySprite);
	~MovableGameEntity(void);

	void Respawn(const Vector2f& aPosition);

	void SetNextTile(int anX, int anY);
	int GetCurrentTileX() const { return myCurrentTileX; }
	int GetCurrentTileY() const { return myCurrentTileY; }

	bool IsAtDestination();

protected:

	int myCurrentTileX;
	int myCurrentTileY;

	int myNextTileX;
	int myNextTileY;

private:
	void SubstractCurrentMoveDirection();
	void GetAvailableDirectionsToMove();
	void GetSmallerVectorDirection();
	void GetPriorityVector();
};

#endif // MOVABLEGAMEENTITY_H