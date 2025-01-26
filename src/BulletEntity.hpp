#pragma once
#include "MovementComponent.hpp"

struct Bullet {
	Transform transform;
	Motion motion;
	Vec2 vec2;
};

Bullet create_bullet(Transform);
bool bullet_out_screen(Bullet, Transform);