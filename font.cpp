#include "font.hpp"

#include "shape.hpp"

Font::Font(std::vector<uint8_t> record, std::vector<Palette> &pal)
{
    Shape *newshape = new Shape(record, pal);

    for(int i = 0; i < int(newshape->m_Images.size()); i++)
    {
        m_Frames.push_back( new Frame(&newshape->m_Images[i], newshape->m_Offsets[i]));
    }

    delete newshape;
}

Font::~Font()
{

}

////////////////////////////////////////////////////////////////////////////////
//  FONT STRING
FontString::FontString(Font *tfont, std::string tstring)
{
    m_Font = tfont;
    m_RenderTexture = new sf::RenderTexture;
    m_String = tstring;
    setString(m_String);
}

FontString::~FontString()
{
    delete m_RenderTexture;
}

void FontString::setString(std::string tstring)
{
    sf::FloatRect strsize;
    std::vector<sf::Sprite*> sprites;
    m_String = tstring;

    for(int i = 0; i < int(m_String.size()); i++)
    {
        int cval = int(m_String[i]);
        if(cval >= 0 && cval < int(m_Font->m_Frames.size()) )
        {
            Frame *tframe = m_Font->m_Frames[cval];
            sf::Vector2u fsize = tframe->texture.getSize();
            sprites.push_back(new sf::Sprite(tframe->texture));
            sprites.back()->setPosition(strsize.width,0);
            strsize.width += fsize.x;
            if(fsize.y > strsize.height) strsize.height = fsize.y;
        }

    }

    m_RenderTexture->create(strsize.width,strsize.height);
    m_RenderTexture->clear(sf::Color::Transparent);
    for(int i = 0; i < int(sprites.size()); i++)
    {
        m_RenderTexture->draw(*sprites[i]);
    }
    m_RenderTexture->display();
    m_Sprite = sf::Sprite(m_RenderTexture->getTexture());

    for(int i = 0; i < int(sprites.size()); i++)
    {
        delete sprites[i];
    }

}
