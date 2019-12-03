#ifndef CLASS_FONT
#define CLASS_FONT

#include <vector>

#include "shape.hpp"
#include "palette.hpp"

class Font
{
private:

    Shape *m_Shape;
    std::vector<Palette*> m_Palettes;
    std::vector< std::vector<sf::Texture*> > m_Textures;


public:
    Font(Shape *fshape);
    ~Font();

    bool addPalette(Palette *pal);
};
#endif // CLASS_FONT
