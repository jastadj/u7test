#ifndef CLASS_SHAPE
#define CLASS_SHAPE

#include <cstdlib>
#include <string>
#include <vector>

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
    };

    std::vector<ShapeFrame> m_Frames;

public:
    Shape(std::vector<uint8_t> record);
    ~Shape();

    void show();
};
#endif // CLASS_SHAPE
