#include "font.hpp"

Font::Font(Shape *fshape)
{
    m_Shape = fshape;
}

Font::~Font()
{
    for(int i = 0; i < int(m_Textures.size()); i++)
    {
        for(int n = 0; n < int(m_Textures[i].size()); n++) delete m_Textures[i][n];
    }
}

bool Font::addPalette(Palette *pal)
{
    if(!pal) return false;

    // check if palette has already been added
    for(int i = 0; i < int(m_Palettes.size()); i++)
    {
        if(m_Palettes[i] == pal) return false;
    }

    // add palette and generate graphics for this palette
    m_Palettes.push_back(pal);

    m_Textures.push_back( std::vector<sf::Texture*>());

    for(int i = 0; i < m_Shape->getFrameCount(); i++)
    {
        sf::Image *img = m_Shape->toImage(i, *pal);
        sf::Texture *txt = new sf::Texture;
        txt->loadFromImage(*img);
        m_Textures.back().push_back(txt);

        delete img;
    }

    return true;
}
