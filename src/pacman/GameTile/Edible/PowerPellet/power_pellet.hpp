#ifndef POWER_PELLET_HPP
#define POWER_PELLET_HPP

#include <SFML/Graphics.hpp>

#include "../edible.hpp"

class PowerPellet : public Edible {

  public:
    PowerPellet(const sf::Vector2f &);

}; //class PowerPellet

#endif //POWER_PELLET_HPP