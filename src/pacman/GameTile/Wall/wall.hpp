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
    void update(const sf::RenderTarget *, vec3pGT&) override;

    // No character can walk over a wall tile.
    bool isWalkable() const override { return false; }

private:
    char type;

}; //class Wall

#endif //WALL_HPP