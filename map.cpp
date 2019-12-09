#include "map.hpp"

#include <fstream>

U7Map::U7Map(std::string mapfile, int rwidth, int rheight)
{
    std::ifstream ifile;
    ifile.open(mapfile.c_str(), std::ios::binary);
    if(!ifile.is_open()) { std::cout << "Error opening map file:" << mapfile << std::endl; return;}

    for(int i = 0; i < rheight; i++)
    {
        m_Regions.push_back( std::vector<Region>());

        for(int n = 0; n < rwidth; n++)
        {
            Region newregion;

            for(int y = 0; y < REGION_SIZE; y++)
            {
                for(int x = 0; x < REGION_SIZE; x++)
                {
                    newregion.chunk_ids[y][x] = uint8_t(ifile.get()) + ( uint8_t(ifile.get()) << 8);
                }
            }
            m_Regions.back().push_back(newregion);
        }
    }

    ifile.close();
}

U7Map::~U7Map()
{

}

int U7Map::getChunkID(int regionx, int regiony, int x, int y)
{
    if(regiony < 0 || regiony >= int(m_Regions.size()) ) {std::cout << "Error getting map chunk id, region y(" << regiony << ") out of bounds!\n";return 0;}
    if(regionx < 0 || regionx >= int(m_Regions[regiony].size())) {std::cout << "Error getting map chunk id, region x(" << regionx << ") out of bounds!\n";return 0;}
    if(y < 0 || y >= int(m_Regions[regiony][regionx].chunk_ids.size())) {std::cout << "Error getting map chunk id, chunk y(" << y << ") out of bounds!\n";return 0;}
    if(x < 0 || x >= int(m_Regions[regiony][regionx].chunk_ids[y].size())) {std::cout << "Error getting map chunk id, chunk x(" << x << ") out of bounds!\n";return 0;}

    return 0;
}

void U7Map::show()
{
    if(m_Regions.empty()) std::cout << "map regions empty\n";
    else
    {
        std::cout << std::dec << "map region dims:" << m_Regions[0].size() << "x" << m_Regions.size() << std::endl;
    }
}

void U7Map::showRegion(int x, int y)
{
    Region *region;

    if(y < 0 || y >= int(m_Regions.size()))
    {
        std::cout << "Error showing map region, y val (" << y << ") out of bounds!\n";
        return;
    }
    if(x < 0 || x >= int(m_Regions[y].size()) )
    {
        std::cout << "Error showing map region, x val (" << x << ") out of bounds!\n";
        return;
    }

    region = &m_Regions[y][x];
    std::cout << "Region:" << x << "," << y << std::endl;

    for(int i = 0; i < REGION_SIZE; i++)
    {
        for(int n = 0; n < REGION_SIZE; n++)
        {
            std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') << int(region->chunk_ids[i][n]) << " ";
        }
        std::cout << std::dec << std::endl;
    }

}
