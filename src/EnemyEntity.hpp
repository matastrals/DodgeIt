#pragma once
#include <SFML/System/Clock.hpp>

#include "system_manager.hpp"

class EnemyEntity
{
public:
	void createEnemy();
};

class EnemySystem : public ecs::System
{
public:
	void followPlayer(ecs::Entity);
	bool animationEnemy(sf::Time, sf::Clock);
};
