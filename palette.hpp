#ifndef CLASS_PALETTE
#define CLASS_PALETTE

#include <SFML/Graphics.hpp>
#include <vector>

#define MIN_TRANSLUCENT_PAL 244

struct Palette
{
    std::vector<sf::Color> colors;
};

#endif // CLASS_PALETTE
