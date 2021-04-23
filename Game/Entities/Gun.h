#pragma once

#include <SFML/Graphics.hpp>

typedef std::map<std::string, std::string> Config;

namespace HideAndSeekAndShoot
{

class Person;

/**
 * A class representing a (person's) gun in the game.
 * It has a sprite as visual representation and is linked to a person in some way.
 * The gun is always pointed towards its owner's target point,
 * and can shoot bullets to it.
 */
class Gun : public sf::Drawable
{

  public:

    /**
     * Constructs a gun for a person
     * 
     * @param[in] person
     *  A pointer to the person whose gun this will be
     * @param[in] tex
     *  A pointer to the texture to be used for the gun
     */
    Gun(
        Person const* person,
        sf::Texture const* tex
    );

    /// Updates gun for next frame
    void Update();

  private: /* functions */

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /**
     * Sets a texture to the gun's sprite.
     * Makes sure it is scaled appropriately to match the size from the config
     * 
     * @param[in] tex
     *  A point to the texture to be used
     */
    void SetGunTexture(sf::Texture const* tex);

    /**
     * Rotates the gun sprite so that it points towards a target point
     * 
     * @param[in] targetPoint (or xTarget, yTarget)
     *  Point towards which the gun will be pointed
     */
    void PointGunTowards(sf::Vector2f const targetPoint);
    void PointGunTowards(float xTarget, float yTarget);

  private: /* variables */

    /// Pointer to the person owner of the gun
    Person const* _person;

    /// Sprite for visualising the gun
    sf::Sprite _sprite;

    /// Gun configuration
    Config _config;
};

} // namespace HideAndSeekAndShoot