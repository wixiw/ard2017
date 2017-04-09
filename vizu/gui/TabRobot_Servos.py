#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from ArdWidgets import *
from core import *

#
# This class is a pre-built widget which is designed to display 
# robot state related to onboard actuators/sensors.
#
class TabRobot_Servos(QWidget):
    
    def __init__(self, parent):
        super().__init__(parent)
        
        self.sections = dict()
        
        self.servos = dict()
        self.servos["lifter"] = ServoWidget(self, "lifter")
        
        #create layouts objects
        self.layout = QVBoxLayout(self)
        for k, w in self.servos.items():
            self.layout.addWidget(w)
        self.layout.addStretch(1)
      
   
class ServoWidget(QWidget): 
    move = pyqtSignal(int)
    
    def __init__(self, parent, name):
        super().__init__(parent)
        
        self.inputs = QSlider(self)
        
        self.layoutBox = QVBoxLayout()
        self.layoutBox.addWidget(self.inputs)
        self.layoutBox.addStretch()
        
        self.box = QGroupBox(name)
        self.box.setLayout(self.layoutBox)
        
        self.layout = QHBoxLayout(self)
        self.layout.addWidget(self.box)
        self.layout.addStretch()
    
