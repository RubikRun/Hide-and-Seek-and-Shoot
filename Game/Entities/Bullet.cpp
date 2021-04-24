#include "Bullet.h"

#include "Gun.h"
#include "Person.h"
#include "../World.h"

#include "../utils/geometryUtils.hpp"

namespace
{

float const SPEED_DEFAULT = 5.f;

} // namespace

namespace HideAndSeekAndShoot
{

Bullet::Bullet(
    Gun const* gun,
    sf::Texture const* tex,
    Config const* config,
    sf::Vector2f targetDir)
    : _gun(gun),
    _config(config)
{
    SetTexture(tex);
    InitVelocity(targetDir);

    sf::Transformable::setPosition(gun->getPosition());
}

void Bullet::Update()
{
    sf::Transformable::move(_velocity);
    UpdateTransform();
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

void Bullet::UpdateTransform()
{
    _sprite.setPosition(sf::Transformable::getPosition());
    _sprite.setRotation(sf::Transformable::getRotation());
}

void Bullet::SetTexture(sf::Texture const* tex)
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
    auto const spriteSizeXConfig = _config->find("size_x");
    auto const spriteSizeYConfig = _config->find("size_y");
    if (spriteSizeXConfig != _config->end()
        && spriteSizeYConfig != _config->end())
    {
        /* Getting the relative sizes from the config.
           They are written as a number between 0 and 1,
           relative to the world's size */
        float spriteRelSizeX = std::stof(spriteSizeXConfig->second);
        float spriteRelSizeY = std::stof(spriteSizeYConfig->second);

        // Calculate actual sizes by multiplying relative sizes with world's size
        float spriteSizeX = spriteRelSizeX * _gun->GetPerson()->GetWorld()->GetSize().x;
        float spriteSizeY = spriteRelSizeY * _gun->GetPerson()->GetWorld()->GetSize().y;

        // Set sprite's scale accordingly to get the calculated size
        _sprite.setScale(
            spriteSizeX / _sprite.getLocalBounds().width,
            spriteSizeY / _sprite.getLocalBounds().height
        );
    }
}

void Bullet::InitVelocity(sf::Vector2f targetDir)
{
    // TODO: move speed to config, and make relative
    _velocity = GeometryUtils::NormaliseVector(targetDir) * SPEED_DEFAULT;
}

} // namespace HideAndSeekAndShoot