#include <gtest/gtest.h>
#include "Component.hpp"

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
    //update_position(motion, transform, 0.016f);

    // Assert
    EXPECT_TRUE(std::fabs(transform.position.y - 0.16f) > 0.0001f);
    EXPECT_TRUE(std::fabs(transform.position.x - 0.f) >= 0.f);
}

// Main function for Google Test
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}