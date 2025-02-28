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
#include "MapRender.hpp"

#include <windows.h>

#include "EnemyEntity.hpp"

void InitComponent()
{
    auto& componentManager = ecs::ComponentManager::singleton();
    componentManager.register_component<Transform>();
    componentManager.register_component<Motion>();
    componentManager.register_component<Health>();
    componentManager.register_component<RenderSprite>();
    componentManager.register_component<sf::CircleShape>();
    componentManager.register_component<Enemy>();
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

void InitEnemySystem()
{
    auto enemySystem = std::make_shared<EnemySystem>();
    ecs::Signature enemySystemSig;
    enemySystemSig.set(ecs::ComponentManager::singleton().get_component_type<Transform>(), true);
    enemySystemSig.set(ecs::ComponentManager::singleton().get_component_type<Motion>(), true);
    enemySystemSig.set(ecs::ComponentManager::singleton().get_component_type<RenderSprite>(), true);
    enemySystemSig.set(ecs::ComponentManager::singleton().get_component_type<Enemy>(), true);
    ecs::SystemManager::singleton().register_system("EnemySystem", enemySystem, enemySystemSig);
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
    InitComponent();
	InitBulletRender();
    InitBulletCollide();
    InitEnemySystem();
    InitMovingSystem();

    auto renderSystem = InitRenderSystem();
    ecs::Entity player = setPlayer();


    sf::Font font;
    if (!font.loadFromFile("assets/DailyBubble.ttf"))
    {
        std::cout << "Pas de police..." << std::endl;
        exit(2);
    }
    std::vector<std::vector<sf::Sprite>> tiles = setMap();
    Transform windowSize;
    windowSize.position.x = 0; windowSize.position.y = 0;
    windowSize.size.x = 960;  windowSize.size.y = 640;
    sf::RenderWindow window(
        sf::VideoMode(windowSize.size.x, windowSize.size.y),
        "Le GOTY",
        sf::Style::Default);
    window.setFramerateLimit(60);

    //------------------------------------------- GESTION TIMER ---------------------------------------------------

    sf::Time timerSpawnBullet = sf::milliseconds(200.0f);
    sf::Clock clockSpawnBullet;

    sf::Time timerSpawnEnemy = sf::seconds(10.f);
    sf::Clock clockSpawnEnemy;

    sf::Time animationTimePlayer = sf::milliseconds(200.f);
    sf::Clock resetAnimationPlayer;

    sf::Time animationTimeEnemy = sf::milliseconds(200.f);
    sf::Clock resetAnimationEnemy;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
        }

        //------------------------------------------- GESTION PLAYER ANIMATION/MOVEMENT ---------------------------------------------------

        Motion& playerMotion = ecs::ComponentManager::singleton().get_component<Motion>(player);
        RenderSprite& playerRenderSprite = ecs::ComponentManager::singleton().get_component<RenderSprite>(player);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            playerMotion.direction_normalized = Vec2(0.f, -1.f);
            playerRenderSprite.currentSprite[1] = 3;
            if (animationTimePlayer < resetAnimationPlayer.getElapsedTime())
            {
                if (playerRenderSprite.currentSprite[0] == 3) { playerRenderSprite.currentSprite[0] = 0; }
                playerRenderSprite.currentSprite[0] += 1;
                resetAnimationPlayer.restart();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            playerMotion.direction_normalized = Vec2(0.f, 1.f);
            playerRenderSprite.currentSprite[1] = 0;
            if (animationTimePlayer < resetAnimationPlayer.getElapsedTime())
            {
                if (playerRenderSprite.currentSprite[0] == 3) { playerRenderSprite.currentSprite[0] = 0; }
                playerRenderSprite.currentSprite[0] += 1;
                resetAnimationPlayer.restart();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            playerMotion.direction_normalized = Vec2(-1.f, 0.f);
            playerRenderSprite.currentSprite[1] = 1;
            if (animationTimePlayer < resetAnimationPlayer.getElapsedTime())
            {
                if (playerRenderSprite.currentSprite[0] == 3) { playerRenderSprite.currentSprite[0] = 0; }
                playerRenderSprite.currentSprite[0] += 1;
                resetAnimationPlayer.restart();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            playerMotion.direction_normalized = Vec2(1.f, 0.f);
            playerRenderSprite.currentSprite[1] = 2;
            if (animationTimePlayer < resetAnimationPlayer.getElapsedTime())
            {
                if (playerRenderSprite.currentSprite[0] == 3) { playerRenderSprite.currentSprite[0] = 0; }
                playerRenderSprite.currentSprite[0] += 1;
                resetAnimationPlayer.restart();
            }
        }
        else
        {
            playerMotion.direction_normalized = Vec2(0.f, 0.f);
            playerRenderSprite.currentSprite = { 0 , playerRenderSprite.currentSprite[1] };
            resetAnimationPlayer.restart();
        }

        //------------------------------------------- GESTION ENEMY ---------------------------------------------------

        auto enemySystem = ecs::SystemManager::singleton().get_system<EnemySystem>("EnemySystem");
        enemySystem->followPlayer(player);
        if (enemySystem->animationEnemy(animationTimeEnemy, resetAnimationEnemy)) { resetAnimationEnemy.restart(); }

        if (timerSpawnEnemy < clockSpawnEnemy.getElapsedTime())
        {
            clockSpawnEnemy.restart();
            EnemyEntity enemyEntity;
            enemyEntity.createEnemy();
        }

        //------------------------------------------- GESTION BULLET ---------------------------------------------------

        if (timerSpawnBullet < clockSpawnBullet.getElapsedTime())
        {
            clockSpawnBullet.restart();
            BulletEntity bulletEntity;
            bulletEntity.createBullet(windowSize);
        }

        auto bulletSystemCollide = ecs::SystemManager::singleton().get_system<BulletSystemCollide>("BulletSystemCollide");
        bulletSystemCollide->bulletCollide(player);
        bulletSystemCollide->bulletOutScreen(windowSize);

        //------------------------------------------- GESTION RENDER/END ---------------------------------------------------

        if (isPlayerHaveNoHealth(player))
        {
            break;
        }

        Health& playerHealth = ecs::ComponentManager::singleton().get_component<Health>(player);
        sf::Text text;
        text.setFont(font);
        text.setString("Health : " + std::to_string(static_cast<int> (playerHealth.currentHealth)));
        text.setCharacterSize(30);  // Taille du texte
        text.setFillColor(sf::Color::White);  // Couleur
        text.setPosition(windowSize.get_max_bound().x - 200, windowSize.get_min_bound().y + 20);
        auto movingSystem = ecs::SystemManager::singleton().get_system<MovingSystem>("MovingSystem");
        movingSystem->updatePosition(0.16f);
        window.clear(sf::Color::Black);

        for (size_t i = 0; i < 30; ++i) {
            for (size_t j = 0; j < 20; ++j) {
                sf::Sprite sprite = tiles[i][j];
                sprite.setPosition(i * 32, j * 32);
                window.draw(sprite);
            }
        }
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