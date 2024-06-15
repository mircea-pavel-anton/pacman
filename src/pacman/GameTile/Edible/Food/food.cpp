#include "food.hpp"

// Initialize counter.
int Food::counter = 0;

Food::Food(const sf::Vector2f &_position) : Edible("food") {
  initVars();
  position = _position;

  // increase food counter on food tile creation.
  counter++;

  // Food is not animated.
  // It has the same texture every frame.
  texture_names = {"food"};
  score_modifier = 5;

  initSprite();
}

void Food::toEatenState() {
  Edible::toEatenState();
  counter--;
}