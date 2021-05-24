#include "config.hpp"

Config *Config::instance = nullptr;

Config::Config() { /* nothing to do here */ }
Config::~Config() { /* nothing to do here */ }

Config *Config::getInstance() {
    if (instance == nullptr) instance = new Config();
    return instance;
}