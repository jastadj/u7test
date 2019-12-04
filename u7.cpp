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
    std::ifstream chunkfile;
    if(initialized) {std::cout << "U7 is already initialized!\n"; return 1;}

    std::cout << "Initializing...\n";

    // init screen
    std::cout << "Initializing screen...\n";
    m_Screen = new sf::RenderWindow(sf::VideoMode(800,600,32), "U7Test");
    m_Screen->clear();
    m_Screen->display();

    // load palettes
    std::cout << "Loading palettes...\n";
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
    std::cout << "Loading fonts...\n";
    FLXFile fontflx;
    if(!fontflx.open( std::string(U7_DIR) + "/STATIC/FONTS.VGA")) std::cout << "Error opening FONTS.VGA" << std::endl;
    else
    {
        for(int i = 0; i < fontflx.getRecordCount(); i++)
        {
            std::vector<uint8_t> trecord = fontflx.getRecord(i);
            if( int(trecord.size()) != 0)
            {
                Shape *fshape = new Shape(trecord);
                Font *newfont = new Font(fshape);
                m_Fonts.push_back( newfont );
                m_Fonts.back()->addPalette(&m_Palettes[0]);
            }
        }
        std::cout << m_Fonts.size() << " fonts loaded.\n";
    }

    // the shapes.vga file contains both tiles and shapes for world
    FLXFile shapes;
    if(!shapes.open( std::string(U7_DIR) + "/STATIC/SHAPES.VGA"))
    {
        std::cout << "Error opening SHAPES.VGA" << std::endl;
        return 1;
    }

    // tiles (records 0x00 - 0x95)
    std::cout << "Loading tiles...\n";
    for(int i = 0; i <= 0x95; i++)
    {
        std::vector<uint8_t> trecord = shapes.getRecord(i);
        m_TileSets.push_back( new TileSet(trecord, &m_Palettes[0]));

        // copy tile references to master tile list and generate master tile list sprites
        std::vector<Tile*> ttiles = m_TileSets.back()->getTiles();
        for(int n = 0; n < int(ttiles.size()); n++)
        {
            m_Tiles.push_back(ttiles[n]);
            m_TileSprites.push_back(ttiles[n]->getSprite());
        }
    }
    std::cout << m_Tiles.size() << " tiles loaded, " << m_TileSprites.size() << " tile sprites created.\n";

    // load chunks
    std::cout << "Loading chunks...\n";
    chunkfile.open( std::string( std::string(U7_DIR) + "/STATIC/U7CHUNKS").c_str(), std::ios::binary | std::ios::in);
    if(!chunkfile.is_open())
    {
        std::cout << "Error opening u7chunks.\n";
        return 3;
    }
    else
    {
        size_t cfile_len = 0;
        int chunk_count = 0;
        chunkfile.seekg(0, std::ios::end);
        cfile_len = chunkfile.tellg();
        chunkfile.seekg(0, std::ios::beg);
        chunk_count = int(cfile_len/512);
        //std::cout << "Chunk file size:" << cfile_len << ", chunks:" << chunk_count << std::endl;

        for(int k = 0; k < chunk_count; k++)
        {
            // create new chunk
            std::vector<uint8_t> cdata;
            Chunk newchunk;
            for(int i = 0; i < (CHUNK_SIZE*CHUNK_SIZE)*2; i++)
            {
                unsigned char c = chunkfile.get();
                cdata.push_back( uint8_t(c));
            }

            newchunk.setData(cdata);
            m_Chunks.push_back(newchunk);
        }
        chunkfile.close();
        std::cout << m_Chunks.size() << " chunks loaded.\n";
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
                else if(event.key.code == sf::Keyboard::F2) showTiles();
                else if(event.key.code == sf::Keyboard::F3) showChunks();
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
    const float max_scale = 8.0;
    int cur_index = 0;
    int prev_index = -1;
    int cur_pal = 0;
    int prev_pal = -1;
    int frame_count = tshape->getFrameCount();
    sf::Texture *texture = new sf::Texture;
    sf::Image *image = new sf::Image;
    sf::Sprite *sprite = new sf::Sprite;

    if(!frame_count) return;

    while(!quit)
    {
        m_Screen->clear();

        sf::Event event;

        // if palette is changed, update sprites
        if( (cur_pal != prev_pal) || (cur_index != prev_index) )
        {
            if(image) delete image;
            image = tshape->toImage(cur_index, m_Palettes[cur_pal] );
            texture->loadFromImage(*image);
            if(sprite) delete sprite;
            sprite = new sf::Sprite(*texture);
            prev_pal = cur_pal;
            prev_index = cur_index;
            std::cout << "Frame:" << cur_index << ", pal:" << cur_pal << std::endl;
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
                    if(shape_scalar > max_scale) shape_scalar = max_scale;
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

        if(sprite)
        {
            sprite->setScale(shape_scalar, shape_scalar);
            sprite->setPosition(m_Screen->getSize().x/2, m_Screen->getSize().y/2);
            m_Screen->draw(*sprite);
        }


        m_Screen->display();
    }

    // cleanup
    if(sprite) delete sprite;
    if(texture) delete texture;
    if(image) delete image;
}

void U7::showTiles()
{
    bool quit = false;
    static float scalar = 1.0;
    const float max_scalar = 8.0;
    int cur_tile = 0;
    int prev_tile = -1;
    sf::Sprite *sprite = new sf::Sprite(*m_TileSprites[cur_tile]);

    if(m_TileSprites.empty()) return;

    while(!quit)
    {
        m_Screen->clear();

        sf::Event event;

        // update tile
        if(prev_tile != cur_tile)
        {
            delete sprite;
            sprite = new sf::Sprite(*m_TileSprites[cur_tile]);
            prev_tile = cur_tile;
            std::cout << "Tile #" << cur_tile << " - 0x" << std::hex << std::setw(4) << std::setfill('0') << cur_tile << std::endl << std::dec;
        }
        while(m_Screen->pollEvent(event))
        {
            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
                else if(event.key.code == sf::Keyboard::Left)
                {
                    cur_tile--;
                    if(cur_tile < 0) cur_tile = int(m_TileSprites.size()-1);
                }
                else if(event.key.code == sf::Keyboard::Right)
                {
                    cur_tile++;
                    if(cur_tile >= int(m_TileSprites.size())) cur_tile = 0;
                }
                else if(event.key.code == sf::Keyboard::Add)
                {
                    scalar += 1.0;
                    if(scalar > max_scalar) scalar = max_scalar;
                }
                else if(event.key.code == sf::Keyboard::Subtract)
                {
                    scalar -= 1.0;
                    if(scalar < 1.0) scalar = 1.0;
                }
                else if(event.key.code == sf::Keyboard::PageUp)
                {
                    cur_tile += 0x10;
                    if(cur_tile >= int(m_TileSprites.size())) cur_tile = int(m_TileSprites.size()-1);
                }
                else if(event.key.code == sf::Keyboard::PageDown)
                {
                    cur_tile -= 0x10;
                    if(cur_tile < 0) cur_tile = 0;
                }
            }
        }

        sprite->setScale(scalar, scalar);
        sprite->setPosition(m_Screen->getSize().x/2, m_Screen->getSize().y/2);
        m_Screen->draw(*sprite);

        m_Screen->display();
    }

    // cleanup
    if(sprite) delete sprite;

}

void U7::showChunks()
{
    bool quit = false;
    int cur_chunk = 0;
    int prev_chunk = -1;
    int tile_sprites = int(m_TileSprites.size());

    while(!quit)
    {
        m_Screen->clear();

        sf::Event event;

        if(cur_chunk != prev_chunk)
        {
            std::cout << "Chunk #" << cur_chunk << std::endl;
            prev_chunk = cur_chunk;
        }

        while(m_Screen->pollEvent(event))
        {
            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
                else if(event.key.code == sf::Keyboard::Left)
                {
                    cur_chunk--;
                    if(cur_chunk < 0) cur_chunk = int(m_Chunks.size()-1);
                }
                else if(event.key.code == sf::Keyboard::Right)
                {
                    cur_chunk++;
                    if(cur_chunk >= int(m_Chunks.size())) cur_chunk = 0;
                }
                else if(event.key.code == sf::Keyboard::Add)
                {
                }
                else if(event.key.code == sf::Keyboard::Subtract)
                {
                }
                else if(event.key.code == sf::Keyboard::Up)
                {
                }
                else if(event.key.code == sf::Keyboard::Down)
                {
                }
                else if(event.key.code == sf::Keyboard::Space)
                {
                    m_Chunks[cur_chunk].show();
                }
            }
        }

        // draw
        for(int i = 0; i < CHUNK_SIZE; i++)
        {
            for(int n = 0; n < CHUNK_SIZE; n++)
            {
                int tile = m_Chunks[cur_chunk].tiles[i][n];
                if(tile >= 0 && tile < tile_sprites )
                {

                    m_TileSprites[tile]->setPosition(sf::Vector2f(n*TILE_SIZE, i*TILE_SIZE));
                    m_Screen->draw( *m_TileSprites[tile]);
                }
            }
        }


        m_Screen->display();
    }

}
