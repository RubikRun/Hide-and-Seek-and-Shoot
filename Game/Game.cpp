#include "Game.h"

#include "utils/configUtils.hpp"

namespace
{

auto constexpr GAME_CONFIG_FILENAME = "Game/config/game.conf";

int const FRAMERATE_LIMIT_DEFAULT = 60;

sf::Keyboard::Key const KEY_QUIT_GAME = sf::Keyboard::Escape;

} // namespace

namespace HideAndSeekAndShoot
{

Game::Game()
    : _window( // Initialize window to be fullscreen
        sf::VideoMode(
            sf::VideoMode::getDesktopMode().width,
            sf::VideoMode::getDesktopMode().height),
        "",
        sf::Style::Fullscreen),
    _config(ConfigUtils::ReadConfig(GAME_CONFIG_FILENAME))
{
    // Get framerate limit from the config, if specified, otherwise use default
    int frameRateLimit = FRAMERATE_LIMIT_DEFAULT;
    if (_config.find("framerate_limit") != _config.end())
    {
        frameRateLimit = std::stoi(_config["framerate_limit"]);
    }
    // Set framerate limit to not torture the GPU too much and for smoother movement
    _window.setFramerateLimit(frameRateLimit);

    // Enable vertical sync for screens that get screen tearing
    _window.setVerticalSyncEnabled(true);
}

void Game::Run()
{
    /* The game loop.
       Updating and rendering until the player closes the game */
    while (_window.isOpen())
    {
        sf::Event event;
        while (_window.pollEvent(event))
        {
            // If the player has pressed the quit key, we close the window
            if (event.type == sf::Event::KeyPressed
                && event.key.code == KEY_QUIT_GAME)
            {
                _window.close();
            }
        }

        // first clear previous frame
        _window.clear();
        // then update game for the next frame
        Update();
        // then draw the next frame
        Draw();
        // and render it on the window
        _window.display();
    }
}

Game::~Game()
{ /* nothing */ }

void Game::Update()
{ /* nothing */ }

void Game::Draw()
{ /* nothing */ }

} // namespace HideAndSeekAndShoot