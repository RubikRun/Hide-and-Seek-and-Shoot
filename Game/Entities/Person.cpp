#include "Person.h"

#include "../World.h"

#include "../utils/configUtils.hpp"

namespace
{
    auto constexpr PERSON_CONFIG_FILENAME = "Game/config/person.conf";
}

namespace HideAndSeekAndShoot
{

Person::Person(World const* world)
    : _world(world), 
    _headTex(nullptr),
    _config(ConfigUtils::ReadConfig(PERSON_CONFIG_FILENAME))
{
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
}

void Person::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_headSprite);
}

} // namespace HideAndSeekAndShoot