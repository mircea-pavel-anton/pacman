#ifndef CLYDE_HPP
#define CLYDE_HPP

#include "../ghost.hpp"

class Clyde : public Ghost {
  public:
    Clyde(const sf::Vector2f &_position) : Ghost("Clyde", _position) {};

  protected:
    sf::Vector2i getChasePosition() override {
        const sf::Vector2i delta = pacman->getMapPosition() - getMapPosition();
        const int distance = delta.x * delta.x + delta.y * delta.y;
        if (distance >= 64) return pacman->getMapPosition();
        return {0, 0};
    }
};

#endif //CLYDE_HPP