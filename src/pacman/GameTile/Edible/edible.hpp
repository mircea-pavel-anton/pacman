#ifndef EDIBLE_HPP
#define EDIBLE_HPP

#include "../game_tile.hpp"

class Edible : public GameTile {

enum EdibleState { Active, Eaten };

public:
    Edible(const std::string &);
    Edible(const Edible &); // copy constructor
    virtual ~Edible();
    Edible &operator=(const Edible &); // assignment operator

    // Edibles have nothing to update.
    // We just update the sprite to get the desired animation.
    void update(const sf::RenderTarget *, vec3pGT&) override;

    // Every character can walk over a edible tile.
    inline bool isWalkable() const override { return true; }

    // Once pacman walks on top of an edible tile, it should
    // trigger this method to update the sprite.
    void toEatenState();

    // The sound made when the edible tile is eaten.
    sf::Sound sound;
    sf::SoundBuffer buffer;

    // Gets the number of points gained by eating this tile.
    int getScoreModifier() { return score_modifier; }
    bool isEaten() { return state == EdibleState::Eaten; }
protected:
    EdibleState state;
    int score_modifier;

    void initSound();

};

#endif //EDIBLE_HPP