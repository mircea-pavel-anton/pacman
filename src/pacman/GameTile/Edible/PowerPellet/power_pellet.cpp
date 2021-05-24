#include "power_pellet.hpp"

PowerPellet::PowerPellet(const sf::Vector2f &_position) : Edible(Config::getInstance()->sounds["pellet"]) {
    initVars();
    position = _position;

    // PowerPellets are not animated.
    // They have the same texture every frame.
    texture_paths = { textures_root_dir + "power_pellet.png" };
    score_modifier = 0;
    
    initSprite();
}