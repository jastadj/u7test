#include "tile.hpp"

Tile::Tile(std::vector<uint8_t> record)
{
    int recsize = int(record.size());
    int tile_count = recsize/(TILE_SIZE*TILE_SIZE);

    if(recsize == 0 || recsize%(TILE_SIZE*TILE_SIZE))
    {
        std::cout << "Tile record has unexpected size!  Empty or not divisible by " << std::dec << (TILE_SIZE*TILE_SIZE) << std::endl;
        return;
    }



}

Tile::~Tile()
{

}
