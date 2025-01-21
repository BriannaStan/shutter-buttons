#include <memory>

#include <light_color.h>

// Note that this code also requires LATEST Adafruit_NeoPixel and Adafruit_ZeroDMA libraries.
#include <Adafruit_NeoPixel_ZeroDMA.h>

#ifndef LIGHTANIMATION_H
#define LIGHTANIMATION_H

/** Abstract parent class for LED animation **/
class LightAnimation 
{
    public:

    std::shared_ptr<Adafruit_NeoPixel_ZeroDMA> strip;   //!< pointer to led strip

    /** Constructor
     * \param s: shared pointer to LED strip
     **/
    LightAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s);

    /** Main method that must be customized by all animations to update led coloring **/
    virtual void update() = 0;

    typedef enum AnimType {
        OFFLIGHTANIMATION=0,
        SINGLECOLORANIMATION=1,
        COLORWIPEANIMATION=2,
        COLORWHEELANIMATION=3
    } AnimType;
};

/** Button off animation **/
class OffLightAnimation : public LightAnimation
{
    public:

    /** Constructor
     * \param s: shared pointer to LED strip
     **/
    OffLightAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s);

    /** Turns LEDs off **/
    void update();
};

/** Light up all the button with a single color */
class SingleColorAnimation : public LightAnimation
{
    protected:
    uint32_t color;     //!< 32-bit type color
    uint8_t intensity;  //!< led intensity

    public:

    /** Constructor
     * \param s: shared pointer to LED strip
     * \param r: red value (between 0 and 255)
     * \param g: green value (between 0 and 255)
     * \param b: blue value (between 0 and 255)
     * \param w: white value (between 0 and 255)
     * \param i: intensity (between 0 and 255)
     **/
    SingleColorAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s, uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t i);

    /** Constructor
     * \param s: shared pointer to LED strip
     * \param c: packged r,g,b,w color values
     * \param i: intensity (between 0 and 255)
     **/
    SingleColorAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s, LightColor c, uint8_t i);

    /** Turns LEDs on at a given color and intensity **/
    void update();

};

/** Color wipe with one color wiping around another color **/
class ColorWipeAnimation : public LightAnimation
{
    protected:
    uint32_t foreground_color;     //!< 32-bit type color
    uint32_t background_color;     //!< 32-bit type color
    uint8_t intensity;             //!< led intensity
    uint16_t wipe_position;        //!< wipe position
    bool flip_color;               //!< flip color variable to swap between foreground and background once a full circle is rendered

    public:

    /** Constructor
     * \param s: shared pointer to LED strip
     * \param fr: foreground red value (between 0 and 255)
     * \param fg: foreground green value (between 0 and 255)
     * \param fb: foreground blue value (between 0 and 255)
     * \param fw: foreground white value (between 0 and 255)
     * \param br: background red value (between 0 and 255)
     * \param bg: background green value (between 0 and 255)
     * \param bb: background blue value (between 0 and 255)
     * \param bw: background white value (between 0 and 255)
     * \param i: intensity (between 0 and 255)
     **/
    ColorWipeAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s, 
                       uint8_t fr, uint8_t fg, uint8_t fb, uint8_t fw, 
                       uint8_t br, uint8_t bg, uint8_t bb, uint8_t bw, 
                       uint8_t i);

     /** Constructor
     * \param s: shared pointer to LED strip
     * \param fc: Packaged r,g,b,w color values for the foreground color
     * \param fc: Packaged r,g,b,w color values for the background color
     * \param i: intensity (between 0 and 255)
     **/
    ColorWipeAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s, 
                       LightColor fc, LightColor bc, uint8_t i);

    /** Turns LEDs on at a given color and intensity **/
    void update();
};

/** Color wheel **/
class ColorWheelAnimation : public LightAnimation
{
    protected:
    uint8_t intensity;              //!< led intensity
    uint32_t start_time;            //!< elapsed time since the wheel started spinning

    public:

    /** Constructor
     * \param s: shared pointer to LED strip
     * \param i: intensity (between 0 and 255)
     **/
    ColorWheelAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s, uint8_t i);

    /** Turns LEDs to render color wheel
     * \note The logic is inspired by https://github.com/adafruit/Adafruit_NeoPixel_ZeroDMA/blob/master/examples/strandtest/strandtest.ino  
     **/
    void update();

    /** Helper function to render color wheel **/
    uint32_t colorWheel(byte wheel_pos);
};

#endif