#include "ControlState.h"

namespace HideAndSeekAndShoot
{

ControlState::ControlState(
    sf::RenderWindow const& window,
    sf::Keyboard::Key upKey,
    sf::Keyboard::Key downKey,
    sf::Keyboard::Key leftKey,
    sf::Keyboard::Key rightKey)
    : _window(window),
    _upKey(upKey),
    _downKey(downKey),
    _leftKey(leftKey),
    _rightKey(rightKey)
{}

void ControlState::Update()
{
    _upPressed = sf::Keyboard::isKeyPressed(_upKey);
    _downPressed = sf::Keyboard::isKeyPressed(_downKey);
    _leftPressed = sf::Keyboard::isKeyPressed(_leftKey);
    _rightPressed = sf::Keyboard::isKeyPressed(_rightKey);

    _shootButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    _mousePosition = (sf::Vector2f)sf::Mouse::getPosition(_window);
}

bool ControlState::IsUpPressed() const
{
    return _upPressed;
}

bool ControlState::IsDownPressed() const
{
    return _downPressed;
}

bool ControlState::IsLeftPressed() const
{
    return _leftPressed;
}

bool ControlState::IsRightPressed() const
{
    return _rightPressed;
}

bool ControlState::IsShootButtonPressed() const
{
    return _shootButtonPressed;
}

sf::Vector2f ControlState::GetMousePosition() const
{
    return _mousePosition;
}

} // namespace HideAndSeekAndShoot