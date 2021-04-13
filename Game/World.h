#pragma once

#include "Entities/Person.h"

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
     */
    World(Game const* game);

    /**
     * Creates a world with size and textures
     * 
     * @param[in] game
     *  Pointer to the game that creates the world
     * @param[in] size
     *  Size of the world, width and height, in pixels
     * @param[in] bgTex
     *  Pointer to loaded texture to be used for the background of the world
     * @param[in] wallTex
     *  Pointer to loaded texture to be used for walls
     */
    World(
        Game const* game,
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

    /// Generate walls according to the current world size
    void GenerateWalls();

    /// Returns a pointer to the game owner/creater of the world
    Game const* GetGame() const;

    /// Player getter (testing Person class, TODO: remove later)(probably shouldn't return reference to an object owned by unique_ptr)
    Person& GetPlayer();

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

    /// Player object (testing Person class, TODO: remove later)
    std::unique_ptr<Person> _player;

    /// Declare Person class to be a friend class so that it can access world's walls
    /// (TODO: think about it, probably not the greatest idea)
    friend class Person;
};

} // namespace HideAndSeekAndShoot