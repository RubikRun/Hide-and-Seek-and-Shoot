#include "Enemy.h"
#include "../utils/geometryUtils.hpp"

#include <iostream>

namespace
{

auto constexpr ENEMY_CONFIG_FILENAME = "Game/config/enemy.conf";

} // namespace

namespace HideAndSeekAndShoot
{

Enemy::Enemy(
    World const* world,
    sf::Texture const* headTex,
    Player const* player)
    : Person(world, headTex, ENEMY_CONFIG_FILENAME),
    _player(player),
    _fieldOfView(world)
{}

void Enemy::Update()
{
    SetTargetPoint(_player->getPosition());

    Person::Update();

    MoveTowards(_targetPoint);

    sf::Vector2f const& pos = sf::Transformable::getPosition();
    _fieldOfView.SetOrigin(pos);
    _fieldOfView.SetTargetDirection(
        GeometryUtils::GetVector(pos, _player->getPosition())
    );
    _fieldOfView.Update();
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_fieldOfView, states);

    Person::draw(target, states);
}

} // namespace HideAndSeekAndShoot