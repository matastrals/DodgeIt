#pragma once 
#include <SFML/Graphics.hpp>

#include "Component.hpp"
#include "component_manager.hpp"
#include "system_manager.hpp"

class CharacterSystem : public ecs::System {
public:
	void renderSprite(sf::RenderWindow& window) {
		for (auto entity : mEntities) {
			auto& renderSprite = ecs::ComponentManager::singleton().get_component<RenderSprite>(entity);
			auto& transform = ecs::ComponentManager::singleton().get_component<Transform>(entity);
			renderSprite.sprite.setPosition(transform.position.x, transform.position.y);
			renderSprite.sprite.setTextureRect(sf::IntRect(renderSprite.allSprite[0][renderSprite.currentSprite[0]] + 20,
				renderSprite.allSprite[1][renderSprite.currentSprite[1]] + 5, 65, 80));
			window.draw(renderSprite.sprite);
		}
	}
};

class BulletSystem : public ecs::System {
public:
	void renderBullet(sf::RenderWindow& window) {
		for (auto entity : mEntities)
		{
			auto& circleShape = ecs::ComponentManager::singleton().get_component<sf::CircleShape>(entity);
			auto& transform = ecs::ComponentManager::singleton().get_component<Transform>(entity);
			circleShape.setPosition(transform.position.x, transform.position.y);
			window.draw(circleShape);
		}
	}
};