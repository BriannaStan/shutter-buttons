#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/bool.h>
#include <msgs/msg/light_change.h>
#include <msgs/srv/light_change_array.h>

// custom classes
#include <shutter_button.h>
#include <light_animation.h>

// Which pin on the Arduino is connected to the NeoPixels?
// For Metro M4, has to be one of: 3, 6, 8, 11, A3 and MOSI
// See https://github.com/adafruit/Adafruit_NeoPixel_ZeroDMA/blob/master/examples/strandtest/strandtest.ino
#define NEOPIN1 3
#define NEOPIN2 6
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

// micro-ROS entities
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;

// Button publishers
rcl_publisher_t button1_pub;
rcl_publisher_t button2_pub;
rcl_publisher_t button3_pub;

// Service
rcl_service_t light_service;

// Executor
rclc_executor_t executor;

// Messages
std_msgs__msg__Bool button_msg;
msgs__srv__LightChangeArray_Request light_req;
msgs__srv__LightChangeArray_Response light_res;

// Button instances
ShutterButton button1(BUTTONPIN1, NEOPIN1, NUM_LED, DEBOUNCE_DELAY);
ShutterButton button2(BUTTONPIN2, NEOPIN2, NUM_LED, DEBOUNCE_DELAY);
ShutterButton button3(BUTTONPIN3, NEOPIN3, NUM_LED, DEBOUNCE_DELAY);

// Error handling macro
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; (void)temp_rc; }

void error_loop() {
    while(1) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(100);
    }
}

// Service callback
void lightChangeCb(const void* req, void* res) {
    const msgs__srv__LightChangeArray_Request* request =
        (const msgs__srv__LightChangeArray_Request*)req;
    msgs__srv__LightChangeArray_Response* response =
        (msgs__srv__LightChangeArray_Response*)res;

    bool legal_index = true;
    for (size_t i = 0; i < 3; i++) {
        const msgs__msg__LightChange* lc = &request->light_changes[i];

        if (lc->button_id == 1) {
            button1.lightChangeCb(lc);
        } else if (lc->button_id == 2) {
            button2.lightChangeCb(lc);
        } else if (lc->button_id == 3) {
            button3.lightChangeCb(lc);
        } else {
            legal_index = false;
        }
    }
    response->result = legal_index;
}

void setup() {
    // initialize serial for micro-ROS
    Serial.begin(115200);
    set_microros_serial_transports(Serial);

    // setup LED indicator
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // setup buttons HW
    button1.setup();
    button2.setup();
    button3.setup();

    delay(2000);  // wait for micro-ROS agent

    // initialize micro-ROS
    allocator = rcl_get_default_allocator();
    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

    // create node
    RCCHECK(rclc_node_init_default(&node, "buttons_interface", "", &support));

    // create publishers
    RCCHECK(rclc_publisher_init_default(
        &button1_pub, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "button1/state"
    ));
    RCCHECK(rclc_publisher_init_default(
        &button2_pub, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "button2/state"
    ));
    RCCHECK(rclc_publisher_init_default(
        &button3_pub, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "button3/state"
    ));

    // create service
    RCCHECK(rclc_service_init_default(
        &light_service, &node,
        ROSIDL_GET_SRV_TYPE_SUPPORT(msgs, srv, LightChangeArray),
        "light_change"
    ));

    // create executor with handle service
    RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
    RCCHECK(rclc_executor_add_service(
        &executor, &light_service,
        &light_req, &light_res,
        lightChangeCb
    ));

    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    // update button hardware state and animations
    button1.update();
    button2.update();
    button3.update();

    // LED feedback for any button press
    bool button_pressed = button1.isButtonPressed() ||
                          button2.isButtonPressed() ||
                          button3.isButtonPressed();
    digitalWrite(LED_BUILTIN, button_pressed ? HIGH : LOW);

    // publish button states for each button
    button_msg.data = button1.isButtonPressed();
    RCSOFTCHECK(rcl_publish(&button1_pub, &button_msg, NULL));

    button_msg.data = button2.isButtonPressed();
    RCSOFTCHECK(rcl_publish(&button2_pub, &button_msg, NULL));

    button_msg.data = button3.isButtonPressed();
    RCSOFTCHECK(rcl_publish(&button3_pub, &button_msg, NULL));

    // spin executor (handles service callbacks)
    RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10)));

    // might not be needed anymore
    delay(10); //!< changing this delay might result in "wrong checksum for topic id and msg" errors on the ROS side
}
