#include <SFML/Graphics.hpp>
#include "BulletEntity.hpp"
#include <iostream>
#include <array>

#include <windows.h>

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
    player.health.CurrentHealth = 100;
    player.transform.position = Vec2(250.f, 250.f);
    player.motion.speed = 100.f;

    sf::Texture texture;
    if (!texture.loadFromFile("assets/SimplePlayerSprite.png"))
    {
        std::cout << "Ca marche po" << std::endl;
        exit(1);
    }
    sf::Font font;
    if (!font.loadFromFile("assets/DailyBubble.ttf"))
    {
        std::cout << "Pas de police..." << std::endl;
        exit(2);
    }
    player.sprite.setTexture(texture);
	constexpr std::array<std::array<int, 4>, 2> allSprite = { {
            {0, 65, 130, 195},
			{0, 90, 180, 270}
        } };
    std::vector<int> currentSprite = { 0, 0 };

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
    sf::Time timerSpawnBullet = sf::seconds(0.2f);
    sf::Clock timer;

    sf::Time animationTime = sf::milliseconds(200.f);
    sf::Clock resetAnimation;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            player.motion.direction_normalized = Vec2(0.f, -1.f);
            currentSprite[1] = 3;
            if (animationTime < resetAnimation.getElapsedTime())
            {
                if (currentSprite[0] == 3) { currentSprite[0] = 0; }
                currentSprite[0] += 1;
                resetAnimation.restart();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            player.motion.direction_normalized = Vec2(0.f, 1.f);
            currentSprite[1] = 0;
            if (animationTime < resetAnimation.getElapsedTime())
            {
                if (currentSprite[0] == 3) { currentSprite[0] = 0; }
                currentSprite[0] += 1;
                resetAnimation.restart();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            player.motion.direction_normalized = Vec2(-1.f, 0.f);
            currentSprite[1] = 1;
            if (animationTime < resetAnimation.getElapsedTime())
            {
                if (currentSprite[0] == 3) { currentSprite[0] = 0; }
                currentSprite[0] += 1;
                resetAnimation.restart();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.motion.direction_normalized = Vec2(1.f, 0.f);
            currentSprite[1] = 2;
            if (animationTime < resetAnimation.getElapsedTime())
            {
                if (currentSprite[0] == 3) { currentSprite[0] = 0; }
                currentSprite[0] += 1;
                resetAnimation.restart();
            }
        }
        else
        {
            player.motion.direction_normalized = Vec2(0.f, 0.f);
            currentSprite = { 0 , currentSprite[1] };
            resetAnimation.restart();
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

        for (int iterationAllBullet = allBulletEntity.size() - 1; iterationAllBullet >= 0; --iterationAllBullet)
        {
            if (bullet_out_screen(allBulletComponent[iterationAllBullet], windowSize))
            {
                allBulletComponent.erase(allBulletComponent.begin() + iterationAllBullet);
                allBulletEntity.erase(allBulletEntity.begin() + iterationAllBullet);
            }
        }

        for (int iterationAllBullet = allBulletEntity.size() - 1; iterationAllBullet >= 0; --iterationAllBullet)
        {
            sf::FloatRect BulletBoxCollision = allBulletEntity[iterationAllBullet].getGlobalBounds();
            sf::FloatRect playerBoxCollision = player.sprite.getGlobalBounds();
            if (playerBoxCollision.intersects(BulletBoxCollision))
            {
                allBulletComponent.erase(allBulletComponent.begin() + iterationAllBullet);
                allBulletEntity.erase(allBulletEntity.begin() + iterationAllBullet);
                player.health.CurrentHealth -= 10.0f;
            }
        }
        if (player.health.CurrentHealth <= 0)
        {
            break;
        }
        sf::Text text;
        text.setFont(font);
        text.setString("Health : " + std::to_string(static_cast<int> (player.health.CurrentHealth)));
        text.setCharacterSize(30);  // Taille du texte
        text.setFillColor(sf::Color::White);  // Couleur
        text.setPosition(windowSize.get_max_bound().x - 200, windowSize.get_min_bound().y + 20);
        update_position(player.motion, player.transform, 0.16f);
        window.clear(sf::Color::Black);
        player.sprite.setPosition(player.transform.position.x, player.transform.position.y);
        player.sprite.setTextureRect(sf::IntRect(allSprite[0][currentSprite[0]] + 20, allSprite[1][currentSprite[1]] + 5, 65, 80));
        window.draw(player.sprite);


    	if (!allBulletComponent.empty())
        {
            for (int i = 0; i < allBulletEntity.size(); i++)
            {
                allBulletEntity[i].setPosition(allBulletComponent[i].transform.position.x, allBulletComponent[i].transform.position.y);
                window.draw(allBulletEntity[i]);
            }
        }
        window.draw(text);
        window.display();
        
    }
    window.clear(sf::Color::Black);
    sf::Text endText;
    endText.setFont(font);
    endText.setString("Game Over");
    endText.setCharacterSize(40);  // Taille du texte
    endText.setFillColor(sf::Color::Red);  // Couleur
    endText.setPosition(windowSize.get_max_bound().x/2 - 100, windowSize.get_max_bound().y/2 - 20);
    window.draw(endText);
    window.display();

    Sleep(5000);
}

int main(int argc, char** argv) {
    Graphics();
    return 1;
}