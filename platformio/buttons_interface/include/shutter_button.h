#ifndef SHUTTERBUTTON_H
#define SHUTTERBUTTON_H

#include <memory>

// Note that this code also requires LATEST Adafruit_NeoPixel and Adafruit_ZeroDMA libraries.
#include <Adafruit_NeoPixel_ZeroDMA.h>

#include <light_animation.h>

// Forward declaration for micro-ROS message type
struct msgs__msg__LightChange;

class ShutterButton {

protected:

    bool button_pressed;                                //!< indicator variable for the button being pressed
    int button_state;                                   //!< current button state
    unsigned long last_debounce_time;                   //< the last time the button pin was toggled
    unsigned long debounce_delay;                       //< the debounce time; increase if the output flickers
    int button_pin;                                     //!< pin number for the button switch
    int neo_pin;                                        //!< pin number for the neopixel inside the button
    int num_led;                                        //!< number of leds inside the button
    std::shared_ptr<Adafruit_NeoPixel_ZeroDMA> strip;   //!< pointer to led strip
    std::shared_ptr<LightAnimation> unpressed_anim;     //!< unpressed anim
    std::shared_ptr<LightAnimation> pressed_anim;       //!< pressed anim

public:

    /** Constructor
     * \param bpin: pin number for button switch
     * \param npin: pin number for neopixel
     * \param nled: number of leds in round neopixel inside button
     * \param delay: debounce delay for the button (in milliseconds)
     **/
    ShutterButton(int bpin, int npin, int nled, int delay = 50);

    /** Setup code for any button related things
     * \param strip_brightness: initial strip brightness
     **/
    void setup(uint8_t strip_brightness = 100);

    /** Read button state taking care of potential debounce
     **/
    void read_button_state();

    /** Main function that checks the button state and updates coloring
     **/
    void update();

    /** Get button pressed state
     **/
    inline bool isButtonPressed()
    {
        return button_pressed;
    }

    /** Change animation function (internal helper)
     * \param lc: LightChange message pointer
     * \param animation: animation pointer to update
     * \return: true if the change was successful; false otherwise
     **/
    bool change_animation(const msgs__msg__LightChange* lc,
                          std::shared_ptr<LightAnimation>& animation);

    /** Light change callback - called from service handler
     * \param lc: LightChange message pointer
     **/
    void lightChangeCb(const msgs__msg__LightChange* lc);

};

#endif
