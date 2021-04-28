#include "ControlState.h"

#include "utils/configUtils.hpp"

namespace
{
    auto constexpr CONTROLS_CONFIG_FILENAME = "Game/config/controls.conf";

    int TIME_BETWEEN_SHOOTS_DEFAULT = 10;
}

namespace HideAndSeekAndShoot
{

ControlState::ControlState(
    sf::RenderWindow const& window,
    sf::Keyboard::Key upKey,
    sf::Keyboard::Key downKey,
    sf::Keyboard::Key leftKey,
    sf::Keyboard::Key rightKey)
    : _upKey(upKey),
    _downKey(downKey),
    _leftKey(leftKey),
    _rightKey(rightKey),
    _window(window),
    _config(ConfigUtils::ReadConfig(CONTROLS_CONFIG_FILENAME))
{
    ConfigTimeBetweenShoots();
    _timeSinceLastShootButtonPress = _timeBetweenShoots;
}

void ControlState::Update()
{
    _upPressed = sf::Keyboard::isKeyPressed(_upKey);
    _downPressed = sf::Keyboard::isKeyPressed(_downKey);
    _leftPressed = sf::Keyboard::isKeyPressed(_leftKey);
    _rightPressed = sf::Keyboard::isKeyPressed(_rightKey);

    _mousePosition = (sf::Vector2f)sf::Mouse::getPosition(_window);

    _shootButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    if (_shootButtonPressed)
    {
        if (_timeSinceLastShootButtonPress >= _timeBetweenShoots)
        {
            _timeSinceLastShootButtonPress = 0;
        }
        else
        {
            _shootButtonPressed = false;
        }
    }
    _timeSinceLastShootButtonPress++;
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

void ControlState::ConfigTimeBetweenShoots()
{
    auto timeBetweenShootsConfig = _config.find("time_between_shoots");
    if (timeBetweenShootsConfig != _config.end())
    {
        _timeBetweenShoots = std::stoi(timeBetweenShootsConfig->second);
    }
    else
    {
        _timeBetweenShoots = TIME_BETWEEN_SHOOTS_DEFAULT;
    }
}

} // namespace HideAndSeekAndShoot