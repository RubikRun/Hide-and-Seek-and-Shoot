#pragma once

#include "World.h"
#include "ControlState.h"
#include "resources/ResourceHandler.hpp"
#include "resources/ResourceIDs.hpp"

#include <SFML/Graphics.hpp>

#include <string>

typedef std::map<std::string, std::string> Config;

namespace HideAndSeekAndShoot
{

/**
 * A class for easily creating and running the Game.
 */
class Game
{

  public:

    /**
     * Sets up a new game.
     */
    Game();

    /**
     * Runs the game.
     */
    void Run();

    /// Returns game's framerate limit
    int GetFramerateLimit() const;

    /**
     * Cleans up after the game has ended.
     */
    ~Game();

  private: /* functions */

    /// Updates the game for the next frame.
    void Update();

    /// Draws the game to the window
    void Draw();

    /// Configures and creates the window with resolution and framerate limit specified in the config.
    void ConfigWindow();

    /// Loads all needed resources into the resource handlers
    void LoadResources();

  private: /* variables */

    /// The window where the game is rendered
    sf::RenderWindow _window;

    /// World of the game
    std::unique_ptr<World> _world;

    /// Current control state
    ControlState _controlState;

    /// Framerate limit of the game
    int _framerateLimit;

    /// Game configuration
    Config _config;

    /// Handler for texture resources
    Resources::ResourceHandler<Resources::Texture::Id, sf::Texture> _textureHandler;
};

} // namespace HideAndSeekAndShoot