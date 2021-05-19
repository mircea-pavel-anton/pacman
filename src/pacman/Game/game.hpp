#ifndef GAME_HPP
#define GAME_HPP

#include "../GameTile/__barrel__.hpp"
#include "../config.hpp"

class Game {

public:
    Game();
    ~Game();

    // Listen for events on the RenderWindow.
    void pollEvents();

    // Update all the game elements.
    // Game logic takes place here.
    void update();

    // Update the window by rendering new frames.
    void render() const;

    // Returns whether or not the RenderWindow is open, i.e. if
    // the game is currently running.
    bool isRunning() const;

private:
    // ---------------- MEMBERS ---------------- //
    sf::RenderWindow *window;
    GameTile *map[MAP_WIDTH][MAP_HEIGHT];
    sf::Text title;
    sf::Font font;
    bool pause = false;

    // ---------------- METHODS ---------------- //
    void initWindow(); // intialize the RenderWindow.
    void initMap();
    void initTitle();

}; //class Game

#endif //GAME_HPP