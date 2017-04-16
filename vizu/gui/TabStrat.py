#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from core import *
from TableDrawing import *
import math


# This class is a pre-built widget which is designed to display 
# strategy state and a 2D table view with a moving robot.
#
class TabStrat(QWidget):
    
    getConfig = pyqtSignal()
    
    #@param robot : the prowy providing telemetry data
    def __init__(self, parent = None):
        super().__init__(parent)
        
        self.overview = TableOverview(self)
        
        #build info tab
        self.label = dict()
        self.buildGeneralInfo()
        self.buildPosInfo()
        self.buildMotionInfo()
        self.buildStratInfo()
        self.layoutInfo = QVBoxLayout()
        self.layoutInfo.addWidget(self.box_general)
        self.layoutInfo.addWidget(self.box_pos)
        self.layoutInfo.addWidget(self.box_motion)
        self.layoutInfo.addWidget(self.box_stratInfo)
        self.layoutInfo.addSpacerItem(QSpacerItem(175,0,QSizePolicy.Minimum,QSizePolicy.Expanding))
        
        self.layout = QHBoxLayout(self)
        self.layout.addLayout(self.layoutInfo)
        self.layout.addWidget(self.overview)
        
        self.robotConfig = None
        self.robotState = RemoteControl_pb2.Telemetry()
        
    def updateRobot(self, name):
        if name == "Pen":
            self.overview.robot = self.overview.robotPen
        elif name == "Tration":
            self.overview.robot = self.overview.robotTration
        else:
            self.overview.robot = None
            self.robotConfig = None
            print("[ERROR] TabStrat.updateRobot : unknown name " + name)
        
    def buildGeneralInfo(self):
        self.box_general = QGroupBox("General")
        self.label["bootTime"] = QLabel("0")
        self.label["bootTime"].setAlignment(Qt.AlignRight)
        box_layout = QFormLayout()
        box_layout.addRow("boot time (s): ", self.label["bootTime"])
        self.box_general.setLayout(box_layout)
            
    def buildPosInfo(self):
        self.box_pos = QGroupBox("Position")
        self.label["x"] = QLabel("0")
        self.label["y"] = QLabel("0")
        self.label["h"] = QLabel("0")
        self.label["x"].setAlignment(Qt.AlignRight)
        self.label["y"].setAlignment(Qt.AlignRight)
        self.label["h"].setAlignment(Qt.AlignRight)
        box_layout = QFormLayout()
        box_layout.addRow("x (mm) : ", self.label["x"])
        box_layout.addRow("y (mm) : ", self.label["y"])
        box_layout.addRow("h (°)  : ", self.label["h"])
        self.box_pos.setLayout(box_layout)
        
    def buildMotionInfo(self):
        self.box_motion = QGroupBox("Motion")
        self.label["state"] = QLabel("?")
        self.label["order"] = QLabel("?")
        self.label["state"].setAlignment(Qt.AlignRight)
        self.label["order"].setAlignment(Qt.AlignRight)
        box_layout = QFormLayout()
        box_layout.addRow("state : ", self.label["state"])
        box_layout.addRow("order : ", self.label["order"])
        self.box_motion.setLayout(box_layout)
        
    def buildStratInfo(self):
        self.box_stratInfo = QGroupBox("Strat Info")
        self.label["stock"] = QLabel("?")
        self.label["stock"].setAlignment(Qt.AlignRight)
        box_layout = QFormLayout()
        box_layout.addRow("stock : ", self.label["stock"])
        self.box_stratInfo.setLayout(box_layout)
       
    #@return Pose2D : the last received telemetry position
    def getRobotPosition(self):
        return Pose2D.fromPoseMsg(self.robotState.nav.pos)
    
    def getMotionStateStr(self):
        return Types_pb2.eNavState.Name(self.robotState.nav.state)

    def getMotionOrderStr(self):
        return Types_pb2.eNavOrder.Name(self.robotState.nav.order)
    
    def getObjectColorStr(self):
        return Types_pb2.eObjectColor.Name(self.robotState.actuators.colorSensor.color)
    
    def paintEvent(self, event):
        if self.robotConfig == None:
            #print("Requesting current config")
            self.getConfig.emit()    #telemetry reply data callback
        else:
            super().paintEvent(event)
        
        
    @pyqtSlot(RemoteControl_pb2.Telemetry)     
    def _telemetryDataCb(self, msg):
        if self.isVisible():
            #--- DEBUG --- print("Telemetry received.")
            #--- DEBUG --- print(str(msg))
            self.robotState = msg
            pose = self.getRobotPosition()
            color = self.robotState.actuators.colorSensor
            colorStr = self.getObjectColorStr()
            self.overview.robotPose = pose
            self.label["bootTime"].setText("%0.1f" % (self.robotState.date/1000.))
            self.label["x"].setText("%0.0f" %pose.x)
            self.label["y"].setText("%0.0f" %pose.y)
            self.label["h"].setText("%0.0f" % math.degrees(pose.h))
            self.label["state"].setText(self.getMotionStateStr())
            self.label["order"].setText(self.getMotionOrderStr())
            self.label["stock"].setText(str(msg.stratInfo.robotCylinderStockNb))
            self.update()

    @pyqtSlot(RemoteControl_pb2.Configuration)    
    def _updateConfig(self, msg):
        #print("New config received from robot, updating tab")
        self.robotConfig = msg

