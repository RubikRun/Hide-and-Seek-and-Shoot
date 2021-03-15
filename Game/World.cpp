#include "World.h"

#include "utils/configUtils.hpp"
#include "utils/textureUtils.hpp"

#include <stdexcept>

namespace
{

auto constexpr WALLS_CONFIG_FILENAME = "Game/config/walls.conf";

} // namespace

namespace HideAndSeekAndShoot
{

World::World()
    : _wallTex(nullptr),
    _player(this)
{
    LoadRelWalls();
}

World::World(
    sf::Vector2f size,
    sf::Texture const* bgTex,
    sf::Texture const* wallTex)
    : _wallTex(wallTex),
    _player(this)
{
    SetSize(size);
    SetBackgroundTexture(bgTex);

    LoadRelWalls();
    GenerateWalls();
}

void World::SetBackgroundTexture(sf::Texture const* bgTex)
{
    if (bgTex == nullptr)
    {
        return;
    }

    _bgSprite.setTexture(*bgTex);
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
            wallTex,
            sf::Vector2i(0, 0)
        );
    }
}

sf::Vector2f World::GetSize() const
{
    return _size;
}

void World::SetSize(sf::Vector2f const& size)
{
    if (size.x < 0 || size.y < 0)
    {
        throw std::runtime_error("Error: Cannot set world's size to be negative.");
    }
    _size = size;
}

void World::Update()
{ /* nothing */ }

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

Person& World::GetPlayer()
{
    return _player;
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_bgSprite, states);
    
    for (int wallInd = 0; wallInd < _walls.size(); wallInd++)
    {
        target.draw(_walls[wallInd], states);
    }

    target.draw(_player);
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

} // namespace HideAndSeekAndShoot