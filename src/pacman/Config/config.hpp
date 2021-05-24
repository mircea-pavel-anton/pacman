#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../PerfLogger/performance_logger.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;
class GameTile;
typedef vector<vector<vector<GameTile*>>> vec3pGT;
typedef vector<vector<GameTile*>> vec2pGT;
typedef vector<GameTile*> vec1pGT;

class Config {

public:
    ~Config();
    static Config *getInstance();
    Config(const Config&) = delete;
    Config &operator=(const Config&) = delete;

    // Map params.
    sf::Vector2i map_size = {0, 0}; // map size, in tiles
    const float tile_size = 22.f;   // tile size, in pixels.

    // Window params.
    sf::Vector2i window_size = {0, 0};  // window size, in pixels
    const int window_framerate = 60;    // window framerate, in Hz.
    const string window_title = "Pacman";

    // Offset values used to center the map on the window.
    sf::Vector2f offset = {250.f, 75.f};

    // Movement speed, in pixels/frame.
    const float speed = 1;

    // Default durations for ghost states.
    const int frightened_timer = 5 * window_framerate / tile_size;
    const int chase_timer = 30 * window_framerate / tile_size;
    const int scatter_timer = 10 * window_framerate / tile_size;
    const int hurt_timer = 2 * window_framerate / tile_size;

    const std::string empty_texture = "res/sprites/empty.png";
    const std::string font_file = "res/fonts/emulogic.ttf";

    std::map<const string, const string> sounds = {
        { "background_music", "res/audio/background_music.wav" },
        { "food", "res/audio/food_chomp.wav" },
        { "fruit", "res/audio/fruit_chomp.wav" },
        { "pellet", "res/audio/power_pellet_chomp.wav" },
        { "ghost", "res/audio/ghost_chomp.wav" },
        { "pacman", "res/audio/hurt.wav" },
        { "start", "res/audio/press_start.wav" },
        { "game_over", "res/audio/game_over.wav" },
        { "thank_you", "res/audio/thank_you.wav" },
    };

    std::map<const string, const string> maps = {
        { "single-player", "res/maps/single-player.map" },
        { "dual-player", "res/maps/dual-player.map" },
    };

private:
    static Config *instance;
    Config();

}; //Config

#endif //CONFIG_HPP