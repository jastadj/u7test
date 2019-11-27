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
    };

    std::vector<ShapeFrame> m_Frames;

public:
    Shape(std::vector<uint8_t> record);
    ~Shape();

    void show();
};
#endif // CLASS_SHAPE
