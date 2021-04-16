#include "Enemy.h"

#include <iostream>

namespace HideAndSeekAndShoot
{

Enemy::Enemy(
    World const* world,
    sf::Texture const* headTex,
    Player const* player)
    : Person(world, headTex),
    _player(player)
{
    _speed *= 0.5f;
}

void Enemy::Update()
{
    SetTargetPoint(_player->getPosition());

    Person::Update();

    MoveTowardsTargetPoint();
}

} // namespace HideAndSeekAndShoot