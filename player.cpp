#include "player.hpp"

Player::Player()
{

}

Player::~Player()
{

}

bool Player::setPosition(sf::Vector2f tpos)
{
    m_Position = tpos;
    return true;
}
