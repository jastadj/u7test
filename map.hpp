#ifndef CLASS_U7MAP
#define CLASS_U7MAP

#define REGION_SIZE 16

#include <vector>
#include "chunk.hpp"

class U7Map
{
private:

    struct Region
    {
        std::vector< std::vector< uint16_t > > chunk_ids;

        Region()
        {
            chunk_ids.resize(REGION_SIZE);
            for(int i = 0; i < REGION_SIZE; i++)
            {
                for(int n = 0; n < REGION_SIZE; n++) chunk_ids[i].push_back(uint16_t(0x0000));
            }
        }
    };

    std::vector< std::vector<Region> > m_Regions;

public:
    U7Map(std::string mapfile, int rwidth = 12, int rheight = 12);
    ~U7Map();

    void show();
    void showRegion(int x, int y);

    int getChunkID(int regionx, int regiony, int x, int y);
};
#endif // CLASS_U7MAP
