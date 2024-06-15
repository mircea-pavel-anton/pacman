#include "pacman/Game/game.hpp"
#include "pacman/MainMenu/main_menu.hpp"

int main() {
    MainMenu().show();

    sf::sleep(sf::milliseconds(500));

    Game().run();

    return EXIT_SUCCESS;
}