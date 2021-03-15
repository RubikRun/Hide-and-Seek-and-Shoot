#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

typedef std::map<std::string, std::string> Config;

namespace HideAndSeekAndShoot
{

/**
 * A class representing the world in the game.
 * Keeps track of all the entities and handles control states.
 */
class World : public sf::Drawable
{

  public:

    /**
     * Creates a world of size 0.
     */
    World();

    /**
     * Creates a world with size and textures
     * 
     * @param[in] size
     *  Size of the world, width and height, in pixels
     * @param[in] bgTex
     *  Pointer to loaded texture to be used for the background of the world
     * @param[in] wallTex
     *  Pointer to loaded texture to be used for walls
     */
    World(
        sf::Vector2f size,
        sf::Texture const* bgTex = nullptr,
        sf::Texture const* wallTex = nullptr
    );

    /// Setter for the background of the world
    void SetBackgroundTexture(sf::Texture const* bgTex);

    /// Setter for the texture used for walls
    void SetWallTexture(sf::Texture const* wallTex);

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
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /// Generate walls according to the current world size
    void GenerateWalls();

  private: /* functions */

    /// Loads walls' relative coordinates from config file
    void LoadRelWalls();

  private: /* variables */
    
    /// Size of the world, in pixels
    sf::Vector2f _size;

    /// Texture and sprite for the background of the world
    sf::Texture const* _bgTex;
    sf::Sprite _bgSprite;

    /// Vector of walls, as convex shapes
    std::vector<sf::ConvexShape> _walls;
    /// Vector of walls coordinates relative to the size of the world
    std::vector<std::vector<sf::Vector2f>> _relWalls;
    /// Config for walls' relative coordinates
    Config _wallsConfig;
    /// Pointer to a loaded texture to be used for walls
    sf::Texture const* _wallTex;
};

} // namespace HideAndSeekAndShoot