#pragma once

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

void update_position(const Motion &motion, Transform &transform, float deltatime);
