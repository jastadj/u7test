#ifndef CLASS_FONT
#define CLASS_FONT

#include <vector>

#include "shape.hpp"
#include "palette.hpp"

class Font
{
private:

    Shape *m_Shape;

public:
    Font(Shape *fshape);
    ~Font();
};
#endif // CLASS_FONT
