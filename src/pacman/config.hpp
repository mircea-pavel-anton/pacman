#ifndef CONFIG_HPP
#define CONFIG_HPP

static const int MAP_WIDTH = 28;       // game map width, in tiles.
static const int MAP_HEIGHT = 31;      // game map height, in tiles.
static const float TILE_SIZE = 22;     // tile size, in pixels.

static const int WINDOW_HEIGHT = 900;  // window height, in pixels.
static const int WINDOW_WIDTH = 1200;  // window widthm in pixels.

// offset values used to center the map on the window.
static const float X_OFFSET = ( WINDOW_WIDTH - MAP_WIDTH * TILE_SIZE ) / 2;
static const float Y_OFFSET = ( WINDOW_HEIGHT - MAP_HEIGHT * TILE_SIZE ) / 2;

static const int WINDOW_FRAMERATE = 60; // window framerate, in Hz.

// Movement speed, in pixels/frame.
static const float SPEED = 1;

#endif //CONFIG_HPP