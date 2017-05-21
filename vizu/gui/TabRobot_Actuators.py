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
        
        #create layouts objects
        self.layout = QVBoxLayout(self)
        self.layout.addWidget(self.sections["lifter"])
        self.layout.addStretch(1)
        
    @pyqtSlot(RemoteControl_pb2.Telemetry)     
    def _telemetryDataCb(self, msg):
        if self.isVisible():     
            if( msg.actuators.lifterReady):
                self.sections["lifter"].lifterReady.light(True)
            else:
                self.sections["lifter"].lifterReady.light(False)
            
            if( msg.actuators.omronCylinder):
                self.sections["lifter"].cylinderPresent.light(True)
            else:
                self.sections["lifter"].cylinderPresent.light(False)
        
class LifterCmds(QWidget):   
    actCmd = pyqtSignal(int)
     
    def __init__(self, parent):
        super().__init__(parent)       
        
        self.btn_cmds = dict()
        self.layout=dict()
        
        self.btn_cmds["lifter_start"] = QPushButton('Lifter start', self)
        self.btn_cmds["lifter_start"].clicked.connect(self._lifter_start) 
        
        self.btn_cmds["lifter_lift"] = QPushButton('Lifter lift', self)
        self.btn_cmds["lifter_lift"].clicked.connect(self._lifter_lift) 
        
        self.btn_cmds["lifter_fastPoo"] = QPushButton('Lifter fastPoo', self)
        self.btn_cmds["lifter_fastPoo"].clicked.connect(self._lifter_fastPoo) 
        
        self.btn_cmds["lifter_pooEnded"] = QPushButton('Lifter pooEnded', self)
        self.btn_cmds["lifter_pooEnded"].clicked.connect(self._lifter_pooEnded)
        
        self.btn_cmds["lifter_stop"] = QPushButton('Lifter stop', self)
        self.btn_cmds["lifter_stop"].clicked.connect(self._lifter_stop)  
        
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
        
    def _lifter_start(self):
        print("Lifter : start")
        self.actCmd.emit(Types_pb2.AC_LIFTER_START)

    def _lifter_lift(self):
        print("Lifter : lift")
        self.actCmd.emit(Types_pb2.AC_LIFTER_LIFT)
        
    def _lifter_fastPoo(self):
        print("Lifter : poo")
        self.actCmd.emit(Types_pb2.AC_LIFTER_FASTPOO)
        
    def _lifter_pooEnded(self):
        print("Lifter : pooEnded") 
        self.actCmd.emit(Types_pb2.AC_LIFTER_POOENDED) 
        
    def _lifter_stop(self):
        print("Lifter : stop")
        self.actCmd.emit(Types_pb2.AC_LIFTER_STOP)
        
                
                