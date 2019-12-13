#include "tile.hpp"

Tile::Tile(std::string tname, std::vector<uint8_t> record, std::vector<Palette> &pal, uint32_t flags):Object(tname,flags)
{
    const int TILE_BLOCK_SIZE = TILE_SIZE*TILE_SIZE;

    int tilecount = int(record.size()) / TILE_BLOCK_SIZE;

    for(int i = 0; i < tilecount; i++)
    {
        sf::Image image;
        int record_offset = i*TILE_BLOCK_SIZE;

        image.create(TILE_SIZE, TILE_SIZE);

        for(int y = 0; y < TILE_SIZE; y++)
        {
            for(int x = 0; x < TILE_SIZE; x++)
            {
                int pixel_color = record[record_offset + y*TILE_SIZE + x];
                image.setPixel(x,y, pal[0].colors[pixel_color]);
            }
        }

        m_Frames.push_back(new Frame(&image));

    }
}

Tile::~Tile()
{

}

void Tile::show()
{
    std::cout << "Tile:" << m_Name << ", " << m_Frames.size() << " tiles\n";
}
