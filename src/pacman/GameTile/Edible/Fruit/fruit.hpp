#ifndef FRUIT_HPP
#define FRUIT_HPP

#include <SFML/Graphics.hpp>
#include <random>

#include "../edible.hpp"

class Fruit : public Edible {
 public:
  Fruit(const sf::Vector2f &);

};  // Fruit

#endif  // FRUIT_HPP