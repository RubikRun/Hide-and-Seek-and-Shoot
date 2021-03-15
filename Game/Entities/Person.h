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