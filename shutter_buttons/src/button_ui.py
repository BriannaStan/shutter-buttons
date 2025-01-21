import signal

from PySide2 import QtGui, QtWidgets
from PySide2.QtCore import QObject, QRect, SIGNAL
from PySide2.QtGui import QColor, QPalette


class ButtonUI(QtWidgets.QWidget):

    def __init__(self):
        
        super(ButtonUI, self).__init__()
        signal.signal(signal.SIGINT, signal.SIG_DFL)

        self.setGeometry(0, 0, 320, 240)
        self.setWindowTitle("Button Board")

        self.button_1 = QtWidgets.QPushButton("1")
        self.button_1.setGeometry(QRect(10, 80, 91, 81))
        self.button_1.setPalette(QPalette(QColor(0, 100, 0)))
        QObject.connect(self.button_1, SIGNAL('pressed()'), self.button_1_press)
        QObject.connect(self.button_1, SIGNAL('released()'), self.button_1_unpress)

        self.button_2 = QtWidgets.QPushButton("2")
        self.button_2.setGeometry(QRect(110, 80, 91, 81))
        QObject.connect(self.button_2, SIGNAL('pressed()'), self.button_2_press)
        QObject.connect(self.button_2, SIGNAL('released()'), self.button_2_unpress)

        self.button_3 = QtWidgets.QPushButton("3")
        self.button_3.setGeometry(QRect(220, 80, 91, 81))
        self.button_3.setPalette(QPalette(QColor(128, 0, 0)))
        QObject.connect(self.button_3, SIGNAL('pressed()'), self.button_3_press)
        QObject.connect(self.button_3, SIGNAL('released()'), self.button_3_unpress)

        self.layout = QtWidgets.QHBoxLayout()
        self.layout.addWidget(self.button_1)
        self.layout.addWidget(self.button_2)
        self.layout.addWidget(self.button_3)
        self.setLayout(self.layout)

        self.button_colors = [[0, 100, 0], [255, 255, 255], [128, 0, 0]]

        self.setup()
        self.setConnections()
        self.show()


    def setup(self):
        """Setup method. Prepare the interface and anything that is necessary for proper operation of the GUI"""
        pass


    def setConnections(self):
        """Set constant connections"""
        pass


    def button_1_press(self):
        pass


    def button_1_unpress(self):
        pass


    def button_2_press(self):
        pass


    def button_2_unpress(self):
        pass


    def button_3_press(self):
        pass


    def button_3_unpress(self):
        pass


    def paintEvent(self, event):
        qp = QtGui.QPainter()
        qp.begin(self)

        qp.setRenderHint(QtGui.QPainter.Antialiasing)
        self.updateColors()
        qp.end()

    
    def updateColors(self):
        self.button_1.setPalette(QPalette(QColor(self.button_colors[0][0], self.button_colors[0][1], self.button_colors[0][2])))
        self.button_2.setPalette(QPalette(QColor(self.button_colors[1][0], self.button_colors[1][1], self.button_colors[1][2])))
        self.button_3.setPalette(QPalette(QColor(self.button_colors[2][0], self.button_colors[2][1], self.button_colors[2][2])))
