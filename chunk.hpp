#ifndef CLASS_CHUNK
#define CLASS_CHUNK

#include <cstdlib>
#include <vector>
#include <iostream>
#include <iomanip>

#define CHUNK_SIZE 16

struct Chunk
{
    std::vector< std::vector<int> > tiles;

    Chunk()
    {
        tiles.resize(CHUNK_SIZE);
        for(int i = 0; i < CHUNK_SIZE; i++)
        {
            for(int n = 0; n < CHUNK_SIZE; n++) tiles[i].push_back(0);
        }
    }

    void setData(std::vector<uint8_t> data)
    {
        for(int i = 0; i < CHUNK_SIZE; i++)
        {
            for(int n = 0; n < CHUNK_SIZE; n++)
            {
                int offset = ((i*16) + n)*2;
                tiles[i][n] = data[offset] + (data[offset+1] << 8);
            }
        }
    }

    void show()
    {
        for(int i = 0; i < CHUNK_SIZE; i++)
        {
            for(int n = 0; n < CHUNK_SIZE; n++)
            {
                std::cout << std::hex << std::setw(4) << std::setfill('0') << int(tiles[i][n]) << " ";
            }
            std::cout << std::dec << std::endl;
        }
    }
};


#endif // CLASS_CHUNK
