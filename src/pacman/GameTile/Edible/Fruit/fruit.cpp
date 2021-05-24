#include "fruit.hpp"

Fruit::Fruit(const sf::Vector2f &_position) : Edible("fruit") {
    initVars();
    
    position = _position;

    // Fruits are not animated.
    // They have the same texture every frame.
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 6);
    texture_names = { "fruit_" + std::to_string(dist(rng)) };

    score_modifier = 100;

    initSprite();
}