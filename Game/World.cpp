#include "World.h"

#include <stdexcept>

namespace HideAndSeekAndShoot
{

World::World()
{}

World::World(
    sf::Vector2f size,
    sf::Texture const& bgTex)
{
    SetSize(size);
    SetBackgroundTexture(bgTex);
}

void World::SetBackgroundTexture(sf::Texture const& bgTex)
{
    _bgSprite.setTexture(bgTex);

    _bgSprite.scale(
        _size.x / _bgSprite.getGlobalBounds().width,
        _size.y / _bgSprite.getGlobalBounds().height
    );
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
}

} // namespace HideAndSeekAndShoot