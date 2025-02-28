#include "PlayerEntity.hpp"

#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include "entity_manager.hpp"
#include "component_manager.hpp"
#include <iostream>

#include "system_manager.hpp"

ecs::Entity setPlayer() {
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
	constexpr std::vector<std::vector<sf::IntRect>> allSprite;
	std::vector<sf::IntRect> animation1 = 
	{
		sf::IntRect(20, 5, 65, 80),
		sf::IntRect(85, 5, 65, 80),
		sf::IntRect(150, 5, 65, 80),
		sf::IntRect(215, 5, 65, 80)
	};
	std::vector<sf::IntRect> animation2 =
	{
		sf::IntRect(20, 95, 65, 80),
		sf::IntRect(85, 95, 65, 80),
		sf::IntRect(150, 95, 65, 80),
		sf::IntRect(215, 95, 65, 80)
	};
	std::vector<sf::IntRect> animation3 =
	{
		sf::IntRect(20, 185, 65, 80),
		sf::IntRect(85, 185, 65, 80),
		sf::IntRect(150, 185, 65, 80),
		sf::IntRect(215, 185, 65, 80)
	};
	std::vector<sf::IntRect> animation4 =
	{
		sf::IntRect(20, 275, 65, 80),
		sf::IntRect(85, 275, 65, 80),
		sf::IntRect(150, 275, 65, 80),
		sf::IntRect(215, 275, 65, 80)
	};

	std::vector<int> currentSprite = { 0, 0 };
	RenderSprite playerRenderSprite = {
		.spriteSheet = playerSprite,
		.sprite = playerSprite,
		.allSprite = allSprite,
		.currentSprite = currentSprite,
	};
	playerRenderSprite.allSprite.push_back(animation1);
	playerRenderSprite.allSprite.push_back(animation2);
	playerRenderSprite.allSprite.push_back(animation3);
	playerRenderSprite.allSprite.push_back(animation4);

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


void removeHealth(ecs::Entity player)
{
	Health& playerHealth = ecs::ComponentManager::singleton().get_component<Health>(player);
	playerHealth.currentHealth -= 10.0f;
}

bool isPlayerHaveNoHealth(ecs::Entity player)
{
	Health& playerHealth = ecs::ComponentManager::singleton().get_component<Health>(player);
	if (playerHealth.currentHealth <= 0)
	{
		return true;
	}
	return false;
}