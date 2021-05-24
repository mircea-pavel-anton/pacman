#ifndef MENU_TITLE_HPP
#define MENU_TITLE_HPP

#include "../../Config/config.hpp"

class MenuTitle {

public:
    MenuTitle(const std::string&, const sf::Vector2f&);
    ~MenuTitle();

    // Does absolutely nothing. Nada.
    void update();

    // Draws the title onto the given target.
    void render(sf::RenderTarget*) const;

private:
    // ---------------- MEMBERS ---------------- //
    std::string content;// the actual text content of the title
    sf::Text text;      // the render-able object

}; //MenuTitle

#endif //MENU_TITLE_HPP