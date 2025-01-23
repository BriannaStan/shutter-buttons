#include <ros.h>
#include <std_msgs/Bool.h>
#include <shutter_buttons_msgs/LightChange.h>
// #include <shutter_buttons/LightAnimationCmd.h>

#include <memory>

// Note that this code also requires LATEST Adafruit_NeoPixel and Adafruit_ZeroDMA libraries.
#include <Adafruit_NeoPixel_ZeroDMA.h>

#include <light_animation.h>

#ifndef SHUTTERBUTTON_H
#define SHUTTERBUTTON_H

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

    // ROS vars
    std_msgs::Bool button_pressed_msg;
    std::shared_ptr<ros::Publisher> button_publisher;

public:

    /** Constructor
     * \param bpin: pin number for button switch
     * \param npin: pin number for neopixel
     * \param nled: number of leds in round neopixel inside button
     * \param delay: debounce delay for the button (in milliseconds)
     * \param pub_name: name of the topic through which the button state will be published
     * \param sub_name: name of the topic through which animation commands will be received
     **/
    ShutterButton(int bpin, int npin, int nled, int delay = 50, 
                  const char * pub_name = "/button/state"); 

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

    /** Get publisher 
     **/
    inline std::shared_ptr<ros::Publisher>& getPublisher()
    {
        return button_publisher;
    }

    /** Publish current button state
     **/
    inline void publishButtonState()
    {
        button_pressed_msg.data = button_pressed;
        button_publisher->publish( &button_pressed_msg );
    }

    /** Change animation function
     * \param req: LightChange request
     * \param return: true if the change was successful; false otherwise
     **/
    bool change_animation( const shutter_buttons_msgs::LightChange& req, std::shared_ptr<LightAnimation>& animation);

    void lightChangeCb(shutter_buttons_msgs::LightChange const* req);

};


#endif
