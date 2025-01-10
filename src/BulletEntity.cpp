#include "BulletEntity.hpp"
#include <random>
#include <iostream>

Bullet create_bullet(Transform windowSize)
{
	static std::random_device rd;
	static std::mt19937 generator(rd());
	static std::uniform_int_distribution<> whichSide (0, 3);
	static std::uniform_int_distribution<> basicPositionX (0, windowSize.size.x);
	static std::uniform_int_distribution<> basicPositionY (0, windowSize.size.y);

	Bullet newBullet;

	newBullet.transform.size = { 10.0f, 10.0f };
	newBullet.motion.speed = 30.0f;

	switch (whichSide(generator)) // Spawn where
	{
		case 0: // Left
			newBullet.transform.position.x = windowSize.get_min_bound().x - newBullet.transform.size.x;
			newBullet.transform.position.y = basicPositionY(generator);
			newBullet.motion.direction_normalized = Vec2(1, 0);
			break;
		case 1: // Right
			newBullet.transform.position.x = windowSize.get_max_bound().x + newBullet.transform.size.x;
			newBullet.transform.position.y = basicPositionY(generator);
			newBullet.motion.direction_normalized = Vec2(-1, 0);
			break;
		case 2: // Up
			newBullet.transform.position.x = basicPositionX(generator);
			newBullet.transform.position.y = windowSize.get_min_bound().y - newBullet.transform.size.y;
			newBullet.motion.direction_normalized = Vec2(0, 1);
			break;
		case 3: // Down
			newBullet.transform.position.x = basicPositionX(generator);
			newBullet.transform.position.y = windowSize.get_max_bound().y + newBullet.transform.size.y;
			newBullet.motion.direction_normalized = Vec2(0, -1);
			break;
	}

	return newBullet;
}