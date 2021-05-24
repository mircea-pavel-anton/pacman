#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

#include "../GameTile/__barrel__.hpp"
#include "../Config/config.hpp"

class Game {

public:
    Game();
    ~Game();

    void run();

private:
    // ---------------- MEMBERS ---------------- //
    sf::RenderWindow *window;
    sf::Sound *ty_sound, *background_music;
    vec3pGT map;
    sf::Text title;
    bool pause = false;

    // ---------------- METHODS ---------------- //
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

    void initWindow(); // intialize the RenderWindow.
    void initMap();
    vector<vector<char>> readMap();
    void initTitle();
    void initSounds();

}; //class Game

#endif //GAME_HPP