#pragma once
#include "PlayerEntity.hpp"
#include "Render.hpp"
#include "type.hpp"

void InitComponent();

void InitBullet();

void InitEnemySystem();

void InitMovingSystem();

std::shared_ptr<CharacterSystem> InitRenderSystem();

void Run();