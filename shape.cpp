#include "shape.hpp"



Shape::Shape(std::vector<uint8_t> record)
{
    int count = 0;
    m_Size = 0;

    // get size of shape (first 4 bytes)
    for(int i = 0; i < 4; i++) m_Size += int(record[i]) << (i*8);
    // get frame count in shape ( (next 4 bytes)
    for(int i = 4; i < 4+4; i++) count += int(record[i]) << ((i-4)*8);
    // since the first offset starts right after the offset list,
    // determine how many offsets there are total
    count = (count - 4) / 4;

    // read in frame data and create frame image
    for(int i = 0; i < count; i++)
    {
        // data offset
        ShapeFrame *newframe = new ShapeFrame;
        int doffset = 4 + (i*4);
        int offset = 0;
        int span_offset;
        for(int n = doffset; n < doffset+4; n++) offset += int(record[n]) << ((n-doffset)*8);
        newframe->offset = offset;

        // dims
        newframe->max_x = int(record[offset]) + ( int(record[offset+1]) << 8);
        newframe->offset_x = int(record[offset+2]) + ( int(record[offset+2+1]) << 8);
        newframe->offset_y = int(record[offset+4]) + ( int(record[offset+4+1]) << 8);
        newframe->max_y = int(record[offset+6]) + ( int(record[offset+6+1]) << 8);
        span_offset = offset+8;

        // initialize pixel array (0xff is transparent)
        newframe->pixels.resize(newframe->getDims().y);
        for(int n = 0; n < int(newframe->pixels.size()); n++)
        {
            for(int k = 0; k < int(newframe->getDims().x); k++)
            {
                newframe->pixels[n].push_back(0xff);
            }
        }
        // read in pixel data
        while(1)
        {
            // read block data
            uint16_t blockdata = record[span_offset] + ( record[span_offset+1] << 8);
            uint16_t len = blockdata >> 1;
            bool uncompressed = !(blockdata & 1);
            int16_t start_x = int16_t(record[span_offset+2] + (record[span_offset+3] << 8)) + newframe->offset_x;
            int16_t start_y = int16_t(record[span_offset+4] + (record[span_offset+5] << 8)) + newframe->offset_y;
            span_offset += 6;

            // if block data is 0, this is span terminator
            if( blockdata == 0) break;

            // if reading uncompressed raw pixel data
            if( uncompressed)
            {
                for(unsigned int n = 0; n < len; n++)
                {
                    newframe->pixels[start_y][start_x+n] = record[span_offset+n];
                }
                span_offset += len;
            }
            // else pixel data is RLE, may have multiple RLE blocks (block len is all run lengths added)
            else
            {
                int block_offset = 0;
                // while still have run lengths to process
                while(block_offset < len)
                {
                    uint8_t run_data = record[span_offset];
                    int run_len = run_data >> 1;
                    bool is_raw = !(run_data & 1);
                    span_offset++;

                    // read run length of raw pixels
                    if(is_raw)
                    {
                        for(int n = 0; n < run_len; n++) newframe->pixels[start_y][start_x+n+block_offset] = record[span_offset+n];
                        span_offset += run_len;
                    }
                    // or repeat byte for run length
                    else
                    {
                        for(int n = 0; n < run_len; n++) newframe->pixels[start_y][start_x+n+block_offset] = record[span_offset];
                        span_offset++;
                    }
                    block_offset += run_len;
                }

            }


        }

        // for optimization later, make this a vector of pointers
        m_Frames.push_back(newframe);
    }



}

Shape::~Shape()
{
    for(int i = 0; i < int(m_Frames.size()); i++)
    {
        delete m_Frames[i];
    }
}

sf::Image *Shape::toImage(int frame_index, Palette &pal)
{
    sf::Image *img = NULL;
    ShapeFrame *tframe = NULL;
    if(frame_index < 0 || frame_index >= int(m_Frames.size())) return img;

    tframe = m_Frames[frame_index];

    img = new sf::Image;
    img->create(tframe->getDims().x, tframe->getDims().y);

    for(uint32_t y = 0; y < tframe->getDims().y; y++)
    {
        for(uint32_t x = 0; x < tframe->getDims().x; x++)
        {
            img->setPixel(x, y, pal.colors[tframe->pixels[y][x]]);
        }
    }

    return img;
}

void Shape::show()
{
    std::cout << "shape size:" << std::dec << m_Size << std::endl;
    std::cout << "frames:" << std::dec << m_Frames.size() << std::endl;

    for(int i = 0; i < int(m_Frames.size()); i++)
    {
        std::cout << std::dec << std::setw(3) << std::setfill('0') << i << ":0x";
        std::cout << std::hex << std::setw(8) << std::setfill('0') << m_Frames[i]->offset << std::endl;
    }
}

void Shape::showFrame(int frame)
{
    if(frame < 0 || frame >= int(m_Frames.size()) ) {std::cout << "Shape frame " << frame << " is invalid!\n"; return;}


    std::cout << "Frame #" << std::dec << frame << std::endl;
    std::cout << "  Offset:0x" << std::hex << std::setw(8) << std::setfill('0') << m_Frames[frame]->offset << std::endl;
    m_Frames[frame]->show();
}
