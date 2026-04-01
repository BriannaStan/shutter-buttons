#!/usr/bin/env python3

from shutter_buttons_msgs.msg import LightChange
from shutter_buttons_msgs.srv import LightChangeArray

import rclpy
from rclpy.node import Node


class Rainbow(Node):
    def __init__(self, setup_timeout=5.0):
        super().__init__("set_button")
        self.client = self.create_client(LightChangeArray, "light_change")
        while not self.client.wait_for_service(timeout_sec=setup_timeout):
            self.get_logger().warn("LightChange service not available, waiting again...")

    @staticmethod
    def construct_light_change_request():
        color = "FF000000"
        background_color = "000000FF"

        return [
                LightChange(
                    button_id=button_id,
                    pressed=False,
                    anim_type=3,
                    intensity=100,
                    color_r=int(color[0:2], 16),
                    color_g=int(color[2:4], 16),
                    color_b=int(color[4:6], 16),
                    color_w=int(color[6:8], 16),
                    bcolor_r=int(background_color[0:2], 16),
                    bcolor_g=int(background_color[2:4], 16),
                    bcolor_b=int(background_color[4:6], 16),
                    bcolor_w=int(background_color[6:8], 16)
                )
                for button_id in (1, 2, 3)
        ]

    def display(self):
        if self.client is None:
            self.get_logger().error("Service client not initialized")
            raise RuntimeError
        request = LightChangeArray.Request(light_changes=self.construct_light_change_request())
        return self.client.call_async(request)


def main():
    rclpy.init(args=None)
    node = Rainbow()
    light_change_future = node.display()
    rclpy.spin_until_future_complete(node, light_change_future)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
