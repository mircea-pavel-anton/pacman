#ifndef MOVABLE_HPP
#define MOVABLE_HPP

#include <math.h>

#include "../../config.hpp"
#include "../game_tile.hpp"

namespace Directions {
  static const sf::Vector2i Up    = { 0, -1};
  static const sf::Vector2i Down  = { 0,  1};
  static const sf::Vector2i Left  = {-1,  0};
  static const sf::Vector2i Right = { 1,  0};
  static const sf::Vector2i None  = { 0,  0};
}

class Movable : public GameTile {
  public:
    Movable(); // empty constructor
    Movable(const Movable &); // copy constructor
    virtual ~Movable();
    Movable &operator=(const Movable &); // Assignment operator


    // Get the direction in which this tile is moving.
    sf::Vector2i getDirection() { return direction; }
  protected:
    // ---------------- MEMBERS ----------------
    // The direction in which the character is moving now.
    sf::Vector2i direction;

    // The speed at which the character moves, in pixels/frame.
    float speed;

    // ---------------- METHODS ----------------
    // Initialization methods
    virtual void initVars();

    // Movement-related methods
    virtual void updateMovementDirection(vec3pGT&) = 0;
    virtual void updateAnimation() = 0;
}; //Movable

#endif //MOVABLE_HPP