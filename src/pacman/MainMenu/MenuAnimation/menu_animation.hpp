#ifndef MENU_ANIMATION_HPP
#define MENU_ANIMATION_HPP

#include "../../Config/config.hpp"

class MenuAnimation {
 public:
  MenuAnimation(const sf::Vector2f &);
  ~MenuAnimation();

  void update();
  void render(sf::RenderTarget *) const;

 private:
  // ---------------- MEMBERS ---------------- //
  // The sprite that will get rendered onto the screen.
  sf::Sprite sprite;

  // The vector of animation frames that will be used to animate the sprite.
  std::vector<sf::Texture *> textures;

  // A simple counter that keeps track of the current frame index,
  // from 0 to WINDOW_FRAMERATE - 1.
  // Used for determining at which animation frame we currently are.
  int frame_counter;

  // The number of actual rendered frames that should occur between 2 subsequent
  // animation frame changes.
  // In other words, for how many frames the animation should be stationary.
  const int animation_fps = 8;

};  // MenuAnimation

#endif  // MENU_ANIMATION_HPP