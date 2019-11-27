#include "u7.hpp"

#include <iostream>

#include "flx.hpp"
#include "shape.hpp"

U7 *U7::m_Instance = NULL;

U7::U7()
{
    m_Screen = NULL;
}

U7::~U7()
{

}

int U7::init()
{
    static bool initialized = false;
    if(initialized) {std::cout << "U7 is already initialized!\n"; return 1;}

    std::cout << "Initializing...\n";

    // load palettes
    FLXFile paletteflx;
    if(!paletteflx.open( std::string(U7_DIR) + "/STATIC/PALETTES.FLX")) std::cout << "Error opening palettes.flx" << std::endl;
    else
    {
        for(int i = 0; i < paletteflx.getRecordCount(); i++)
        {
            std::vector<uint8_t> pdata = paletteflx.getRecord(i);

            // ignore records that do not have 256 colors
            if( int(pdata.size())/3 != 256) continue;

            Palette newpal;

            for(int n = 0; n < int(pdata.size())/3; n++)
            {
                int pdi = n*3;
                newpal.colors.push_back( sf::Color( pdata[pdi]*4, pdata[pdi+1]*4, pdata[pdi+2]*4) );
            }
            m_Palettes.push_back(newpal);

        }
        std::cout << m_Palettes.size() << " palettes loaded." << std::endl;

    }

    // load font
    FLXFile fontflx;
    if(!fontflx.open( std::string(U7_DIR) + "/STATIC/FONTS.VGA")) std::cout << "Error opening FONTS.VGA" << std::endl;
    else
    {
        fontflx.show();
        Shape newshape(fontflx.getRecord(0));
    }

    // init screen
    m_Screen = new sf::RenderWindow(sf::VideoMode(800,600,32), "U7Test");

    initialized = true;

    // start main loop
    mainLoop();

    return 0;
}

int U7::mainLoop()
{
    bool quit = false;

    while(!quit)
    {
        // clear screen
        m_Screen->clear();

        // event queue
        sf::Event event;

        // process event queue
        while(m_Screen->pollEvent(event))
        {
            if(event.type == sf::Event::Closed) quit = true;
            // key pressed
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
                else if(event.key.code == sf::Keyboard::F1) showPalettes();
            }

        }

        // draw

        // display
        m_Screen->display();
    }

    return 0;
}

void U7::showPalettes()
{
    const float PRECT_SIZE = 4.0;
    bool quit = false;

    std::vector< std::vector<sf::RectangleShape> > pal;
    int prev_pal = -1;
    int cur_pal = 0;

    // init palette rects
    for(int i = 0; i < 16; i++)
    {
        pal.push_back( std::vector<sf::RectangleShape>());
        for(int n = 0; n < 16; n++)
        {
            sf::RectangleShape nrect( sf::Vector2f(PRECT_SIZE, PRECT_SIZE));
            nrect.setFillColor( m_Palettes[cur_pal].colors[(i*16)+n] );
            nrect.setPosition(sf::Vector2f(i*PRECT_SIZE, n*PRECT_SIZE));
            pal[i].push_back(nrect);
        }
    }

    while(!quit)
    {
        m_Screen->clear();
        sf::Event event;
        while(m_Screen->pollEvent(event))
        {
            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
                else if(event.key.code == sf::Keyboard::Left)
                {
                    cur_pal--;
                    if(cur_pal < 0) cur_pal = int(m_Palettes.size()-1);
                }
                else if(event.key.code == sf::Keyboard::Right)
                {
                    cur_pal++;
                    if(cur_pal >= int(m_Palettes.size())) cur_pal = 0;
                }
            }
        }

        // if changing palettes, update colors
        if(prev_pal != cur_pal)
        {
            for(int i = 0; i < 16; i++)
            {
                for(int n = 0; n < 16; n++)
                {
                    pal[i][n].setFillColor( m_Palettes[cur_pal].colors[(i*16)+n] );
                }
            }

            prev_pal = cur_pal;
            std::cout << "Viewing palette #" << cur_pal << std::endl;
        }

        // draw palette
        for(int i = 0; i < 16; i++)
        {
            for(int n = 0; n < 16; n++)
            {
                m_Screen->draw(pal[i][n]);
            }
        }

        m_Screen->display();

    }
}
