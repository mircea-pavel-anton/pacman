#include "pacman.hpp"

// Counter initialization
int Pacman::counter = 0;

Pacman::Pacman(const sf::Vector2f &_position) {
    initVars();

    position = _position;

    initText();
    initSprite();
}

Pacman::Pacman(const Pacman &_pacman) { *this = _pacman; }
Pacman &Pacman::operator=(const Pacman &_other) {
    this->Movable::operator=(_other);
    next_direction = _other.next_direction;
    score = _other.score;
    font = _other.font;
    text = _other.text;

    return *this;
}

void Pacman::initVars() {
    Movable::initVars();

    counter++;
    index = counter;

    // Init score
    score = 0;
    is_dead = false;
    textures_root_dir = "res/sprites/Pacman" + std::to_string(index) + "/";

    // Load a default texture
    texture_paths = { EMPTY_TEXTURE };
}

void Pacman::pollEvents() {
    if (index == 1) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) next_direction = Directions::Up;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) next_direction = Directions::Down;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) next_direction = Directions::Left;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) next_direction = Directions::Right;
    } else {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) next_direction = Directions::Up;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) next_direction = Directions::Down;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) next_direction = Directions::Left;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) next_direction = Directions::Right;
    }
}

void Pacman::initText() {
    if (font.loadFromFile("res/fonts/emulogic.ttf") == false) {
        std::cout << "ERROR: Failed to load font file!" << std::endl;
        abort();
    }
    
    text.setFont(font);
    text.setCharacterSize(20.f);
    text.setFillColor(sf::Color::Yellow);
    text.setStyle(sf::Text::Regular);

    text.setString(
        "Player " + std::to_string(index) + "\nScore: 0"
    );
    if (index == 1) {
        text.setPosition({
            X_OFFSET + TILE_SIZE * MAP_WIDTH + 20.f,
            Y_OFFSET,
        });
    } else {
        text.setPosition({ 20.f, Y_OFFSET });
    }
}

void Pacman::interact(GameTile *_map[MAP_WIDTH][MAP_HEIGHT]) {
    if ( dynamic_cast<Edible*>(old_tile) != nullptr ) {
        Edible *edible = (Edible*)old_tile;
        score += edible->getScoreModifier();
        edible->toEatenState();
    }
    // TODO interact with power pellet
    // TODO interact with ghost
}

void Pacman::updateMovementDirection(GameTile *_map[MAP_WIDTH][MAP_HEIGHT]) {
    const sf::Vector2i map_position = getMapPosition();

    // Pacman can only change directions when fully inside a tile.
    if (position.x != map_position.x * TILE_SIZE) return;
    if (position.y != map_position.y * TILE_SIZE) return;

    // Get the coordinates of the tile pacman is trying to enter.
    const sf::Vector2i tile_position = map_position + next_direction;

    // Get a reference to that tile.
    const GameTile *tile = _map[tile_position.x][tile_position.y];

    // if it is not walkable, return and don't change the current direction.
    if (tile != nullptr && tile->isWalkable() == false) return;

    // If we got here, it means that the tile is walkable and it's ok
    // to change our movement direction.
    direction = next_direction;

    // Update the animations to reflect the direction change.
    updateAnimation();
}

void Pacman::updateAnimation() {
    std::string direction_name = "";

    switch (direction.x + direction.y * 10) {
        case -10: direction_name = "up"; break;
        case -1: direction_name = "left"; break;
        case 10: direction_name = "down"; break;
        default:
        case 1: direction_name = "right"; break;
    }

    texture_paths = {
        textures_root_dir + "neutral.png",              // mouth closed
        textures_root_dir + direction_name + "_2.png",  // mouth halfway opened
        textures_root_dir + direction_name + "_1.png",  // mouth opened
        textures_root_dir + direction_name + "_2.png"   // mouth halfway opened
    };

    loadTextures();
}

void Pacman::update(const sf::RenderTarget *_target, GameTile *_map[MAP_WIDTH][MAP_HEIGHT]) {
    if (is_dead) {
        updateSprite();
        return;
    }

    // Change the value of @next_direction based on the received event.
    pollEvents();

    // Replace the Pacman tile with the tile that was previously there.
    sf::Vector2i map_pos = getMapPosition();
    _map[map_pos.x][map_pos.y] = old_tile;

    // Update the position of pacman.
    updateMovementDirection(_map);
    position = {
        position.x + direction.x * speed,   // X axis
        position.y + direction.y * speed    // Y axis
    };

    // Check for collisions with other objects, and undo the position
    // change if necessary.
    checkCollisions(_target, _map);

    // Get a copy of the REFERENCE of the tile that Pacman is currently
    // on top of.
    map_pos = getMapPosition();
    old_tile = _map[map_pos.x][map_pos.y];

    // Interact with the tile Pacman is currently on top of.
    interact(_map);
    updateScore();

    // Place Pacman down onto the map at the new position.
    _map[map_pos.x][map_pos.y] = this;

    // Update the sprite to reflect the position change.
    updateSprite();
}

void Pacman::render(sf::RenderTarget *_target) const {
    _target->draw(sprite);
    _target->draw(text);
}

void Pacman::checkWindowCollisions(const sf::RenderTarget *_target) {
    const sf::Vector2u window_size = _target->getSize();
    const sf::Vector2f size = {
        sprite.getGlobalBounds().width,
        sprite.getGlobalBounds().height,
    };

    // X-axis collision
    if (position.x < 0.f) {
        position.x = 0.f;
    } else if (position.x + size.x > window_size.x) {
        position.x = window_size.x - size.x;
    }

    // Y-axis collision
    if (position.y < 0.f) {
        position.y = 0.f;
    } else if (position.y + size.y > window_size.y) {
        position.y = window_size.y - size.y;
    }
}

void Pacman::collideWithObjects(GameTile *_map[MAP_WIDTH][MAP_HEIGHT]) {
    // Get the coordinates for the next tile.
    const sf::Vector2i delta = {
        (direction.x > 0) ? 1 : 0,
        (direction.y > 0) ? 1 : 0
    };
    const sf::Vector2i current_position = getMapPosition() + delta;

    // If the tile Pacman is currently moving into is NOT walkable,
    // go back to the previous tile.
    GameTile *tile = _map[current_position.x][current_position.y];
    if (tile != nullptr) {
        if (tile->isWalkable() == false || dynamic_cast<Pacman*>(tile) != nullptr) {
            position = {
                round(position.x / TILE_SIZE) * TILE_SIZE,
                round(position.y / TILE_SIZE) * TILE_SIZE
            };
        }
    }
}

void Pacman::checkCollisions(const sf::RenderTarget *_target, GameTile *_map[MAP_WIDTH][MAP_HEIGHT]) {
    checkWindowCollisions(_target);
    collideWithObjects(_map);
}

void Pacman::toDeadState() {
    is_dead = true;
    texture_paths = {
        textures_root_dir + "dead_1.png",
    };
    loadTextures();
}

void Pacman::updateScore() {
    text.setString(
        "Player " + std::to_string(index) + "\nScore: " + std::to_string(score)
    );
}
