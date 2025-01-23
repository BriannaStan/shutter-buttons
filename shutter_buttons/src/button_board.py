#!/usr/bin/env python3
"""
Buttons dashboard ROS node
"""

import sys

import rospy
from PySide2 import QtWidgets
from PySide2.QtCore import QThread, SIGNAL, SLOT
from std_msgs.msg import Bool

from shutter_buttons_msgs.srv import LightChangeArray
from button_ui import ButtonUI


class ButtonBoardNode(QThread):
    """ Buttons Dashboard Node. Handles all ROS communication for the buttons GUI"""

    def __init__(self):

        QThread.__init__(self)
        rospy.init_node("ButtonBoardNode", anonymous=False)

        # publishers for each button
        self.button_1_pub = rospy.Publisher('/button1/state', Bool, queue_size=1)
        self.button_2_pub = rospy.Publisher('/button2/state', Bool, queue_size=1)
        self.button_3_pub = rospy.Publisher('/button3/state', Bool, queue_size=1)

        # dummy server for light change
        self.dummy_light_change = rospy.Service('light_change', LightChangeArray, self.light_changer)


    def light_changer(self, srv_request):
        button_1_r = srv_request.light_changes[0].color_r
        button_1_g = srv_request.light_changes[0].color_g
        button_1_b = srv_request.light_changes[0].color_b

        button_2_r = srv_request.light_changes[1].color_r
        button_2_g = srv_request.light_changes[1].color_g
        button_2_b = srv_request.light_changes[1].color_b

        button_3_r = srv_request.light_changes[2].color_r
        button_3_g = srv_request.light_changes[2].color_g
        button_3_b = srv_request.light_changes[2].color_b

        self.emit(SIGNAL("change_color(int, int, int, int)"), 0, button_1_r, button_1_g, button_1_b)
        self.emit(SIGNAL("change_color(int, int, int, int)"), 1, button_2_r, button_2_g, button_2_b)
        self.emit(SIGNAL("change_color(int, int, int, int)"), 2, button_3_r, button_3_g, button_3_b)
        return True


class ButtonBoard(ButtonUI):
    """
    Buttons dashboard
    Buttons can be pressed on the GUI, which changes the button/state from false to true
    This may also change the button light color
    """

    def __init__(self):
        # start ROS node
        self.node = ButtonBoardNode()
        super(ButtonBoard, self).__init__()


    def button_1_press(self):
        super(ButtonBoard, self).button_1_press()
        self.node.button_1_pub.publish(True)


    def button_1_unpress(self):
        super(ButtonBoard, self).button_1_unpress()
        rospy.sleep(0.5)
        self.node.button_1_pub.publish(False)


    def button_2_press(self):
        super(ButtonBoard, self).button_2_press()
        self.node.button_2_pub.publish(True)


    def button_2_unpress(self):
        super(ButtonBoard, self).button_2_unpress()
        rospy.sleep(0.5)
        self.node.button_2_pub.publish(False)


    def button_3_press(self):
        super(ButtonBoard, self).button_3_press()
        self.node.button_3_pub.publish(True)


    def button_3_unpress(self):
        super(ButtonBoard, self).button_3_unpress()
        rospy.sleep(0.5)
        self.node.button_3_pub.publish(False)


    def change_color(self, button_num, button_r, button_g, button_b):
        self.button_colors[button_num] = [button_r, button_g, button_b]
        self.update()


    def setup(self):
        super(ButtonBoard, self).setup()


    def setConnections(self):
        """Set connections"""
        super(ButtonBoard, self).setConnections()

        # connect ROS signals with Qt
        self.connect(self.node, SIGNAL("change_color(int, int, int, int)"), self.change_color)


def main():
    app = QtWidgets.QApplication(sys.argv)
    buttons_gui = ButtonBoard()

    qscreen = app.screens()[0]
    geo = qscreen.geometry()
    buttons_gui.move(geo.left(), geo.top())

    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
