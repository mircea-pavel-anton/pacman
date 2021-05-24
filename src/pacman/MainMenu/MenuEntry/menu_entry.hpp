#ifndef MENU_ENTRY_HPP
#define MENU_ENTRY_HPP

#include "../../Config/config.hpp"

class MenuEntry {

public:
    MenuEntry(const std::string&, const sf::Vector2f&);
    ~MenuEntry();

    // Change the entry color based on its active status.
    void update();

    // Draw the menu entry onto the given target.
    void render(sf::RenderTarget*) const;

    // Change the active status.
    void setActive(bool);

private:
    // ---------------- MEMBERS ---------------- //
    std::string content;// The actual text content of the entry.
    sf::Text text;      // The render-able object.
    bool is_active;     // The active status of the entry.

}; //MenuEntry

#endif //MENU_ENTRY_HPP