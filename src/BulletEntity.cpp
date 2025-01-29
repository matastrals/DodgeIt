#include "BulletEntity.hpp"
#include <random>
#include <iostream>

Bullet create_bullet(Transform windowSize)
{
	static std::random_device rd;
	static std::mt19937 generator(rd());
	static std::uniform_int_distribution<> whichSide (0, 3);
	static std::uniform_int_distribution<> basicPositionX (5, windowSize.size.x - 5);
	static std::uniform_int_distribution<> basicPositionY (5, windowSize.size.y - 5);

	Bullet newBullet;

	newBullet.transform.size = { 10.0f, 10.0f };
	newBullet.motion.speed = 75.0f;

	switch (whichSide(generator)) // Where Bullet spawn
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



bool bullet_out_screen(Bullet bullet, Transform windowSize)
{
	if (bullet.motion.direction_normalized.x == 1.f)
	{
		if (bullet.transform.position.x > windowSize.get_max_bound().x + 10)
		{
			return true;
		}
	} else if (bullet.motion.direction_normalized.x == -1.f)
	{
		if (bullet.transform.position.x < windowSize.get_min_bound().x - 10)
		{
			return true;
		}
	} else if (bullet.motion.direction_normalized.y == 1.f)
	{
		if (bullet.transform.position.y > windowSize.get_max_bound().y + 10)
		{
			return true;
		}
	} else if (bullet.motion.direction_normalized.y == -1.f)
	{
		if (bullet.transform.position.y < windowSize.get_min_bound().y - 10)
		{
			return true;
		}
	}
	return false;
}