#include "wall.hpp"

Wall::Wall(const sf::Vector2f &_position, const char &_type) {
    initVars();
    position = _position;

    // Food is not animated.
    // It has the same texture every frame.
    texture_paths = { texture_map[_type] };

    initSprite();
}
Wall::Wall(const Wall &_wall) { *this = _wall; }
Wall::~Wall() { /* nothing to do here */ }

Wall &Wall::operator=(const Wall &_other) {
    this->operator=(_other);
    return *this;
}

void Wall::update(const sf::RenderTarget *_target, GameTile *_map[MAP_WIDTH][MAP_HEIGHT]) {
    updateSprite();
}
