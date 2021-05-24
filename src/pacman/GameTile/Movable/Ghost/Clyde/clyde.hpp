#ifndef CLYDE_HPP
#define CLYDE_HPP

#include "../ghost.hpp"

class Clyde : public Ghost {

public:
    Clyde(const sf::Vector2f &_position) : Ghost("Clyde", _position) {
        scatter_position = {0, Config::getInstance()->map_size.y};
    };

protected:
    sf::Vector2i getChasePosition() override {
        PERFLOGGER_START_JOB("Ghost::" + name + "::getChasePosition");

        if (chasing.empty()) {
            std::cout << "ERROR: " + name + " has no target!" << std::endl;
            PERFLOGGER_STOP_JOB("Ghost::" + name + "::getChasePosition");
            return home_position;
        }

        Movable *target = nullptr;
        int distance = 9999;
        sf::Vector2i delta = {0, 0};

        if (chasing.size() == 1) {
            target = chasing.front();
            delta = target->getMapPosition() - getMapPosition();
            distance = delta.x * delta.x + delta.y * delta.y;
        } else{
            for (Movable *t : chasing) {
                delta = t->getMapPosition() - getMapPosition();
                int t_distance = delta.x * delta.x + delta.y * delta.y;
                if (distance > t_distance) {
                    distance = t_distance;
                    target = t;
                }
            }
        }

        // If clyde is more than 8 tiles away from his target, he
        // wil be chasing after it.
        // Once he gets within 8 tiles of it, he will run back to
        // his spawn point.
        PERFLOGGER_STOP_JOB("Ghost::" + name + "::getChasePosition");
        if (distance >= 64) return target->getMapPosition();
        return scatter_position;
    }
};

#endif //CLYDE_HPP