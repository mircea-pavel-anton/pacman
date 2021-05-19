#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "../movable.hpp"
#include "../../../config.hpp"
#include "../../Edible/__barrel__.hpp"
#include "../Ghost/ghost.hpp"

class Ghost;

class Pacman : public Movable {
  
public:
    Pacman(const sf::Vector2f &); // default constructor
    Pacman(const Pacman&); // copy constructor
    Pacman &operator=(const Pacman&); // assignment operator

    void update(const sf::RenderTarget *, GameTile *[MAP_WIDTH][MAP_HEIGHT]) override;
    void render(sf::RenderTarget *) const override;

    // Ghosts can "walk" on top of Pacman to kill him.
    inline bool isWalkable() const override { return true; }

    // This action gets called when PacMan interacts to a Ghost
    // that is currently in case mode.
    void toDeadState();

protected:
    // ---------------- MEMBERS ----------------
    // The direction that will be assigned to @direction
    // once the character has finished moving into the 
    // tile it is currently moving into.
    sf::Vector2i next_direction;

    // The number of points that Pacman has gathered by eating pellets.
    int score;

    // Used to render the score onto the screen.
    sf::Font font; // the font used to render
    sf::Text text; // the text object itself, that will get rendered

    bool is_dead;

    // ---------------- METHODS ----------------
    // Handles interactions with tiles that PacMan is currently on top of.
    void interact(GameTile *[MAP_WIDTH][MAP_HEIGHT]);

    // Listens for keyboard events in order to change the movement direction.
    void pollEvents();

    // Initializes all members to their default values.
    void initVars() override;

    // Initializes the text and font objects.
    void initText();

    // Updates the animation vector in order to change animation for different
    // states and movement directions.
    void updateAnimation() override;

    // Makes sure to update the @text variable with the latest @score value.
    void updateScore();

    // Updates the current direction with the value of next_direction
    // IF such a thing is possible.
    void updateMovementDirection(GameTile *[MAP_WIDTH][MAP_HEIGHT]) override;

    // A wrapper method that calls all the other collision detection methods.
    void checkCollisions(const sf::RenderTarget *, GameTile *[MAP_WIDTH][MAP_HEIGHT]);

    // Checks if the sprite is currently colliding with the window itself.
    // Doesn't let the sprite get off-screen.
    void checkWindowCollisions(const sf::RenderTarget *);

    // Checks if the sprite is currently colliding with any other object on
    // the game map, such as walls, pellets, fruits or ghosts.
    void collideWithObjects(GameTile *[MAP_WIDTH][MAP_HEIGHT]);

private:
    static int counter;
    int index;

}; //Pacman

#endif //PACMAN_HPP