#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "component_manager.hpp"
#include "entity_manager.hpp"
#include "BulletEntity.hpp"
#include "PlayerEntity.hpp"
#include <iostream>
#include "system_manager.hpp"
#include "Render.hpp"
#include "Collider.hpp"

#include <windows.h>

void Init()
{
    auto& componentManager = ecs::ComponentManager::singleton();
    componentManager.register_component<Transform>();
    componentManager.register_component<Motion>();
    componentManager.register_component<Health>();
    componentManager.register_component<RenderSprite>();
    componentManager.register_component<sf::CircleShape>();
}

void InitPlayerSystem()
{
    auto player = std::make_shared<PlayerSystem>();
    ecs::Signature characterSig;
    characterSig.set(ecs::ComponentManager::singleton().get_component_type<Transform>(), true);
    characterSig.set(ecs::ComponentManager::singleton().get_component_type<Motion>(), true);
    characterSig.set(ecs::ComponentManager::singleton().get_component_type<Health>(), true);
    characterSig.set(ecs::ComponentManager::singleton().get_component_type<RenderSprite>(), true);
    ecs::SystemManager::singleton().register_system("PlayerSystem", player, characterSig);
}

void InitBulletRender()
{
    auto bulletSystemRender = std::make_shared<BulletSystemRender>();
    ecs::Signature bulletSig;
    bulletSig.set(ecs::ComponentManager::singleton().get_component_type<Transform>(), true);
    bulletSig.set(ecs::ComponentManager::singleton().get_component_type<Motion>(), true);
    bulletSig.set(ecs::ComponentManager::singleton().get_component_type<sf::CircleShape>(), true);
    ecs::SystemManager::singleton().register_system("BulletSystemRender", bulletSystemRender, bulletSig);
}

void InitBulletCollide()
{
    auto bulletSystemCollide = std::make_shared<BulletSystemCollide>();
    ecs::Signature bulletSig;
    bulletSig.set(ecs::ComponentManager::singleton().get_component_type<Transform>(), true);
    bulletSig.set(ecs::ComponentManager::singleton().get_component_type<sf::CircleShape>(), true);
    ecs::SystemManager::singleton().register_system("BulletSystemCollide", bulletSystemCollide, bulletSig);
}

void InitMovingSystem()
{
    auto movingSystem = std::make_shared<MovingSystem>();
    ecs::Signature movingSystemSig;
    movingSystemSig.set(ecs::ComponentManager::singleton().get_component_type<Transform>(), true);
    movingSystemSig.set(ecs::ComponentManager::singleton().get_component_type<Motion>(), true);
    ecs::SystemManager::singleton().register_system("MovingSystem", movingSystem, movingSystemSig);
}

std::shared_ptr<CharacterSystem> InitRenderSystem()
{
    auto spriteRenderSystem = std::make_shared<CharacterSystem>();
    ecs::Signature renderSpriteSig;
    renderSpriteSig.set(ecs::ComponentManager::singleton().get_component_type<RenderSprite>(), true);
    ecs::SystemManager::singleton().register_system("CharacterSystem", spriteRenderSystem, renderSpriteSig);
    return(spriteRenderSystem);
}

void Run()
{
    Init();
	InitPlayerSystem();
    InitBulletRender();
    InitBulletCollide();
    InitMovingSystem();
    auto renderSystem = InitRenderSystem();
    auto playerSystem = ecs::SystemManager::singleton().get_system<PlayerSystem>("PlayerSystem");
    ecs::Entity player = playerSystem->set_player();

    sf::Font font;
    if (!font.loadFromFile("assets/DailyBubble.ttf"))
    {
        std::cout << "Pas de police..." << std::endl;
        exit(2);
    }

    Transform windowSize;
    windowSize.position.x = 0; windowSize.position.y = 0;
    windowSize.size.x = 800;  windowSize.size.y = 600;
    sf::RenderWindow window(
        sf::VideoMode(windowSize.size.x, windowSize.size.y),
        "Le GOTY",
        sf::Style::Default);
    window.setFramerateLimit(60);

    sf::Time timerSpawnBullet = sf::seconds(1.0f);
    sf::Clock timer;

    sf::Time animationTime = sf::milliseconds(200.f);
    sf::Clock resetAnimation;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
        }
        Motion& playerMotion = ecs::ComponentManager::singleton().get_component<Motion>(player);
        RenderSprite& playerRenderSprite = ecs::ComponentManager::singleton().get_component<RenderSprite>(player);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            playerMotion.direction_normalized = Vec2(0.f, -1.f);
            playerRenderSprite.currentSprite[1] = 3;
            if (animationTime < resetAnimation.getElapsedTime())
            {
                if (playerRenderSprite.currentSprite[0] == 3) { playerRenderSprite.currentSprite[0] = 0; }
                playerRenderSprite.currentSprite[0] += 1;
                resetAnimation.restart();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            playerMotion.direction_normalized = Vec2(0.f, 1.f);
            playerRenderSprite.currentSprite[1] = 0;
            if (animationTime < resetAnimation.getElapsedTime())
            {
                if (playerRenderSprite.currentSprite[0] == 3) { playerRenderSprite.currentSprite[0] = 0; }
                playerRenderSprite.currentSprite[0] += 1;
                resetAnimation.restart();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            playerMotion.direction_normalized = Vec2(-1.f, 0.f);
            playerRenderSprite.currentSprite[1] = 1;
            if (animationTime < resetAnimation.getElapsedTime())
            {
                if (playerRenderSprite.currentSprite[0] == 3) { playerRenderSprite.currentSprite[0] = 0; }
                playerRenderSprite.currentSprite[0] += 1;
                resetAnimation.restart();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            playerMotion.direction_normalized = Vec2(1.f, 0.f);
            playerRenderSprite.currentSprite[1] = 2;
            if (animationTime < resetAnimation.getElapsedTime())
            {
                if (playerRenderSprite.currentSprite[0] == 3) { playerRenderSprite.currentSprite[0] = 0; }
                playerRenderSprite.currentSprite[0] += 1;
                resetAnimation.restart();
            }
        }
        else
        {
            playerMotion.direction_normalized = Vec2(0.f, 0.f);
            playerRenderSprite.currentSprite = { 0 , playerRenderSprite.currentSprite[1] };
            resetAnimation.restart();
        }

        if (timerSpawnBullet < timer.getElapsedTime())
        {
            timer.restart();
            BulletEntity bulletEntity;
            bulletEntity.create_bullet(windowSize);
        }

        auto bulletSystemCollide = ecs::SystemManager::singleton().get_system<BulletSystemCollide>("BulletSystemCollide");
        sf::FloatRect playerCollider = ecs::ComponentManager::singleton().get_component<RenderSprite>(player).sprite.getGlobalBounds();
        bulletSystemCollide->bulletCollide(playerCollider);

        Health& playerHealth = ecs::ComponentManager::singleton().get_component<Health>(player);
		
 /*       for (auto bullet : bulletSystem)
        {
            sf::FloatRect BulletBoxCollision = allBulletEntity[iterationAllBullet].getGlobalBounds();
            sf::FloatRect playerBoxCollision = playerSprite.getGlobalBounds();
            if (playerBoxCollision.intersects(BulletBoxCollision))
            {
                allBulletComponent.erase(allBulletComponent.begin() + iterationAllBullet);
                allBulletEntity.erase(allBulletEntity.begin() + iterationAllBullet);
                playerHealth.currentHealth -= 10.0f;
            }
            std::cout << "oui";
        }*/

        //if (playerHealth.currentHealth <= 0)
        //{
        //    break;
        //}
        sf::Text text;
        text.setFont(font);
        text.setString("Health : " + std::to_string(static_cast<int> (playerHealth.currentHealth)));
        text.setCharacterSize(30);  // Taille du texte
        text.setFillColor(sf::Color::White);  // Couleur
        text.setPosition(windowSize.get_max_bound().x - 200, windowSize.get_min_bound().y + 20);
        auto movingSystem = ecs::SystemManager::singleton().get_system<MovingSystem>("MovingSystem");
        movingSystem->update_position(0.16f);
        window.clear(sf::Color::Black);
        renderSystem->renderSprite(window);
        auto bulletSystemRender = ecs::SystemManager::singleton().get_system<BulletSystemRender>("BulletSystemRender");
        bulletSystemRender->renderBullet(window);



        window.draw(text);
        window.display();
    }
    window.clear(sf::Color::Black);
    sf::Text endText;
    endText.setFont(font);
    endText.setString("Game Over");
    endText.setCharacterSize(40);  // Taille du texte
    endText.setFillColor(sf::Color::Red);  // Couleur
    endText.setPosition(windowSize.get_max_bound().x / 2 - 100, windowSize.get_max_bound().y / 2 - 20);
    window.draw(endText);
    window.display();

}