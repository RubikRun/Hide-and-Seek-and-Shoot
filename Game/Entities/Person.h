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
     * Moves the person in the given direction with their speed.
     * Perople have constant speed and so only a direction
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

    /**
     * Checks if a position is valid,
     * meaning that it doesn't collide with anything and is within the map.
     * 
     * @param[in] position
     *  Position whose validity to check
     * 
     * @return true for valid, false for invalid
     */
    bool IsPositionValid(sf::Vector2f const& position) const;

  private: /* variables */

    /// The world to which the person belongs
    World const* _world;

    /// Sprite and texture for the head of the player
    sf::Texture const* _headTex;
    sf::Sprite _headSprite;

    /// Person configuration
    Config _config;
};

} // namespace HideAndSeekAndShoot