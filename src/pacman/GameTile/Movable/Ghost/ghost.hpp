#ifndef GHOST_HPP
#define GHOST_HPP

#include "../../../RNG/rng.hpp"
#include "../Pacman/pacman.hpp"
#include "../movable.hpp"

class Pacman;

enum class GhostStates { Frightened, Dead, Chase, Scatter, Escape };

class Ghost : public Movable {
 public:
  Ghost(const std::string &, const sf::Vector2f &);  // default constructor
  Ghost(const Ghost &);                              // copy constructor
  Ghost &operator=(const Ghost &);                   // assignment operator

  void update(const sf::RenderTarget *, vec3pGT &) override;
  void render(sf::RenderTarget *) const override;

  // Ghosts can walk through other ghosts.
  // Pacman can walk over a ghost and die, or kill it.
  inline bool isWalkable() const override { return true; }

  // Set the Pacman that this ghost will chase.
  void addChasing(Movable *_target) { chasing.push_back(_target); }

  // State-Changing methods
  void toDeadState() { next_state = GhostStates::Dead; };
  void toFrightenedState() { next_state = GhostStates::Frightened; };
  void toChaseState() { next_state = GhostStates::Chase; };
  void toScatterState() { next_state = GhostStates::Scatter; };

  // Random accessors
  bool isScared() const { return next_state == GhostStates::Frightened; };
  bool isChasing() const { return next_state == GhostStates::Chase; };
  bool isDead() const { return next_state == GhostStates::Dead; };
  std::string getName() const { return name; }

 protected:
  // ---------------- MEMBERS ----------------
  // The pacman that this ghost will be using to calculate the
  // target that it will be chasing.
  std::vector<Movable *> chasing;

  // A simple random number generator.
  RNG rng;

  // The state that the ghost is currently in.
  GhostStates state, next_state;

  // A timer used to get out of the scared state after a given number of
  // seconds.
  int frightened_timer, chase_timer, scatter_timer;

  // A sound that is played whenever a ghost is killed.
  sf::Sound *sound;

  // The coordinates of the tile the ghost will go to when killed.
  // Right in front of the ghost house.
  const sf::Vector2i home_position = {15, 13}, escape_position = {11, 11};
  sf::Vector2i scatter_position;

  // The name of the ghost.
  std::string name;

  // A shape used to highlight the tile that this ghost is chasing.
  sf::RectangleShape trail;

  // The on-screen coordinates of the trail.
  sf::Vector2i trail_position;

  // ---------------- METHODS ----------------
  // initialization methods
  void initVars() override;
  void initTrail();

  // Update the on-screen trail Tile.
  void updateTrail();

  // Change the animation based on state and movement direction.
  void updateAnimation() override;

  // Update the ghosts movement direction based on the current state.
  void updateMovementDirection(vec3pGT &) override;

  // Given a pair of coordinates for the destination tile and the game map,
  // this method computes the direction that would get the ghost the
  // closest to the destination.
  sf::Vector2i chase(vec3pGT &, const sf::Vector2i &) const;

  // Ranks a move in terms of how close it gets the ghost to the given
  // destination tile.
  // This is used for the path finding algorithm implemented for the
  // ghosts in order to classify a given move as better or worse.
  int rankMove(vec3pGT &, const sf::Vector2i &, const sf::Vector2i &) const;

  // Returns a random valid move for the ghost to take.
  sf::Vector2i frightened(vec3pGT &);

  // Checks whether or not the given direction is a valid move for the ghost.
  bool canMove(vec3pGT &, const sf::Vector2i &) const;

  // Check both internal and external state-switching triggers.
  void updateState();

  // Timer related methods.
  void resetTimers();
  void updateTimers();

  bool isInsideGhostHouse();
  bool isInFrontOfGhostHouse();

  // Returns the coordinates of the tile that this ghost is chasing.
  virtual sf::Vector2i getChasePosition() = 0;
};  // Ghost

#endif  // GHOST_HPP