#pragma once

#include <SFML/Graphics.hpp>

namespace HideAndSeekAndShoot
{

/**
 * A class representing the state of the game controls at an instance.
 * The game controls are 4 keys for UP, DOWN, LEFT and RIGHT, and the mouse.
 * So a control state object keeps track of which of those keys is currently pressed,
 * and where the mouse is.
 */
struct ControlState
{

  public:

    /**
     * Creates a control state object for a window.
     * Keys for UP, DOWN, LEFT and RIGHT can be specified.
     * 
     * @param[in] window
     *  The window on which the controls will be applied
     * @param[in] upKey (optional)
     *  Key to be used as UP key
     * @param[in] downKey (optional)
     *  Key to be used as DOWN key
     * @param[in] leftKey (optional)
     *  Key to be used as LEFT key
     * @param[in] rightKey (optional)
     *  Key to be used as RIGHT key
     */
    ControlState(
        sf::RenderWindow const& window,
        sf::Keyboard::Key upKey = sf::Keyboard::W,
        sf::Keyboard::Key downKey = sf::Keyboard::S,
        sf::Keyboard::Key leftKey = sf::Keyboard::A,
        sf::Keyboard::Key rightKey = sf::Keyboard::D
    );

    /**
     * Updates the control state according to the user input at the current moment.
     * Has to be called each frame if we want the ControlState object to correspond to controls in real time.
     */
    void Update();

    /// Functions used to check whether the UP, DOWN, LEFT or RIGHT key is currently pressed
    bool IsUpPressed() const;
    bool IsDownPressed() const;
    bool IsLeftPressed() const;
    bool IsRightPressed() const;

    /// Checks whether the button used to shoot bullets is currently pressed
    bool IsShootButtonPressed() const;

    /**
     * Returns current mouse position relative to the window.
     * 
     * @return mouse position
     */
    sf::Vector2f GetMousePosition() const;

  private:

    /// Keys on the keyboard that correspond to UP, DOWN, LEFT and RIGHT
    sf::Keyboard::Key _upKey, _downKey, _leftKey, _rightKey;
    
    /// Variables indicating whether the UP, DOWN, LEFT and RIGHT keys are currently pressed
    bool _upPressed, _downPressed, _leftPressed, _rightPressed;

    /// Variable indicating whether the button for shooting is currently pressed
    bool _shootButtonPressed;

    /// Position of the mouse on the window
    sf::Vector2f _mousePosition;

    /// Window on which the controls are applied
    sf::RenderWindow const& _window;
};

} // namespace HideAndSeekAndShoot