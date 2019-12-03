#include "u7.hpp"

#include <iostream>

#include "flx.hpp"

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

    // init screen
    std::cout << "Initializing screen...\n";
    m_Screen = new sf::RenderWindow(sf::VideoMode(800,600,32), "U7Test");
    m_Screen->clear();
    m_Screen->display();

    // load palettes
    std::cout << "Initializing palettes...\n";
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

    // load fonts
    std::cout << "Initializing fonts...\n";
    FLXFile fontflx;
    if(!fontflx.open( std::string(U7_DIR) + "/STATIC/FONTS.VGA")) std::cout << "Error opening FONTS.VGA" << std::endl;
    else
    {
        for(int i = 0; i < fontflx.getRecordCount(); i++)
        {
            std::vector<uint8_t> trecord = fontflx.getRecord(i);

            if( int(trecord.size()) != 0)
            {
                m_FontShapes.push_back( Shape(trecord));
                m_Fonts.push_back( m_FontShapes.back().toSprites(m_Palettes[0]));
            }
        }
        std::cout << m_Fonts.size() << " fonts loaded.\n";

    }

    initialized = true;

    // start main loop
    std::cout << "Starting main loop...\n";
    mainLoop();

    return 0;
}

int U7::mainLoop()
{
    bool quit = false;

    sf::Sprite tsprite(*m_Fonts[0][65]);
    tsprite.setPosition(sf::Vector2f(20,20));
    tsprite.setScale(sf::Vector2f(2.0,2.0));

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
                else if(event.key.code == sf::Keyboard::F2) showShape(&m_FontShapes[0]);
            }

        }

        // draw
        m_Screen->draw(tsprite);

        // display
        m_Screen->display();
    }

    return 0;
}

void U7::showPalettes()
{
    float rect_scalar = 4.0;
    float max_rect_scalar = 16.0;
    bool quit = false;
    bool needs_init = true;
    std::vector< std::vector<sf::RectangleShape> > pal;
    int prev_pal = -1;
    int cur_pal = 0;



    while(!quit)
    {
        m_Screen->clear();
        sf::Event event;

        // if needs init
        if(needs_init)
        {
            pal.clear();
            for(int i = 0; i < 16; i++)
            {
                pal.push_back( std::vector<sf::RectangleShape>());
                for(int n = 0; n < 16; n++)
                {
                    sf::RectangleShape nrect( sf::Vector2f(rect_scalar, rect_scalar));
                    nrect.setFillColor( m_Palettes[cur_pal].colors[(i*16)+n] );
                    nrect.setPosition(sf::Vector2f(i*rect_scalar, n*rect_scalar));
                    pal[i].push_back(nrect);
                }
            }
            needs_init = false;
        }


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
                else if(event.key.code == sf::Keyboard::Add)
                {
                    if(rect_scalar < max_rect_scalar)
                    {
                        rect_scalar += 1.0;
                        needs_init = true;
                    }
                }
                else if(event.key.code == sf::Keyboard::Subtract)
                {
                    if(rect_scalar != 1.0)
                    {
                        rect_scalar -= 1.0;
                        needs_init = true;
                    }
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

void U7::showShape(Shape *tshape)
{
    if(!tshape) return;
    bool quit = false;
    static float shape_scalar = 1.0;
    int cur_index = 0;
    int prev_index = -1;
    int cur_pal = 0;
    int prev_pal = -1;
    std::vector<sf::Sprite*> sprites;
    int frame_count = tshape->getFrameCount();

    if(!frame_count) return;

    while(!quit)
    {
        m_Screen->clear();

        sf::Event event;

        // if palette is changed, update sprites
        if(cur_pal != prev_pal)
        {
            sprites.clear();
            sprites = tshape->toSprites(m_Palettes[cur_pal]);
            prev_pal = cur_pal;
            std::cout << "Current pal:" << cur_pal << std::endl;
        }

        if(cur_index != prev_index)
        {
            prev_index = cur_index;
            std::cout << "Current frame:" << cur_index << std::endl;
        }

        while(m_Screen->pollEvent(event))
        {
            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
                else if(event.key.code == sf::Keyboard::Left)
                {
                    cur_index--;
                    if(cur_index < 0) cur_index = frame_count-1;
                }
                else if(event.key.code == sf::Keyboard::Right)
                {
                    cur_index++;
                    if(cur_index >= frame_count) cur_index = 0;
                }
                else if(event.key.code == sf::Keyboard::Add)
                {
                    shape_scalar += 1.0;
                    if(shape_scalar > 4.0) shape_scalar = 4.0;
                }
                else if(event.key.code == sf::Keyboard::Subtract)
                {
                    shape_scalar -= 1.0;
                    if(shape_scalar < 1.0) shape_scalar = 1.0;
                }
                else if(event.key.code == sf::Keyboard::Up)
                {
                    cur_pal--;
                    if(cur_pal < 0) cur_pal = int(m_Palettes.size()-1);
                }
                else if(event.key.code == sf::Keyboard::Down)
                {
                    cur_pal++;
                    if(cur_pal >= int(m_Palettes.size())) cur_pal = 0;
                }
            }
        }

        sprites[cur_index]->setScale(shape_scalar, shape_scalar);
        sprites[cur_index]->setPosition(m_Screen->getSize().x/2, m_Screen->getSize().y/2);
        m_Screen->draw(*sprites[cur_index]);

        m_Screen->display();
    }
}
