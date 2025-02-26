#include "Component.hpp"

void update_position(const Motion &motion, Transform &transform, float deltatime)
{
    transform.position.x += motion.direction_normalized.x * motion.speed * deltatime;
    transform.position.y += motion.direction_normalized.y * motion.speed * deltatime;
}