#define VISUAL_DEBUG 0

#include "Person.h"

#include "../World.h"
#include "../Game.h"

#include "../utils/configUtils.hpp"
#include "../utils/geometryUtils.hpp"

#include <cmath>

namespace
{
    float const SPEED_DEFAULT = 10.f;

    float const MOVEMENT_PRECISION_DEFAULT = 1.f;

    sf::Vector2f const INTIAL_POSITION_DEFAULT(0.2f, 0.7f);

    float GO_AROUND_PRECISION_DEFAULT = 30.f;
}

namespace HideAndSeekAndShoot
{

World const* Person::GetWorld() const
{
    return _world;
}

sf::Vector2f Person::GetTargetPoint() const
{
    return _targetPoint;
}

Person::Person(
    World const* world,
    sf::Texture const* headTex,
    sf::Texture const* gunTex,
    std::string const& configFilename)
    : _world(world),
    _config(ConfigUtils::ReadConfig(configFilename)),
    _gun(this, gunTex)
{
    SetHeadTexture(headTex);

    ConfigInitialPosition();
    ConfigPersonSpeed();
    ConfigMovementPrecision();
    ConfigGoAroundPrecision();
}

void Person::Update()
{
    UpdateTransform();
    PointHeadTowards(_targetPoint);
    _gun.Update();
}

void Person::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_headSprite, states);

#if VISUAL_DEBUG == 1
    // Visual debugging to see the collision circle
    sf::CircleShape circle(_collisionRadius);
    circle.setOrigin(_collisionRadius, _collisionRadius);
    circle.setPosition(_headSprite.getPosition());
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color(255, 0, 0, 150));
    circle.setOutlineThickness(2.f);

    target.draw(circle, states);
#endif

    target.draw(_gun, states);
}

void Person::SetHeadTexture(sf::Texture const* headTex)
{
    if (headTex == nullptr)
    {
        return;
    }

    _headSprite.setTexture(*headTex);

    // Sets head's origin to be its center, instead of the upper-left corner
    _headSprite.setOrigin(
        _headSprite.getLocalBounds().width / 2,
        _headSprite.getLocalBounds().height / 2
    );

    // Scale texture to fit the head size from the config, if specified
    auto const headSizeXConfig = _config.find("head_size_x");
    auto const headSizeYConfig = _config.find("head_size_y");
    if (headSizeXConfig != _config.end()
        && headSizeYConfig != _config.end())
    {
        /* Getting the relative sizes from the config.
           They are written as a number between 0 and 1,
           relative to the world's size */
        float headRelSizeX = std::stof(headSizeXConfig->second);
        float headRelSizeY = std::stof(headSizeYConfig->second);

        // Calculate actual sizes by multiplying relative sizes with world's size
        float headSizeX = headRelSizeX * _world->GetSize().x;
        float headSizeY = headRelSizeY * _world->GetSize().y;

        // Set head sprite's scale accordingly to get the calculated head size
        _headSprite.setScale(
            headSizeX / _headSprite.getLocalBounds().width,
            headSizeY / _headSprite.getLocalBounds().height
        );
    }

    float headSizeX = _headSprite.getGlobalBounds().width;
    float headSizeY = _headSprite.getGlobalBounds().height;

    // Set collision radius to be the radius from the center of the texture to its corner
    _collisionRadius = sqrt(
        headSizeX * headSizeX / 4 +
        headSizeY * headSizeY / 4);
    // If scale for the collision radius is specified in the config, the radius should be scaled with it
    auto collisionRadiusScaleConfig = _config.find("collision_radius_scale");
    if (collisionRadiusScaleConfig != _config.end())
    {
        _collisionRadius *= std::stof(collisionRadiusScaleConfig->second);
    }
}

void Person::SetTargetPoint(sf::Vector2f const targetPoint)
{
    _targetPoint = targetPoint;
}

void Person::MoveInDirection(sf::Vector2f const dirVector)
{
    // Calculate velocity vector in the given direction with the person's constant speed
    sf::Vector2f velocity = GeometryUtils::NormaliseVector(dirVector) * _speed;
    sf::Vector2f nextPosition = sf::Transformable::getPosition() + velocity;

    // If the next position is valid, move the person there
    if (IsPositionValid(nextPosition))
    {
        sf::Transformable::setPosition(nextPosition);
    }
    /* Otherwise move the person to that direction, step by step, until its position stops being valid.
        This is needed so that the person can go as close as possible to objects,
        and not be stopped when he's a few pixels away. */
    else
    {
        nextPosition = sf::Transformable::getPosition();
        sf::Vector2f stepVector = GeometryUtils::NormaliseVector(dirVector) * _movementPrecision;
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

void Person::MoveTowards(sf::Vector2f const targetPoint)
{
    sf::Vector2f const& currPos = sf::Transformable::getPosition();
    // Calculate velocity vector in the given direction with the person's constant speed
    sf::Vector2f velocity = GeometryUtils::NormaliseVector(
        targetPoint - currPos)
        * _speed;
    sf::Vector2f nextPosition = currPos + velocity;

    // If the next position is valid, move the person there
    if (IsPositionValid(nextPosition))
    {
        sf::Transformable::setPosition(nextPosition);
    }
    else
    {
        float angleStep = M_PI / _goAroundPrecision;
        sf::Vector2f lVel = velocity, rVel = velocity;
        for (float i = 1; i <= _goAroundPrecision; i += 1.f)
        {
            lVel = GeometryUtils::RotateVector(lVel, angleStep);
            if (IsPositionValid(currPos + lVel))
            {
                sf::Transformable::setPosition(currPos + lVel);
                break;
            }
            rVel = GeometryUtils::RotateVector(rVel, angleStep);
            if (IsPositionValid(currPos + rVel))
            {
                sf::Transformable::setPosition(currPos + rVel);
                break;
            }
        }
    }
}

void Person::MoveTowards(float xTarget, float yTarget)
{
    MoveTowards(sf::Vector2f(xTarget, yTarget));
}

void Person::ConfigPersonSpeed()
{
    auto personSpeedConfig = _config.find("speed");
    if (personSpeedConfig != _config.end())
    {
        // Person speed relative to the window's width, in pixels/second
        float personSpeedRel = std::stof(personSpeedConfig->second);

        _speed = personSpeedRel * _world->GetSize().x / (float)_world->GetGame()->GetFramerateLimit();
    }
    else
    {
        _speed = SPEED_DEFAULT;
    }
}

void Person::ConfigMovementPrecision()
{
    auto movementPrecisionConfig = _config.find("movement_precision");
    if (movementPrecisionConfig != _config.end())
    {
        _movementPrecision = std::stof(movementPrecisionConfig->second);
    }
    else
    {
        _movementPrecision = MOVEMENT_PRECISION_DEFAULT;
    }
}

void Person::ConfigInitialPosition()
{
    sf::Vector2f relInitialPosition = INTIAL_POSITION_DEFAULT;

    auto initialPositionXConfig = _config.find("initial_position_x");
    auto initialPositionYConfig = _config.find("initial_position_y");
    if (initialPositionXConfig != _config.end() && initialPositionYConfig != _config.end())
    {
        relInitialPosition = {
            std::stof(initialPositionXConfig->second),
            std::stof(initialPositionYConfig->second)
        };
    }

    sf::Transformable::setPosition(
        relInitialPosition.x * _world->GetSize().x,
        relInitialPosition.y * _world->GetSize().y
    );
}

void Person::ConfigGoAroundPrecision()
{
    auto goAroundPrecisionConfig = _config.find("go_around_precision");
    if (goAroundPrecisionConfig != _config.end())
    {
        _goAroundPrecision = std::stof(goAroundPrecisionConfig->second);
    }
    else
    {
        _goAroundPrecision = GO_AROUND_PRECISION_DEFAULT;
    }
}

void Person::UpdateTransform()
{
    _headSprite.setPosition(sf::Transformable::getPosition());
}

void Person::PointHeadTowards(sf::Vector2f const targetPoint)
{
    // Get normal direction vector from the head to the target point
    sf::Vector2f dirVector = GeometryUtils::NormaliseVector(
        _headSprite.getPosition() - targetPoint);

    float angle = acos(dirVector.x);
    if (dirVector.y < 0)
    {
        angle = -angle;
    }

    _headSprite.setRotation(angle * 180.f / M_PI);
}

void Person::PointHeadTowards(float xTarget, float yTarget)
{
    PointHeadTowards(sf::Vector2f(xTarget, yTarget));
}

bool Person::IsPositionInWorld(sf::Vector2f const position) const
{
    return (position.x + _collisionRadius < _world->GetSize().x
        && position.x - _collisionRadius >= 0
        && position.y + _collisionRadius < _world->GetSize().y
        && position.y - _collisionRadius >= 0);
}

bool Person::IsPositionOutsideWalls(sf::Vector2f const position) const
{
    for (sf::ConvexShape const& wall : _world->GetWalls())
    {
        /* Checking if any edge of the wall intersects the collision circle.
            Which is not 100% correct, because if the collision cirlce
            is completely inside a wall, the function will return true.
            Since the player is initially outside of all walls,
            this can only happen if the player "jumps" to the inside of a wall
            in a single frame. This is possible, but very unlikely,
            because the player's speed will have to be at least 4 times more than the collision radius,
            and that is not a realistic speed to be used in the game. */
        for (int i = 0; i < wall.getPointCount(); i++)
        {
            if (GeometryUtils::SegmentIntersectsCircle(
                    wall.getPoint(i),
                    wall.getPoint((i + 1) % wall.getPointCount()),
                    position,
                    _collisionRadius
                )
            )
            {
                return false;
            }
        }
    }

    return true;
}

bool Person::IsPositionValid(sf::Vector2f const position) const
{
    return IsPositionInWorld(position) && IsPositionOutsideWalls(position);
}

} // namespace HideAndSeekAndShoot