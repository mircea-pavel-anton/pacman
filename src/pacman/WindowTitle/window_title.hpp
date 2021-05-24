#ifndef WINDOW_TITLE_HPP
#define WINDOW_TITLE_HPP

#include "../Config/config.hpp"

class WindowTitle {

public:
    WindowTitle(const std::string&, const sf::Vector2f&);
    ~WindowTitle();

    // Does absolutely nothing. Nada.
    void update();

    // Draws the title onto the given target.
    void render(sf::RenderTarget*) const;

private:
    // ---------------- MEMBERS ---------------- //
    std::string content;// the actual text content of the title
    sf::Text text;      // the render-able object

}; //WindowTitle

#endif //WINDOW_TITLE_HPP