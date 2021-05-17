#ifndef EDIBLE_HPP
#define EDIBLE_HPP

#include "../game_tile.hpp"

class Edible : public GameTile {

  public:
    Edible();
    Edible(const Edible &); // copy constructor
    virtual ~Edible();
    Edible &operator=(const Edible &); // assignment operator

    // Edibles have nothing to update.
    // We just update the sprite to get the desired animation.
    void update(const sf::RenderTarget *, GameTile *[MAP_WIDTH][MAP_HEIGHT]) override;

    // Every character can walk over a edible tile.
    inline bool isWalkable() const override { return true; }

    // Once pacman walks on top of an edible tile, it should
    // trigger this method to update the sprite.
    void toEatenState();

    // Gets the number of points gained by eating this tile.
    int getScoreModifier() { return score_modifier; }

  protected:
    int score_modifier;

};

#endif //EDIBLE_HPP