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
  protected: /* functions */
    
    /**
     * Constructs a person.
     * 
     * @param[in] world
     *  Pointer to the world from which we are creating the person
     * @param[in] headTex
     *  Pointer to the texture to be used for person's head
     * @param[in] configFilename
     *  Name of the file with Person's config (config file for concrete derived class)
     */
    Person(
      World const* world,
      sf::Texture const* headTex,
      std::string const& configFilename
    );

    /// Updates the person for next frame
    virtual void Update();

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
    void SetTargetPoint(sf::Vector2f const targetPoint);

    /**
     * Moves the person in the given direction with their speed.
     * People have constant speed and so only a direction
     * can be specified for their movement.
     * 
     * @param[in] dirVector (or xDir, yDir)
     *  Vector specifying the direction of the movement.
     *  Not to be confused with velocity vector.
     *  The length of this vector doesn't matter, only its direction.
     */
    void MoveInDirection(sf::Vector2f const dirVector);
    void MoveInDirection(float xDir, float yDir);

    /**
     * Moves the person towards a target point with their speed.
     * If there is an obstacle in the way, the person will try to go around it.
     * The way the person tries to go around object is a little simplistic.
     * (I will probably implement Dijkstra, or better yet A*, in the future,
     *  but it will be too computationally intense if it's on a graph of each pixel.
     *  It will have to choose some sparse pixel grid, for example taking every 10-th pixel.)
     * If the person cannot go directly in the target direction,
     * he will choose a valid direction closest to the target direction and go to it.
     * It does that by trying d + phi, d - phi, d + 2phi, d - 2phi, d + 3phi, d - 3phi, ...
     * (where d is the direction towards the target point and phi is some tiny angle)
     * until it finds a direction it can go to.
     * In practice this works fine for small enough phi,
     * and also it has a maybe positive side effect:
     * The movement of the person, when going around obstacles,
     * is not choosing the best path, but at each point choosing the best direction to go to,
     * based only on the direction of the target point, and not its absolute position.
     * Which is cool because it looks more like the person does not see the target point itself
     * (which makes sense because obviously the target point is blocked out of view)
     * but knows its general direction.
     * 
     * @param[in] targetPoint (or xTarget, yTarget)
     *  Point towards which the person will move.
     */
    void MoveTowards(sf::Vector2f const targetPoint);
    void MoveTowards(float xTarget, float yTarget);

    /**
     * Rotates the head sprite so that it points towards a target point
     * 
     * @param[in] targetPoint (or xTarget, yTarget)
     *  Point towards which the person will point their head
     */
    void PointHeadTowards(sf::Vector2f const targetPoint);
    void PointHeadTowards(float xTarget, float yTarget);

  protected: /* variables */

    /// Target point, towards which the Person is always looking and can shoot
    sf::Vector2f _targetPoint;

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

    /// Configures the person speed, as specified in the config
    void ConfigPersonSpeed();

    /// Configures the movement precision, as specified in the config
    void ConfigMovementPrecision();

    /// Configures person's initial position - reads it from the config and sets it to the person
    void ConfigInitialPosition();

    /// Configures person's precision when it comes to going around obstacles
    void ConfigGoAroundPrecision();

    /// Updates the person according to the data derived from sf::Transformable
    void UpdateTransform();

    /**
     * Checks if a position is within the borders of the world
     * 
     * @param[in] position
     *  Position to check
     * 
     * @return true for inside world, false otherwise
     */
    bool IsPositionInWorld(sf::Vector2f const position) const;

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
    bool IsPositionOutsideWalls(sf::Vector2f const position) const;

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
    bool IsPositionValid(sf::Vector2f const position) const;

  private: /* variables */

    /// The world to which the person belongs
    World const* _world;

    /// Sprite for the head of the player
    sf::Sprite _headSprite;

    /// Speed of the person's movement, in pixels/frame
    float _speed;

    /* Person collisions with other objects are detected if the other object is
        within this radius to the player's center.
        Essentially a person is a circle for the collision detection. */
    float _collisionRadius;

    /* A person moves with its speed every frame.
    But frames are finite, so technically the person does not move,
    but jump from place to place very rapidly.
    This is not a problem when the person moves normally,
    but can be a problem when the person tries to move, say speed=10, pixels in one frame,
    but 7 pixels in that direction there is an object, and a collision is detected.
    In that situation we don't want the person to stop 7 pixels away from the object,
    we want to move it towards the object with some finer precision than the speed,
    and that is exactly what this variable is.
    When a collision is detected, the person is moved with that movement precision
    until he collides with the object.
    Let's say the movement precision is 2 and there is an object at 7 pixels away.
    The person will move 2 pixels, then 2 pixels more, 2 more and it will be 1 pixel away,
    then it will stop because moving 2 pixels again would collide with the object.
    So the person did not reach the object fully, because the movement precision is not small enough.
    Recommended value for that variable is 1 or less. */
    float _movementPrecision;

    /* Precision of the person's ability to go around obstacles when moving towards a target point.
    If there is an obstacle in front of the person, and he cannot move directly towards the target point,
    he will try to change his direction either to the left or to the right,
    by trying turning left/right some tiny step angle until he can move in the resulting direction.
    This variable - the precision - is what specifies that tiny step angle.
    If the precision is say 30 (recommended), then the tiny angle will be pi / 30.
    */
    float _goAroundPrecision;

    /// Person configuration
    Config _config;
};

} // namespace HideAndSeekAndShoot