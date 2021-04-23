#pragma once

#include "Person.h"
#include "Player.h"
#include "FieldOfView.h"

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
     *  Pointer to the world from which we are creating the enemy
     * @param[in] headTex
     *  Pointer to the texture to be used for enemy's head
     * @param[in] gunTex
     *  Pointer to the texture to be used for enemy's gun
     * @param[in] player
     *  Pointer to a player object - the one that will be chased by the constructed enemy.
     */
    Enemy(
        World const* world,
        sf::Texture const* headTex,
        sf::Texture const* gunTex,
        Player const* player
    );

    /// Updates the enemy for next frame - moves it towards the player
    void Update();

  private: /* functions */

    /**
     * Draws the enemy on a render target
     * 
     * @param[in] target
     *  Render target on which to draw the enemy
     * @param[in] states
     *  States/mode of the rendering
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  private: /* variables */

    /// Pointer to the player that is being chased by this enemy
    Player const* _player;

    /// Field of view of the enemy
    FieldOfView _fieldOfView;
};

} // namespace HideAndSeekAndShoot