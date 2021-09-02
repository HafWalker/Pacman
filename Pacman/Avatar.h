#ifndef AVATAR_H
#define AVATAR_H

#include "MovableGameEntity.h"
#include "Vector2f.h"

class Avatar : public MovableGameEntity
{
public:
	float mouthAnimationTimer = 0.f;
	bool isMouthOpen = false;

	Avatar(const Vector2f& aPosition, Sprite* entitySprite);
	~Avatar(void);

	void Update(float aTime);
	void ChangeAvatarSpriteByDirection(Vector2f direction);

private:
	void SwitchAvatarOpenCloseMouth();
};

#endif //AVATAR_H