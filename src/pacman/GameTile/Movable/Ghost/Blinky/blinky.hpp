#ifndef BLINKY_HPP
#define BLINKY_HPP

#include "../ghost.hpp"

class Blinky : public Ghost {

public:
    Blinky(const sf::Vector2f &_position) : Ghost("Blinky", _position) {
        scatter_position = {Config::getInstance()->map_size.x, 0};
    };

protected:
    sf::Vector2i getChasePosition() override {
        if (chasing.empty()) {
            std::cout << "ERROR: " + name + " has no target!" << std::endl;
            return home_position;
        }

        Movable *target = nullptr;

        if (chasing.size() == 1) {
            target = chasing.front();
        } else{
            int distance = 9999;
            sf::Vector2i delta = {0, 0};
            for (auto &t : chasing) {
                delta = t->getMapPosition() - getMapPosition();
                int t_distance = delta.x * delta.x + delta.y * delta.y;
                if (distance > t_distance) {
                    distance = t_distance;
                    target = t;
                }
            }
        }
        
        // Blinky always chases his target directly.
        return target->getMapPosition();
    }
};

#endif //BLINKY_HPP