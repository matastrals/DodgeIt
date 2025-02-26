#pragma once
#include "PlayerEntity.hpp"
#include "Render.hpp"
#include "type.hpp"

void Init();

void InitPlayerSystem();

void InitBullet();

std::shared_ptr<CharacterSystem> InitRenderSystem();

void Run();