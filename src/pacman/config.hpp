#ifndef CONFIG_HPP
#define CONFIG_HPP

// Common libraries used in most if not all source files
#include "PerfLogger/performance_logger.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
using std::vector;
class GameTile;
typedef vector<vector<vector<GameTile*>>> vec3pGT;
typedef vector<vector<GameTile*>> vec2pGT;
typedef vector<GameTile*> vec1pGT;

// Map params.
static int MAP_WIDTH;               // game map width, in tiles.
static int MAP_HEIGHT;              // game map height, in tiles.
static const float TILE_SIZE = 22;  // tile size, in pixels.

// Window params.
static int WINDOW_HEIGHT;               // window height, in pixels.
static int WINDOW_WIDTH;                // window widthm in pixels.
static const int WINDOW_FRAMERATE = 60; // window framerate, in Hz.
static const std::string WINDOW_TITLE = "Pacman";

// Offset values used to center the map on the window.
static float X_OFFSET = 250;
static float Y_OFFSET = 75;

// Movement speed, in pixels/frame.
static const float SPEED = 1;

// Default durations for ghost states.
const int SCARED_DURATION = 5 * WINDOW_FRAMERATE / TILE_SIZE;
const int CHASE_DURATION = 30 * WINDOW_FRAMERATE / TILE_SIZE;
const int SCATTER_DURATION = 10 * WINDOW_FRAMERATE / TILE_SIZE;
const int HURT_DURATION = 2 * WINDOW_FRAMERATE / TILE_SIZE;

static const std::string EMPTY_TEXTURE = "res/sprites/empty.png";
static const std::string FONT_FILE_PATH = "res/fonts/emulogic.ttf";
static const std::string BACKGROUND_MUISC_FILE = "res/audio/background_music.wav";
static const std::string FOOD_SOUND = "res/audio/food_chomp.wav";
static const std::string FRUIT_SOUND = "res/audio/fruit_chomp.wav";
static const std::string PELLET_SOUND = "res/audio/power_pellet_chomp.wav";
static const std::string GHOST_SOUND = "res/audio/ghost_chomp.wav";
static const std::string HURT_SOUND = "res/audio/hurt.wav";
static const std::string START_SOUND = "res/audio/press_start.wav";
static const std::string GAME_OVER_SOUND = "res/audio/game_over.wav";
static const std::string TY_SOUND = "res/audio/thank_you.wav";

#endif //CONFIG_HPP
