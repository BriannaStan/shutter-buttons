#include <Adafruit_NeoPixel_ZeroDMA.h>
#undef max // hack for redefinition of min and max by arduino lib. See https://community.platformio.org/t/project-build-fails-after-latest-updates/8731/2
#undef min

#include <cstring>

#include <shutter_button.h>

// Constructor
ShutterButton::ShutterButton(int bpin, int npin, int nled, int delay, const char * pub_name) 
{
  button_pressed = false;
  last_debounce_time = 0;
  debounce_delay = delay;
  button_state = HIGH;
  button_pin = bpin;
  neo_pin = npin;
  num_led = nled;
  strip = std::shared_ptr<Adafruit_NeoPixel_ZeroDMA>(
    new Adafruit_NeoPixel_ZeroDMA(num_led, neo_pin, NEO_GRBW + NEO_KHZ800));

  // ROS setup
  button_publisher = std::shared_ptr<ros::Publisher>(new ros::Publisher(pub_name, &button_pressed_msg));
}

// Setup
void
ShutterButton::setup(uint8_t strip_brightness)
{
  // set pin modes
  pinMode(button_pin, INPUT_PULLUP);

  // setup LEDs
  strip->begin();
  strip->setBrightness(strip_brightness);
  strip->show();

  // default button behavior
  pressed_anim = std::shared_ptr<LightAnimation>(new SingleColorAnimation(strip, LIGHT_COLOR_BLUE, 5));
  unpressed_anim = std::shared_ptr<LightAnimation>(new SingleColorAnimation(strip, LIGHT_COLOR_BLUE, 100));
}

// Read button state taking care of potential debounce 
void 
ShutterButton::read_button_state()
{
    // read button
    int new_button_state = digitalRead(button_pin);

    // update debounce time if the switch changed
    if (new_button_state != button_state) {
        last_debounce_time = millis();
    } 

    // update button variable only after debounce delay
    if ((millis() - last_debounce_time) > debounce_delay)
    {
      button_pressed = (new_button_state == LOW);
    }

    // save reading for next update
    button_state = new_button_state;
}

// Main function that checks the button state and updates coloring
void 
ShutterButton::update()
{
  read_button_state();

  if (button_pressed) {
    if (pressed_anim) {
      pressed_anim->update();
    }
  } else {
    if (unpressed_anim){
      unpressed_anim->update();
    }
  }
}

// Change animation function
bool 
ShutterButton::change_animation( const shutter_buttons::LightChange& req, std::shared_ptr<LightAnimation>& animation)
{
    if (req.anim_type == LightAnimation::AnimType::OFFLIGHTANIMATION) {
      animation = std::shared_ptr<LightAnimation>(new OffLightAnimation(strip));

    } else if (req.anim_type == LightAnimation::AnimType::SINGLECOLORANIMATION) {
      LightColor color(req.color_r, req.color_g, req.color_b, req.color_w);
      animation = std::shared_ptr<LightAnimation>(new SingleColorAnimation(strip, color, req.intensity));

    } else if (req.anim_type == LightAnimation::AnimType::COLORWHEELANIMATION) {
      animation = std::shared_ptr<LightAnimation>(new ColorWheelAnimation(strip, req.intensity));

    } else if (req.anim_type == LightAnimation::AnimType::COLORWIPEANIMATION) {
      LightColor fcolor(req.color_r, req.color_g, req.color_b, req.color_w);
      LightColor bcolor(req.bcolor_r, req.bcolor_g, req.bcolor_b, req.bcolor_w);
      animation = std::shared_ptr<LightAnimation>(new ColorWipeAnimation(strip, fcolor, bcolor, req.intensity));

    } else {
      return false;
      
    }
    return true;
}

// Change light animation callback function
void 
ShutterButton::lightChangeCb(shutter_buttons::LightChange const* req)
{
  // process the request based on the type of animation we want to change
  const shutter_buttons::LightChange& req1 = *req;
  if (req1.pressed) {
    change_animation(req1, pressed_anim);
  } else {
    change_animation(req1, unpressed_anim);
  }
}
