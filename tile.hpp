#ifndef CLASS_TILE
#define CLASS_TILE

#include <cstdlib>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "palette.hpp"

#define TILE_SIZE 8

class Tile
{
private:

    std::vector<uint8_t> m_Data;
    sf::Texture *m_Texture;
    Palette *m_Palette;


public:
    Tile(std::vector<uint8_t> tiledata, Palette *pal);
    ~Tile();

    std::vector<uint8_t> getData() { return m_Data;}
    const Palette *getPalette() const { return m_Palette;}

    sf::Sprite *getSprite();
};

class TileSet
{
private:

    std::vector<Tile*> m_Tiles;

public:
    TileSet(std::vector<uint8_t> record, Palette *pal);
    ~TileSet();

    int getTileCount() { return int(m_Tiles.size());}
    sf::Sprite *getSprite(int tileindex);
};
#endif // CLASS_TILE
