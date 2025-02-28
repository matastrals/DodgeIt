#pragma once
#include <SFML/System/Clock.hpp>

#include "Component.hpp"
#include "system_manager.hpp"

class EnemyEntity
{
public:
	void createEnemy(Transform);
};

class EnemySystem : public ecs::System
{
public:
	void followPlayer(ecs::Entity);
	void animationEnemy();
	void collidePlayer(ecs::Entity);
};
