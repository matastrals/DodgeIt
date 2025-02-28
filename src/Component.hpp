#pragma once
#include <SFML/Graphics.hpp>
#include <array>

#include "system_manager.hpp"


struct Vec2
{
    float x;
    float y;
};

struct Motion
{
    Vec2 direction_normalized;
    float speed;
};

struct Transform
{
    Vec2 position;
    Vec2 size;

    [[nodiscard]] Vec2 get_min_bound() const
    {
        return position;
    }

    [[nodiscard]] Vec2 get_max_bound() const
    {
        return { .x = position.x + size.x, .y = position.y + size.y };
    }
};

struct Health
{
    float maxHealth;
    float currentHealth;
};

struct RenderSprite
{
    sf::Sprite  spriteSheet;
    sf::Sprite sprite;
    std::vector<std::vector<sf::IntRect>> allSprite;
    std::vector<int> currentSprite;
};

struct Enemy {}; //Tag

class MovingSystem : public ecs::System {
public:
    void updatePosition(float deltatime);
};

