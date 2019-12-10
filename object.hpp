#ifndef CLASS_OBJECT
#define CLASS_OBJECT

#include <string>
#include <vector>
#include "frame.hpp"

class Object
{
protected:

    std::string m_Name;
    int m_Weight; // 0.1 stone
    int m_Volume;

    std::vector<Frame*> m_Frames;

    // flags

public:
    Object(std::string name);
    virtual ~Object();

    std::string getName() { return m_Name;}
    int getWeight() { return m_Weight;}
    int getVolume() { return m_Volume;}
    int getFrameCount() { return int(m_Frames.size());}

    virtual sf::Sprite *newFrameSprite(int frameindex, bool apply_frame_offsets=false);
    sf::Vector2f getFrameOffset(int frameindex);

    virtual void show()=0;
};

class WorldObject:public Object
{
protected:

public:
    WorldObject(std::string name);
    ~WorldObject();

    void show();
};
#endif // CLASS_OBJECT
