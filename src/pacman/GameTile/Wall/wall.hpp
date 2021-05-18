#ifndef WALL_HPP
#define WALL_HPP

#include "../game_tile.hpp"

class Wall : public GameTile {

public:
    Wall(const sf::Vector2f &, const char &);
    Wall(const Wall &); // copy constructor
    ~Wall();

    // Assignment operator
    Wall &operator=(const Wall &);

    // Walls have nothing to update.
    // We just update the sprite to get the desired animation.
    void update(const sf::RenderTarget *, GameTile *[MAP_WIDTH][MAP_HEIGHT]) override;

    // No character can walk over a wall tile.
    bool isWalkable() const override { return false; }

private:
    char type;
    std::map<const char, const std::string> texture_map = {
        // Regular walls, found inside the map
        { '1', "res/sprites/Walls/left-top.png" },
        { '2', "res/sprites/Walls/horizontal.png" },
        { '3', "res/sprites/Walls/right-top.png" },
        { '4', "res/sprites/Walls/vertical.png" },
        { '5', "res/sprites/Walls/left-bottom.png" },
        { '6', "res/sprites/Walls/right-bottom.png" },

        // Double walls, used as map and ghost-hous borders
        { '!', "res/sprites/Walls/double-top-left.png" },
        { '@', "res/sprites/Walls/double-horizontal.png" },
        { '#', "res/sprites/Walls/double-top-right.png" },
        { '$', "res/sprites/Walls/double-vertical.png" },
        { '%', "res/sprites/Walls/double-bottom-left.png" },
        { '^', "res/sprites/Walls/double-bottom-right.png" },
    };

}; //class Wall

#endif //WALL_HPP