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

    struct ShapeFrame
    {
        int offset;

        uint16_t max_x;
        uint16_t max_y;
        uint16_t offset_x;
        uint16_t offset_y;

        sf::Image *image;
        sf::Texture *texture;
        std::vector< std::vector<uint8_t> > pixels;

        sf::Vector2u getDims()
        {
            return sf::Vector2u( max_x + offset_x + 1, max_y + offset_y + 1);
        }

        void show()
        {
            std::cout << std::dec << "  max_x:" << max_x << std::endl;
            std::cout << std::dec << "  max_y:" << max_y << std::endl;
            std::cout << std::dec << "  offset_x:" << offset_x << std::endl;
            std::cout << std::dec << "  offset_y:" << offset_y << std::endl;
            std::cout << std::dec << "  dimensions:" << getDims().x << "," << getDims().y << std::endl;
            for(int i = 0; i < int(getDims().y); i++)
            {
                for(int n = 0; n < int(getDims().x); n++)
                {
                    std::cout << std::hex << std::setw(2) << std::setfill('0') << int(pixels[i][n]) << " ";
                }
                std::cout << std::dec << std::endl;
            }
        }

        ShapeFrame()
        {
            image = new sf::Image;
            texture = new sf::Texture;
        }

        ~ShapeFrame()
        {
            delete image;
            delete texture;
        }
    };

    int m_Size;
    std::vector<ShapeFrame*> m_Frames;


public:
    Shape(std::vector<uint8_t> record);
    ~Shape();

    std::vector<sf::Sprite*> toSprites(Palette tpal, int transparency_index = 0xff);
    int getFrameCount() { return int(m_Frames.size());}

    void show();
    void showFrame(int frame);
};
#endif // CLASS_SHAPE
