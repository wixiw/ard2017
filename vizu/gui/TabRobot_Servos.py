#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from ArdWidgets import *
from core import *
import time

#
# This class is a pre-built widget which is designed to display 
# robot state related to onboard actuators/sensors.
#
class TabRobot_Servos(QWidget):
    
    lifterCmd           = pyqtSignal(int)
    leftArmCmd          = pyqtSignal(int)
    rightArmCmd         = pyqtSignal(int)
    leftWheelCmd        = pyqtSignal(int)
    rightWheelCmd       = pyqtSignal(int)
    funnyCmd            = pyqtSignal(int)
    rotatorCmd          = pyqtSignal(int)
    servo8Cmd           = pyqtSignal(int)
    requestRotatorTurn  = pyqtSignal(int)
    
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
        self.columns[2] = QVBoxLayout()
        self.layout.addLayout(self.columns[0])
        self.layout.addLayout(self.columns[1])
        self.layout.addLayout(self.columns[2])
        self.layout.addStretch(1)
        
        #create servos
        self.addServo("lifter",         0, self.lifterCmd
                      )
        self.addServo("funny action",   1, self.funnyCmd)
        self.addServo("left arm",       0, self.leftArmCmd)
        self.addServo("left wheel",     0, self.leftWheelCmd)
        self.addServo("right arm",      1, self.rightArmCmd)
        self.addServo("right wheel",    1, self.rightWheelCmd)
        self.addServo("rotator",        0, self.rotatorCmd)
        self.addServo("servo8",         1, self.servo8Cmd)
      
        self.btn_rotatorTurn = QPushButton("RotTurn : idle")
        self.btn_rotatorTurn.setCheckable(True)
        self.btn_rotatorTurn.toggled[bool].connect(self._rotatorTurn)
        self.columns[2].addWidget(self.btn_rotatorTurn)
      
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
            self.servos["rotator"].setValue(msg.actuators.servoRotator)
            self.servos["servo8"].setValue(msg.actuators.servo8)
   
    @pyqtSlot(bool)
    def _rotatorTurn(self, pressed): 
        if pressed:
            print("Rotator turn : Turning") 
            self.btn_rotatorTurn.setText("RotTurn : turning")
        else:
            print("Rotator turn : Idle") 
            self.btn_rotatorTurn.setText("RotTurn : idle")
        self.requestRotatorTurn.emit(pressed)
   
class ServoWidget(QWidget): 
    move = pyqtSignal(int)

    def __init__(self, parent, name, signal):
        super().__init__(parent)
        self.name = name
        self.signal = signal
        
        self.label = QLabel("0/1000")
        self.label.setFixedWidth(50)
        self.label.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        
        self.slider = QSlider(Qt.Horizontal)
        self.slider.setMinimum(0)
        self.slider.setMaximum(+1000) 
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
        self.label.setText(str(value)+"/1000")
        self.signal.emit(value)
        time.sleep(0.025)

    #Use this when a new value comes from the robot (ie not from the HMI)
    def setValue(self, value):
        self.label.setText(str(value)+"/1000")
        self.slider.setValue(value)
        
