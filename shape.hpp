#ifndef CLASS_SHAPE
#define CLASS_SHAPE

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include <SFML/Graphics.hpp>

#include "palette.hpp"

class Shape
{
private:

public:
    Shape(std::vector<uint8_t> record, std::vector<Palette> &pals);
    ~Shape();

    std::vector<sf::Vector2f> m_Offsets;
    std::vector<sf::Image> m_Images;

};
#endif // CLASS_SHAPE
