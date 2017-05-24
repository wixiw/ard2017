#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from ArdWidgets import *
from core import *

class TabRobot_Calib(QWidget):
    
    def __init__(self, parent):
        super().__init__(parent)
        
        self.sections = dict()

        #Create sections   
        self.sections["Linear"] = LinearCalibration(self)
        self.sections["Rotation"] = RotationCalibration(self)
        
        #create layouts objects
        self.layout = QVBoxLayout(self)
        self.layout.addWidget(self.sections["Linear"])
        self.layout.addWidget(self.sections["Rotation"])
        self.layout.addStretch(1)
        
        
class LinearCalibration(RobotConfigWidget):   
    move = pyqtSignal(float)
     
    def __init__(self, parent):
        super().__init__(parent)
        self.nbMeasures = 5
        self.btn_cmds = dict()
        self.layout=dict()
        self.layout["L1"] = QVBoxLayout(self)
        self.layout["L2"] = QHBoxLayout()
        self.layout["L3"] = QHBoxLayout()
        self.layout["L4"] = QHBoxLayout()
        self.layout["L5"] = QVBoxLayout()
        
        self.layout["L1"].addLayout(self.layout["L2"])
        self.layout["L1"].addLayout(self.layout["L3"])
        self.layout["L1"].addLayout(self.layout["L4"])
        self.layout["L1"].addLayout(self.layout["L5"])
        self.layout["L1"].addStretch(1)
        
        #First line : input/move cmd
        self.in_moveDist = IntegerInput(self, 0, 5000)
        self.in_moveDist.setValue(1000)
        self.btn_cmds["move"] = QPushButton('Move !', self)
        self.btn_cmds["move"].clicked.connect(self._moveRequest) 
        self.btn_cmds["back"] = QPushButton('Back home', self)
        self.btn_cmds["back"].clicked.connect(self._moveBack) 
        self.layout["L2"].addWidget(self.in_moveDist)
        self.layout["L2"].addWidget(self.btn_cmds["move"])
        self.layout["L2"].addWidget(self.btn_cmds["back"])
        self.layout["L2"].addStretch(1)
        
        #Second line : measures inputs
        self.in_measures = list()
        self.layout["L3"].addWidget(QLabel("Measures : "))
        for i in range(0,self.nbMeasures):
            self.in_measures.append(IntegerInput(self, 0, 5000))
            self.layout["L3"].addWidget(self.in_measures[i])
        self.layout["L3"].addStretch(1)
        self.btn_cmds["compute"] = QPushButton('Compute', self)
        self.btn_cmds["compute"].clicked.connect(self._compute) 
        self.btn_cmds["clear"] = QPushButton('Clear', self)
        self.btn_cmds["clear"].clicked.connect(self._clear)
        
        #Third line : measures control 
        self.layout["L4"].addWidget(self.btn_cmds["compute"])
        self.layout["L4"].addWidget(self.btn_cmds["clear"])
        self.layout["L4"].addStretch(1)
        
        #Fourth line : results display
        self.layout["L5"].addWidget(QLabel("Results : "))
        self.lab_leftWheel = QLabel("")
        self.lab_rightWheel = QLabel("")
        self.lab_voie = QLabel("")
        self.lab_leftWheel.setFont(QFont("Courier", 10))
        self.lab_rightWheel.setFont(QFont("Courier", 10))
        self.lab_voie.setFont(QFont("Courier", 10))
        self.layout["L5"].addWidget(self.lab_leftWheel)
        self.layout["L5"].addWidget(self.lab_rightWheel)
        self.layout["L5"].addWidget(self.lab_voie)
        self.layout["L5"].addStretch(1)
        
        self.box = QGroupBox("Linear calibration")
        self.box.setLayout(self.layout["L1"])
        
        self.layout = QHBoxLayout(self)
        self.layout.addWidget(self.box)
        self.layout.addStretch()
                  
        
    @pyqtSlot(RemoteControl_pb2.Configuration)    
    def updateConfig(self, msg):
        super().updateConfig(msg)
        self.lab_leftWheel.setText( "Left wheel diameter  : " + str(self.config.leftWheelDiameter) + "mm")
        self.lab_rightWheel.setText("Right wheel diameter : " + str(self.config.rightWheelDiameter) + "mm")
        self.lab_voie.setText(      "Voie                 : " + str(self.config.voie) + "mm")
                        
    @pyqtSlot()
    def _moveRequest(self): 
       print("Move")
       self.move.emit(self.in_moveDist.getValue())
       
    @pyqtSlot()
    def _moveBack(self): 
       print("Back home")
       self.move.emit(-self.in_moveDist.getValue())
       
    @pyqtSlot()
    def _compute(self): 
       print("Compute")
       mean = 0
       nbMeasure = 0
       for input in self.in_measures:
           if input.getValue() != 0:
               mean += float(input.getValue())
               nbMeasure += 1
       if nbMeasure == 0 :
           print("No measure entered")
           return
       
       mean = mean / nbMeasure
       print("Mean measure : " + str(mean) + " (from a panel of " + str(nbMeasure) + " measures)")
       newleftW = mean*self.config.leftWheelDiameter/self.in_moveDist.getValue()
       newrightW = mean*self.config.rightWheelDiameter/self.in_moveDist.getValue()
       
       self.lab_leftWheel.setText(  "Left wheel diameter  : " + str(newleftW) + "mm")
       self.lab_rightWheel.setText( "Right wheel diameter : " + str(newrightW) + "mm")

    @pyqtSlot()
    def _clear(self): 
        print("Clear")
        for input in self.in_measures:
           input.setValue("")
        self.lab_leftWheel.setText( "Left wheel diameter  : " + str(self.config.leftWheelDiameter) + "mm")
        self.lab_rightWheel.setText("Right wheel diameter : " + str(self.config.rightWheelDiameter) + "mm")
        self.lab_voie.setText(      "Voie                 : " + str(self.config.voie) + "mm")
   
class RotationCalibration(RobotConfigWidget): 
    move = pyqtSignal(float)
    
    def __init__(self, parent):
        super().__init__(parent)
        self.btn_cmds = dict()
        self.layout=dict()
        
        self.btn_cmds["CW"] = QPushButton('One Turn CW', self)
        self.btn_cmds["CW"].clicked.connect(self._moveCW) 
        self.btn_cmds["CCW"] = QPushButton('One Turn CCW', self)
        self.btn_cmds["CCW"].clicked.connect(self._moveCCW) 
        
#         self.btn_cmds["compute"] = QPushButton('Compute', self)
#         self.btn_cmds["compute"].clicked.connect(self._compute) 
        
        self.layout["L1"] = QVBoxLayout(self)
        self.layout["L1"].addWidget(QLabel("When robot is turning too much => reduce voie."))
        for button in self.btn_cmds:    
            self.layout["L1"].addWidget(self.btn_cmds[button])
        self.layout["L1"].addStretch()
        
        self.box = QGroupBox("Linear calibration")
        self.box.setLayout(self.layout["L1"])
        
        self.layout = QHBoxLayout(self)
        self.layout.addWidget(self.box)
        self.layout.addStretch()
        
    @pyqtSlot()
    def _moveCW(self): 
       print("Move CW")
       self.move.emit(-360)
       
    @pyqtSlot()
    def _moveCCW(self): 
       print("Move CCW")
       self.move.emit(360)
   
   
   
   