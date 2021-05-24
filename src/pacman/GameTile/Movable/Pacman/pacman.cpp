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
    state = next_state = PacmanState::Chasing;
    resetTimer();
    textures_root_dir = "res/sprites/Pacman" + std::to_string(index) + "/";

    // Load a default texture
    texture_paths = { Config::getInstance()->empty_texture };

    // Load the audio file into memory.
    if (buffer.loadFromFile(Config::getInstance()->sounds["pacman"]) == false) {
        std::cout << "ERROR: Failed to load " << Config::getInstance()->sounds["pacman"] << std::endl;
        abort();
    }
    sound.setBuffer(buffer);
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
    Config *config = Config::getInstance();
    if (font.loadFromFile(config->font_file) == false) {
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
    text.setPosition({
        (config->offset.x + config->tile_size * config->map_size.x) * (index - 1) + 5,
        config->offset.y + config->tile_size,
    });
}

void Pacman::interact(vec3pGT &_map) {
    PerfLogger::getInstance()->startJob("Pacman::" + std::to_string(index) + "::interact");

    // When pacman is hurt, he can't interact with the map.
    if (state == PacmanState::Hurt) {
        PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::interact");
        return;
    }

    sf::Vector2i map_pos = getMapPosition();
    vec1pGT &vector = _map[map_pos.y][map_pos.x];
    for (GameTile *tile : vector) {
        if (dynamic_cast<PowerPellet*>(tile) != nullptr) {
            PowerPellet *pellet = (PowerPellet*)tile;
            if (pellet->isEaten() == false) {
                for (vec2pGT &vec2 : _map) {
                    for (vec1pGT &vec1 : vec2) {
                        for (GameTile *gt : vec1) {
                            if (dynamic_cast<Ghost*>(gt) != nullptr) {
                                ((Ghost*)gt)->toFrightenedState();
                            }
                        }
                    }
                }
                pellet->toEatenState();
                PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::interact");
                return;
            }
        }

        if (dynamic_cast<Edible*>(tile) != nullptr) {
            Edible *edible = (Edible*)tile;
            if (edible->isEaten() == false) {
                score += edible->getScoreModifier();
                updateScore();
                edible->toEatenState();
            }
        }

        if (dynamic_cast<Ghost*>(tile) != nullptr) {
            Ghost *ghost = dynamic_cast<Ghost*>(tile);
            if (ghost->isScared() && ghost->isDead() == false) {
                score += 200;
                updateScore();
                ghost->toDeadState();
                PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::interact");
                return;
            }
            
            if (ghost->isScared() == false && ghost->isDead() == false) {
                toDeadState();
                PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::interact");
                return;
            }
        }
    }

    PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::interact");
}

void Pacman::updateState() {
    // if an external state switch occured, aka if pacman was hurt by a ghost
    if (state != next_state) {
        switch (next_state) {
            case PacmanState::Hurt:
                sound.play(); // play sound on transition to Hurt state
                state = PacmanState::Hurt;
                score -= 200;
                updateScore();
                break;
        
            // This shouldn't happen.
            // No external switch can trigger pacman from hurt to chasing.
            case PacmanState::Chasing: break;
        }
        
        updateAnimation();
        return;
    }

    // If pacman is currently hurt, check for the hurt timer.
    if (state == PacmanState::Hurt) {
        if (hurt_timer > 0) return;
        state = PacmanState::Chasing;
        next_state = state;
        resetTimer();
        updateAnimation();
        return;
    }
}

void Pacman::updateMovementDirection(vec3pGT &_map) {
    PerfLogger::getInstance()->startJob("Pacman::" + std::to_string(index) + "::updateMovementDirection");

    const sf::Vector2i map_position = getMapPosition();

    // Pacman can only change directions when fully inside a tile.
    const sf::Vector2f closest_position = {
        map_position.x * Config::getInstance()->tile_size,
        map_position.y * Config::getInstance()->tile_size,
    };
    if (position != closest_position) {
        PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::updateMovementDirection");
        return;
    }

    updateState();
    // Get the coordinates of the vector pacman is trying to enter.
    const sf::Vector2i tile_position = map_position + next_direction;

    // If ANY tile in that vector is not walkable, then pacman cannot enter the vector
    // so we return without updating the direction, since it was invalid.
    vec1pGT &vector = _map[tile_position.y][tile_position.x];
    for (GameTile *tile : vector) {
        if (tile != nullptr && tile->isWalkable() == false) {
            PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::updateMovementDirection");
            return;
        }
    }

    // If we got here, it means that the tile is walkable and it's ok
    // to change our movement direction.
    direction = next_direction;
    updateTimer();
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

    if (state == PacmanState::Chasing) {
        texture_paths = {
            textures_root_dir + "neutral.png",              // mouth closed
            textures_root_dir + direction_name + "_2.png",  // mouth halfway opened
            textures_root_dir + direction_name + "_1.png",  // mouth opened
            textures_root_dir + direction_name + "_2.png"   // mouth halfway opened
        };
    } else {
        texture_paths = {
            textures_root_dir + "neutral.png",              // mouth closed
            Config::getInstance()->empty_texture,
            textures_root_dir + direction_name + "_2.png",  // mouth halfway opened
            Config::getInstance()->empty_texture,
            textures_root_dir + direction_name + "_1.png",  // mouth opened
            Config::getInstance()->empty_texture,
            textures_root_dir + direction_name + "_2.png",  // mouth halfway opened
            Config::getInstance()->empty_texture,
        };
        
    }

    loadTextures();
}

void Pacman::update(const sf::RenderTarget *_target, vec3pGT &_map) {
    PerfLogger::getInstance()->startJob("Pacman::" + std::to_string(index) + "::update");

    // Change the value of @next_direction based on the received event.
    pollEvents();

    // Replace the Pacman tile with the tile that was previously there.
    sf::Vector2i map_pos = getMapPosition();
    vec1pGT &vector = _map[map_pos.y][map_pos.x];
    if (vector.empty() == false) 
        vector.erase( std::remove(vector.begin(), vector.end(), this) );

    // Update the position of pacman.
    updateMovementDirection(_map);
    position = {
        position.x + direction.x * speed,   // X axis
        position.y + direction.y * speed    // Y axis
    };
    // Check for collisions with other objects, and undo the position
    // change if necessary.
    checkCollisions(_target, _map);

    // Interact with the tile Pacman is currently on top of.
    interact(_map);

    // Place Pacman down onto the map at the new position.
    map_pos = getMapPosition();
    _map[map_pos.y][map_pos.x].push_back(this);

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

void Pacman::collideWithObjects(vec3pGT &_map) {
    PerfLogger::getInstance()->startJob("Pacman::" + std::to_string(index) + "::collideWithObjects");

    // Get the coordinates for the next tile.
    const sf::Vector2i delta = {
        (direction.x > 0) ? 1 : 0,
        (direction.y > 0) ? 1 : 0
    };
    const sf::Vector2i current_position = getMapPosition() + delta;

    // If the tile Pacman is currently moving into is NOT walkable,
    // go back to the previous tile.
    vec1pGT &vector = _map[current_position.y][current_position.x];
    for (GameTile *tile : vector) {
        if (tile != nullptr) {
            if (tile->isWalkable() == false || dynamic_cast<Pacman*>(tile) != nullptr) {
                const int tile_size = Config::getInstance()->tile_size;
                position = {
                    round(position.x / tile_size) * tile_size,
                    round(position.y / tile_size) * tile_size,
                };
                PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::collideWithObjects");
                return;
            }
        }
    }
    PerfLogger::getInstance()->stopJob("Pacman::" + std::to_string(index) + "::collideWithObjects");
}

void Pacman::checkCollisions(const sf::RenderTarget *_target, vec3pGT &_map) {
    checkWindowCollisions(_target);
    collideWithObjects(_map);
}

void Pacman::toDeadState() { next_state = PacmanState::Hurt; }

void Pacman::updateScore() {
    text.setString(
        "Player " + std::to_string(index) + "\nScore: " + std::to_string(score)
    );
}
