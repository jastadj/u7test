#ifndef CLASS_U7
#define CLASS_U7

#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>

#include "palette.hpp"

#define U7_DIR "./u7"

class U7
{
private:
    U7();
    ~U7();
    static U7 *m_Instance;

    sf::RenderWindow *m_Screen;

    std::vector<Palette> m_Palettes;

    int mainLoop();

    // test
    void showPalettes();

public:
    static U7 *getInstance()
    {
        if(!m_Instance) m_Instance = new U7;
        return m_Instance;
    }

    int init();
};
#endif // CLASS_U7
