#include "game.hpp"

Game::Game() {
    initMap();
    initWindow();
}
Game::~Game() {
    if (window != nullptr) {
        delete window;
    }

    // Delete the map
    for (auto &row : map) {
        for (auto &tile : row) {
            if (tile != nullptr) delete tile;
        }
    }
}

void Game::pollEvents() const {
    sf::Event event;

    while (window->pollEvent(event)) {
        // If the window "x" button was pressed, close the window.
        if (event.type == sf::Event::Closed) {
            window->close();
        }
    }
}

void Game::update() {
    pollEvents();
    
    // Delete the map
    for (auto &row : map) {
        for (auto &tile : row) {
            if (tile != nullptr) tile->update(window, map);
        }
    }
}

void Game::render() const {
    // Clear the old frame from the window.
    window->clear();

    // Render every game tile.
    for (auto &row : map) {
        for (auto &tile : row) {
            if (tile != nullptr) tile->render(window);
        }
    }

    // Display the newly rendered frame onto the window.
    window->display();
}

bool Game::isRunning() const {
    if (window == nullptr) return false;
    return window->isOpen();
}

void Game::initWindow() {
    // Create a 4:3 non-resizeable window.
    const sf::VideoMode videoMode = sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    const sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;

    // Limit the frame-rate to 60fps and enable V-Sync.
    window = new sf::RenderWindow(videoMode, WINDOW_TITLE, style);
    window->setFramerateLimit(WINDOW_FRAMERATE);
    window->setVerticalSyncEnabled(true);

    // Show the window.
    window->setVisible(true);   
}

void Game::initMap() {
    std::ifstream file("res/maps/default.map", std::ios::binary);
    std::string line;
    int line_count = 0;

    if (!file.is_open()) {
        std::cout << "ERROR: Cannot find map file" << std::endl;
        abort();
    }

    while (std::getline(file, line)) {
        if (line.length() != MAP_WIDTH) {
            std::cout << "ERROR: Unable to parse map. Invalid dimensions." << std::endl;
            abort();
        }
        
        for (int i = 0; i < MAP_WIDTH; i++) {
            const sf::Vector2f pos = {
                i * TILE_SIZE,           // x axis coordinate
                line_count * TILE_SIZE   // y axis coordinate
            };

            GameTile *tile = nullptr;
            switch (line.at(i)) {
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '!':
                case '@':
                case '#':
                case '$':
                case '%':
                case '^':
                    tile = new Wall(pos, line.at(i));
                    break;
                default: 
                    tile = nullptr;
            }
            map[i][line_count] = tile;
        }
        line_count++;
    }
}
