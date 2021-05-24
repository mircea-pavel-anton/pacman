#include "game.hpp"

Game::Game() {
    initSounds();
    background_music->play();

    initMap();
    initWindow();
}
Game::~Game() {
    // Play "Thank you" sound, and wait for it to finish before proceeding.
    ty_sound->play();
    while (ty_sound->getStatus() == sf::Sound::Status::Playing) {
        sf::sleep(sf::milliseconds(100.f));
    }

    // stop background music
    background_music->stop();

    // Delete the config instance.
    delete Config::getInstance();

    // Delete the window.
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

        // Once the game is over, allow the user to exit on any key press.
        if (game_over && event.type == sf::Event::KeyPressed) {
            window->close();
        }
    }

    PerfLogger::getInstance()->stopJob("Game::pollEvents");
}

void Game::update() {
    pollEvents();
    if (!game_over) {
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

        if (Food::counter <= 0) game_over = true;;

        PerfLogger::getInstance()->stopJob("Game::Update");
    } else {
        sf::sleep(sf::milliseconds(200));
    }
}

void Game::render() const {
    PerfLogger::getInstance()->startJob("Game::Render");

    // Clear the old frame from the window.
    window->clear();

    title->render(window); // Render the title at the top of the screen.
    // Render every game tile.
    for (const vec2pGT &vec2 : map) {
        for (const vec1pGT &vec1 : vec2) {
            for (GameTile *tile : vec1) {
                if (tile != nullptr) tile->render(window);
            }
        }
    }

    if (game_over) game_over_title->render(window);

    // Display the newly rendered frame onto the window.
    window->display();

    PerfLogger::getInstance()->stopJob("Game::Render");
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

    initTitles();

    // Show the window.
    window->setVisible(true);

    PerfLogger::getInstance()->stopJob("Game::initWindow");
}

vector<vector<char>> Game::readMap() {
    PerfLogger::getInstance()->startJob("Game::readMap");

    std::ifstream file(Config::getInstance()->selected_map, std::ios::binary);
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

void Game::initTitles() {
    PerfLogger::getInstance()->startJob("Game::initTitle");

    Config *config = Config::getInstance();
    
    const sf::Vector2f title_position = {
        static_cast<float>(config->window_size.x) / 2 - 100,
        static_cast<float>(config->offset.y) / 2 - 15,
    };
    title = new WindowTitle(Config::getInstance()->window_title, title_position);

    const sf::Vector2f go_title_position = {
        static_cast<float>(config->window_size.x) / 2 - 135,
        static_cast<float>(config->window_size.y) / 2 - 40,
    };
    game_over_title = new WindowTitle("Game Over", go_title_position);

    PerfLogger::getInstance()->stopJob("Game::initTitle");
}

void Game::initSounds() {
    PerfLogger::getInstance()->startJob("Game::initSounds");
 
    Config *config = Config::getInstance();

    // Get a copy of the sound objects pointers.
    ty_sound = config->sounds["thank_you"];
    background_music = config->sounds["background_music"];

    // Turn down the volume of the background track
    background_music->setVolume(50.f);
 
    PerfLogger::getInstance()->stopJob("Game::initSounds");
}

void Game::run() {
    while (isRunning()) {
        update();
        render();
    }
}

