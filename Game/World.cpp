#include "World.h"

#include "utils/configUtils.hpp"

#include <stdexcept>

namespace
{

auto constexpr WALLS_CONFIG_FILENAME = "Game/config/walls.conf";

} // namespace

namespace HideAndSeekAndShoot
{

World::World()
    : _bgTex(nullptr),
    _wallTex(nullptr)
{
    LoadRelWalls();
}

World::World(
    sf::Vector2f size,
    sf::Texture const* bgTex,
    sf::Texture const* wallTex)
    : _bgTex(bgTex),
    _wallTex(wallTex)
{
    SetSize(size);
    SetBackgroundTexture(bgTex);

    LoadRelWalls();
    GenerateWalls();
}

void World::SetBackgroundTexture(sf::Texture const* bgTex)
{
    _bgTex = bgTex;

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
        _walls[wallInd].setTexture(wallTex);
        // TODO: fix stretching bug
        /* stretching bug description:
        When a wall is very thin (not in same ratio as the texture),
        I don't want it to use the whole texture and stretch it,
        but rather crop from the texture a rectangle with appropriate ratio.
        This can be done with setTextureRect() method of ConvexShape,
        but it doesn't act well when the wall is bigger than the texture.
        The areas that are not covered by the texture are drawn with
        the outermost color of the texture, which is not nice.
        When it happens that the wall is bigger than the texture
        I want to crop the appropriate ratio textureRect from the texture
        and stretch it so that it fills the whole wall. */
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

void World::DrawOn(sf::RenderTarget& renderTarget) const
{
    renderTarget.draw(_bgSprite);
    
    for (int wallInd = 0; wallInd < _walls.size(); wallInd++)
    {
        renderTarget.draw(_walls[wallInd]);
    }
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
            SetWallTexture(_wallTex);
        }
    }
}

} // namespace HideAndSeekAndShoot