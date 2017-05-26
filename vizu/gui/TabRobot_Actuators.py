#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from ArdWidgets import *
from core import *

class TabRobot_Actuators(QWidget):
    
    def __init__(self, parent):
        super().__init__(parent)
        
        self.sections = dict()

        #Create sections   
        self.sections["lifter"] = LifterCmds(self)
        self.sections["arms"]   = ArmsCmds(self)
        self.sections["funny"]   = FunnyCmds(self)
        
        #create layouts objects
        self.layout = QHBoxLayout(self)
        self.columns = dict()
        self.columns[0] = QVBoxLayout()
        self.columns[1] = QVBoxLayout()
        self.columns[2] = QVBoxLayout()
        self.columns[3] = QVBoxLayout()
        for key, column in self.columns.items():
            self.layout.addLayout(column)
        self.layout.addStretch(1)
            
        self.columns[0].addWidget(self.sections["lifter"])
        self.columns[0].addStretch(1)
        self.columns[1].addWidget(self.sections["arms"])
        self.columns[1].addStretch(1)
        self.columns[2].addWidget(self.sections["funny"])
        self.columns[2].addStretch(1)
        
    @pyqtSlot(RemoteControl_pb2.Telemetry)     
    def _telemetryDataCb(self, msg):
        if self.isVisible():
            #Lifter status     
            if( msg.actuators.lifterReady):
                self.sections["lifter"].lifterReady.light(True)
            else:
                self.sections["lifter"].lifterReady.light(False)
            if( msg.actuators.lifterTimeout):
                self.sections["lifter"].lifterTimeout.light(True)
            else:
                self.sections["lifter"].lifterTimeout.light(False)
            if( msg.actuators.lifterError):
                self.sections["lifter"].lifterError.light(True)
            else:
                self.sections["lifter"].lifterError.light(False)
                
            if( msg.actuators.omronCylinder):
                self.sections["lifter"].cylinderPresent.light(True)
                self.sections["arms"].cylinderPresent.light(True)
            else:
                self.sections["lifter"].cylinderPresent.light(False)
                self.sections["arms"].cylinderPresent.light(False)
                
            if( msg.actuators.armsReady):
                self.sections["arms"].armsReady.light(True)
            else:
                self.sections["arms"].armsReady.light(False)
            if( msg.actuators.armsTimeout):
                self.sections["arms"].armsTimeout.light(True)
            else:
                self.sections["arms"].armsTimeout.light(False)
            if( msg.actuators.armsError):
                self.sections["arms"].armsError.light(True)
            else:
                self.sections["arms"].armsError.light(False)
        
class LifterCmds(QWidget):   
    actCmd = pyqtSignal(int)
     
    def __init__(self, parent):
        super().__init__(parent)       
        
        self.btn_cmds = dict()
        self.layout=dict()
        
        self.btn_cmds["start"] = QPushButton('start', self)
        self.btn_cmds["start"].clicked.connect(self._start) 
        
        self.btn_cmds["stop"] = QPushButton('stop', self)
        self.btn_cmds["stop"].clicked.connect(self._stop)  
        
        self.btn_cmds["lift"] = QPushButton('lift', self)
        self.btn_cmds["lift"].clicked.connect(self._lift) 
        
        self.btn_cmds["prepareNextToPoo"] = QPushButton('prepareNextToPoo', self)
        self.btn_cmds["prepareNextToPoo"].clicked.connect(self._prepareNextToPoo) 
        
        self.btn_cmds["pooEnded"] = QPushButton('pooEnded', self)
        self.btn_cmds["pooEnded"].clicked.connect(self._pooEnded)
        
        self.lifterReady = LedIndicator(self)
        self.lifterTimeout = LedIndicator(self)
        self.lifterError = LedIndicator(self)
        self.cylinderPresent = LedIndicator(self)
        
        self.states = QFormLayout()
        self.states.addRow("ready : ", self.lifterReady)
        self.states.addRow("timeout : ", self.lifterTimeout)
        self.states.addRow("error : ", self.lifterError)
        self.states.addRow("cylinder : ", self.cylinderPresent)
        
        self.layout["Commands"] = QVBoxLayout(self)
        for button in self.btn_cmds:    
            self.layout["Commands"].addWidget(self.btn_cmds[button])
        self.layout["Commands"].addStretch()
        self.layout["Commands"].addLayout(self.states)
        
        self.box = QGroupBox("Lifter")
        self.box.setLayout(self.layout["Commands"])
        
        self.layout = QHBoxLayout(self)
        self.layout.addWidget(self.box)
        self.layout.addStretch()
        
    def _start(self):
        print("Lifter : start")
        self.actCmd.emit(Types_pb2.AC_LIFTER_START)

    def _lift(self):
        print("Lifter : lift")
        self.actCmd.emit(Types_pb2.AC_LIFTER_LIFT)
        
    def _prepareNextToPoo(self):
        print("Lifter : prepareNextToPoo")
        self.actCmd.emit(Types_pb2.AC_LIFTER_PREPARE_NEXT_TO_POO)
        
    def _poo(self):
        print("NOT IMPLEMENTED")
        #print("Lifter : poo") 
        #self.actCmd.emit(Types_pb2.AC_LIFTER_POO) 
        
    def _pooEnded(self):
        print("Lifter : pooEnded") 
        self.actCmd.emit(Types_pb2.AC_LIFTER_POOENDED) 
        
    def _stop(self):
        print("Lifter : stop")
        self.actCmd.emit(Types_pb2.AC_LIFTER_STOP)
        
   
   
class ArmsCmds(QWidget):   
    actCmd = pyqtSignal(int)
     
    def __init__(self, parent):
        super().__init__(parent)       
        
        self.btn_cmds = dict()
        self.layout=dict()
        
        self.btn_cmds["start"] = QPushButton('start', self)
        self.btn_cmds["start"].clicked.connect(self._start) 
        
        self.btn_cmds["stop"] = QPushButton('stop', self)
        self.btn_cmds["stop"].clicked.connect(self._stop)  
                
        self.btn_cmds["swallow"] = QPushButton('swallow', self)
        self.btn_cmds["swallow"].clicked.connect(self._swallow) 
        
        self.btn_cmds["dispenser"] = QPushButton('dispenser', self)
        self.btn_cmds["dispenser"].clicked.connect(self._dispenser) 
        
        self.btn_cmds["fastPoo"] = QPushButton('fastPoo', self)
        self.btn_cmds["fastPoo"].clicked.connect(self._fastPoo) 
        
        self.btn_cmds["poo"] = QPushButton('poo', self)
        self.btn_cmds["poo"].clicked.connect(self._poo)
        self.btn_cmds["poo"].setEnabled(False)
        
        self.btn_cmds["retract"] = QPushButton('retract', self)
        self.btn_cmds["retract"].clicked.connect(self._retract)
        
        self.btn_cmds["rot_engage"] = QPushButton('engage rotator', self)
        self.btn_cmds["rot_engage"].clicked.connect(self._rot_engage) 
        
        self.btn_cmds["rot_retract"] = QPushButton('retract rotator', self)
        self.btn_cmds["rot_retract"].clicked.connect(self._rot_retract)
        
        self.armsReady = LedIndicator(self)
        self.armsTimeout = LedIndicator(self)
        self.armsError = LedIndicator(self)
        self.cylinderPresent = LedIndicator(self)
        
        self.states = QFormLayout()
        self.states.addRow("ready : ", self.armsReady)
        self.states.addRow("timeout : ", self.armsTimeout)
        self.states.addRow("error : ", self.armsError)
        self.states.addRow("cylinder : ", self.cylinderPresent)
        
        self.layout["Commands"] = QVBoxLayout(self)
        for button in self.btn_cmds:    
            self.layout["Commands"].addWidget(self.btn_cmds[button])
        self.layout["Commands"].addStretch()
        self.layout["Commands"].addLayout(self.states)
        
        self.box = QGroupBox("Arms")
        self.box.setLayout(self.layout["Commands"])
        
        self.layout = QHBoxLayout(self)
        self.layout.addWidget(self.box)
        self.layout.addStretch()
        
    def _start(self):
        print(": start")
        self.actCmd.emit(Types_pb2.AC_ARMS_START)

    def _swallow(self):
        print("Arms : swallow")
        self.actCmd.emit(Types_pb2.AC_ARMS_SWALLOW)

    def _dispenser(self):
        print("Arms : dispenser")
        self.actCmd.emit(Types_pb2.AC_ARMS_DISPENSER)        
        
        
    def _fastPoo(self):
        print("Arms : fastPoo")
        self.actCmd.emit(Types_pb2.AC_ARMS_FASTPOO)
        
    def _poo(self):
        print("NOT IMPLEMENTED")
        #print("Arms : poo") 
        #self.actCmd.emit(Types_pb2.AC_ARMS_POO) 
       
    def _retract(self):
        print("Arms : retract")
        self.actCmd.emit(Types_pb2.AC_ARMS_RETRACT)
        
    def _rot_engage(self):
        print("Arms : rotator engage")
        self.actCmd.emit(Types_pb2.AC_ARMS_ROT_ENGAGE)
        
    def _rot_retract(self):
        print("Arms : rotator retract")
        self.actCmd.emit(Types_pb2.AC_ARMS_ROT_RETRACT)
        
    def _stop(self):
        print("Arms : stop")
        self.actCmd.emit(Types_pb2.AC_ARMS_STOP)
        
        
class FunnyCmds(QWidget):   
    actCmd = pyqtSignal(int)
     
    def __init__(self, parent):
        super().__init__(parent)       
        
        self.btn_cmds = dict()
        self.layout=dict()
        
        self.btn_cmds["start"] = QPushButton('start', self)
        self.btn_cmds["start"].clicked.connect(self._start) 
        
        self.btn_cmds["stop"] = QPushButton('stop', self)
        self.btn_cmds["stop"].clicked.connect(self._stop)  
        
        self.btn_cmds["launch"] = QPushButton('launch', self)
        self.btn_cmds["launch"].clicked.connect(self._launch)  
        
        self.layout["Commands"] = QVBoxLayout(self)
        for button in self.btn_cmds:    
            self.layout["Commands"].addWidget(self.btn_cmds[button])
        self.layout["Commands"].addStretch()
        
        self.box = QGroupBox("Funny action")
        self.box.setLayout(self.layout["Commands"])
        
        self.layout = QHBoxLayout(self)
        self.layout.addWidget(self.box)
        self.layout.addStretch()
        
    def _start(self):
        print("Funny : start")
        self.actCmd.emit(Types_pb2.AC_FUNNY_START)

    def _stop(self):
        print("Funny : stop")
        self.actCmd.emit(Types_pb2.AC_FUNNY_STOP)
        
    def _launch(self):
        print("Funny : launch")
        self.actCmd.emit(Types_pb2.AC_FUNNY_LAUNCH)
                