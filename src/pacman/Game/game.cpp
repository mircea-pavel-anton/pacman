#include "game.hpp"

Game::Game() {
    initSounds();
    background_music.play();

    initMap();
    initWindow();
}
Game::~Game() {
    // Play "Thank you" sound, and wait for it to finish before proceeding.
    ty_sound.play();
    while (ty_sound.getStatus() == sf::Sound::Status::Playing) {
        sf::sleep(sf::milliseconds(100.f));
    }

    // stop background music
    background_music.stop();

    if (window != nullptr) {
        delete window;
    }

    // Delete every tile on the map.
    for (vec2pGT &vec2 : map) {
        for (vec1pGT &vec1 : vec2) {
            for (GameTile *tile : vec1) {
                if (tile != nullptr) delete tile;
            }
        }
    }

    // Write all the collected metrics to a CSV file.
    PerfLogger::getInstance()->commit();
}

void Game::pollEvents() {
    PerfLogger::getInstance()->startJob("Game::pollEvents");

    sf::Event event;

    while (window->pollEvent(event)) {
        // If the window "x" button was pressed, close the window.
        if (event.type == sf::Event::Closed) {
            window->close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                pause = !pause;
            }
        }
    }

    PerfLogger::getInstance()->stopJob("Game::pollEvents");
}

void Game::update() {
    pollEvents();
    if (!pause) {
        PerfLogger::getInstance()->startJob("Game::Update");

        // Call the update method on every map tile.
        for (vec2pGT &vec2 : map) {
            for (vec1pGT &vec1 : vec2) {
                for (GameTile *tile : vec1) {
                    if (tile != nullptr) {
                        tile->update(window, map);
                    }
                }
            }
        }

        PerfLogger::getInstance()->stopJob("Game::Update");
    }
}

void Game::render() const {
    if (!pause) {
        PerfLogger::getInstance()->startJob("Game::Render");

        // Clear the old frame from the window.
        window->clear();

        window->draw(title); // Render the title at the top of the screen.
        // Render every game tile.
        for (const vec2pGT &vec2 : map) {
            for (const vec1pGT &vec1 : vec2) {
                for (GameTile *tile : vec1) {
                    if (tile != nullptr) tile->render(window);
                }
            }
        }

        // Display the newly rendered frame onto the window.
        window->display();

        PerfLogger::getInstance()->stopJob("Game::Render");
    }
}

bool Game::isRunning() const {
    if (window == nullptr) return false;
    return window->isOpen();
}

void Game::initWindow() {
    PerfLogger::getInstance()->startJob("Game::initWindow");

    Config *config = Config::getInstance();
    config->window_size.x = config->map_size.x * config->tile_size + config->offset.x * 2;
    config->window_size.y = config->map_size.y * config->tile_size + config->offset.y * 2;

    // Create a 4:3 non-resizeable window.
    const sf::VideoMode videoMode = sf::VideoMode(config->window_size.x, config->window_size.y);
    const sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;

    // Limit the frame-rate to 60fps and enable V-Sync.
    window = new sf::RenderWindow(videoMode, config->window_title, style);
    window->setFramerateLimit(config->window_framerate);
    window->setVerticalSyncEnabled(true);

    initTitle();

    // Show the window.
    window->setVisible(true);

    PerfLogger::getInstance()->stopJob("Game::initWindow");
}

vector<vector<char>> Game::readMap() {
    PerfLogger::getInstance()->startJob("Game::readMap");

    std::ifstream file("res/maps/multi-player.map", std::ios::binary);
    std::string line= "";
    unsigned long int line_count = 0, line_length = 0;
    vector<vector<char>> char_map = {};

    if (!file.is_open()) {
        std::cout << "ERROR: Cannot find map file" << std::endl;
        abort();
    }

    while (std::getline(file, line)) {
        if (line.size() > line_length) line_length = line.size();

        vector<char> temp = {};
        for (long unsigned int i = 0; i < line.size(); i++) {
            temp.push_back(line[i]);
        }
        char_map.push_back( temp );
        line_count++;
    }

    // Set map sizes.
    Config::getInstance()->map_size.x = line_length;
    Config::getInstance()->map_size.y = line_count;

    PerfLogger::getInstance()->stopJob("Game::readMap");
    return char_map;
}

void Game::initMap() {
    PerfLogger::getInstance()->startJob("Game::initMap");

    vector<vector<char>> char_map = readMap();
    std::vector<Ghost*> ghosts[2];
    Pacman* pacmans[2];

    int i = 0, j = 0;
    for (auto &line : char_map) {
        vec2pGT temp = {};
        
        for (char &c : line) {
            sf::Vector2f position = {
                i * Config::getInstance()->tile_size,
                j * Config::getInstance()->tile_size,
            };
            GameTile *tile = nullptr;

            if (std::string("123456!@#$%^").find(c) != std::string::npos ) {
                tile = new Wall(position, c);
            } else if (c == '*') {
                tile = new Food(position);
            } else if (c == '0') {
                tile = new PowerPellet(position);
            } else if (c == 'f') {
                tile = new Fruit(position);
            } else {
                int index = islower(c) == 0 ? 1 : 0;
                if (c == 'O' || c == 'o') {
                    tile = new Pacman(position);
                    pacmans[index] = (Pacman*)tile;
                } else if (c == 'B' || c == 'b') {
                    tile = new Blinky(position);
                    ghosts[index].push_back((Ghost*)tile);
                } else if (c == 'C' || c == 'c') {
                    tile = new Clyde(position);
                    ghosts[index].push_back((Ghost*)tile);
                } else if (c == 'I' || c == 'i') {
                    tile = new Inky(position);
                    ghosts[index].push_back((Ghost*)tile);
                } else if (c == 'P' || c == 'p') {
                    tile = new Pinky(position);
                    ghosts[index].push_back((Ghost*)tile);
                }
            }
            temp.push_back( {tile} );
            i++;
        }
        j++; i = 0;
        map.push_back(temp);
    }

    for (int i = 0; i < 2; i++) {
        for (Ghost *g : ghosts[i]) {
            g->addChasing(pacmans[i]);

            if (g->getName() == "Inky") {
                for (Ghost *b : ghosts[i]) {
                    if (b->getName() == "Blinky") {
                        g->addChasing(b);
                    }
                }
            }
        }
    }

    PerfLogger::getInstance()->stopJob("Game::initMap");
}

void Game::initTitle() {
    PerfLogger::getInstance()->startJob("Game::initTitle");

    Config *config = Config::getInstance();
    font = sf::Font();

    if (font.loadFromFile(config->font_file) == false) {
        std::cout << "ERROR: Unable to load font file." << std::endl;
        abort();
    }

    title.setFont(font);
    title.setString(config->window_title);
    title.setCharacterSize(26.f);
    title.setLetterSpacing(1.25);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({
        (config->window_size.x - title.getGlobalBounds().width ) /2,
        (config->offset.y - title.getGlobalBounds().height ) / 2,
    });

    PerfLogger::getInstance()->stopJob("Game::initTitle");
}

void Game::initSounds() {
    Config *config = Config::getInstance();
    if (press_start_buffer.loadFromFile(config->sounds["start"]) == false) {
        std::cout << "ERROR: Failed to load " << config->sounds["start"] << std::endl;
        abort();
    }
    if (game_over_buffer.loadFromFile(config->sounds["game_over"]) == false) {
        std::cout << "ERROR: Failed to load " << config->sounds["game_over"] << std::endl;
        abort();
    }
    if (ty_buffer.loadFromFile(config->sounds["thank_you"]) == false) {
        std::cout << "ERROR: Failed to load " << config->sounds["thank_you"] << std::endl;
        abort();
    }
    if (background_music.openFromFile(config->sounds["background_music"]) == false) {
        std::cout << "ERROR: Failed to load background music audio file!" << std::endl;
        abort();
    }

    // Turn down the volume of the background track
    background_music.setVolume(50.f);

    // Load buffers into sound objects.
    press_start_sound.setBuffer(press_start_buffer);
    game_over_sound.setBuffer(game_over_buffer);
    ty_sound.setBuffer(ty_buffer);
}

