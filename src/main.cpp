#include <SFML/Graphics.hpp>
#include "BulletEntity.hpp"
#include <iostream>

struct Health
{
    float maxHealth;
    float CurrentHealth;
};
struct Player {
    Transform transform;
    Motion motion;
    Health health;
    sf::Sprite sprite;
};

void Graphics()
{
    Player player;
    player.transform.position = Vec2(250.f, 250.f);
    player.motion.speed = 10.f;

    sf::Texture texture;
    if (!texture.loadFromFile("assets/SimplePlayerSprite.jpg"))
    {
        std::cout << "Ca marche po";
    }
    player.sprite.setTexture(texture);


    Transform windowSize;
    windowSize.position.x = 0; windowSize.position.y = 0;
    windowSize.size.x = 800;  windowSize.size.y = 600;
    sf::RenderWindow window(
        sf::VideoMode(windowSize.size.x, windowSize.size.y),
        "Le GOTY",
        sf::Style::Default);
    window.setFramerateLimit(60);

    std::vector<Bullet> allBulletComponent;
    std::vector<sf::CircleShape> allBulletEntity;
    sf::Time timerSpawnBullet = sf::seconds(1.f);
    sf::Clock timer;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            player.motion.direction_normalized = Vec2(0.f, -1.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            player.motion.direction_normalized = Vec2(0.f, 1.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            player.motion.direction_normalized = Vec2(-1.f, 0.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.motion.direction_normalized = Vec2(1.f, 0.f);
        }
        else
        {
            player.motion.direction_normalized = Vec2(0.f, 0.f);
        }
        if (timerSpawnBullet < timer.getElapsedTime())
        {
            timer.restart();
            Bullet bulletComponent = create_bullet(windowSize);
            allBulletComponent.push_back(bulletComponent);
            sf::CircleShape bulletEntity(15.0f);
            bulletEntity.setFillColor(sf::Color::Red);
            bulletEntity.setPosition(bulletComponent.transform.position.x, bulletComponent.transform.position.y);
            allBulletEntity.push_back(bulletEntity);
        }
        if (!allBulletComponent.empty())
        {
            for (int i = 0; i < allBulletEntity.size(); i++)
            {
                update_position(allBulletComponent[i].motion, allBulletComponent[i].transform, 0.16f);
            }
        }
        update_position(player.motion, player.transform, 0.16f);
        window.clear(sf::Color::Black);
        player.sprite.setPosition(player.transform.position.x, player.transform.position.y);
        player.sprite.setTextureRect(sf::IntRect(70, 10, 150, 220));
        window.draw(player.sprite);
        if (!allBulletComponent.empty())
        {
            for (int i = 0; i < allBulletEntity.size(); i++)
            {
                allBulletEntity[i].setPosition(allBulletComponent[i].transform.position.x, allBulletComponent[i].transform.position.y);
                window.draw(allBulletEntity[i]);
            }
        }
        window.display();
        
    }
}

int main(int argc, char** argv) {
    Graphics();
    return 1;
}