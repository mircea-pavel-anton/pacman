#ifndef CONFIG_HPP
#define CONFIG_HPP

// Common libraries used in most if not all source files
#include "PerfLogger/performance_logger.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using std::vector;
class GameTile;
typedef vector<vector<vector<GameTile*>>> vec3pGT;
typedef vector<vector<GameTile*>> vec2pGT;
typedef vector<GameTile*> vec1pGT;

// Map params.
static int MAP_WIDTH = 28;       // game map width, in tiles.
static int MAP_HEIGHT = 31;      // game map height, in tiles.
static const float TILE_SIZE = 22;     // tile size, in pixels.

// Window params.
static const int WINDOW_HEIGHT = 900;  // window height, in pixels.
static const int WINDOW_WIDTH = 1200;  // window widthm in pixels.
static const int WINDOW_FRAMERATE = 60; // window framerate, in Hz.
static const std::string WINDOW_TITLE = "Pacman";

// offset values used to center the map on the window.
static const float X_OFFSET = ( WINDOW_WIDTH - MAP_WIDTH * TILE_SIZE ) / 2;
static const float Y_OFFSET = ( WINDOW_HEIGHT - MAP_HEIGHT * TILE_SIZE ) / 2;

// Movement speed, in pixels/frame.
static const float SPEED = 1;

// Default durations for ghost states.
const int SCARED_DURATION = 5 * WINDOW_FRAMERATE / TILE_SIZE;
const int CHASE_DURATION = 30 * WINDOW_FRAMERATE / TILE_SIZE;
const int SCATTER_DURATION = 10 * WINDOW_FRAMERATE / TILE_SIZE;

static const std::string EMPTY_TEXTURE = "res/sprites/empty.png";

#endif //CONFIG_HPP