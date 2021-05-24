#include "menu_animation.hpp"

MenuAnimation::MenuAnimation(const sf::Vector2f &_position) {
    sprite.setPosition(_position);
    sprite.setScale({2.f, 2.f});
    frame_counter = 0;
    textures = {
        Config::getInstance()->textures["Pacman_1_neutral"],
        Config::getInstance()->textures["Pacman_1_right_2"],
        Config::getInstance()->textures["Pacman_1_right_1"],
        Config::getInstance()->textures["Pacman_1_right_2"],
    };
}

MenuAnimation::~MenuAnimation() { /* Nothing to do here. */ }

void MenuAnimation::update() {
    int index = ( frame_counter / animation_fps ) % 4;
    sprite.setTexture( *(textures[index]) );    
    frame_counter++;
}

void MenuAnimation::render(sf::RenderTarget *_target) const {
    _target->draw(sprite);
}