#pragma once

#include <SFML/Graphics.hpp>

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

  private:

    /// Updates the game for the next frame.
    void Update();

    /// Draws the game to the window
    void Draw();

  private:

    /// The window where the game is rendered
    sf::RenderWindow _window;
};