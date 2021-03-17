#include "Person.h"

#include "../World.h"

#include "../utils/configUtils.hpp"
#include "../utils/geometryUtils.hpp"

#include <iostream> // TODO: remove later

namespace
{
    auto constexpr PERSON_CONFIG_FILENAME = "Game/config/person.conf";

    // TODO: this will later be a variable or in a config file, so that it can be different for different derived classes
    float const PERSON_SPEED = 4.f;

    // TODO: probably move to a config file
    float const PERSON_MOVEMENT_STEP = 1.f;

    // TODO: in config, also make it use relative coords
    sf::Vector2f const PERSON_INTIAL_POSITION(300.f, 300.f);
}

namespace HideAndSeekAndShoot
{

Person::Person(World const* world)
    : _world(world), 
    _headTex(nullptr),
    _config(ConfigUtils::ReadConfig(PERSON_CONFIG_FILENAME))
{
    sf::Transformable::setPosition(PERSON_INTIAL_POSITION);
}

void Person::SetHeadTexture(sf::Texture const* headTex)
{
    _headTex = headTex;

    if (headTex == nullptr)
    {
        return;
    }

    _headSprite.setTexture(*_headTex);

    // Scale texture to fit the head size in the config, if specified
    if (_config.find("head_size_x") != _config.end()
        && _config.find("head_size_y") != _config.end())
    {
        /* Getting the relative sizes from the config.
           They are written as a number between 0 and 1,
           relative to the world's size */
        float headRelSizeX = std::stof(_config["head_size_x"]);
        float headRelSizeY = std::stof(_config["head_size_y"]);

        // Calculate actual sizes by multiplying relative sizes with world's size
        float headSizeX = headRelSizeX * _world->GetSize().x;
        float headSizeY = headRelSizeY * _world->GetSize().y;

        // Set head sprite's scale accordingly to get the calculated head size
        _headSprite.setScale(
            headSizeX / _headSprite.getLocalBounds().width,
            headSizeY / _headSprite.getLocalBounds().height
        );    
    }

    // Sets head's origin to be its center, instead of the upper-left corner
    _headSprite.setOrigin(
        (float)_headSprite.getLocalBounds().width / 2,
        (float)_headSprite.getLocalBounds().height / 2
    );
}

void Person::SetTargetPoint(sf::Vector2f const& targetPoint)
{
    _targetPoint = targetPoint;
}

void Person::MoveInDirection(sf::Vector2f const dirVector)
{
    sf::Vector2f velocity = GeometryUtils::NormaliseVector(dirVector) * PERSON_SPEED;
    sf::Vector2f nextPosition = sf::Transformable::getPosition() + velocity;

    if (IsPositionValid(nextPosition))
    {
        sf::Transformable::setPosition(nextPosition);
    }
    else
    {
        nextPosition = sf::Transformable::getPosition();
        sf::Vector2f stepVector = GeometryUtils::NormaliseVector(dirVector) * PERSON_MOVEMENT_STEP;
        while (IsPositionValid(nextPosition + stepVector))
        {
            nextPosition += stepVector;
        }
        sf::Transformable::setPosition(nextPosition);
    }
}

void Person::MoveInDirection(float xDir, float yDir)
{
    MoveInDirection(sf::Vector2f(xDir, yDir));
}

void Person::Update()
{
    UpdateTransform();
    PointHeadTowardsTargetPoint();
}

void Person::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_headSprite);
}

void Person::UpdateTransform()
{
    _headSprite.setPosition(sf::Transformable::getPosition());
}

void Person::PointHeadTowardsTargetPoint()
{
    // Get normal direction vector from the head to the target point
    sf::Vector2f dirVector = GeometryUtils::NormaliseVector(
        _headSprite.getPosition() - _targetPoint);

    float angle = acos(dirVector.x);
    if (dirVector.y < 0)
    {
        angle = -angle;
    }

    _headSprite.setRotation(angle * 180.f / M_PI);
}

bool Person::IsPositionValid(sf::Vector2f const& position) const
{
    return (position.x < _world->GetSize().x
        && position.x >= 0
        && position.y < _world->GetSize().y
        && position.y >= 0);
}

} // namespace HideAndSeekAndShoot