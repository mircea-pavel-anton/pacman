#include "game_tile.hpp"

GameTile::GameTile() { /* nothing to do here */
}
GameTile::~GameTile() { /* nothing to do here */
}

void GameTile::initVars() {
  // @sprite has it's own init method.
  // @position should be set from the child class constructor.
  // @texture_paths should be set from the child class constructor.
  // @textures are loaded in the @initSprite() method.
  frame_counter = 0;
}

void GameTile::initSprite() {
  sprite = sf::Sprite();

  loadTextures();

  // Set on-screen position.
  sprite.setPosition(position);
}

void GameTile::loadTextures() {
  // Make sure vector is empty, just in case.
  if (textures.empty() == false) textures.clear();

  // Load each texture into the animation vector.
  Config *config = Config::getInstance();
  for (std::string &name : texture_names) {
    textures.push_back(config->textures[name]);
  }
}

void GameTile::updateSprite() {
  // Load the texture from the animation vector
  const int animation_index = (frame_counter / animation_fps) % textures.size();
  sprite.setTexture(*(textures[animation_index]));

  // Frame counter only goes from 0 to WINDOW_FRAMERATE - 1
  frame_counter = (frame_counter + 1) % 60;

  // Update on-screen position
  sprite.setPosition({
      position.x + Config::getInstance()->offset.x,
      position.y + Config::getInstance()->offset.y,
  });
}

void GameTile::render(sf::RenderTarget *_target) const {
  _target->draw(sprite);
}

GameTile &GameTile::operator=(const GameTile &_other) {
  position = _other.position;
  texture_names = _other.texture_names;
  textures = _other.textures;
  frame_counter = _other.frame_counter;
  sprite = _other.sprite;

  return *this;
}
