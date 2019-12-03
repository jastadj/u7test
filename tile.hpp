#ifndef CLASS_TILE
#define CLASS_TILE

#include <cstdlib>
#include <iostream>
#include <vector>

#define TILE_SIZE 8

class Tile
{
private:

public:
    Tile(std::vector<uint8_t> record);
    ~Tile();
};

struct TileSet
{
    std::vector<Tile> tiles;
};

#endif // CLASS_TILE
