#ifndef PINKY_HPP
#define PINKY_HPP

#include <SFML/Graphics.hpp>

#include "../ghost.hpp"

class Pinky : public Ghost {
  public:
    Pinky(const sf::Vector2f &_position) : Ghost("Pinky", _position) {};

  protected:
    sf::Vector2i getChasePosition() override {
        sf::Vector2i target = pacman->getMapPosition() + 4 * pacman->getDirection();

        if (target.x < 0.f) target.x = 0.f;
        if (target.y < 0.f) target.y = 0.f;
        if (target.x > MAP_WIDTH) target.y = MAP_WIDTH;
        if (target.y > MAP_HEIGHT) target.y = MAP_HEIGHT;

        return target;
    }
};

#endif //PINKY_HPP