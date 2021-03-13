#pragma once

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

    /**
     * Cleans up after the game has ended.
     */
    ~Game();

  private: /* functions */

    /// Updates the game for the next frame.
    void Update();

    /// Draws the game to the window
    void Draw();

    /**
     * Configures and creates the window with resolution and framerate limit specified in the config.
     */
    void ConfigWindow();

  private: /* variables */

    /// The window where the game is rendered
    sf::RenderWindow _window;

    /// Game configuration
    Config _config;
};

} // namespace HideAndSeekAndShoot