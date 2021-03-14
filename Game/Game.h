#pragma once

#include "World.h"
#include "resources/ResourceHandler.hpp"
#include "resources/ResourceIDs.hpp"

#include <SFML/Graphics.hpp>

#include <string>

typedef std::map<std::string, std::string> Config;

using namespace Resources;

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

    /**
     * Sets up the world of the game with its resources and configurations.
     * It is expected that before that window has been configured with ConfigWindow() method,
     * and resources have been loaded with LoadResources() method.
     */
    void SetupWorld();

  private: /* variables */

    /// The window where the game is rendered
    sf::RenderWindow _window;

    /// World of the game
    World _world;

    /// Game configuration
    Config _config;

    /// Handler for texture resources
    ResourceHandler<Resources::Texture::Id, sf::Texture> _textureHandler;
};

} // namespace HideAndSeekAndShoot