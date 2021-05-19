#ifndef GHOST_HPP
#define GHOST_HPP

#include "../../../RNG/rng.hpp"
#include "../movable.hpp"
#include "../Pacman/pacman.hpp"

class Pacman;

enum GhostStates { Frightened, Dead, Chase, Scatter };

class Ghost : public Movable {

public:
    Ghost(const std::string &, const sf::Vector2f &); // default constructor
    Ghost(const Ghost&); // copy constructor
    Ghost &operator=(const Ghost&); // assignment operator

    void update(const sf::RenderTarget *, GameTile *[MAP_WIDTH][MAP_HEIGHT]) override;
    void render(sf::RenderTarget *) const override;

    // Ghosts can walk through other ghosts.
    // Pacman can walk over a ghost and die, or kill it.
    inline bool isWalkable() const override { return true; }

    // Set the Pacman that this ghost will chase.
    void addChasing(Movable *_target) { chasing.push_back(_target); }

    // State-Changing methods
    void toDeadState();
    void toFrightenedState();
    void toChaseState();
    void toScatterState();

    // Random accessors
    bool isScared() const { return state == GhostStates::Frightened; };
    bool isChasing() const { return state == GhostStates::Chase; };
    bool isDead() const { return state == GhostStates::Dead; };
    std::string getName() const { return name; }

protected:
    // ---------------- MEMBERS ----------------
    // The pacman that this ghost will be using to calculate the
    // target that it will be chasing.
    std::vector<Movable*> chasing;

    // A simple random number generator.
    RNG rng;

    // The state that the ghost is currently in.
    GhostStates state;

    // A timer used to get out of the scared state after a given number of seconds.
    int scared_timer, chase_timer, scatter_timer;

    // The coordinates of the tile the ghost will go to when killed.
    // Right in front of the ghost house.
    const sf::Vector2i home_position = {13, 11};
    sf::Vector2i scatter_position;
    
    // The name of the ghost.
    std::string name;

    // A shape used to highlight the tile that this ghost is chasing.
    sf::RectangleShape trail;

    // The on-screen coordinates of the trail.
    sf::Vector2f trail_position;

    // ---------------- METHODS ----------------
    // initialization methods
    void initVars() override;
    void initTrail();

    // Update the on-screen trail Tile.
    void updateTrail();

    // Change the animation based on state and movement direction.
    void updateAnimation() override;

    // Update the ghosts movement direction based on the current state.
    void updateMovementDirection(GameTile *_map[MAP_WIDTH][MAP_HEIGHT]) override;

    // Given a pair of coordinates for the destination tile and the game map,
    // this method computes the direction that would get the ghost the
    // closest to the destination.
    sf::Vector2i chase(GameTile *[MAP_WIDTH][MAP_HEIGHT], const sf::Vector2i &) const;

    // Ranks a move in terms of how close it gets the ghost to the given
    // destination tile.
    // This is used for the path finding algorithm implemented for the
    // ghosts in order to classify a given move as better or worse.
    int rankMove(GameTile *[MAP_WIDTH][MAP_HEIGHT], const sf::Vector2i&, const sf::Vector2i&) const;

    // Returns a random valid move for the ghost to take.
    sf::Vector2i frightened(GameTile *[MAP_WIDTH][MAP_HEIGHT]);

    // Checks whether or not the given direction is a valid move for the ghost.
    bool canMove(GameTile *[MAP_WIDTH][MAP_HEIGHT], const sf::Vector2i &) const;

    inline void resetTimers() {
        scared_timer = SCARED_DURATION;
        chase_timer = CHASE_DURATION;
        scatter_timer = SCATTER_DURATION;
    }

    // Returns the coordinates of the tile that this ghost is chasing.
    virtual sf::Vector2i getChasePosition() = 0;
}; // Ghost

#endif //GHOST_HPP