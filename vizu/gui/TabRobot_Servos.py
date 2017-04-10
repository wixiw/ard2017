#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from ArdWidgets import *
from core import *
import _tkinter

#
# This class is a pre-built widget which is designed to display 
# robot state related to onboard actuators/sensors.
#
class TabRobot_Servos(QWidget):
    
    lifterCmd       = pyqtSignal(int)
    leftArmCmd      = pyqtSignal(int)
    rightArmCmd     = pyqtSignal(int)
    leftWheelCmd    = pyqtSignal(int)
    rightWheelCmd   = pyqtSignal(int)
    funnyCmd        = pyqtSignal(int)
    
    def __init__(self, parent):
        super().__init__(parent)
        
        self.sections = dict()
        self.servos = dict()
        self.columns = dict()
        
        # Set this field to true when you want to read servo position from robot
        self.readNewData = False
        
        #create layouts objects
        self.layout = QHBoxLayout(self)
        self.columns[0] = QVBoxLayout()
        self.columns[1] = QVBoxLayout()
        self.layout.addLayout(self.columns[0])
        self.layout.addLayout(self.columns[1])
        self.layout.addStretch(1)
        
        #create servos
        self.addServo("lifter",         0, self.lifterCmd
                      )
        self.addServo("funny action",   1, self.funnyCmd)
        self.addServo("left arm",       0, self.leftArmCmd)
        self.addServo("left wheel",     0, self.leftWheelCmd)
        self.addServo("right arm",      1, self.rightArmCmd)
        self.addServo("right wheel",    1, self.rightWheelCmd)
            
      
    def addServo(self, name, column, signal):
        self.servos[name] = ServoWidget(self, name, signal)
        self.columns[column].addWidget(self.servos[name])
      
    #Override in order to initialize the view each time the widget is shown
    def showEvent (self, QShowEvent):
        self.readNewData = True

    #telemetry reply data callback
    @pyqtSlot(RemoteControl_pb2.Telemetry)     
    def _telemetryDataCb(self, msg):
        if self.readNewData:
            self.readNewData = False
            self.servos["lifter"].setValue(msg.actuators.servoLifter)
            self.servos["funny action"].setValue(msg.actuators.servoFunnyAction)
            self.servos["left arm"].setValue(msg.actuators.servoLeftArm)
            self.servos["left wheel"].setValue(msg.actuators.servoLeftWheel)
            self.servos["right arm"].setValue(msg.actuators.servoRightArm)
            self.servos["right wheel"].setValue(msg.actuators.servoRightWheel)
   
class ServoWidget(QWidget): 
    move = pyqtSignal(int)
    
    def __init__(self, parent, name, signal):
        super().__init__(parent)
        self.name = name
        self.signal = signal
        
        self.label = QLabel("0°")
        self.label.setFixedWidth(50)
        self.label.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        
        self.slider = QSlider(Qt.Horizontal)
        self.slider.setMinimum(0)
        self.slider.setMaximum(+180) 
        self.slider.setPageStep(10)
        self.slider.setSingleStep(1)
        self.slider.setTracking(True)
        self.slider.setTickPosition(15)
        self.slider.setTickInterval(45)
        self.slider.valueChanged.connect(self.valueChanged)
        self.slider.setFixedWidth(250)
        
        self.layoutBox = QHBoxLayout()
        self.layoutBox.addWidget(self.slider)
        self.layoutBox.addWidget(self.label)
        
        self.box = QGroupBox(name)
        self.box.setLayout(self.layoutBox)
        
        self.layout = QHBoxLayout(self)
        self.layout.addWidget(self.box)
        self.layout.addStretch()
        
    #Signal generated when the value of the servo cmd is changed 
    #(either from HMI or from network request)
    @pyqtSlot(int)
    def valueChanged(self, value):  
        self.label.setText(str(value)+"°")
        self.signal.emit(value)

    #Use this when a new value comes from the robot (ie not from the HMI)
    def setValue(self, value):
        self.label.setText(str(value)+"°")
        self.slider.setValue(value)