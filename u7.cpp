#include "u7.hpp"

#include <iostream>
#include <sstream>

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
    int tile_count = 0;
    for(int i = 0; i <= 0x95; i++)
    {
        std::vector<uint8_t> trecord = shapes.getRecord(i);
        m_TileSets.push_back( new TileSet(trecord, &m_Palettes[0]));
        tile_count += m_TileSets.back()->getTileCount();
    }
    std::cout << tile_count << " tiles loaded.\n";

    // world objects
    std::cout << "Loading shapes...\n";
    for(int i = 0x96; i < shapes.getRecordCount(); i++)
    {
        std::vector<uint8_t> trecord = shapes.getRecord(i);
        std::cout << "object " << m_Objects.size() + 1 << " size = " << trecord.size() << std::endl;
        if(trecord.empty()) continue;
        m_Objects.push_back( new Shape(trecord));
        std::cout << "objects loaded:" << m_Objects.size() << std::endl;
    }

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

    // load faces
    std::cout << "Loading faces...\n";
    FLXFile faces;
    if(!faces.open(std::string(U7_DIR) + "/STATIC/FACES.VGA"))
    {
        std::cout << "Error loading FACES.VGA.\n";
        exit(5);
    }
    else
    {
        for(int i = 0; i < faces.getRecordCount(); i++)
        {
            std::vector<uint8_t> record = faces.getRecord(i);
            if(!record.empty())
            {
                Shape *newshape = new Shape(record);
                m_Faces.push_back(newshape);
            }
        }
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
                else if(event.key.code == sf::Keyboard::F5) showShapes(m_Faces);
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

void U7::showShapes(std::vector<Shape*> shapes)
{
    if(shapes.empty()) return;
    bool quit = false;
    static float scalar = 1.0;
    const float max_scale = 8.0;
    int cur_shape = 0;
    int prev_shape = -1;
    int cur_frame = 0;
    int prev_frame = -1;
    int cur_pal = 0;
    int prev_pal = -1;

    sf::Texture *texture = new sf::Texture;
    sf::Sprite *sprite = new sf::Sprite;

    while(!quit)
    {
        m_Screen->clear();

        sf::Event event;

        // shape changed
        if(prev_shape != cur_shape)
        {
            cur_frame = 0;
            prev_frame = -1;
            prev_shape = cur_shape;
        }

        // frame or palette change - generate new sprite
        if(prev_frame != cur_frame || prev_pal != cur_pal)
        {
            if(sprite) delete sprite;
            sf::Image *img = shapes[cur_shape]->toImage(cur_frame, m_Palettes[cur_pal]);
            texture->loadFromImage(*img);
            sprite = new sf::Sprite(*texture);
            delete img;
            prev_frame = cur_frame;
            prev_pal = cur_pal;
            std::cout << "shape (in list):" << cur_shape << " frame: 0x" << std::hex << std::setw(2) << std::setfill('0') << cur_frame;
            std::cout << std::dec << " pal:" << cur_pal << std::endl;
        }

        while(m_Screen->pollEvent(event))
        {
            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
                else if(event.key.code == sf::Keyboard::Left)
                {
                    cur_frame--;
                    if(cur_frame < 0) cur_frame = 0;
                }
                else if(event.key.code == sf::Keyboard::Right)
                {
                    cur_frame++;
                    if(cur_frame >= shapes[cur_shape]->getFrameCount()) cur_frame = shapes[cur_shape]->getFrameCount()-1;
                }
                else if(event.key.code == sf::Keyboard::Add)
                {
                    scalar += 1.0;
                    if(scalar > max_scale) scalar = max_scale;
                }
                else if(event.key.code == sf::Keyboard::Subtract)
                {
                    scalar -= 1.0;
                    if(scalar < 1.0) scalar = 1.0;
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
                else if(event.key.code == sf::Keyboard::PageUp)
                {
                    cur_shape++;
                    if(cur_shape >= int(shapes.size())) cur_shape = int(shapes.size()-1);
                }
                else if(event.key.code == sf::Keyboard::PageDown)
                {
                    cur_shape--;
                    if(cur_shape < 0) cur_shape = 0;
                }
                else if(event.key.code == sf::Keyboard::Space)
                {
                    shapes[cur_shape]->showFrame(cur_frame);
                }
            }
        }

        if(sprite)
        {
            sprite->setScale(scalar, scalar);
            sprite->setPosition(m_Screen->getSize().x/2, m_Screen->getSize().y/2);
            m_Screen->draw(*sprite);
        }


        m_Screen->display();
    }

    // cleanup
    if(sprite) delete sprite;
    if(texture) delete texture;
}

void U7::showTiles()
{
    bool quit = false;
    static float scalar = 1.0;
    const float max_scalar = 8.0;
    int cur_tile = 0;
    int prev_tile = -1;
    int cur_set = 0;
    int prev_set = -1;
    sf::Sprite *sprite = NULL;

    while(!quit)
    {
        m_Screen->clear();

        sf::Event event;

        // update tile set
        if(prev_set != cur_set)
        {
            cur_tile = 0;
            prev_tile = -1;
            prev_set = cur_set;
        }

        // update tile
        if(prev_tile != cur_tile)
        {
            if(sprite) delete sprite;
            sprite = m_TileSets[cur_set]->getSprite(cur_tile);
            prev_tile = cur_tile;
            std::cout << "Tile set: 0x" << std::hex << std::setw(9) << std::setfill('0') << cur_set;
            std::cout << " - tile: 0x" << std::setw(2) << std::setfill('0') << cur_tile << std::dec << std::endl;
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
                    if(cur_tile < 0) cur_tile = 0;
                }
                else if(event.key.code == sf::Keyboard::Right)
                {
                    cur_tile++;
                    if(cur_tile >= int(m_TileSets[cur_set]->getTileCount())) cur_tile = int(m_TileSets[cur_set]->getTileCount()-1);
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
                    cur_set++;
                    if(cur_set >= int(m_TileSets.size())) cur_set = int(m_TileSets.size()-1);
                }
                else if(event.key.code == sf::Keyboard::PageDown)
                {
                    cur_set--;
                    if(cur_set < 0) cur_set = 0;
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
    float scalar = 1.0;
    float max_scalar = 8.0;

    sf::Font testfont;
    testfont.loadFromFile("font.ttf");
    sf::Text teststring("this is a test", testfont, 12);

    // create render texture for drawing tiles to
    sf::RenderTexture *rtxt = new sf::RenderTexture;
    rtxt->create( TILE_SIZE*CHUNK_SIZE, TILE_SIZE*CHUNK_SIZE);
    rtxt->clear();
    rtxt->display();

    sf::Sprite *chunksprite = new sf::Sprite(rtxt->getTexture());

    while(!quit)
    {
        m_Screen->clear();

        sf::Event event;

        // if current chunk has changed, update the rendertexture/sprite
        if(cur_chunk != prev_chunk)
        {
            rtxt->clear();
            for(int y = 0; y < CHUNK_SIZE; y++)
            {
                for(int x = 0; x < CHUNK_SIZE; x++)
                {
                    int data = m_Chunks[cur_chunk].tiles[y][x];
                    int shape_id = 0x3ff & data;
                    int frame = (data & 0x7c00) >> 10;

                    // if this shape id is a tile
                    if(shape_id <= 0x95)
                    {
                        sf::Sprite *tsprite = m_TileSets[shape_id]->getSprite(frame);
                        if(tsprite)
                        {
                            tsprite->setPosition( sf::Vector2f(x*TILE_SIZE, y*TILE_SIZE) );
                            rtxt->draw(*tsprite);
                            delete tsprite;
                        }
                    }
                }
            }
            // update render texture and create new sprite from it
            rtxt->display();
            delete chunksprite;
            chunksprite = new sf::Sprite(rtxt->getTexture());
            std::cout << "Chunk #" << cur_chunk << " - 0x" << std::hex << std::setw(4) << std::setfill('0') << cur_chunk << std::dec << std::endl;
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
                    scalar += 1.0;
                    if(scalar > max_scalar) scalar = max_scalar;
                }
                else if(event.key.code == sf::Keyboard::Subtract)
                {
                    scalar -= 1.0;
                    if(scalar < 0.0) scalar = 1.0;
                }
                else if(event.key.code == sf::Keyboard::Space)
                {
                    m_Chunks[cur_chunk].show();
                }
                else if(event.key.code == sf::Keyboard::PageUp)
                {
                    cur_chunk += 100;
                    if(cur_chunk >= int(m_Chunks.size())) cur_chunk = 0;
                }
                else if(event.key.code == sf::Keyboard::PageDown)
                {
                    cur_chunk -= 100;
                    if(cur_chunk < 0) cur_chunk = int(m_Chunks.size()-1);
                }
            }
        }

        // update/draw chunk sprite
        if(chunksprite)
        {
            //chunksprite->setOrigin(sf::Vector2f(TILE_SIZE*CHUNK_SIZE/2, TILE_SIZE*CHUNK_SIZE/2));
            //chunksprite->setPosition(sf::Vector2f(m_Screen->getSize().x/2, m_Screen->getSize().y/2));
            m_Screen->draw(*chunksprite);
        }

        // update/draw debug info
        sf::Vector2i mouse_pos(sf::Mouse::getPosition(*m_Screen));
        sf::Vector2i tile_pos(mouse_pos.x/TILE_SIZE, mouse_pos.y/TILE_SIZE);
        if(tile_pos.x >= 0 && tile_pos.x < CHUNK_SIZE && tile_pos.y >= 0 && tile_pos.y < CHUNK_SIZE)
        {
            int chunkdata = m_Chunks[cur_chunk].tiles[tile_pos.y][tile_pos.x];
            int spriteid = chunkdata & 0x3ff;
            int frame = (chunkdata & 0x7c00) >> 10;

            std::stringstream ss;
            ss << tile_pos.x << "," << tile_pos.y << "=0x" << std::hex << std::setw(4) << std::setfill('0') << chunkdata;
            ss << "  -  spriteid: 0x" << spriteid;
            ss << std::dec << "(" << spriteid << ")";
            ss << ",frame: 0x" << std::hex << std::setw(2) << std::setfill('0') << frame;
            ss << std::dec << "(" << frame << ")";
            teststring.setString(ss.str());
        }
        else teststring.setString("");
        teststring.setPosition(5, m_Screen->getSize().y - 20);

        m_Screen->draw(teststring);

        m_Screen->display();
    }

}
