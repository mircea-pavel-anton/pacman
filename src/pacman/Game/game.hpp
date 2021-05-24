#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

#include "../GameTile/__barrel__.hpp"
#include "../config.hpp"
#include <SFML/Audio.hpp>

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
    sf::Music background_music;
    sf::SoundBuffer press_start_buffer, game_over_buffer, ty_buffer;
    sf::Sound press_start_sound, game_over_sound, ty_sound;
    vec3pGT map;
    sf::Text title;
    sf::Font font;
    bool pause = false;

    // ---------------- METHODS ---------------- //
    void initWindow(); // intialize the RenderWindow.
    void initMap();
    vector<vector<char>> readMap();
    void initTitle();
    void initSounds();

}; //class Game

#endif //GAME_HPP