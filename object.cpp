#include "object.hpp"

#include <iostream> // debug

Object::Object(std::string tname)
{
    m_Name = tname;
}

Object::~Object()
{
    for(int i = 0; i < int(m_Frames.size()); i++) delete m_Frames[i];
}


sf::Sprite *Object::newFrameSprite(int frameindex, bool apply_frame_offsets)
{
    if(frameindex < 0 || frameindex >= int(m_Frames.size())) return NULL;

    sf::Sprite *newsprite = new sf::Sprite(m_Frames[frameindex]->texture);
    newsprite->setOrigin( sf::Vector2f(m_Frames[frameindex]->offset));
}

sf::Vector2f Object::getFrameOffset(int frameindex)
{
    if(frameindex < 0 || frameindex >= int(m_Frames.size())) return sf::Vector2f();

    return m_Frames[frameindex]->offset;
}

//////////////////////////////////////////////////////////////////////////////////
//  WORLD OBJECT
WorldObject::WorldObject(std::string name):Object(name)
{

}

WorldObject::~WorldObject()
{

}

void WorldObject::show()
{
    std::cout << "Object: " << m_Name << ", frames:" << getFrameCount() << std::endl;
}
