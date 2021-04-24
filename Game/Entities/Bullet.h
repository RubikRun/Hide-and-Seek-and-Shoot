#pragma once

#include <SFML/Graphics.hpp>

typedef std::map<std::string, std::string> Config;

namespace HideAndSeekAndShoot
{

class Gun;

/**
 * A class representing a bullet shot from a person's gun.
 * When shot, the bullet starts moving from its gun towards some target direction,
 * and moves with constant speed, until it hits something, or goes out of the map.
 */
class Bullet : public sf::Transformable, public sf::Drawable
{

  public:  

    /**
     * Constructs a bullet object
     * 
     * @param[in] gun
     *  A pointer to the gun from which the bullet has been shot
     * @param[in] tex
     *  A pointer to the texture to be used for the bullet
     * @param[in] config
     *  A pointer to the bullet config
     * @param[in] targetDir
     *  Target direction of the bullet.
     *  A direction vector, specifying only direction, length will be ignored
     */
    Bullet(
        Gun const* gun,
        sf::Texture const* tex,
        Config const* config,
        sf::Vector2f targetDir
    );

    /// Updates the bullet for next frame
    void Update();

  private: /* functions */

    /// Draws the bullet to a render target
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /// Updates the gun according to the data derived from sf::Transformable
    void UpdateTransform();

    /**
     * Sets a texture to the bullet,
     * and scales it appropriately so that it matches the size in the config.
     * 
     * @param[in] tex
     *  A pointer to the texture to be set
     */
    void SetTexture(sf::Texture const* tex);

    /**
     * Initializes bullet's velocity according to a target direction.
     * 
     * @param[in] targetDir
     *  Target direction of the bullet
     */
    void InitVelocity(sf::Vector2f targetDir);

  private: /* variables */

    /// The gun from which the bullet has been shot
    Gun const* _gun;

    /// Sprite for visualising the bullet
    sf::Sprite _sprite;

    /// Vector's velocity
    sf::Vector2f _velocity;

    /// Pointer to a bullet's config
    Config const* _config;
};

} // namespace HideAndSeekAndShoot