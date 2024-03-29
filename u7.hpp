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
#include "face.hpp"

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
    std::vector<Tile*> m_Tiles;
    std::vector<Chunk> m_Chunks;
    std::vector<Face*> m_Faces;
    std::vector<WorldObject*> m_Objects;

    U7Map *m_CurrentMap;
    Player *m_Player;

    int init();
    int mainLoop();
    void drawChunk(int x, int y);

    // test
    void showPalettes();
    void showTiles();
    void showChunks();

public:
    static U7 *getInstance()
    {
        if(!m_Instance) m_Instance = new U7;
        return m_Instance;
    }

    void start();
};
#endif // CLASS_U7
