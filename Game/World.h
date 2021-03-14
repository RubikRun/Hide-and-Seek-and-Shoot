#pragma once

#include <SFML/Graphics.hpp>

#include <string>

namespace HideAndSeekAndShoot
{

/**
 * A class representing the world in the game.
 * Keeps track of all the entities and handles control states.
 */
class World
{

  public:

    /**
     * Creates a world of size 0.
     */
    World();

    /**
     * Creates a world with size and texture for the background
     * 
     * @param[in] size
     *  Size of the world, width and height, in pixels
     * @param[in] bgTexFilename
     *  Texture for the background of the world
     */
    World(
        sf::Vector2f size,
        sf::Texture const& bgTex
    );

    /// Setter for the background of the world
    void SetBackgroundTexture(sf::Texture const& bgTex);

    /// Getter and setter for world's size
    sf::Vector2f GetSize() const;
    void SetSize(sf::Vector2f const& size);

    /// Updates world for next frame
    void Update();

    /**
     * Draws the world on the given render target
     * 
     * @param[in] renderTarget
     *  RenderTarget object on which to draw the world
     */
    void DrawOn(sf::RenderTarget& renderTarget) const;

  private:
    
    /// Size of the world, in pixels
    sf::Vector2f _size;

    /// Texture and sprite for the background of the world
    sf::Texture _bgTex;
    sf::Sprite _bgSprite;
};

} // namespace HideAndSeekAndShoot