#pragma once
#include <SFML/Graphics.hpp>
#include <array>


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
    sf::Sprite sprite;
    std::array<std::array<int, 4>, 2> allSprite;
    std::vector<int> currentSprite;
};


void update_position(const Motion &motion, Transform &transform, float deltatime);
