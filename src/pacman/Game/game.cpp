#include "game.hpp"

Game::Game() {
    initMap();
    initWindow();
}
Game::~Game() {
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

    // Create a 4:3 non-resizeable window.
    const sf::VideoMode videoMode = sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    const sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;

    // Limit the frame-rate to 60fps and enable V-Sync.
    window = new sf::RenderWindow(videoMode, WINDOW_TITLE, style);
    window->setFramerateLimit(WINDOW_FRAMERATE);
    window->setVerticalSyncEnabled(true);

    initTitle();

    // Show the window.
    window->setVisible(true);

    PerfLogger::getInstance()->stopJob("Game::initWindow");
}

void Game::initMap() {
    PerfLogger::getInstance()->startJob("Game::initMap");

    std::ifstream file("res/maps/default.map", std::ios::binary);
    std::string line= "";
    int line_count = 0;
    std::vector<Ghost*> ghosts1, ghosts2;
    std::vector<Pacman*> pacmans;

    if (!file.is_open()) {
        std::cout << "ERROR: Cannot find map file" << std::endl;
        abort();
    }

    while (std::getline(file, line)) {        
        vec2pGT temp = {};
        for (long unsigned int i = 0; i < line.size(); i++) {
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
                case '*':
                    tile = new Food(pos);
                    break;
                case '0':
                    tile = new PowerPellet(pos);
                    break;
                case 'f':
                    tile = new Fruit(pos);
                    break;
                case 'O':
                    tile = new Pacman(pos);
                    pacmans.push_back((Pacman*)(tile));
                    break;
                case 'o':
                    tile = new Pacman(pos);
                    pacmans.push_back((Pacman*)(tile));
                    break;
                case 'b':
                    tile = new Blinky(pos);
                    ghosts1.push_back((Blinky*)(tile));
                    break;
                case 'B':
                    tile = new Blinky(pos);
                    ghosts2.push_back((Blinky*)(tile));
                    break;
                case 'c':
                    tile = new Clyde(pos);
                    ghosts1.push_back((Clyde*)(tile));
                    break;
                case 'C':
                    tile = new Clyde(pos);
                    ghosts2.push_back((Clyde*)(tile));
                    break;
                case 'p':
                    tile = new Pinky(pos);
                    ghosts1.push_back((Pinky*)(tile));
                    break;
                case 'P':
                    tile = new Pinky(pos);
                    ghosts2.push_back((Pinky*)(tile));
                    break;
                case 'i':
                    tile = new Inky(pos);
                    ghosts1.push_back((Inky*)(tile));
                    break;
                case 'I':
                    tile = new Inky(pos);
                    ghosts2.push_back((Inky*)(tile));
                    break;
                default: 
                    tile = nullptr;
            }
            temp.push_back( {tile} );
        }
        map.push_back(temp);
        line_count++;
    }
    for (auto g : ghosts1) {
        if (g != nullptr && pacmans.front() != nullptr) {
            g->addChasing(pacmans.front());
            if (g->getName() == "Inky") {
                for (auto b : ghosts1) {
                    if (b->getName() == "Blinky")
                        g->addChasing(b);
                }
            }
        }
    }
    for (auto g : ghosts2) {
        if (g != nullptr && pacmans.front() != nullptr && pacmans.front() != pacmans.back()) {
             g->addChasing(pacmans.back());
            if (g->getName() == "Inky") {
                for (auto b : ghosts1) {
                    if (b->getName() == "Blinky")
                        g->addChasing(b);
                }
            }
        }
    }

    PerfLogger::getInstance()->stopJob("Game::initMap");
}

void Game::initTitle() {
    PerfLogger::getInstance()->startJob("Game::initTitle");

    font = sf::Font();

    if (font.loadFromFile("res/fonts/emulogic.ttf") == false) {
        std::cout << "ERROR: Unable to load font file." << std::endl;
        abort();
    }

    title.setFont(font);
    title.setString(WINDOW_TITLE);
    title.setCharacterSize(26.f);
    title.setLetterSpacing(1.25);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({
        (WINDOW_WIDTH - title.getGlobalBounds().width ) /2,
        (Y_OFFSET - title.getGlobalBounds().height ) / 2,
    });

    PerfLogger::getInstance()->stopJob("Game::initTitle");
}