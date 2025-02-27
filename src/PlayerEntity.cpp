#include "PlayerEntity.hpp"

#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include "entity_manager.hpp"
#include "component_manager.hpp"
#include <iostream>

#include "system_manager.hpp"

ecs::Entity PlayerSystem::set_player() {
	ecs::Entity player = ecs::EntityManager::singleton().create_entity();
	Transform playerTransform = {
		.position = {250.f, 250.f},
		.size = {0.f, 0.f}
	};
	ecs::ComponentManager::singleton().add_component<Transform>(player, std::move(playerTransform));

	Health playerHealth = {
		.maxHealth = 100,
		.currentHealth = 100
	};
	ecs::ComponentManager::singleton().add_component<Health>(player, std::move(playerHealth));

	Motion playerMotion = {
		.direction_normalized = {0.f, 0.f},
		.speed = 50.0f
	};
	ecs::ComponentManager::singleton().add_component<Motion>(player, std::move(playerMotion));

	static std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();  //Without this sprite is blank
	if (!texture->loadFromFile("assets/SimplePlayerSprite.png"))
	{
		std::cout << "Texture pas charger" << std::endl;
		exit(1);
	}
	sf::Sprite playerSprite;
	playerSprite.setTexture(*texture);
	constexpr std::array<std::array<int, 4>, 2> allSprite = { {
		{0, 65, 130, 195},
		{0, 90, 180, 270}
	} };
	std::vector<int> currentSprite = { 0, 0 };
	RenderSprite playerRenderSprite = {
		.spriteSheet = playerSprite,
		.sprite = playerSprite,
		.allSprite = allSprite,
		.currentSprite = currentSprite,
	};
	ecs::ComponentManager::singleton().add_component<RenderSprite>(player, std::move(playerRenderSprite));

	ecs::Signature signature;
	auto& entityManager = ecs::EntityManager::singleton();
	signature.set(ecs::ComponentManager::singleton().get_component_type<Transform>(), true);
	signature.set(ecs::ComponentManager::singleton().get_component_type<Motion>(), true);
	signature.set(ecs::ComponentManager::singleton().get_component_type<Health>(), true);
	signature.set(ecs::ComponentManager::singleton().get_component_type<RenderSprite>(), true);
	entityManager.set_entity_signature(player, signature);
	ecs::SystemManager::singleton().update_entity_signature(player, signature);
	return(player);
}
