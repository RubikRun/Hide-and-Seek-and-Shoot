#include "Enemy.h"

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
    _player(player)
{}

void Enemy::Update()
{
    SetTargetPoint(_player->getPosition());

    Person::Update();

    MoveTowardsTargetPoint();
}

} // namespace HideAndSeekAndShoot