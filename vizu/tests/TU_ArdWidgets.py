#!/usr/bin/python3
# -*- coding: utf-8 -*-

#expand path to find modules :
import sys
sys.path.append("..")
sys.path.append("../proto")
sys.path.append("../../devenv/nanopb-0.3.7-windows-x86/generator/proto")

import signal
import math
from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from gui import *

class TU_ArdWidgets(QWidget):
    def __init__(self):
        super().__init__()
        self.intInput = IntegerInput(self, -100, 100)
        self.floatInput = FloatInput(self, -1. , 1., 3)
        self.hInput = HeadingInput(self)
        self.dirInput = DirectionInput(self)
        self.read = QPushButton('Read', self)
        layout = QFormLayout(self)
        layout.addRow("intInput", self.intInput)
        layout.addRow("floatInput", self.floatInput)
        layout.addRow("hInput", self.hInput)
        layout.addRow("dirInput", self.dirInput)
        layout.addRow("", self.read) 
        self.read.pressed.connect(self.readCB)
        
    @pyqtSlot()
    def readCB(self):
        print("int : "      + str(self.intInput.getValue()))
        print("float : "    + str(self.floatInput.getValue()))
        print("h : "        + str(self.hInput.getValue()))
        print("dir : "      + str(self.dirInput.getValue()))

if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    app = QApplication(sys.argv)
    main = TU_ArdWidgets()
    main.show()
    sys.exit(app.exec_())
    
    
    