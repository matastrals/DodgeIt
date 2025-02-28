#include "EnemyEntity.hpp"
#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include "entity_manager.hpp"
#include "component_manager.hpp"
#include <iostream>
#include <random>

#include "PlayerEntity.hpp"
#include "system_manager.hpp"

void EnemyEntity::createEnemy(Transform windowSize) {
	ecs::Entity enemy = ecs::EntityManager::singleton().create_entity();

	static std::random_device rd;
	static std::mt19937 generator(rd());
	static std::uniform_int_distribution<> whichSide(0, 3);
	static std::uniform_int_distribution<> basicPositionX(5, windowSize.size.x - 5);
	static std::uniform_int_distribution<> basicPositionY(5, windowSize.size.y - 5);

	Vec2 localNewEntityVec2;

	switch (whichSide(generator)) 
	{
	case 0: // Left
		localNewEntityVec2.x = windowSize.get_min_bound().x - 80 ;
		localNewEntityVec2.y = basicPositionY(generator);
		break;
	case 1: // Right
		localNewEntityVec2.x = windowSize.get_max_bound().x + 80;
		localNewEntityVec2.y = basicPositionY(generator);
		break;
	case 2: // Up
		localNewEntityVec2.x = basicPositionX(generator);
		localNewEntityVec2.y = windowSize.get_min_bound().y - 80;
		break;
	case 3: // Down
		localNewEntityVec2.x = basicPositionX(generator);
		localNewEntityVec2.y = windowSize.get_max_bound().y + 80;
		break;
	}
	Transform enemyTransform = {
		.position = localNewEntityVec2,
		.size = {0.f, 0.f}
	};
	ecs::ComponentManager::singleton().add_component<Transform>(enemy, std::move(enemyTransform));

	Motion enemyMotion = {
		.direction_normalized = {0.f, 0.f},
		.speed = 10.0f
	};
	ecs::ComponentManager::singleton().add_component<Motion>(enemy, std::move(enemyMotion));

	static std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

	if (!texture->loadFromFile("assets/Christmas_GingerBread_Move.png"))
	{
		std::cout << "Texture pas charger" << std::endl;
		exit(1);
	}

	sf::Sprite enemySprite;
	enemySprite.setTexture(*texture);
	std::vector<std::vector<sf::IntRect>> allSprite;
	int rectLeft = 54;
	int rectTop = 44;
	int rectHeight = 40;
	int rectWidth = 20;
	std::vector<sf::IntRect> animation1 =
	{
		sf::IntRect(rectLeft, rectTop, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 128, rectTop, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 256, rectTop, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 384, rectTop, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 512, rectTop, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 650, rectTop, rectWidth, rectHeight),
	};
	std::vector<sf::IntRect> animation2 =
	{
		sf::IntRect(rectLeft, rectTop + 128, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 128, rectTop + 128, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 256, rectTop + 128, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 384, rectTop + 128, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 512, rectTop + 128, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 650, rectTop + 128, rectWidth, rectHeight),
	};
	std::vector<sf::IntRect> animation3 =
	{
		sf::IntRect(rectLeft, rectTop + 256, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 128, rectTop + 256, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 256, rectTop + 256, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 384, rectTop + 256, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 512, rectTop + 256, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 650, rectTop + 256, rectWidth, rectHeight),
	};
	std::vector<sf::IntRect> animation4 =
	{
		sf::IntRect(rectLeft, rectTop + 384, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 128, rectTop + 384, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 256, rectTop + 384, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 384, rectTop + 384, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 512, rectTop + 384, rectWidth, rectHeight),
		sf::IntRect(rectLeft + 650, rectTop + 384, rectWidth, rectHeight),
	};

	std::vector<int> currentSprite = { 0, 0 };

	enemySprite.setTextureRect(animation1[0]);
	RenderSprite enemyRenderSprite = {
		.sprite = enemySprite,
		.allSprite = allSprite,
		.currentSprite = currentSprite,
	};
	enemyRenderSprite.allSprite.push_back(animation1);
	enemyRenderSprite.allSprite.push_back(animation2);
	enemyRenderSprite.allSprite.push_back(animation3);
	enemyRenderSprite.allSprite.push_back(animation4);

	ecs::ComponentManager::singleton().add_component<RenderSprite>(enemy, std::move(enemyRenderSprite));

	Enemy enemyTag = {};

	ecs::ComponentManager::singleton().add_component<Enemy>(enemy, std::move(enemyTag));

	ecs::Signature signature;
	auto& entityManager = ecs::EntityManager::singleton();
	signature.set(ecs::ComponentManager::singleton().get_component_type<Transform>(), true);
	signature.set(ecs::ComponentManager::singleton().get_component_type<Motion>(), true);
	signature.set(ecs::ComponentManager::singleton().get_component_type<RenderSprite>(), true);
	signature.set(ecs::ComponentManager::singleton().get_component_type<Enemy>(), true);
	entityManager.set_entity_signature(enemy, signature);
	ecs::SystemManager::singleton().update_entity_signature(enemy, signature);
}

void EnemySystem::followPlayer(ecs::Entity player)
{
	Transform& playerTransform = ecs::ComponentManager::singleton().get_component<Transform>(player);
	for (auto entity : mEntities)
	{
		Transform& enemyTransform = ecs::ComponentManager::singleton().get_component<Transform>(entity);
		Motion& enemyMotion = ecs::ComponentManager::singleton().get_component<Motion>(entity);
 		if (playerTransform.position.x + 20 <= enemyTransform.position.x)
		{
			enemyMotion.direction_normalized.x = -1;
		} else
		{
			enemyMotion.direction_normalized.x = 1;
		}
		if (playerTransform.position.y + 20 <= enemyTransform.position.y)
		{
			enemyMotion.direction_normalized.y = -1;
		}
		else
		{
			enemyMotion.direction_normalized.y = 1;
		}
	}
}


void EnemySystem::animationEnemy()
{
	for (auto entity : mEntities)
	{
		Motion& enemyMotion = ecs::ComponentManager::singleton().get_component<Motion>(entity);
		RenderSprite& enemyRenderSprite = ecs::ComponentManager::singleton().get_component<RenderSprite>(entity);
		if (enemyMotion.direction_normalized.x == -1)
		{
			enemyRenderSprite.currentSprite[1] = 1;
		}
		else if (enemyMotion.direction_normalized.x == 1)
		{
			enemyRenderSprite.currentSprite[1] = 2;
		}
		else if (enemyMotion.direction_normalized.y == -1)
		{
			enemyRenderSprite.currentSprite[1] = 0;
		}
		else if (enemyMotion.direction_normalized.y == 1)
		{
			enemyRenderSprite.currentSprite[1] = 3;
		} else
		{
			enemyRenderSprite.currentSprite[0] = 1;
		}
	}
}

void EnemySystem::collidePlayer(ecs::Entity player)
{
	sf::FloatRect playerCollide = ecs::ComponentManager::singleton().get_component<RenderSprite>(player).sprite.getGlobalBounds();
	std::vector<ecs::Entity> enemyToDestroy;
	auto entityManager = ecs::EntityManager::singleton();

	for (auto entity : mEntities) {
		auto enemyRenderSprite = ecs::ComponentManager::singleton().get_component<RenderSprite>(entity);
		sf::FloatRect enemyBounds = enemyRenderSprite.sprite.getGlobalBounds();


		if (playerCollide.intersects(enemyBounds))
		{
			enemyToDestroy.push_back(entity);
		}
	}
	for (auto entity : enemyToDestroy) {
		entityManager.destroy_entity(entity);
		ecs::SystemManager::singleton().remove_entity(entity);
		removeHealth(player, 25);
	}
}

