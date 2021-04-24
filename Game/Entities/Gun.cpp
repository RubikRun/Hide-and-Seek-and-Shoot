#include "Gun.h"

#include "Person.h"
#include "../World.h"

#include "../utils/configUtils.hpp"
#include "../utils/geometryUtils.hpp"

namespace
{

auto constexpr GUN_CONFIG_FILENAME = "Game/config/gun.conf";

float const DIST_PERSON_REL_DEFAULT = 0.8f;

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
    ConfigDistPerson();

    sf::Transformable::setPosition(300.f, 300.f);
}

void Gun::Update()
{
    PointGunTowards(_person->GetTargetPoint());
    FollowPerson(_person);
    UpdateTransform();
}

void Gun::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

void Gun::UpdateTransform()
{
    _sprite.setPosition(sf::Transformable::getPosition());
    _sprite.setRotation(sf::Transformable::getRotation());
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
        sf::Transformable::getPosition() - targetPoint);

    float angle = acos(dirVector.x);
    if (dirVector.y < 0)
    {
        angle = -angle;
    }

    sf::Transformable::setRotation(angle * 180.f / M_PI);
}

void Gun::PointGunTowards(float xTarget, float yTarget)
{
    PointGunTowards(sf::Vector2f(xTarget, yTarget));
}

void Gun::FollowPerson(Person const* person)
{
    // Direction vector of where the person is looking at
    sf::Vector2f lookDir = GeometryUtils::GetVector(
        person->getPosition(),
        person->GetTargetPoint()
    );

    // The vector from person to gun should be perpendicular to where the person is looking at
    sf::Vector2f personToGunNormalVector = sf::Vector2f(
        -lookDir.y,
        lookDir.x
    );
    personToGunNormalVector = GeometryUtils::NormaliseVector(personToGunNormalVector);

    // The gun should be positioned some contant distance away from the person, in the perpendicular direction
    sf::Transformable::setPosition(
        person->getPosition()
            + personToGunNormalVector * _distPerson
    );
}

void Gun::ConfigDistPerson()
{
    auto distPersonConfig = _config.find("dist_person");
    if (distPersonConfig != _config.end())
    {
        float distPersonRel = std::stof(distPersonConfig->second);
        _distPerson = distPersonRel * _person->GetHeadSize().x;
    }
    else
    {
        _distPerson = DIST_PERSON_REL_DEFAULT * _person->GetHeadSize().x;
    }
}

} // namespace HideAndSeekAndShoot