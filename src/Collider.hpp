#pragma once
#include <SFML/Graphics.hpp>

#include "component_manager.hpp"
#include "entity_manager.hpp"
#include "system_manager.hpp"
#include "PlayerEntity.hpp"

class BulletSystemCollide : public ecs::System {
public: 
	void bulletCollide(ecs::Entity player)
	{
		PlayerSystem playerSystem;
		sf::FloatRect playerCollide = ecs::ComponentManager::singleton().get_component<RenderSprite>(player).sprite.getGlobalBounds();
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
			ecs::SystemManager::singleton().remove_entity(entity);
			playerSystem.removeHealth(player);
		}
	}

	void bulletOutScreen(Transform windowSize)
	{
		std::vector<ecs::Entity> bulletsToDestroy;
		auto entityManager = ecs::EntityManager::singleton();

		for (auto entity : mEntities)
		{
			Motion& bulletMotion = ecs::ComponentManager::singleton().get_component<Motion>(entity);
			Transform& bulletTransform = ecs::ComponentManager::singleton().get_component<Transform>(entity);
			if (bulletMotion.direction_normalized.x == 1.f)
			{
				if (bulletTransform.position.x > windowSize.get_max_bound().x + 10)
				{
					bulletsToDestroy.push_back(entity);
				}
			}
			else if (bulletMotion.direction_normalized.x == -1.f)
			{
				if (bulletTransform.position.x < windowSize.get_min_bound().x - 10)
				{
					bulletsToDestroy.push_back(entity);
				}
			}
			else if (bulletMotion.direction_normalized.y == 1.f)
			{
				if (bulletTransform.position.y > windowSize.get_max_bound().y + 10)
				{
					bulletsToDestroy.push_back(entity);
				}
			}
			else if (bulletMotion.direction_normalized.y == -1.f)
			{
				if (bulletTransform.position.y < windowSize.get_min_bound().y - 10)
				{
					bulletsToDestroy.push_back(entity);
				}
			}
		}
		std::cout << "Number of bullet : " << mEntities.size() << std::endl;
		for (auto entity : bulletsToDestroy) {
			entityManager.destroy_entity(entity);
			ecs::SystemManager::singleton().remove_entity(entity);
		}
	}
};

