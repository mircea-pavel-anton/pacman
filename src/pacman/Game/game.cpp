#include "game.hpp"

Game::Game() {
    initWindow();
}
Game::~Game() {
    if (window != nullptr) {
        delete window;
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
}

void Game::render() const {
    // Clear the old frame from the window.
    window->clear();

    //TODO: Render new frame

    // Display the newly rendered frame onto the window.
    window->display();
}

bool Game::isRunning() const {
    if (window == nullptr) return false;
    return window->isOpen();
}

void Game::initWindow() {
    // Create a 4:3 non-resizeable window.
    const sf::VideoMode videoMode = sf::VideoMode(1200, 900);
    const sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;
    const std::string title = "Pacman";

    // Limit the frame-rate to 60fps and enable V-Sync.
    window = new sf::RenderWindow(videoMode, title, style);
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(true);

    // Show the window.
    window->setVisible(true);
}