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
auto constexpr PLAYER_HEAD_TEXTURE_FILENAME = "Game/resources/textures/playerHead.png";
auto constexpr ENEMY_HEAD_TEXTURE_FILENAME = "Game/resources/textures/enemyHead.png";
auto constexpr GUN_TEXTURE_FILENAME = "Game/resources/textures/gun.png";
auto constexpr BULLET_TEXTURE_FILENAME = "Game/resources/textures/bullet.png";

sf::Keyboard::Key const KEY_QUIT_GAME = sf::Keyboard::Escape;

} // namespace

namespace HideAndSeekAndShoot
{

Game::Game()
    : _config(ConfigUtils::ReadConfig(GAME_CONFIG_FILENAME)),
    _controlState(_window)
{
    ConfigWindow();
    LoadResources();

    _world = std::make_unique<World>(
        this,
        &_textureHandler,
        (sf::Vector2f)_window.getSize()
    );
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

int Game::GetFramerateLimit() const
{
    return _framerateLimit;
}

Game::~Game()
{ /* nothing */ }

void Game::Update()
{
    _controlState.Update();
    _world->Update(_controlState);
}

void Game::Draw()
{
    _window.draw(*_world);
}

void Game::ConfigWindow()
{
    auto const fullscreenConfig = _config.find("fullscreen");
    if (fullscreenConfig != _config.end()
        && fullscreenConfig->second == "on")
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
    }
    else
    {
        int width = WINDOW_WIDTH_DEFAULT;
        int height = WINDOW_HEIGHT_DEFAULT;
        auto const resolutionConfig = _config.find("resolution");
        if (resolutionConfig != _config.end())
        {
            std::string const resolution = resolutionConfig->second;

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
    }

    // Get framerate limit from the config, if specified, otherwise use default
    int framerateLimit = FRAMERATE_LIMIT_DEFAULT;
    auto const framerateLimitConfig = _config.find("framerate_limit");
    if (framerateLimitConfig != _config.end())
    {
        framerateLimit = std::stoi(framerateLimitConfig->second);
    }
    _window.setFramerateLimit(framerateLimit);
    _framerateLimit = framerateLimit;

    // Enable vertical sync for screens that get screen tearing
    _window.setVerticalSyncEnabled(true);
}

void Game::LoadResources()
{
    _textureHandler.Load(Resources::Texture::Id::Background, BACKGROUND_TEXTURE_FILENAME);
    _textureHandler.Load(Resources::Texture::Id::Wall, WALL_TEXTURE_FILENAME);
    _textureHandler.Load(Resources::Texture::Id::PlayerHead, PLAYER_HEAD_TEXTURE_FILENAME);
    _textureHandler.Load(Resources::Texture::Id::EnemyHead, ENEMY_HEAD_TEXTURE_FILENAME);
    _textureHandler.Load(Resources::Texture::Id::Gun, GUN_TEXTURE_FILENAME);
    _textureHandler.Load(Resources::Texture::Id::Bullet, BULLET_TEXTURE_FILENAME);
}

} // namespace HideAndSeekAndShoot