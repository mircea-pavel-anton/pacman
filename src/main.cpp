#include "pacman/Game/game.hpp"
#include "pacman/MainMenu/main_menu.hpp"

int main(int argc, char **argv) {
    MainMenu().show();

    sf::sleep(sf::milliseconds(500));

    Game().run();

    return 0;
}