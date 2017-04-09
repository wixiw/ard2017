#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from ArdWidgets import *
from core import *

class TabRobot_Status(QWidget):
        
    def __init__(self, parent):
        super().__init__(parent)
        self.robotState = RemoteControl_pb2.Telemetry()
        
        self.layout    = QHBoxLayout(self)
        self.columns = dict()
        
        self.columns[0] = QFormLayout()
        self.columns[1] = QFormLayout()
        self.columns[2] = QFormLayout()
        self.columns[3] = QFormLayout()
        for key, column in self.columns.items():
            self.layout.addLayout(column)
            column.setLabelAlignment(Qt.AlignVCenter)
        self.layout.addStretch()
        
        #self.addSensorXor("led1", 0) #LED1 is currently driven as ledDue_Rx
        self.addSensorXor("led2", 0)
        self.addSensorXor("led3", 0)
        self.addSensorXor("led4", 0)
        self.addSensorXor("ledRgb", 0)
        self.addSensorXor("ledDue_L", 0)
        self.addSensorXor("ledDue_Tx", 0)
        #self.addSensorXor("ledDue_Rx", 0) #This led is only light on when CPU is Idle ... hence it's stupid to display it's state which will always be off
        
        self.addSensorXor("switchArmLout", 1)
        self.addSensorXor("switchArmLin", 1)
        self.addSensorXor("switchRecalFL", 1)
        self.addSensorXor("omronFL", 1)
        self.addSensorXor("omronRL", 1)
        self.addSensorXor("start", 1)
        self.addSensorXor("colorSwitch", 1)

        
        self.addSensorXor("switchArmRout", 2)
        self.addSensorXor("switchArmRin", 2)
        self.addSensorXor("switchRecalFR", 2)
        self.addSensorXor("omronFR", 2)
        self.addSensorXor("omronRR", 2)
        self.addSensorXor("user1", 2)
        self.addSensorXor("user2", 2)
        
        self.addSensorXor("switchRecalRC", 3)
        self.addSensorXor("omronCylinder", 3)
        self.addSensorXor("switchCylinder", 3)
        self.addSensorXor("omronSpare", 3)
        self.addSensorXor("switchLifterUp", 3)
        self.addSensorXor("switchLifterDown", 3)
        
    def addSensorXor(self, name, column = 0):
        setattr(self, name, LedIndicator(self))
        self.columns[column].addRow(name,getattr(self, name))
        
    #telemetry reply data callback
    @pyqtSlot(RemoteControl_pb2.Telemetry)     
    def _telemetryDataCb(self, msg):
        if self.isVisible():
            #--- DEBUG --- print("Telemetry received.")
            #--- DEBUG --- print(str(msg))
            self.robotState = msg
            
            #self.led1.light(msg.hmi.led1) #LED1 is currently driven as ledDue_Rx
            self.led2.light(msg.hmi.led2)
            self.led3.light(msg.hmi.led3)
            self.led4.light(msg.hmi.led4)
            self.ledRgb.light(msg.hmi.ledRgb)
            self.ledDue_L.light(msg.hmi.ledDue_L)
            self.ledDue_Tx.light(msg.hmi.ledDue_Tx)
            #self.ledDue_Rx.light(msg.hmi.ledDue_Rx) #This led is only light on when CPU is Idle ... hence it's stupid to display it's state which will always be off
            
            self.start.light(msg.hmi.start)
            self.colorSwitch.light(msg.hmi.colorSwitch)
            self.user1.light(msg.hmi.user1)
            self.user2.light(msg.hmi.user2)
            
            self.omronFL.light(msg.nav.omronFL)
            self.omronFR.light(msg.nav.omronFR)
            self.omronRL.light(msg.nav.omronRL)
            self.omronRR.light(msg.nav.omronRR)
            self.switchRecalFL.light(msg.nav.switchRecalFL)
            self.switchRecalFR.light(msg.nav.switchRecalFR)
            self.switchRecalRC.light(msg.nav.switchRecalRC)
            
            self.switchArmLout.light(msg.actuators.switchArmLout)
            self.switchArmLin.light(msg.actuators.switchArmLin)
            self.switchArmRout.light(msg.actuators.switchArmRout)
            self.switchArmRin.light(msg.actuators.switchArmRin)
            self.omronCylinder.light(msg.actuators.omronCylinder)
            self.switchCylinder.light(msg.actuators.switchCylinder)
            self.omronSpare.light(msg.actuators.omronSpare)
            self.switchLifterUp.light(msg.actuators.switchLifterUp)
            self.switchLifterDown.light(msg.actuators.switchLifterDown)
            
            self.update()
   