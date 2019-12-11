#include "object.hpp"

#include <iostream> // debug

Object::Object(std::string tname)
{
    m_Name = tname;
}

Object::~Object()
{
    for(int i = 0; i < int(m_Frames.size()); i++) delete m_Frames[i];
}


sf::Sprite *Object::newFrameSprite(int frameindex, bool apply_frame_offsets)
{
    if(frameindex < 0 || frameindex >= int(m_Frames.size())) return NULL;

    sf::Sprite *newsprite = new sf::Sprite(m_Frames[frameindex]->texture);
    newsprite->setOrigin( sf::Vector2f(m_Frames[frameindex]->offset));
}

sf::Vector2f Object::getFrameOffset(int frameindex)
{
    if(frameindex < 0 || frameindex >= int(m_Frames.size())) return sf::Vector2f();

    return m_Frames[frameindex]->offset;
}

//////////////////////////////////////////////////////////////////////////////////
//  WORLD OBJECT
WorldObject::WorldObject(std::string name, std::vector<uint8_t> record, std::vector<Palette> &pal):Object(name)
{
    int frame_count = 0;
    int shape_size = 0;

    // get basic shape information
    // get size of shape (first 4 bytes)
    for(int i = 0; i < 4; i++) shape_size += int(record[i]) << (i*8);
    // get frame count in shape ( (next 4 bytes)
    for(int i = 4; i < 4+4; i++) frame_count += int(record[i]) << ((i-4)*8);
    // since the first offset starts right after the offset list,
    // determine how many offsets there are total
    frame_count = (frame_count - 4) / 4;

    // read in frame data and create frame image
    for(int i = 0; i < frame_count; i++)
    {
        sf::Image image;
        // data offsets
        int doffset = 4 + (i*4);
        int offset = 0;
        int span_offset;
        int max_x = 0;
        int max_y = 0;
        int offset_x = 0;
        int offset_y = 0;
        for(int n = doffset; n < doffset+4; n++) offset += int(record[n]) << ((n-doffset)*8);

        // image dimensions
        max_x    = int16_t( int(record[offset])   + ( int(record[offset+1])   << 8) );
        offset_x = int16_t( int(record[offset+2]) + ( int(record[offset+2+1]) << 8) );
        offset_y = int16_t( int(record[offset+4]) + ( int(record[offset+4+1]) << 8) );
        max_y    = int16_t( int(record[offset+6]) + ( int(record[offset+6+1]) << 8) );
        span_offset = offset+8;

        // init image dimensions and fill color (transparent)
        image.create(max_x+offset_x+1, max_y+offset_y+1, sf::Color::Transparent);

        // read in pixel data spans
        while(1)
        {
            // read block data
            uint16_t blockdata = record[span_offset] + ( record[span_offset+1] << 8);
            uint16_t len = blockdata >> 1;
            bool uncompressed = !(blockdata & 1);
            int16_t start_x = int16_t(record[span_offset+2] + (record[span_offset+3] << 8)) + offset_x;
            int16_t start_y = int16_t(record[span_offset+4] + (record[span_offset+5] << 8)) + offset_y;
            span_offset += 6;

            // if block data is 0, this is span terminator
            if( blockdata == 0) break;

            // if reading uncompressed raw pixel data
            if( uncompressed)
            {
                for(unsigned int n = 0; n < len; n++)
                {
                    //newframe->pixels[start_y][start_x+n] = record[span_offset+n];
                    image.setPixel(start_x+n, start_y, pal[0].colors[ record[span_offset+n] ]);
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
                        //for(int n = 0; n < run_len; n++) newframe->pixels[start_y][start_x+n+block_offset] = record[span_offset+n];
                        for(int n = 0; n < run_len; n++) image.setPixel(start_x+n+block_offset,start_y, pal[0].colors[ record[span_offset+n] ]);
                        span_offset += run_len;
                    }
                    // or repeat byte for run length
                    else
                    {
                        //for(int n = 0; n < run_len; n++) newframe->pixels[start_y][start_x+n+block_offset] = record[span_offset];
                        for(int n = 0; n < run_len; n++) image.setPixel(start_x+n+block_offset,start_y, pal[0].colors[ record[span_offset] ]);
                        span_offset++;
                    }
                    block_offset += run_len;
                }

            }


        }

        // create frame
        m_Frames.push_back(new Frame(&image, sf::Vector2f(offset_x, offset_y)));

    }


}

WorldObject::~WorldObject()
{

}

void WorldObject::show()
{
    std::cout << "Object: " << m_Name << ", frames:" << getFrameCount() << std::endl;
}
