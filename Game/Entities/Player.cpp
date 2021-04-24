#include "Player.h"

namespace
{

auto constexpr PLAYER_CONFIG_FILENAME = "Game/config/player.conf";

} // namespace

namespace HideAndSeekAndShoot
{

Player::Player(
    World const* world,
    sf::Texture const* headTex,
    sf::Texture const* gunTex,
    sf::Texture const* bulletTex)
    : Person(world, headTex, gunTex, bulletTex, PLAYER_CONFIG_FILENAME)
{}

} // namespace HideAndSeekAndShoot