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
        
        
    @pyqtSlot(RemoteControl_pb2.Telemetry)     
    def _telemetryDataCb(self, msg):
        if self.isVisible():     
            if( msg.actuators.lifterReady):
                self.sections["lifter"].lifterReady.light(True)
            else:
                self.sections["lifter"].lifterReady.light(False)
            
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
        
class LifterCmds(QWidget):   
    actCmd = pyqtSignal(int)
     
    def __init__(self, parent):
        super().__init__(parent)       
        
        self.btn_cmds = dict()
        self.layout=dict()
        
        self.btn_cmds["start"] = QPushButton('Lifter start', self)
        self.btn_cmds["start"].clicked.connect(self._start) 
        
        self.btn_cmds["lift"] = QPushButton('Lifter lift', self)
        self.btn_cmds["lift"].clicked.connect(self._lift) 
        
        self.btn_cmds["fastPoo"] = QPushButton('Lifter fastPoo', self)
        self.btn_cmds["fastPoo"].clicked.connect(self._fastPoo) 
        
        self.btn_cmds["poo"] = QPushButton('Lifter poo', self)
        self.btn_cmds["poo"].clicked.connect(self._poo)
        self.btn_cmds["poo"].setEnabled(False)
        
        self.btn_cmds["pooEnded"] = QPushButton('Lifter pooEnded', self)
        self.btn_cmds["pooEnded"].clicked.connect(self._pooEnded)
        
        self.btn_cmds["stop"] = QPushButton('Lifter stop', self)
        self.btn_cmds["stop"].clicked.connect(self._stop)  
        
        self.lifterReady = LedIndicator(self)
        self.cylinderPresent = LedIndicator(self)
        
        self.states = QFormLayout()
        self.states.addRow("ready : ", self.lifterReady)
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
        
    def _fastPoo(self):
        print("Lifter : poo")
        self.actCmd.emit(Types_pb2.AC_LIFTER_FASTPOO)
        
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
        
        self.btn_cmds["start"] = QPushButton('Arms start', self)
        self.btn_cmds["start"].clicked.connect(self._start) 
        
        self.btn_cmds["swallow"] = QPushButton('Arms swallow', self)
        self.btn_cmds["swallow"].clicked.connect(self._swallow) 
        
        self.btn_cmds["fastPoo"] = QPushButton('Arms fastPoo', self)
        self.btn_cmds["fastPoo"].clicked.connect(self._fastPoo) 
        
        self.btn_cmds["poo"] = QPushButton('Arms poo', self)
        self.btn_cmds["poo"].clicked.connect(self._poo)
        self.btn_cmds["poo"].setEnabled(False)
        
        self.btn_cmds["retract"] = QPushButton('Arms retract', self)
        self.btn_cmds["retract"].clicked.connect(self._retract)
        
        self.btn_cmds["stop"] = QPushButton('Arms stop', self)
        self.btn_cmds["stop"].clicked.connect(self._stop)  
        
        self.armsReady = LedIndicator(self)
        self.cylinderPresent = LedIndicator(self)
        
        self.states = QFormLayout()
        self.states.addRow("ready : ", self.armsReady)
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
        print("Arms : start")
        self.actCmd.emit(Types_pb2.AC_ARMS_START)

    def _swallow(self):
        print("Arms : swallow")
        self.actCmd.emit(Types_pb2.AC_ARMS_SWALLOW)
        
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
        
    def _stop(self):
        print("Arms : stop")
        self.actCmd.emit(Types_pb2.AC_ARMS_STOP)             
                