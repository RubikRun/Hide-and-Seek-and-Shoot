#include "Game.h"

#include "utils/configUtils.hpp"

namespace
{

auto constexpr GAME_CONFIG_FILENAME = "Game/config/game.conf";
int const WINDOW_WIDTH_DEFAULT = 1280;
int const WINDOW_HEIGHT_DEFAULT = 720;
int const FRAMERATE_LIMIT_DEFAULT = 60;

auto constexpr BACKGROUND_TEXTURE_FILENAME = "Game/resources/textures/background.png";
auto constexpr WALL_TEXTURE_FILENAME = "Game/resources/textures/wall.png";

sf::Keyboard::Key const KEY_QUIT_GAME = sf::Keyboard::Escape;

} // namespace

namespace HideAndSeekAndShoot
{

Game::Game()
    : _config(ConfigUtils::ReadConfig(GAME_CONFIG_FILENAME))
{
    ConfigWindow();
    LoadResources();
    SetupWorld();
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
            // If the player has pressed the quit key or X button, we close the window
            if ((event.type == sf::Event::KeyPressed
                && event.key.code == KEY_QUIT_GAME)
                || event.type == sf::Event::Closed)
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
{
    _world.Update();
}

void Game::Draw()
{
    _world.DrawOn(_window);
}

void Game::ConfigWindow()
{
    if (_config.find("fullscreen") != _config.end())
    {
        if (_config["fullscreen"] == "on")
        {
            // if it is specified that fullscreen is on, create fullscreen window
            _window.create(
                sf::VideoMode(
                    sf::VideoMode::getDesktopMode().width,
                    sf::VideoMode::getDesktopMode().height
                ),
                "",
                sf::Style::Fullscreen
            );

            return;
        }
    }

    int width = WINDOW_WIDTH_DEFAULT;
    int height = WINDOW_HEIGHT_DEFAULT;
    if (_config.find("resolution") != _config.end())
    {
        std::string resolution = _config["resolution"];

        // The 'x' acting as the separator between width and height in a resolution (eg. 640x460)
        size_t xIndex = resolution.find('x');
        if (xIndex == std::string::npos)
        {
            throw std::runtime_error("Resolution specified in the game config file is not in the correct format.");
        }
        
        // Separate width and height from the resolution
        width = std::stoi(resolution.substr(0, xIndex));
        height = std::stoi(resolution.substr(xIndex + 1));
    }

    // Create window with the resolution
    _window.create(
        sf::VideoMode(width, height),
        "Hide and Seek and Shoot",
        sf::Style::Default
    );

    // Get framerate limit from the config, if specified, otherwise use default
    int frameRateLimit = FRAMERATE_LIMIT_DEFAULT;
    if (_config.find("framerate_limit") != _config.end())
    {
        frameRateLimit = std::stoi(_config["framerate_limit"]);
    }
    _window.setFramerateLimit(frameRateLimit);

    // Enable vertical sync for screens that get screen tearing
    _window.setVerticalSyncEnabled(true);
}

void Game::LoadResources()
{
    _textureHandler.Load(Resources::Texture::Id::Background, BACKGROUND_TEXTURE_FILENAME);
    _textureHandler.Load(Resources::Texture::Id::Wall, WALL_TEXTURE_FILENAME);
}

void Game::SetupWorld()
{
    _world.SetSize((sf::Vector2f)_window.getSize());
    _world.SetBackgroundTexture(&_textureHandler.Get(Resources::Texture::Id::Background));
    _world.SetWallTexture(&_textureHandler.Get(Resources::Texture::Id::Wall));
    _world.GenerateWalls();
}

} // namespace HideAndSeekAndShoot