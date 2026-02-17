#!/usr/bin/env python3
#
# Set a button color and pattern
#
# Created 02/13/2020 by Nathan Tsoi
# Moved to shutter_buttons  17/01/2026

from shutter_buttons_msgs.msg import LightChange
from shutter_buttons_msgs.srv import LightChangeArray

from py_trees.behaviour import Behaviour
from py_trees.common import Status


class SetButton(Behaviour):
    def __init__(self, name='Set Button', button_id=None, pressed=False, animation=1, intensity=50, color='#FF000000', background_color='#000000FF', batch_dict=None):
        '''
          if button_id is None: all buttons are set
          color and background_color are specified as a hexidecimal RGBW value where W is the white led value
        '''
        super(SetButton, self).__init__(name)
        self.button_id = button_id
        self.pressed = pressed
        self.animation = animation
        self.intensity = intensity
        self.color = color
        self.background_color = background_color
        self.batch_dict = batch_dict
        self.node = None
        self.client = None

    def setup(self, **kwargs):
        try:
            self.node = kwargs.get('node', None)
            if self.node is None:
                self.logger.error("No ROS2 node provided to SetButton.setup()")
                return False

            self.client = self.node.create_client(LightChangeArray, 'light_change')

            # wait for service with timeout
            timeout_sec = kwargs.get('timeout', 5.0)
            if not self.client.wait_for_service(timeout_sec=timeout_sec):
                self.logger.error("Service light_change not available")
                return False

        except Exception as e:
            self.logger.error(f"SetButton setup failed: {e}")
            return False

        return True

    def initialise(self):
        self.light_changes = []
        if self.batch_dict is not None:
            for button_request in self.batch_dict:
                self.display(
                    button_request['button_id'],
                    button_request['pressed'],
                    button_request['animation'],
                    button_request['intensity'],
                    button_request['color'],
                    button_request['background_color']
                )
        else:
            self.display(
                self.button_id,
                self.pressed,
                self.animation,
                self.intensity,
                self.color,
                self.background_color
            )
        self.logger.debug("  %s [SetButton::initialise()]" % self.name)

    def display(self, button_id=None, pressed=False, animation=1, intensity=50, color='#FF000000', background_color='#000000FF'):
        color = color.split('#')[-1]
        if len(color) == 6:
            color += '00'
        background_color = background_color.split('#')[-1]
        if len(background_color) == 6:
            background_color += '00'
        button_ids = [button_id]
        if button_id is None:
            button_ids = [1, 2, 3]
        for button_id in button_ids:
            light_change_msg = LightChange(
                button_id=button_id,
                pressed=pressed,
                anim_type=animation,
                intensity=intensity,
                color_r=int(color[0:2], 16),
                color_g=int(color[2:4], 16),
                color_b=int(color[4:6], 16),
                color_w=int(color[6:8], 16),
                bcolor_r=int(background_color[0:2], 16),
                bcolor_g=int(background_color[2:4], 16),
                bcolor_b=int(background_color[4:6], 16),
                bcolor_w=int(background_color[6:8], 16)
            )
            self.light_changes.append(light_change_msg)

        if len(self.light_changes) == 3:
            if self.client is None:
                self.logger.error("Service client not initialized")
                return

            try:
                request = LightChangeArray.Request()
                request.light_changes = self.light_changes

                # sync call
                future = self.client.call_async(request)

                self.light_changes = []
            except Exception as e:
                self.light_changes = []
                self.logger.error("  %s [SetButton::display()]: %s" % (self.name, e))

    def update(self):
        # TODO: handle errors from button srv
        return Status.SUCCESS

    def visitor_info(self):
        return self.button_id
