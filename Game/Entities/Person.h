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
 *  - managed movement, to keep within the world and not collide with walls or other Person objects
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
     *  but the Enemy should not.
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
     * Checks if a position is outside of all the walls,
     * meaning that it does not intersect with any one of them,
     * meaning that there is no point lying inside the person's collision circle
     * and inside a wall.
     * 
     * @param[in] position
     *  Position to check
     * 
     * @return true for outside walls, false if there is an intersection with a wall
     */
    bool IsPositionOutsideWalls(sf::Vector2f const& position) const;

    /**
     * Checks if a position is valid,
     * meaning that the player is inside the world's borders
     * and he does not intersect with any walls.
     * 
     * @param[in] position
     *  Position to check
     * 
     * @return true for valid position, false for invalid
     */
    bool IsPositionValid(sf::Vector2f const& position) const;

  private: /* variables */

    /// The world to which the person belongs
    World const* _world;

    /// Sprite for the head of the player
    sf::Sprite _headSprite;

    /// Target point, towards which the Person is always looking and can shoot
    sf::Vector2f _targetPoint;

    /// Person configuration
    Config _config;

    /* Person collisions with other objects are detected if the other object is
        within this radius to the player's center.
        Essentially a person is a circle for the collision detection. */
    float _collisionRadius;
};

} // namespace HideAndSeekAndShoot