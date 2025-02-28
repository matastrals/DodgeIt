#pragma once

#include <SFML/Graphics.hpp>
#include <random>


std::vector <std::vector<sf::Sprite>> setMap()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<> randomTile(0, 61);

	static std::shared_ptr<sf::Texture> textureMap = std::make_shared<sf::Texture>();
    if (!textureMap->loadFromFile("assets/Texture/TX Tileset Grass.png"))
    {
        std::cout << "Pas de Tilesheet..." << std::endl;
        exit(2);
    }


    std::vector<sf::Sprite> tiles;
    std::vector<std::vector<sf::Sprite>> allMap(30, std::vector<sf::Sprite>(20));
    int tileSize = 32;
    int columns = textureMap->getSize().x / tileSize;
    int rows = textureMap->getSize().y / tileSize;


    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < columns; ++x) {
            sf::Sprite sprite;
            sprite.setTexture(*textureMap);
            sprite.setTextureRect(sf::IntRect(x * tileSize, y * tileSize, tileSize, tileSize));
            tiles.push_back(sprite);
        }
    }


    for (size_t i = 0; i < 30; ++i) {
        for (size_t j = 0; j < 20; ++j)
        {
            allMap[i][j] = tiles[randomTile(generator)];
        }
    }
    return allMap;
}