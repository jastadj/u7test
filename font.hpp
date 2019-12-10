#ifndef CLASS_FONT
#define CLASS_FONT

#include <vector>
#include <iostream>
#include "palette.hpp"
#include "frame.hpp"

class FontString;

class Font
{
private:

    std::vector<Frame*> m_Frames;

public:
    Font(std::vector<uint8_t> record, std::vector<Palette> &pal);
    ~Font();

    friend FontString;
};

class FontString
{
private:

    sf::RenderTexture *m_RenderTexture;
    Font *m_Font;
    std::string m_String;

public:
    FontString(Font *tfont, std::string tstring = std::string(""));
    ~FontString();

    void setString(std::string tstring);
    sf::Sprite m_Sprite;

};
#endif // CLASS_FONT
