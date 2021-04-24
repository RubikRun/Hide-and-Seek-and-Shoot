#pragma once

#include "Person.h"

namespace HideAndSeekAndShoot
{

/**
 * Player class for the player's entity.
 * The player is a person, so class Player inherits from class Person,
 * with the added functionality that the player objects can be moved from outside.
 */
class Player : public Person
{

  public:

    /**
     * Constructs a player.
     * 
     * @param[in] world
     *  Pointer to the world from which we are creating the player
     * @param[in] headTex
     *  Pointer to the texture to be used for player's head
     * @param[in] gunTex
     *  Pointer to the texture to be used for player's gun
     * @param[in] gunTex
     *  Pointer to the texture to be used for bullets shot from player's gun
     */
    Player(
      World const* world,
      sf::Texture const* headTex,
      sf::Texture const* gunTex,
      sf::Texture const* bulletTex
    );

    // making Update method public
    using Person::Update;
    // making MoveInDirection method public
    using Person::MoveInDirection;
    // making SetTargetPoint method public
    using Person::SetTargetPoint;
};

} // namespace HideAndSeekAndShoot