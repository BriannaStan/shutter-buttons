#include <light_animation.h>

// Parent class constructor
LightAnimation::LightAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s)
{
    strip = s;
}

// Constructor
OffLightAnimation::OffLightAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s) :
    LightAnimation(s)
{}

// Turns LEDs off
void
OffLightAnimation::update()
{
    if (strip->getBrightness() > 0) {
        strip->setBrightness(0);
        strip->show();
    }
}

// Constructor
SingleColorAnimation::SingleColorAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s, uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t i) :
    LightAnimation(s)
{
    color = strip->Color(r,g,b,w);
    intensity = i;
}

// Constructor
SingleColorAnimation::SingleColorAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s, LightColor c, uint8_t i) :
    LightAnimation(s)
{
    color = strip->Color(c.r, c.g, c.b, c.w);
    intensity = i;
}

/** Turns LEDs on at a given color and intensity **/
void SingleColorAnimation::update()
{
    if (strip->getBrightness() != intensity) {
        strip->setBrightness(intensity);
    }
    for(uint16_t i=0; i<strip->numPixels(); i++) {
      strip->setPixelColor(i, color);
    }
    strip->show();
}

// Constructor
ColorWipeAnimation::ColorWipeAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s, 
                                        uint8_t fr, uint8_t fg, uint8_t fb, uint8_t fw, 
                                        uint8_t br, uint8_t bg, uint8_t bb, uint8_t bw, 
                                        uint8_t i) :
    LightAnimation(s)
{
    background_color = strip->Color(br,bg,bb,bw);
    foreground_color = strip->Color(fr,fg,fb,fw);
    intensity = i;
    wipe_position = 0;
    flip_color = false;
}

// Constructor
ColorWipeAnimation::ColorWipeAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s, 
                                        LightColor fc, LightColor bc, uint8_t i) :
    LightAnimation(s)
{
    background_color = strip->Color(bc.r,bc.g,bc.b,bc.w);
    foreground_color = strip->Color(fc.r,fc.g,fc.b,fc.w);
    intensity = i;
    wipe_position = 0;
    flip_color = false;
}

/** Shows wiping effect **/
void 
ColorWipeAnimation::update()
{
    if (strip->getBrightness() != intensity) {
        strip->setBrightness(intensity);
    }

    // draw foreground
    for (uint16_t i=0; i < wipe_position; i++)
    {
        strip->setPixelColor(i, flip_color ? background_color : foreground_color );
    }

    // draw background
    for (uint16_t i=wipe_position; i < strip->numPixels(); i++)
    {
        strip->setPixelColor(i, flip_color ? foreground_color : background_color);
    }
    strip->show();

    wipe_position++;
    wipe_position = wipe_position % strip->numPixels();
    if (wipe_position == 0)
        flip_color = !flip_color;
}

// Constructor
ColorWheelAnimation::ColorWheelAnimation(std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>& s, uint8_t i) :
    LightAnimation(s)
{
    intensity = i;
    start_time = micros();
}

/** Turns LEDs to render color wheel **/
void 
ColorWheelAnimation::update()
{
    if (strip->getBrightness() != intensity) {
        strip->setBrightness(intensity);
    }

    // rainbow cycle
    uint32_t elapsed = micros() - start_time;
    for(uint16_t i=0; i<strip->numPixels(); i++) {
        strip->setPixelColor(i, colorWheel((uint8_t)(elapsed * 256 / 1000000) + i * 256 / strip->numPixels()));
    }
    strip->show();
}

/** Helper function to render wheel inside loop.
 * \param wheel_pos: Input a value 0 to 255 to get a color value. The colours are a transition r - g - b - back to r.
 * \return returns output of strip.Color() call
**/
uint32_t 
ColorWheelAnimation::colorWheel(byte wheel_pos) 
{
  wheel_pos = 255 - wheel_pos;
  if(wheel_pos < 85) {
    return strip->Color(255 - wheel_pos * 3, 0, wheel_pos * 3);
  }
  if(wheel_pos < 170) {
    wheel_pos -= 85;
    return strip->Color(0, wheel_pos * 3, 255 - wheel_pos * 3);
  }
  wheel_pos -= 170;
  return strip->Color(wheel_pos * 3, 255 - wheel_pos * 3, 0);
}