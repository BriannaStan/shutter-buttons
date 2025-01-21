#define USE_USBCON

#include <Arduino.h>
#undef max // hack for redefinition of min and max by arduino lib. See https://community.platformio.org/t/project-build-fails-after-latest-updates/8731/2
#undef min

// Include custom classes
#include <shutter_button.h>
#include <light_animation.h>
#include <shutter_buttons/LightChange.h>
#include <shutter_buttons/LightChangeArray.h>

// Which pin on the Arduino is connected to the NeoPixels?
// For Metro M4, has to be one of: 3, 6, 8, 11, A3 and MOSI
// See https://github.com/adafruit/Adafruit_NeoPixel_ZeroDMA/blob/master/examples/strandtest/strandtest.ino
#define NEOPIN1 3 
#define NEOPIN2 6  // uncomment when we add more buttons...
#define NEOPIN3 8

// Number of LEDs per strand. Total number of pixels is 8X this!
#define NUM_LED 24  

// Switch button
// #define BUTTONPIN1 A0
// #define BUTTONPIN2 A1 // uncomment when we add more buttons
// #define BUTTONPIN3 A2
#define BUTTONPIN1 2
#define BUTTONPIN2 4
#define BUTTONPIN3 5

// Debounce delay
#define DEBOUNCE_DELAY 50

// Uncomment the next line to print debug info through serial
#define SERIAL_VERBOSE

// Create ROS node
ros::NodeHandle nh;                            

// Create button instance(s)
ShutterButton button1(BUTTONPIN1, NEOPIN1, NUM_LED, DEBOUNCE_DELAY, "button1/state");
ShutterButton button2(BUTTONPIN2, NEOPIN2, NUM_LED, DEBOUNCE_DELAY, "button2/state");
ShutterButton button3(BUTTONPIN3, NEOPIN3, NUM_LED, DEBOUNCE_DELAY, "button3/state");

// Create callback for button animation change
void lightChangeCb(const shutter_buttons::LightChangeArrayRequest& req,
                         shutter_buttons::LightChangeArrayResponse& res)
{
  // figure out which button we are changing and process the request
  /* if (req.button_id == 0) {
    button1.lightChangeCb(req, res);
    button2.lightChangeCb(req, res);
    button3.lightChangeCb(req, res);
  }
  else if (req.button_id == 1) {
    button1.lightChangeCb(req, res);
  } 
  else if (req.button_id == 2) { // uncomment when we add more buttons
    button2.lightChangeCb(req, res);
  } 
  else if (req.button_id == 3) {
    button3.lightChangeCb(req, res);
  } 
  else {
    res.result = false;
  } */
  bool legal_index = true;
  for (int i = 0; i < 3; i++) {
    if (req.light_changes[i].button_id == 1) {
      button1.lightChangeCb(&req.light_changes[i]);
    }
    else if (req.light_changes[i].button_id == 2) {
      button2.lightChangeCb(&req.light_changes[i]);
    }
    else if (req.light_changes[i].button_id == 3) {
      button3.lightChangeCb(&req.light_changes[i]);
    }
    else {
      legal_index = false;
    }
    res.result = legal_index;
  }
}

// Create service for changing the way that a button works
ros::ServiceServer<shutter_buttons::LightChangeArrayRequest, 
                   shutter_buttons::LightChangeArrayResponse> light_service("light_change", &lightChangeCb);

void setup() {
  // set button input
  pinMode(LED_BUILTIN, OUTPUT);

  button1.setup();
  button2.setup();
  button3.setup();

  nh.getHardware()->setBaud(57600); // this must match the baud used on the ROS side
  nh.initNode();
  nh.advertise(*(button1.getPublisher()));
  nh.advertise(*(button2.getPublisher()));
  nh.advertise(*(button3.getPublisher()));
  nh.advertiseService<shutter_buttons::LightChangeArrayRequest, 
                      shutter_buttons::LightChangeArrayResponse>(light_service);

#ifdef SERIAL_VERBOSE
    Serial.begin(9600);
#endif

}

void loop() {

  button1.update();
  button2.update();
  button3.update();

  bool button1_pressed = button1.isButtonPressed();
  bool button2_pressed = button2.isButtonPressed();
  bool button3_pressed = button3.isButtonPressed();
  bool button_pressed = button1_pressed || button2_pressed || button3_pressed;

  if (button_pressed) { // button is pressed
    digitalWrite(LED_BUILTIN, HIGH);
  } 
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  button1.publishButtonState();
  button2.publishButtonState();
  button3.publishButtonState();
  nh.spinOnce();


/* #ifdef SERIAL_VERBOSE
  Serial.print("Button1 state:");
  Serial.println(button1.isButtonPressed());
#endif */
  
  delay(10); //!< changing this delay might result in "wrong checksum for topic id and msg" errors on the ROS side
}
