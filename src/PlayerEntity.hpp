#pragma once
#include "Component.hpp"
#include "entity_manager.hpp"
#include "system_manager.hpp"

ecs::Entity setPlayer();
void removeHealth(ecs::Entity);
bool isPlayerHaveNoHealth(ecs::Entity);