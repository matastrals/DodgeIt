#pragma once
#include <SFML/Graphics.hpp>

#include "component_manager.hpp"
#include "entity_manager.hpp"
#include "system_manager.hpp"

class BulletSystemCollide : public ecs::System {
public: 
	void bulletCollide(sf::FloatRect& playerCollide) {
		std::vector<ecs::Entity> bulletsToDestroy;
		auto entityManager = ecs::EntityManager::singleton();

		for (auto entity : mEntities) {
			auto bulletCircleShape = ecs::ComponentManager::singleton().get_component<sf::CircleShape>(entity);
			sf::FloatRect bulletBounds = bulletCircleShape.getGlobalBounds();


			if (playerCollide.intersects(bulletBounds))
			{
				bulletsToDestroy.push_back(entity);
			}
		}
		for (auto entity : bulletsToDestroy) {
			entityManager.destroy_entity(entity);
		}
	}
};