#include "config.hpp"

Config *Config::instance = nullptr;

Config::Config() {
    for (auto &file : sound_files) {
        sf::SoundBuffer *buffer = new sf::SoundBuffer();
        if (buffer->loadFromFile(file.second) == false) {
            std::cout << "ERROR: Failed to load " << file.second << std::endl;
            abort();
        }
        sf::Sound *sound = new sf::Sound(*buffer);

        buffers.emplace(file.first, buffer);
        sounds.emplace(file.first, sound);
    }

    for (auto &file : texture_files) {
        sf::Texture *texture = new sf::Texture();

        if (texture->loadFromFile(file.second) == false) {
            std::cout << "ERROR: Failed to load " << file.second << std::endl;
            abort();
        }

        textures.emplace(file.first, texture);
    }
}
Config::~Config() {
    for (auto &entry : sounds) {
        delete entry.second;
    }

    for (auto &entry : buffers) {
        delete entry.second;
    }

    for (auto &entry : textures) {
        delete entry.second;
    }
}

Config *Config::getInstance() {
    if (instance == nullptr) instance = new Config();
    return instance;
}