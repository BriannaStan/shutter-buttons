#!/usr/bin/env python3
"""
Buttons dashboard ROS2 node
"""

import sys
import time

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy

from PySide2 import QtWidgets
from PySide2.QtCore import QThread, Signal
from std_msgs.msg import Bool

from shutter_buttons_msgs.srv import LightChangeArray
from shutter_buttons.button_ui import ButtonUI


class ButtonBoardNode(QThread):
    """Buttons Dashboard Node. Handles all ROS2 communication for the buttons GUI"""

    # signal declaration
    color_changed = Signal(int, int, int, int)

    def __init__(self):
        QThread.__init__(self)

        # initialize ROS2
        rclpy.init()
        self.node = Node("button_board_node")

        # QoS profile for publishers
        qos = QoSProfile(depth=1, reliability=ReliabilityPolicy.RELIABLE)

        # publishers for each button
        self.button_1_pub = self.node.create_publisher(Bool, '/button1/state', qos)
        self.button_2_pub = self.node.create_publisher(Bool, '/button2/state', qos)
        self.button_3_pub = self.node.create_publisher(Bool, '/button3/state', qos)

        # service for light change
        self.light_change_srv = self.node.create_service(
            LightChangeArray,
            'light_change',
            self.light_changer
        )

        self._running = True
        self.start()

    def run(self):
        """QThread run method - spins ROS2 node"""
        while self._running and rclpy.ok():
            rclpy.spin_once(self.node, timeout_sec=0.1)

    def stop(self):
        """shutdown"""
        self._running = False
        self.wait()
        self.node.destroy_node()
        rclpy.shutdown()

    def light_changer(self, request, response):
        button_1_r = request.light_changes[0].color_r
        button_1_g = request.light_changes[0].color_g
        button_1_b = request.light_changes[0].color_b

        button_2_r = request.light_changes[1].color_r
        button_2_g = request.light_changes[1].color_g
        button_2_b = request.light_changes[1].color_b

        button_3_r = request.light_changes[2].color_r
        button_3_g = request.light_changes[2].color_g
        button_3_b = request.light_changes[2].color_b

        self.color_changed.emit(0, button_1_r, button_1_g, button_1_b)
        self.color_changed.emit(1, button_2_r, button_2_g, button_2_b)
        self.color_changed.emit(2, button_3_r, button_3_g, button_3_b)

        response.result = True
        return response


class ButtonBoard(ButtonUI):
    """
    Buttons dashboard
    Buttons can be pressed on the GUI, which changes the button/state from false to true
    This may also change the button light color
    """

    def __init__(self):
        # start ROS2 node
        self.node = ButtonBoardNode()
        super(ButtonBoard, self).__init__()

    def button_1_press(self):
        super(ButtonBoard, self).button_1_press()
        msg = Bool()
        msg.data = True
        self.node.button_1_pub.publish(msg)

    def button_1_unpress(self):
        super(ButtonBoard, self).button_1_unpress()
        time.sleep(0.5)
        msg = Bool()
        msg.data = False
        self.node.button_1_pub.publish(msg)

    def button_2_press(self):
        super(ButtonBoard, self).button_2_press()
        msg = Bool()
        msg.data = True
        self.node.button_2_pub.publish(msg)

    def button_2_unpress(self):
        super(ButtonBoard, self).button_2_unpress()
        time.sleep(0.5)
        msg = Bool()
        msg.data = False
        self.node.button_2_pub.publish(msg)

    def button_3_press(self):
        super(ButtonBoard, self).button_3_press()
        msg = Bool()
        msg.data = True
        self.node.button_3_pub.publish(msg)

    def button_3_unpress(self):
        super(ButtonBoard, self).button_3_unpress()
        time.sleep(0.5)
        msg = Bool()
        msg.data = False
        self.node.button_3_pub.publish(msg)

    def change_color(self, button_num, button_r, button_g, button_b):
        self.button_colors[button_num] = [button_r, button_g, button_b]
        self.update()

    def setup(self):
        super(ButtonBoard, self).setup()

    def setConnections(self):
        """Set connections"""
        super(ButtonBoard, self).setConnections()

        # connect ROS2 signals with Qt
        self.node.color_changed.connect(self.change_color)

    def closeEvent(self, event):
        """Handle window close - cleanup ROS2"""
        self.node.stop()
        event.accept()


def main(args=None):
    app = QtWidgets.QApplication(sys.argv)
    buttons_gui = ButtonBoard()

    qscreen = app.screens()[0]
    geo = qscreen.geometry()
    buttons_gui.move(geo.left(), geo.top())

    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
