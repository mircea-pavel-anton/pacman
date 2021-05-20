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
    const float extra_offset = (X_OFFSET - text.getGlobalBounds().width ) / 2;
    const float x_offset = X_OFFSET + TILE_SIZE * MAP_WIDTH;
    text.setPosition({
        x_offset * (index - 1) + extra_offset,
        Y_OFFSET + TILE_SIZE,
    });
}

void Pacman::interact(std::vector<GameTile*> _map[MAP_WIDTH][MAP_HEIGHT]) {
    PerfLogger::getInstance()->startJob("Pacman::" + std::to_string(index) + "::interact");

    sf::Vector2i map_pos = getMapPosition();
    std::vector<GameTile*> vector = _map[map_pos.x][map_pos.y];
    for (GameTile *tile : vector) {
        if (dynamic_cast<PowerPellet*>(tile) != nullptr) {
            PowerPellet *pellet = (PowerPellet*)tile;
            if (pellet->isEaten() == false) {
                for (int i = 0; i < MAP_WIDTH; i++) {
                    for (int j = 0; j < MAP_HEIGHT; j++) {
                        for (GameTile *t : _map[i][j]) {
                            if (dynamic_cast<Ghost*>(t) != nullptr) {
                                ((Ghost*)t)->toFrightenedState();
                            }
                        }
                    }
                }
                pellet->toEatenState();
            }
        }

        if (dynamic_cast<Edible*>(tile) != nullptr) {
            Edible *edible = (Edible*)tile;
            score += edible->getScoreModifier();
            edible->toEatenState();
            updateScore();
        }

        if (dynamic_cast<Ghost*>(tile) != nullptr) {
            Ghost *ghost = dynamic_cast<Ghost*>(tile);
            if (ghost->isScared()) {
                ghost->toDeadState();
                score += 200;
            }
            if (ghost->isDead() == false && ghost->isScared() == false) {
                toDeadState();
            }
        }

    }

    PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::interact");
}

void Pacman::updateMovementDirection(std::vector<GameTile*> _map[MAP_WIDTH][MAP_HEIGHT]) {
    PerfLogger::getInstance()->startJob("Pacman::" + std::to_string(index) + "::updateMovementDirection");

    const sf::Vector2i map_position = getMapPosition();

    // Pacman can only change directions when fully inside a tile.
    if (position.x != map_position.x * TILE_SIZE || position.y != map_position.y * TILE_SIZE) {
        PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::updateMovementDirection");
        return;
    }

    // Get the coordinates of the vector pacman is trying to enter.
    const sf::Vector2i tile_position = map_position + next_direction;

    // If ANY tile in that vector is not walkable, then pacman cannot enter the vector
    // so we return without updating the direction, since it was invalid.
    std::vector<GameTile*> vector = _map[tile_position.x][tile_position.y];
    for (GameTile *tile : vector) {
        if (tile != nullptr && tile->isWalkable() == false) {
            PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::updateMovementDirection");
            return;
        }
    }

    // If we got here, it means that the tile is walkable and it's ok
    // to change our movement direction.
    direction = next_direction;

    // Update the animations to reflect the direction change.
    updateAnimation();

    PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::updateMovementDirection");
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

void Pacman::update(const sf::RenderTarget *_target, std::vector<GameTile*> _map[MAP_WIDTH][MAP_HEIGHT]) {
    PerfLogger::getInstance()->startJob("Pacman::" + std::to_string(index) + "::update");

    if (is_dead) {
        updateSprite();
        PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::update");
        return;
    }

    // Change the value of @next_direction based on the received event.
    pollEvents();

    // Replace the Pacman tile with the tile that was previously there.
    sf::Vector2i map_pos = getMapPosition();
    std::vector<GameTile*> &vector = _map[map_pos.x][map_pos.y];
    vector.erase( std::remove(vector.begin(), vector.end(), this) );

    // Update the position of pacman.
    updateMovementDirection(_map);
    position = {
        position.x + direction.x * speed,   // X axis
        position.y + direction.y * speed    // Y axis
    };
    std::cout << "direction changed!" << std::endl;
    // Check for collisions with other objects, and undo the position
    // change if necessary.
    checkCollisions(_target, _map);
    std::cout << "collisions checked!" << std::endl;

    // Interact with the tile Pacman is currently on top of.
    interact(_map);
    std::cout << "interacted!" << std::endl;

    // Place Pacman down onto the map at the new position.
    map_pos = getMapPosition();
    std::cout << "Map position: " << map_pos.x << ", " << map_pos.y << std::endl;
    std::cout << "Vecsize: " << _map[map_pos.x][map_pos.y].size() << std::endl;
    _map[map_pos.x][map_pos.y].push_back(this);

    // Update the sprite to reflect the position change.
    updateSprite();

    PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::update");
}

void Pacman::render(sf::RenderTarget *_target) const {
    PerfLogger::getInstance()->startJob("Pacman::" + std::to_string(index) + "::render");
 
    _target->draw(sprite);
    _target->draw(text);
 
    PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::render");
}

void Pacman::checkWindowCollisions(const sf::RenderTarget *_target) {
    PerfLogger::getInstance()->startJob("Pacman::" + std::to_string(index) + "::checkWindowCollisions");

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

    PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::checkWindowCollisions");
}

void Pacman::collideWithObjects(std::vector<GameTile*> _map[MAP_WIDTH][MAP_HEIGHT]) {
    PerfLogger::getInstance()->startJob("Pacman::" + std::to_string(index) + "::collideWithObjects");

    // Get the coordinates for the next tile.
    const sf::Vector2i delta = {
        (direction.x > 0) ? 1 : 0,
        (direction.y > 0) ? 1 : 0
    };
    const sf::Vector2i current_position = getMapPosition() + delta;

    // If the tile Pacman is currently moving into is NOT walkable,
    // go back to the previous tile.
    std::vector<GameTile*> vector = _map[current_position.x][current_position.y];
    for (GameTile *tile : vector) {
        if (tile != nullptr) {
            if (tile->isWalkable() == false || dynamic_cast<Pacman*>(tile) != nullptr) {
                position = {
                    round(position.x / TILE_SIZE) * TILE_SIZE,
                    round(position.y / TILE_SIZE) * TILE_SIZE
                };
                PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::collideWithObjects");
                return;
            }
        }
    }
    PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::collideWithObjects");
}

void Pacman::checkCollisions(const sf::RenderTarget *_target, std::vector<GameTile*> _map[MAP_WIDTH][MAP_HEIGHT]) {
    checkWindowCollisions(_target);
    collideWithObjects(_map);
}

void Pacman::toDeadState() {
    is_dead = true;
    texture_paths = {
        textures_root_dir + "dead_1.png",
        textures_root_dir + "dead_2.png",
        textures_root_dir + "dead_3.png",
        textures_root_dir + "dead_4.png",
        textures_root_dir + "dead_5.png",
        textures_root_dir + "dead_6.png",
        textures_root_dir + "dead_7.png",
        textures_root_dir + "dead_8.png",
        textures_root_dir + "dead_9.png",
        textures_root_dir + "dead_10.png",
        textures_root_dir + "dead_11.png",
        textures_root_dir + "dead_12.png",
        textures_root_dir + "dead_13.png",
    };
    loadTextures();
}

void Pacman::updateScore() {
    text.setString(
        "Player " + std::to_string(index) + "\nScore: " + std::to_string(score)
    );
}
