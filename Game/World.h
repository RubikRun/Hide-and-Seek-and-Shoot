#pragma once

#include "Entities/Player.h"
#include "Entities/Enemy.h"
#include "Entities/Bullet.h"

#include "resources/ResourceHandler.hpp"
#include "resources/ResourceIDs.hpp"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

typedef std::map<std::string, std::string> Config;

namespace HideAndSeekAndShoot
{

class ControlState;
class Game;

/**
 * A class representing the world in the game.
 * Keeps track of all the entities and handles control states.
 */
class World : public sf::Drawable
{

  public:

    /**
     * Creates a world of size 0.
     * 
     * @param[in] game
     *  Pointer to the game that creates the world
     * @param[in] texHandler
     *  Pointer to textre handler with loaded textures
     */
    World(
      Game const* game,
      Resources::ResourceHandler<Resources::Texture::Id, sf::Texture> const* texHandler,
      sf::Vector2f size
    );

    /// Getter for world's size
    sf::Vector2f GetSize() const;

    /// Generate walls according to the current world size
    void GenerateWalls();

    /// Returns a pointer to the game owner/creater of the world
    Game const* GetGame() const;

    /// Returns a vector of world's walls
    std::vector<sf::ConvexShape> const& GetWalls() const;

    /**
     * Updates world according to a control state
     * 
     * @param[in] controlState
     *  State of user controls according to which the world will be updated
     */
    void Update(ControlState const& controlState);

  private: /* functions */

    /**
     * Draws the world on the given render target
     * 
     * @param[in] renderTarget
     *  RenderTarget object on which to draw the world
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /// Loads walls' relative coordinates from config file
    void LoadRelWalls();

    /// Setter for the background of the world
    void SetBackgroundTexture(sf::Texture const* bgTex);

    /// Setter for the texture used for walls
    void SetWallTexture(sf::Texture const* wallTex);

  private: /* variables */
    
    /// Game object that is an owner/creater of this world
    Game const* _game;

    /// Size of the world, in pixels
    sf::Vector2f _size;

    /// Sprite for the background of the world
    sf::Sprite _bgSprite;

    /// Vector of walls, as convex shapes
    std::vector<sf::ConvexShape> _walls;
    /// Vector of walls coordinates relative to the size of the world
    std::vector<std::vector<sf::Vector2f>> _relWalls;
    /// Config for walls' relative coordinates
    Config _wallsConfig;
    /// Pointer to a loaded texture to be used for walls
    sf::Texture const* _wallTex;

    /// Player object for the player's entity
    std::unique_ptr<Player> _player;

    /// Enemy object for the enemy's entity
    std::unique_ptr<Enemy> _enemy;

    /// List of currently existing bullets (TODO: maybe use linked list instead of vector)
    std::vector<std::unique_ptr<Bullet>> _bullets;
};

} // namespace HideAndSeekAndShoot