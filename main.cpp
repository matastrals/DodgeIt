#include <gtest/gtest.h>

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
    float heightRadius;
    float widthRadius;
};

void update_position(const Motion &motion, Transform &transform, float deltatime)
{
    transform.position.x += motion.direction_normalized.x * motion.speed * deltatime ;
    transform.position.y += motion.direction_normalized.y * motion.speed * deltatime;
}

TEST(BulletHellTest, MovementTest)
{
    // Arrange
    Motion motion = {
        .direction_normalized = {1.f,0.f},
        .speed = 10.0f
    };

    Transform transform = {
        .position = {0.f,0.f}
    };


    // Act
    update_position(motion, transform, 0.016f);


    // Assert
    EXPECT_EQ(0.16f, transform.position.x);
    EXPECT_EQ(0.f, transform.position.y);
}

// Main function for Google Test
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}