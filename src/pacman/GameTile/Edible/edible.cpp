 #include "edible.hpp"

Edible::Edible(const std::string &buffer_name) {
    score_modifier = 0;
    state = EdibleState::Active;

    sound = Config::getInstance()->sounds[buffer_name];
}
Edible::Edible(const Edible &_edible) { *this = _edible; }
Edible::~Edible() { /* nothing to do here */ }

Edible &Edible::operator=(const Edible &_other) {
    this->GameTile::operator=(_other);
    score_modifier = _other.score_modifier;
    sound = _other.sound;

    return *this;
}

void Edible::update(const sf::RenderTarget *, vec3pGT &_map) {
    switch (score_modifier) {
        case 0: // Power Pellet
            PerfLogger::getInstance()->startJob("PowerPellet::update");
            break;
        case 5: // Food
            PerfLogger::getInstance()->startJob("Food::update");
            break;
        case 100: // Fruit
            PerfLogger::getInstance()->startJob("Fruit::update");
            break;
    }

    updateSprite();

    switch (score_modifier) {
        case 0: // Power Pellet
            PerfLogger::getInstance()->stopJob("PowerPellet::update");
            break;
        case 5: // Food
            PerfLogger::getInstance()->stopJob("Food::update");
            break;
        case 100: // Fruit
            PerfLogger::getInstance()->stopJob("Fruit::update");
            break;
    }
};

void Edible::toEatenState() {
    // Once eaten, no more points can be gained from this tile.
    score_modifier = 0;

    // Once eaten, it should no longer display anything, so we
    // load an empty sprite.
    texture_names = { "empty" };
    state = EdibleState::Eaten;

    // Play the sound the tile makes when eaten.
    sound->play();

    loadTextures();
}
