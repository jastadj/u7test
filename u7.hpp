#ifndef CLASS_U7
#define CLASS_U7

#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>

#include "palette.hpp"
#include "shape.hpp"
#include "font.hpp"
#include "tile.hpp"
#include "chunk.hpp"
#include "map.hpp"
#include "player.hpp"

#define U7_DIR "./u7"

class U7
{
private:
    U7();
    ~U7();
    static U7 *m_Instance;

    sf::RenderWindow *m_Screen;
    sf::View m_View;

    std::vector<Palette> m_Palettes;
    std::vector<Font*> m_Fonts;
    std::vector<TileSet*> m_TileSets;
    std::vector<Chunk> m_Chunks;
    std::vector<Shape*> m_Faces;
    std::vector<Shape*> m_Objects;

    U7Map *m_CurrentMap;
    Player *m_Player;

    int mainLoop();
    void drawChunk(int x, int y);

    // test
    void showPalettes();
    void showShapes(std::vector<Shape*> shapes, bool is_world_shape=false);
    void showTiles();
    void showChunks();

public:
    static U7 *getInstance()
    {
        if(!m_Instance) m_Instance = new U7;
        return m_Instance;
    }

    int init();
};
#endif // CLASS_U7
