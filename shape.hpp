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

        int16_t max_x;
        int16_t max_y;
        int16_t offset_x;
        int16_t offset_y;

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
    };

    int m_Size;
    std::vector<ShapeFrame*> m_Frames;
    std::vector< std::vector<sf::Texture*> > m_Textures; // 2d array of textures, 1st dimension is palette
    sf::Image *toImage(int frame_index, Palette &pal);

public:
    Shape(std::vector<uint8_t> record, std::vector<Palette> &pals);
    ~Shape();

    int getFrameCount() { return int(m_Frames.size());}
    int getPaletteCount() { return int(m_Textures.size());}
    sf::Vector2u getLargestFrameDim();

    sf::Sprite *createSprite(int frame, int pal = 0, bool apply_offsets=true);

    void show();
    void showFrame(int frame);
};
#endif // CLASS_SHAPE
