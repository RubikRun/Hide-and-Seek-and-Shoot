#include "Player.h"

namespace
{

auto constexpr PLAYER_CONFIG_FILENAME = "Game/config/player.conf";

} // namespace

namespace HideAndSeekAndShoot
{

Player::Player(
    World const* world,
    sf::Texture const* headTex)
    : Person(world, headTex, PLAYER_CONFIG_FILENAME)
{}

} // namespace HideAndSeekAndShoot