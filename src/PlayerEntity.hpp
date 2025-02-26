#pragma once
#include "Component.hpp"
#include "entity_manager.hpp"
#include "system_manager.hpp"

class PlayerSystem : public ecs::System
{
public:
    ecs::Entity set_player();
};
