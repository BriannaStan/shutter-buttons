#include <stdint.h>


#ifndef LIGHTCOLOR_H
#define LIGHTCOLOR_H

/** Encapsulation for light colors. Types are compatible with Adafruit NeoPixel library **/
typedef struct LightColor {
    uint8_t r; //!< red value
    uint8_t g; //!< green vallue 
    uint8_t b; //!< blue value 
    uint8_t w; //!< white value

    /** Default constructor. Sets all values to zero **/
    LightColor();

    /** Constructor
     * \param ir: red value
     * \param ig: green value
     * \param ib: blue value
     * \param iw: white value
     **/
    LightColor(uint8_t ir, uint8_t ig, uint8_t ib, uint8_t iw);

} LightColor;


// Predefined colors
#define LIGHT_COLOR_RED LightColor(255, 0, 0, 0)
#define LIGHT_COLOR_BLUE LightColor(0, 0, 255, 0)
#define LIGHT_COLOR_GREEN LightColor(0, 255, 0, 0)
#define LIGHT_COLOR_YELLOW LightColor(241, 196, 5, 0)
#define LIGHT_COLOR_ORANGE LightColor(240, 84, 0, 0)
#define LIGHT_COLOR_PURPLE LightColor(81, 46, 95, 0)
#define LIGHT_COLOR_GREY LightColor(120, 120, 120, 0)
#define LIGHT_COLOR_WHITE LightColor(120, 120, 120, 0)
#define LIGHT_COLOR_PINK LightColor(255, 0, 0, 50)
#define LIGHT_COLOR_CREAM LightColor(255, 180, 50, 0)
#define LIGHT_COLOR_CYAN LightColor(118, 240, 180, 0)
#define LIGHT_COLOR_LIGHTPURPLE LightColor(142, 68, 173, 30)



#endif