#ifndef BLINKY_HPP
#define BLINKY_HPP

#include "../ghost.hpp"

class Blinky : public Ghost {
  public:
    Blinky(const sf::Vector2f &_position) : Ghost("Blinky", _position) {};

  protected:
    sf::Vector2i getChasePosition() override {
        return pacman->getMapPosition();
    }
};

#endif //BLINKY_HPP