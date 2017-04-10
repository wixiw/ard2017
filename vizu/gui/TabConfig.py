#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from ArdWidgets import *
from core import *

#
# This class is a pre-built widget which is designed 
# to configure the robot at run time 
#
class TabConfig(QWidget):
    
    getConfig = pyqtSignal()
    setConfig = pyqtSignal(RemoteControl_pb2.RemoteControlRequest)
    
    def __init__(self, parent):
        super().__init__(parent)
        self.layout=dict()

        self.calibConfig = CalibConfigWidget(self)
        self.navConfig = NavigationConfigWidget(self)
        self.avoidanceConfig = AvoidanceConfigWidget(self)
        self.stratConfig = StratConfigWidget(self)
        self.btn_up = QPushButton('Request conf from Robot', self)
        self.btn_up.clicked.connect(self._getConfig) 
        self.btn_exe = QPushButton('Send to Robot', self)
        self.btn_exe.clicked.connect(self._sendConfig) 
        
        #create layouts objects
        self.layout["Top"] = QVBoxLayout(self)
        self.layout["Split"] = QHBoxLayout()
        self.layout["Left"] = QVBoxLayout()
        self.layout["Right"] = QVBoxLayout()
        
        self.layout["Top"].addWidget(self.btn_up)
        self.layout["Top"].addWidget(self.btn_exe)
        self.layout["Top"].addLayout(self.layout["Split"])
        self.layout["Top"].addStretch()

        self.layout["Split"].addLayout(self.layout["Left"])
        self.layout["Top"].addStretch()
        self.layout["Split"].addLayout(self.layout["Right"])
        self.layout["Top"].addStretch()

        self.layout["Left"].addWidget(self.calibConfig)
        self.layout["Left"].addWidget(self.stratConfig)
        self.layout["Right"].addWidget(self.navConfig)
        self.layout["Right"].addWidget(self.avoidanceConfig)
        
        
        
    def _sendConfig(self):
        print("Requesting new config")
        req = RemoteControl_pb2.RemoteControlRequest()
        
        #Meca
        req.setConfig.stepByTurn = self.calibConfig.stepByTurn.getValue()
        req.setConfig.xar = self.calibConfig.xar.getValue()
        req.setConfig.yside = self.calibConfig.yside.getValue()
        req.setConfig.leftWheelDiameter = self.calibConfig.leftWheelDiameter.getValue()
        req.setConfig.rightWheelDiameter = self.calibConfig.rightWheelDiameter.getValue()
        req.setConfig.voie = self.calibConfig.voie.getValue()
        #Nav
        req.setConfig.maxAcc = self.navConfig.maxAcc.getValue()
        req.setConfig.maxTurnAcc = self.navConfig.maxTurnAcc.getValue()
        req.setConfig.maxTurnSpeed = self.navConfig.maxTurnSpeed.getValue()
        req.setConfig.deccDist = self.navConfig.deccDist.getValue()
        
        req.setConfig.recalSpeed = self.navConfig.recalSpeed.getValue()
        #Avoidance
        req.setConfig.detectionWaitForOppMove = self.avoidanceConfig.detectionWaitForOppMove.getValue()
        req.setConfig.detectionActive = self.avoidanceConfig.detectionActive.isChecked()
        
        #Strat
        req.setConfig.matchDuration = self.stratConfig.matchDuration.getValue()
        
        self.setConfig.emit(req)
       
    def _getConfig(self):
        print("Requesting current config")
        self.getConfig.emit()
        
    @pyqtSlot(RemoteControl_pb2.Configuration)    
    def updateConfig(self, msg):
        print("New config received from robot, updating tab")
        #Meca
        self.calibConfig.stepByTurn.setValue(msg.stepByTurn)
        self.calibConfig.xar.setValue(msg.xar)
        self.calibConfig.yside.setValue(msg.yside)
        self.calibConfig.leftWheelDiameter.setValue(msg.leftWheelDiameter)
        self.calibConfig.rightWheelDiameter.setValue(msg.rightWheelDiameter)
        self.calibConfig.voie.setValue(msg.voie)
        #Nav
        self.navConfig.maxAcc.setValue(msg.maxAcc)
        self.navConfig.maxTurnAcc.setValue(msg.maxTurnAcc)
        self.navConfig.maxTurnSpeed.setValue(msg.maxTurnSpeed)
        self.navConfig.recalSpeed.setValue(msg.recalSpeed)
        self.navConfig.deccDist.setValue(msg.deccDist)
        self.navConfig.vmax.setText(str(math.floor(math.sqrt(2*msg.maxAcc*msg.deccDist))))
        
        #Avoidance
        self.avoidanceConfig.detectionWaitForOppMove.setValue(msg.detectionWaitForOppMove)
        self.avoidanceConfig.detectionActive.setChecked(msg.detectionActive)
        #Strat
        self.stratConfig.matchDuration.setValue(msg.matchDuration)
        
    #Override in order to initialize the view each time the widget is shown
    def showEvent (self, QShowEvent):
        self._getConfig()

        
class CalibConfigWidget(QWidget): 
    
    def __init__(self, parent):
        super().__init__(parent)
        
        self.stepByTurn         = IntegerInput(self, 0, 8000)
        self.xar                = IntegerInput(self, 0, 500)
        self.yside              = IntegerInput(self, 0, 500)
        self.leftWheelDiameter  = FloatInput(self, 0, 100, 5)
        self.rightWheelDiameter = FloatInput(self, 0, 100, 5)
        self.voie               = FloatInput(self, 0, 500, 5)
        
        self.layoutH = QHBoxLayout(self)
        self.layoutV = QGroupBox("Calibration")
        self.layoutH.addWidget(self.layoutV)
        self.form = QFormLayout()
        self.layoutV.setLayout(self.form)
        
        self.form.addRow("steps by turn", self.stepByTurn)
        self.form.addRow("x AR (mm)", self.xar)
        self.form.addRow("y side (mm)", self.yside)
        self.form.addRow("left  wheel diameter (mm)", self.leftWheelDiameter)
        self.form.addRow("right wheel diameter (mm)", self.rightWheelDiameter)
        self.form.addRow("voie (mm)", self.voie)
        
class NavigationConfigWidget(QWidget): 
    
    def __init__(self, parent):
        super().__init__(parent)
        
        self.maxAcc          = IntegerInput(self, 0, 2000)
        self.maxTurnAcc      = IntegerInput(self, 0, 2000)
        self.maxTurnSpeed    = IntegerInput(self, 0, 2000)
        self.recalSpeed      = IntegerInput(self, 0, 2000)
        self.deccDist        = IntegerInput(self, 0, 2000)
        self.vmax            = QLabel("0")
        
        self.vmax.setAlignment(Qt.AlignCenter)
        
        self.layoutH = QHBoxLayout(self)
        self.layoutV = QGroupBox("Navigation")
        self.layoutH.addWidget(self.layoutV)
        self.form = QFormLayout()
        self.layoutV.setLayout(self.form)
        
        self.form.addRow("maxAcc (mm/s²)", self.maxAcc)
        self.form.addRow("maxTurnAcc (°/s²)", self.maxTurnAcc)
        self.form.addRow("maxTurnSpeed (mm/s)", self.maxTurnSpeed)
        self.form.addRow("recalSpeed (mm/s)", self.recalSpeed)
        self.form.addRow("decc distance (mm)", self.deccDist)
        self.form.addRow("info : vmax (mm/s)", self.vmax)
        
class AvoidanceConfigWidget(QWidget): 
    
    def __init__(self, parent):
        super().__init__(parent)
        
        self.detectionWaitForOppMove  = IntegerInput(self, 0, 2000)
        self.detectionActive           = QPushButton('Active', self)
        self.detectionActive.setCheckable(True)
        
        self.layoutH = QHBoxLayout(self)
        self.layoutV = QGroupBox("Avoidance")
        self.layoutH.addWidget(self.layoutV)
        self.form = QFormLayout()
        self.layoutV.setLayout(self.form)
        
        self.form.addRow("wait after block (ms)", self.detectionWaitForOppMove)
        self.form.addRow("avoidance ", self.detectionActive)
             
class StratConfigWidget(QWidget): 
    
    def __init__(self, parent):
        super().__init__(parent)
        
        self.matchDuration  = IntegerInput(self, 0, 2000)
        
        self.layoutH = QHBoxLayout(self)
        self.layoutV = QGroupBox("Strategy")
        self.layoutH.addWidget(self.layoutV)
        self.form = QFormLayout()
        self.layoutV.setLayout(self.form)
        
        self.form.addRow("match duration (ms)", self.matchDuration)
        
