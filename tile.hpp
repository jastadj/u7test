#ifndef CLASS_TILE
#define CLASS_TILE

#include <cstdlib>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "object.hpp"
#include "palette.hpp"

#define TILE_SIZE 8

class Tile: public Object
{
private:

public:
    Tile(std::string tname, std::vector<uint8_t> record, std::vector<Palette> &pal, uint32_t flags);
    ~Tile();

    void show();
};
#endif // CLASS_TILE
