#ifndef FOOD_HPP
#define FOOD_HPP

#include <SFML/Graphics.hpp>

#include "../edible.hpp"

class Food : public Edible {
 public:
  Food(const sf::Vector2f &);
  void toEatenState() override;

  static int counter;
};  // Food

#endif  // FOOD_HPP