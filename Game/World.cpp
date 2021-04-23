#include "World.h"

#include "ControlState.h"
#include "utils/configUtils.hpp"
#include "utils/textureUtils.hpp"

#include <stdexcept>

#include <iostream>

namespace
{

auto constexpr WALLS_CONFIG_FILENAME = "Game/config/walls.conf";

} // namespace

namespace HideAndSeekAndShoot
{

World::World(
    Game const* game,
    Resources::ResourceHandler<Resources::Texture::Id, sf::Texture> const* texHandler,
    sf::Vector2f size)
    : _game(game),
    _size(size)
{
    SetBackgroundTexture(&texHandler->Get(Resources::Texture::Id::Background));

    SetWallTexture(&texHandler->Get(Resources::Texture::Id::Wall));
    LoadRelWalls();
    GenerateWalls();

    _player = std::make_unique<Player>(
        this,
        &texHandler->Get(Resources::Texture::Id::PlayerHead),
        &texHandler->Get(Resources::Texture::Id::Gun)
    );

    _enemy = std::make_unique<Enemy>(
        this,
        &texHandler->Get(Resources::Texture::Id::EnemyHead),
        &texHandler->Get(Resources::Texture::Id::Gun),
        &*_player
    );
}

sf::Vector2f World::GetSize() const
{
    return _size;
}

void World::GenerateWalls()
{
    _walls = std::vector<sf::ConvexShape>(
        _relWalls.size(),
        sf::ConvexShape(4)
    );

    for (int wallInd = 0; wallInd < _walls.size(); wallInd++)
    {
        for (int verInd = 0; verInd < 4; verInd++)
        {
            _walls[wallInd].setPoint(verInd, sf::Vector2f(
                _relWalls[wallInd][verInd].x * _size.x,
                _relWalls[wallInd][verInd].y * _size.y
            ));
        }
    }

    SetWallTexture(_wallTex);
}

Game const* World::GetGame() const
{
    return _game;
}

std::vector<sf::ConvexShape> const& World::GetWalls() const
{
    return _walls;
}

void World::Update(ControlState const& controlState)
{
    sf::Vector2f playerDirection;
    if (controlState.IsUpPressed())
        playerDirection.y -= 1.f;
    if (controlState.IsDownPressed())
        playerDirection.y += 1.f;
    if (controlState.IsLeftPressed())
        playerDirection.x -= 1.f;
    if (controlState.IsRightPressed()) 
        playerDirection.x += 1.f;
    _player->MoveInDirection(playerDirection);
    _player->SetTargetPoint(controlState.GetMousePosition());
    _player->Update();

    _enemy->Update();
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_bgSprite, states);
    
    for (int wallInd = 0; wallInd < _walls.size(); wallInd++)
    {
        target.draw(_walls[wallInd], states);
    }

    target.draw(*_enemy);
    target.draw(*_player);
}

void World::LoadRelWalls()
{
    _wallsConfig = ConfigUtils::ReadConfig(WALLS_CONFIG_FILENAME);

    int wallsCount = std::stoi(_wallsConfig["walls_count"]);
    _relWalls = std::vector<std::vector<sf::Vector2f>>(wallsCount, std::vector<sf::Vector2f>(4));

    auto getWallVertCoordKey = [this](
        int wallInd, int verInd, std::string coord) -> std::string {
            return std::string("wall") + std::to_string(wallInd)
            + "_vert" + std::to_string(verInd) + "_" + coord;
    };

    auto getVertex = [this, &getWallVertCoordKey](int wallInd, int verInd) -> sf::Vector2f {
        return sf::Vector2f(
            std::stof(_wallsConfig[getWallVertCoordKey(wallInd, verInd, "x")]),
            std::stof(_wallsConfig[getWallVertCoordKey(wallInd, verInd, "y")])
        );
    };

    for (int wallInd = 0; wallInd < wallsCount; wallInd++)
    {
        for (int verInd = 0; verInd < 4; verInd++)
        {
            _relWalls[wallInd][verInd] = getVertex(wallInd, verInd);
        }
    }
}

void World::SetBackgroundTexture(sf::Texture const* bgTex)
{
    if (bgTex == nullptr)
    {
        return;
    }

    _bgSprite.setTexture(*bgTex);
    // scale background sprite so that it spans the whole world
    _bgSprite.scale(
        _size.x / _bgSprite.getGlobalBounds().width,
        _size.y / _bgSprite.getGlobalBounds().height
    );
}

void World::SetWallTexture(sf::Texture const* wallTex)
{
    _wallTex = wallTex;
    if (wallTex == nullptr)
    {
        return;
    }

    for (int wallInd = 0; wallInd < _walls.size(); wallInd++)
    {
        TextureUtils::SetTextureKeepRatio(
            &_walls[wallInd],
            wallTex
        );
    }
}

} // namespace HideAndSeekAndShoot