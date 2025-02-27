#include "Component.hpp"

#include "component_manager.hpp"


void MovingSystem::update_position(float deltatime)
{
    for (auto entity : mEntities)
    {
        auto& transform = ecs::ComponentManager::singleton().get_component<Transform>(entity);
        auto& motion = ecs::ComponentManager::singleton().get_component<Motion>(entity);
        transform.position.x += motion.direction_normalized.x * motion.speed * deltatime;
        transform.position.y += motion.direction_normalized.y * motion.speed * deltatime;
    }

}
