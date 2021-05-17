#include "edible.hpp"

Edible::Edible() {
    textures_root_dir = "res/sprites/Edibles/";
    score_modifier = 0;
}
Edible::Edible(const Edible &_edible) { *this = _edible; }
Edible::~Edible() { /* nothing to do here */ }

Edible &Edible::operator=(const Edible &_other) {
    this->GameTile::operator=(_other);
    score_modifier = _other.score_modifier;

    return *this;
}

void Edible::update(const sf::RenderTarget *, GameTile *[MAP_WIDTH][MAP_HEIGHT]) {
    updateSprite();
};

void Edible::toEatenState() {
    // Once eaten, no more points can be gained from this tile.
    score_modifier = 0;

    // Once eaten, it should no longer display anything, so we
    // load an empty sprite.
    texture_paths = { EMPTY_TEXTURE };

    loadTextures();
}
