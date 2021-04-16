#pragma once

#include "Person.h"
#include "Player.h"

namespace HideAndSeekAndShoot
{

/**
 * Enemy class for the enemy's entity.
 * The enemy is a person so class Enemy inherits from class Person,
 * with the added functionality that the enemy always moves towards its target point.
 */
class Enemy : public Person
{


  public:

    /**
     * Constructs an enemy.
     * 
     * @param[in] world
     *  Pointer to the world from which we are creating the person
     * @param[in] headTex
     *  Pointer to the texture to be used for person's head
     * @param[in] player
     *  Pointer to a player object - the one that will be chased by the constructed enemy.
     */
    Enemy(
        World const* world,
        sf::Texture const* headTex,
        Player const* player
    );

    /// Updates the enemy for next frame - moves it towards the player
    void Update();

  private:

    /// Pointer to the player that is being chased by this enemy
    Player const* _player;

};

} // namespace HideAndSeekAndShoot