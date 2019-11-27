#include "shape.hpp"

#include <iostream>
#include <iomanip>

Shape::Shape(std::vector<uint8_t> record)
{
    int shapesize = 0;
    int count = 0;

    // get size of shape (first 4 bytes)
    for(int i = 0; i < 4; i++) shapesize += int(record[i]) << (i*8);
    // get frame count in shape ( (next 4 bytes)
    for(int i = 4; i < 4+4; i++) count += int(record[i]) << ((i-4)*8);
    // since the first offset starts right after the offset list,
    // determine how many offsets there are total
    count = (count - 4) / 4;

    // read in frame data
    for(int i = 0; i < count; i++)
    {
        ShapeFrame newframe;
        int doffset = 4 + (i*4);
        int offset = 0;
        for(int n = doffset; n < doffset+4; n++) offset += int(record[n]) << ((n-doffset)*8);
        newframe.offset = offset;

        newframe.max_x = int(record[offset]) + ( int(record[offset+1]) << 8);
        newframe.offset_x = int(record[offset+2]) + ( int(record[offset+2+1]) << 8);
        newframe.offset_y = int(record[offset+4]) + ( int(record[offset+4+1]) << 8);
        newframe.max_y = int(record[offset+6]) + ( int(record[offset+6+1]) << 8);

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
