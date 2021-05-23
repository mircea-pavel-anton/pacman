#include "food.hpp"

Food::Food(const sf::Vector2f &_position) : Edible(FOOD_SOUND) {
    initVars();
    position = _position;

    // Food is not animated.
    // It has the same texture every frame.
    texture_paths = { textures_root_dir + "food.png" };
    score_modifier = 5;

    initSprite();
}