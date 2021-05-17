#include "fruit.hpp"

Fruit::Fruit(const sf::Vector2f &_position) {
    initVars();
    
    position = _position;

    // Fruits are not animated.
    // They have the same texture every frame.
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, fruit_textures.size()-1);
    texture_paths = { fruit_textures.at(dist(rng)) };

    score_modifier = 100;

    initSprite();
}