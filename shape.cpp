#include "shape.hpp"

#include <iostream>
#include <iomanip>

Shape::Shape(std::vector<uint8_t> record)
{
    int shapesize = 0;
    int count = 0;

    // get size of shape
    for(int i = 0; i < 4; i++) shapesize += int(record[i]) << (i*8);
    // get frame count in shape
    for(int i = 4; i < 4+4; i++) count += int(record[i]) << ((i-4)*8);
    count = (count - 4) / 4;

    // get offsets
    for(int i = 0; i < count; i++)
    {
        ShapeFrame newframe;
        int doffset = 4 + (i*4);
        int offset = 0;
        for(int n = doffset; n < doffset+4; n++) offset += int(record[n]) << ((n-doffset)*8);
        newframe.offset = offset;
        m_Frames.push_back(newframe);
    }

    std::cout << "shape size:" << std::dec << shapesize << std::endl;
    std::cout << "shape count:" << std::dec << count << "(" << m_Frames.size() << ")\n";
}

Shape::~Shape()
{

}

void Shape::show()
{

}
