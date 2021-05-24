#include "menu_entry.hpp"

MenuEntry::MenuEntry(const std::string &_content, const sf::Vector2f &_position) {
    content = _content;
    is_active = false;
    text = sf::Text();

    text.setFont( *(Config::getInstance()->font) );
    text.setString(_content);
    text.setCharacterSize(22.f);
    text.setLetterSpacing(1.25);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(_position);
}
MenuEntry::~MenuEntry() { /* nothing to do here */ }

void MenuEntry::setActive(bool _active) { is_active = _active; }

void MenuEntry::update() {
    if (is_active) {
        text.setFillColor(sf::Color::Blue);
    } else {
        text.setFillColor(sf::Color::Yellow);
    }
}

void MenuEntry::render(sf::RenderTarget *_target) const {
    _target->draw(text);
}





