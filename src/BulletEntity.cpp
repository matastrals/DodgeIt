#include "BulletEntity.hpp"
#include <random>

#include "component_manager.hpp"
#include "entity_manager.hpp"

void BulletEntity::createBullet(Transform windowSize)
{
	static std::random_device rd;
	static std::mt19937 generator(rd());
	static std::uniform_int_distribution<> whichSide (0, 3);
	static std::uniform_int_distribution<> basicPositionX (5, windowSize.size.x - 5);
	static std::uniform_int_distribution<> basicPositionY (5, windowSize.size.y - 5);

	ecs::Entity newBullet = ecs::EntityManager::singleton().create_entity();
	Bullet localNewBulletValue; //Local save for later

	switch (whichSide(generator)) // Where Bullet spawn
	{
		case 0: // Left
			localNewBulletValue.transform.position.x = windowSize.get_min_bound().x - localNewBulletValue.transform.size.x;
			localNewBulletValue.transform.position.y = basicPositionY(generator);
			localNewBulletValue.motion.direction_normalized = Vec2(1, 0);
			break;
		case 1: // Right
			localNewBulletValue.transform.position.x = windowSize.get_max_bound().x + localNewBulletValue.transform.size.x;
			localNewBulletValue.transform.position.y = basicPositionY(generator);
			localNewBulletValue.motion.direction_normalized = Vec2(-1, 0);
			break;
		case 2: // Up
			localNewBulletValue.transform.position.x = basicPositionX(generator);
			localNewBulletValue.transform.position.y = windowSize.get_min_bound().y - localNewBulletValue.transform.size.y;
			localNewBulletValue.motion.direction_normalized = Vec2(0, 1);
			break;
		case 3: // Down
			localNewBulletValue.transform.position.x = basicPositionX(generator);
			localNewBulletValue.transform.position.y = windowSize.get_max_bound().y + localNewBulletValue.transform.size.y;
			localNewBulletValue.motion.direction_normalized = Vec2(0, -1);
			break;
	}

	Transform bulletTransform = {
		.position = localNewBulletValue.transform.position,
		.size = { 10.0f, 10.0f}
	};

	Motion bulletMotion = {
		.direction_normalized = localNewBulletValue.motion.direction_normalized,
		.speed = 50.0f
	};

	sf::CircleShape bulletCircleShape(5.0f);
	bulletCircleShape.setFillColor(sf::Color::Red);
	bulletCircleShape.setPosition(localNewBulletValue.transform.position.x, localNewBulletValue.transform.position.y);

	ecs::ComponentManager::singleton().add_component<Transform>(newBullet, std::move(bulletTransform));
	ecs::ComponentManager::singleton().add_component<Motion>(newBullet, std::move(bulletMotion));
	ecs::ComponentManager::singleton().add_component<sf::CircleShape>(newBullet, std::move(bulletCircleShape));

	ecs::Signature signature;
	auto& entityManager = ecs::EntityManager::singleton();
	signature.set(ecs::ComponentManager::singleton().get_component_type<Transform>(), true);
	signature.set(ecs::ComponentManager::singleton().get_component_type<Motion>(), true);
	signature.set(ecs::ComponentManager::singleton().get_component_type<sf::CircleShape>(), true);
	entityManager.set_entity_signature(newBullet, signature);
	ecs::SystemManager::singleton().update_entity_signature(newBullet, signature);

}
