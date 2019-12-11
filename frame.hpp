#ifndef CLASS_FRAME
#define CLASS_FRAME

#include <SFML/Graphics.hpp>

struct Frame
{
    sf::Texture texture;
    sf::Vector2f offset;

    Frame(sf::Texture *ttxt, sf::Vector2f toffset = sf::Vector2f(0,0))
    {
        if(ttxt) texture = sf::Texture(*ttxt);
        offset = toffset;
    }

    Frame(sf::Image *img, sf::Vector2f toffset = sf::Vector2f(0,0))
    {
        if(img) texture.loadFromImage(*img);
        offset = toffset;
    }
};

#endif // CLASS_FRAME
