#pragma once
#include "Component.hpp"
#include "entity_manager.hpp"
#include "system_manager.hpp"

ecs::Entity setPlayer();
void removeHealth(ecs::Entity, int);
bool isPlayerHaveNoHealth(ecs::Entity);