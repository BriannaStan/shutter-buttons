#include <light_color.h>

// Default constructor
LightColor::LightColor()
{
    r = 0;
    g = 0;
    b = 0;
    w = 0;
}

// Default constructor
LightColor::LightColor(uint8_t ir, uint8_t ig, uint8_t ib, uint8_t iw)
{
    r = ir;
    g = ig;
    b = ib;
    w = iw;
}