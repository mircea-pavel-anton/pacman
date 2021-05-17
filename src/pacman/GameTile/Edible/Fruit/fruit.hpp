#ifndef FRUIT_HPP
#define FRUIT_HPP

#include <SFML/Graphics.hpp>
#include <random>

#include "../edible.hpp"

class Fruit : public Edible {

public:
    Fruit(const sf::Vector2f &);

private:
    std::vector<std::string> fruit_textures = {
        textures_root_dir + "apple.png",
        textures_root_dir + "cherry.png",
        textures_root_dir + "lime.png",
        textures_root_dir + "peach.png",
        textures_root_dir + "pineapple.png",
        textures_root_dir + "strawberry.png",
    };

}; //Fruit

#endif //FRUIT_HPP