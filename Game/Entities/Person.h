#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

typedef std::map<std::string, std::string> Config;

namespace HideAndSeekAndShoot
{

class World;

/**
 * A base class representing a person in the game.
 * A person visually consists of:
 *  - a head
 *  - a gun
 * and functionally consists of:
 *  - managed movement, to keep within the world
 *  - pointing the head and gun towards a target point
 *  - shooting a bullet towards a target point
 *  - health points management
 */
class Person : public sf::Drawable, public sf::Transformable
{
  public:
    
    /**
     * Constructs a non-functional Person object.
     * If we want the Person to be drawn, we should set a texture for the head (at least)
     * 
     * @param[in] world
     *  Pointer to the world from which we are creating the person
     */
    Person(World const* world);

    /**
     * Sets a texture for the person's head
     * 
     * @param[in] headTex
     *  Pointer to the texture to be used for person's head
     */
    void SetHeadTexture(sf::Texture const* headTex);

    /**
     * Sets the target point of the Person.
     * That is the point towards which the Person will be looking at all times,
     * and towards which the Person can shoot.
     * 
     * @param[in] targetPoint
     *  Target point to be set on the Person
     */
    void SetTargetPoint(sf::Vector2f const& targetPoint);

    /**
     * Moves the person in the given direction with their speed.
     * People have constant speed and so only a direction
     * can be specified for their movement.
     * 
     * TODO: these methods will probably be made private later,
     *  because the Player should be movable from the outside,
     *  but the Enemy doesn't.
     * 
     * @param[in] dirVector (or xDir, yDir)
     *  Vector specifying the direction of the movement.
     *  Not to be confused with velocity vector.
     *  The length of this vector doesn't matter, only its direction.
     */
    void MoveInDirection(sf::Vector2f const dirVector);
    void MoveInDirection(float xDir, float yDir);

    ///Updates the person for next frame
    void Update();

  private: /* functions */

    /**
     * Draws the person on a render target
     * 
     * @param[in] target
     *  Render target on which to draw the person
     * @param[in] states
     *  States/mode of the rendering
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /// Updates the person according to the data derived from sf::Transformable
    void UpdateTransform();

    /// Rotates the head sprite so that it points towards the target point
    void PointHeadTowardsTargetPoint();

    /**
     * Checks if a position is within the borders of the world
     * 
     * @param[in] position
     *  Position to check
     * 
     * @return true for inside world, false otherwise
     */
    bool IsPositionInWorld(sf::Vector2f const& position) const;

    /**
     * Checks if a move is valid,
     * meaning that it doesn't go through a wall or any other object.
     * 
     * @param[in] startPos
     *  Starting position of the move
     * @param[in] endPos
     *  Ending position of the move
     * 
     * @return true if the move is valid, false otherwise
     */
    bool IsMoveValid(
        sf::Vector2f const& startPos,
        sf::Vector2f const& endPos)
    const;

  private: /* variables */

    /// The world to which the person belongs
    World const* _world;

    /// Sprite and texture for the head of the player
    sf::Texture const* _headTex;
    sf::Sprite _headSprite;

    /// Target point, towards which the Person is always looking and can shoot
    sf::Vector2f _targetPoint;

    /// Person configuration
    Config _config;
};

} // namespace HideAndSeekAndShoot