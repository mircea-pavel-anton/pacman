#include "pacman/Game/game.hpp"

int main(int argc, char **argv) {
    Game game = Game();

    while (game.isRunning()) {
        game.update();
        game.render();
    }

    return 0;
}