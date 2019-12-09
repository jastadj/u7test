#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include <SFML/Graphics.hpp>

class Player
{
private:

    sf::Vector2f m_Position;

public:
    Player();
    ~Player();

    sf::Vector2f getPosition() { return m_Position;}
    bool setPosition(sf::Vector2f tpos);
};
#endif // CLASS_PLAYER
