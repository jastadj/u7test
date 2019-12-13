#ifndef CLASS_OBJECT
#define CLASS_OBJECT

#include <string>
#include <vector>
#include "frame.hpp"
#include "palette.hpp"
#include "shape.hpp"

enum OBJ_TYPE{
    TYPE_SCENERY,
    TYPE_UNUSED1,
    TYPE_USESQUALITY,
    TYPE_USESQUANTITY,
    TYPE_USABLE,
    TYPE_QUALITYFLAGS,
    TYPE_CONTAINER,
    TYPE_ACTIVATEONAPPROACH,
    TYPE_SPELLBOOK,
    TYPE_BARGE,
    TYPE__UNUSED2,
    TYPE_VIRTUESTONE,
    TYPE_ENEMY,
    TYPE_FRIENDLY,
    TYPE_BUILDING,
    TYPE_UNUSED3
    };

class Object
{
protected:

    std::string m_Name;
    int m_Weight; // 0.1 stone
    int m_Volume;

    std::vector<Frame*> m_Frames;

    // flags
    bool m_HasSoundEffect;  // 0x00
    bool m_Rotatable;       // 0x01
    bool m_Animated;        // 0x02
    bool m_NotWalkable;     // 0x03
    bool m_Water;           // 0x04
    int m_TilesZ;           // 0x05 (3 bits for height in tiles)
    int m_Type;             // 0x08 (4 bits for obj type)
    bool m_Trap;            // 0x0c
    bool m_Door;            // 0x0d
    bool m_VehiclePart;     // 0x0e
    bool m_NonSelectable;   // 0x0f
    int m_TilesXMinus1;     // 0x10 (3 bits for x size in tiles)
    int m_TilesYMinus1;     // 0x13 (3  bits for y size in tiles)
    bool m_LightSource;     // 0x16
    bool m_Translucent;     // 0x17

public:
    Object(std::string name, uint32_t flags);
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
    WorldObject(std::string name, std::vector<uint8_t> record, std::vector<Palette> &pal, uint32_t flags);
    ~WorldObject();

    void show();
};
#endif // CLASS_OBJECT
