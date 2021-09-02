#include "Avatar.h"

Avatar::Avatar(const Vector2f& aPosition, Sprite* entitySprite)
: MovableGameEntity(aPosition, entitySprite)
{
	//sprite->SetFrame("open_left_32.png");
}

Avatar::~Avatar(void)
{
}

void Avatar::Update(float aTime)
{
	int tileSize = 22;

	Vector2f destination(myNextTileX * tileSize, myNextTileY * tileSize);
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * 100.0f;

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

	mouthAnimationTimer += aTime;
	if (mouthAnimationTimer > 0.2f) {
		mouthAnimationTimer = 0.f;
		isMouthOpen = !isMouthOpen;
	}
}

void Avatar::ChangeAvatarSpriteByDirection(Vector2f direction) {

	if (direction.myX == 0.f && direction.myY == -1.f) {
		isMouthOpen ? sprite->SetFrame("open_up_32.png") : 
			sprite->SetFrame("closed_up_32.png");
	}
	else if (direction.myX == 0.f && direction.myY == 1.f) {
		isMouthOpen ? sprite->SetFrame("open_down_32.png") : 
			sprite->SetFrame("closed_down_32.png");
	}
	else if (direction.myX == 1.f && direction.myY == 0.f) {
		isMouthOpen ? sprite->SetFrame("open_right_32.png") : 
			sprite->SetFrame("closed_right_32.png");
	}
	else if(direction.myX == -1.f && direction.myY == 0.f){
		isMouthOpen ? sprite->SetFrame("open_left_32.png") : 
			sprite->SetFrame("closed_left_32.png");
	}
}

void Avatar::SwitchAvatarOpenCloseMouth()
{

}
