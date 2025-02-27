#pragma once
#include "Component.hpp"
#include "system_manager.hpp"
#include "type.hpp"

struct Bullet {
	Transform transform;
	Motion motion;
};

class BulletEntity : public ecs::System
{
public:
	void create_bullet(Transform);
	bool bullet_out_screen(Bullet, Transform);
};

