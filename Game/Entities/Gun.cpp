#include "Gun.h"

#include "Person.h"
#include "../World.h"

#include "../utils/configUtils.hpp"
#include "../utils/geometryUtils.hpp"

namespace
{

auto constexpr GUN_CONFIG_FILENAME = "Game/config/gun.conf";

} // namespace

namespace HideAndSeekAndShoot
{

Gun::Gun(
    Person const* person,
    sf::Texture const* tex)
    : _person(person),
    _config(ConfigUtils::ReadConfig(GUN_CONFIG_FILENAME))
{
    SetGunTexture(tex);

    _sprite.setPosition(300.f, 300.f);
}

void Gun::Update()
{
    PointGunTowards(_person->GetTargetPoint());
}

void Gun::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

void Gun::SetGunTexture(sf::Texture const* tex)
{
    if (tex == nullptr)
    {
        return;
    }

    _sprite.setTexture(*tex);

    // Sets sprite's origin to be its center, instead of the upper-left corner
    _sprite.setOrigin(
        _sprite.getLocalBounds().width / 2,
        _sprite.getLocalBounds().height / 2
    );

    // Scale texture to fit the sprite size from the config, if specified
    auto const spriteSizeXConfig = _config.find("size_x");
    auto const spriteSizeYConfig = _config.find("size_y");
    if (spriteSizeXConfig != _config.end()
        && spriteSizeYConfig != _config.end())
    {
        /* Getting the relative sizes from the config.
           They are written as a number between 0 and 1,
           relative to the world's size */
        float spriteRelSizeX = std::stof(spriteSizeXConfig->second);
        float spriteRelSizeY = std::stof(spriteSizeYConfig->second);

        // Calculate actual sizes by multiplying relative sizes with world's size
        float spriteSizeX = spriteRelSizeX * _person->GetWorld()->GetSize().x;
        float spriteSizeY = spriteRelSizeY * _person->GetWorld()->GetSize().y;

        // Set sprite's scale accordingly to get the calculated size
        _sprite.setScale(
            spriteSizeX / _sprite.getLocalBounds().width,
            spriteSizeY / _sprite.getLocalBounds().height
        );
    }
}

void Gun::PointGunTowards(sf::Vector2f const targetPoint)
{
    // Get normal direction vector from the gun to the target point
    sf::Vector2f dirVector = GeometryUtils::NormaliseVector(
        _sprite.getPosition() - targetPoint);

    float angle = acos(dirVector.x);
    if (dirVector.y < 0)
    {
        angle = -angle;
    }

    _sprite.setRotation(angle * 180.f / M_PI);
}

void Gun::PointGunTowards(float xTarget, float yTarget)
{
    PointGunTowards(sf::Vector2f(xTarget, yTarget));
}

} // namespace HideAndSeekAndShoot