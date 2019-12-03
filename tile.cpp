#include "tile.hpp"

Tile::Tile(std::vector<uint8_t> tiledata, Palette *pal)
{
    m_Palette = pal;
    m_Texture = new sf::Texture;
    sf::Image image;
    image.create(TILE_SIZE, TILE_SIZE);

    if( int(tiledata.size()) != (TILE_SIZE*TILE_SIZE))
    {
        std::cout << "Error creating tile.  Tile data length unexpected.  Expecting " << (TILE_SIZE*TILE_SIZE) << " but found " << tiledata.size() << std::endl;
        return;
    }

    if(!pal)
    {
        std::cout << "Error creating tile, no valid palette provided!\n";
        return;
    }

    for(int i = 0; i < TILE_SIZE; i++)
    {
        for(int n = 0; n < TILE_SIZE; n++)
        {
            int data_index = (i*TILE_SIZE) + n;
            image.setPixel(n,i, pal->colors[ tiledata[data_index] ]);
        }
    }

    m_Texture->loadFromImage(image);
}

Tile::~Tile()
{
    delete m_Texture;
}

sf::Sprite *Tile::getSprite()
{
    return new sf::Sprite(*m_Texture);
}

//////////////////////////////////////////////////////////////////
// TILE SET

TileSet::TileSet(std::vector<uint8_t> record, Palette *pal)
{
    int recsize = int(record.size());
    const int tilelen = (TILE_SIZE*TILE_SIZE);
    int tile_count = recsize/tilelen;

    if(recsize == 0 || recsize%(TILE_SIZE*TILE_SIZE))
    {
        std::cout << "Tile record has unexpected size!  Empty or not divisible by " << std::dec << tilelen << std::endl;
        return;
    }

    if(!pal)
    {
        std::cout << "Error creating tileset, no valid palette was provided!\n";
        return;
    }

    // create tiles from record
    for(int i = 0; i < tile_count; i++)
    {
        std::vector<uint8_t> subrecord;

        for(int n = i*tilelen; n < (i*tilelen) + tilelen; n++)
        {
            subrecord.push_back(record[n]);
        }

        Tile *newtile = new Tile(subrecord, pal);
        m_Tiles.push_back(newtile);
    }
}

TileSet::~TileSet()
{
    for(int i = 0; i < int(m_Tiles.size()); i++) delete m_Tiles[i];
}

sf::Sprite *TileSet::getSprite(int tileindex)
{
    if(tileindex < 0 || tileindex >= getTileCount()) return NULL;
    return m_Tiles[tileindex]->getSprite();
}
