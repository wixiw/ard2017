#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from ArdWidgets import *
from core import *
from TableDrawing import *
import math
import _tkinter


# This class is a pre-built widget which is designed to display 
# strategy state and a 2D table view with a moving robot.
#
class TabStrat(QWidget):
    
    #@param robot : the prowy providing telemetry data
    def __init__(self, parent, teleop, robotProxy, clipboard):
        super().__init__(parent)
        self.teleop = teleop
        self.overview = TableOverview(self, robotProxy)
        self.robotConfig = None
        self.robotState = RemoteControl_pb2.Telemetry()
        self.uiStarted = False
        self.uiColor = Types_pb2.UNKNOWN
        self.uiStrategy = 0 
        self.stratCount = 0
        self.actuatorsOut = False
        self.mire = False
        self.clipboard = clipboard
        
        #build info tab
        self.label = dict()
        self.cbox = dict()
        self.buildGeneralInfo()
        self.buildPosInfo()
        self.buildMotionInfo()
        self.buildStratInfo()
        self.buildViewConfigInfo()
        self.buildGhostInfo()
        self.layoutInfo = QVBoxLayout()
        self.layoutInfo.addWidget(self.box_general)
        self.layoutInfo.addWidget(self.box_pos)
        self.layoutInfo.addWidget(self.box_motion)
        self.layoutInfo.addWidget(self.box_stratInfo)
        self.layoutInfo.addWidget(self.box_viewConf)
        self.layoutInfo.addWidget(self.box_ghost)
        self.layoutInfo.addSpacerItem(QSpacerItem(175,0,QSizePolicy.Minimum,QSizePolicy.Expanding))
        
        self.layout = QHBoxLayout(self)
        self.layout.addLayout(self.layoutInfo)
        self.layout.addWidget(self.overview)
        

        
    def updateRobot(self, name):
        if name == "Pen":
            self.overview.robot = self.overview.robotPen
            self._setColorButtonState(Types_pb2.UNKNOWN)
            self._setStartButtonState("color") 
        elif name == "Tration":
            self.overview.robot = self.overview.robotTration
            self._setColorButtonState(Types_pb2.UNKNOWN)
            self._setStartButtonState("color") 
        else:
            self.overview.robot = None
            self.robotConfig = None
            print("[ERROR] TabStrat.updateRobot : unknown name " + name)
        
    def buildGeneralInfo(self):
        self.box_general = QGroupBox("General")
                
        #Strategy choice
        self.comboStratId = QComboBox(self)
        self.addStrategy("Match")
        self.addStrategy("Homol")
        self.addStrategy("Selftest")
        self.addStrategy("WIP")
        self.addStrategy("Old Tanguy")
        self.addStrategy("UT LEDs")
        self.addStrategy("UT Button")
        self.addStrategy("UT Omron")
        self.addStrategy("UT CalibRot")
        self.addStrategy("UT CalibLin")
        self.addStrategy("UT Motion")
        self.comboStratId.currentIndexChanged[int].connect(self.selectStrat)
            #retrieve persisted choice
        settings = QSettings("config.ini", QSettings.IniFormat)
        settings.beginGroup("Strat")
        self.uiStrategy = int(settings.value("id", 0))
        self.comboStratId.setCurrentIndex(self.uiStrategy)
        settings.endGroup()
        
        #Color button
        self.buttonColor = QPushButton()
        self._setColorButtonState(Types_pb2.UNKNOWN)
        self.buttonColor.clicked.connect(self._colorSwitched)
        
        #Start button
        self.buttonStart = QPushButton() 
        self._setStartButtonState("color")    
        self.buttonStart.clicked.connect(self._startPressed)
        
        #Labels
        self.label["bootTime"] = QLabel("0")
        self.label["bootTime"].setAlignment(Qt.AlignRight)
        self.label["chronoMatch"] = QLabel("0")
        self.label["chronoMatch"].setAlignment(Qt.AlignRight)
        self.label["timeLeft"] = QLabel("0")
        self.label["timeLeft"].setAlignment(Qt.AlignRight)
        box_layout = QFormLayout()
        box_layout.addRow("boot time (s): ", self.label["bootTime"])
        box_layout.addRow("chrono match (s): ", self.label["chronoMatch"])
        box_layout.addRow("time left (s): ", self.label["timeLeft"])
        box_layout.addRow("strategy: ", self.comboStratId)
        box_layout.addRow("color: ", self.buttonColor)
        box_layout.addRow("start: ", self.buttonStart)
        self.box_general.setLayout(box_layout)
        
    def addStrategy(self, name):
        self.comboStratId.addItem(str(self.stratCount) + " - " + name, self.stratCount)
        self.stratCount = self.stratCount + 1
        
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
        self.label["score"] = QLabel("?")
        self.label["stock"] = QLabel("?")
        self.label["content"] = QLabel("?")
        self.label["score"].setAlignment(Qt.AlignRight)
        self.label["stock"].setAlignment(Qt.AlignRight)
        self.label["content"].setAlignment(Qt.AlignRight)
        box_layout = QFormLayout()
        box_layout.addRow("score : ", self.label["score"])
        box_layout.addRow("stock : ", self.label["stock"])
        box_layout.addRow("content : ", self.label["content"])
        self.box_stratInfo.setLayout(box_layout)
       
    def buildViewConfigInfo(self):
        self.box_viewConf = QGroupBox("View config")
        self.cbox["actuators out"] = QCheckBox()
        self.cbox["mire"] = QCheckBox()
        self.cbox["traj"] = QCheckBox()
        self.cbox["graph"] = QCheckBox()
        box_layout = QFormLayout()
        box_layout.addRow("actuators out : ", self.cbox["actuators out"])
        box_layout.addRow("mire : ", self.cbox["mire"])
        box_layout.addRow("traj : ", self.cbox["traj"])
        box_layout.addRow("graph : ", self.cbox["graph"])
        self.box_viewConf.setLayout(box_layout)
        self.cbox["actuators out"].stateChanged.connect(self._actuatorsChecked)
        self.cbox["mire"].stateChanged.connect(self._mireChecked) #nearly milk-shake ^^ oOooO
        self.cbox["traj"].stateChanged.connect(self._trajChecked)
        self.cbox["graph"].stateChanged.connect(self._graphChecked)
       
    def buildGhostInfo(self):
        self.box_ghost = QGroupBox("Ghost")
        self.label["xg"] = QLabel("0")
        self.label["yg"] = QLabel("0")
        self.label["hg"] = QLabel("0")
        self.label["xg"].setAlignment(Qt.AlignRight)
        self.label["yg"].setAlignment(Qt.AlignRight)
        self.label["hg"].setAlignment(Qt.AlignRight)
        box_layout = QFormLayout()
        box_layout.addRow("x (mm) : ", self.label["xg"])
        box_layout.addRow("y (mm) : ", self.label["yg"])
        box_layout.addRow("h (°)  : ", self.label["hg"])
        self.box_ghost.setLayout(box_layout)
       
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
            self.teleop.getConfig()    #telemetry reply data callback
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
            chrono = self.robotState.chrono.chrono_ms/1000.
            self.overview.robotPose = pose
            self.label["bootTime"].setText("%0.1f" % (self.robotState.date/1000.))
            self.label["chronoMatch"].setText("%0.1f" % (chrono))
            self.label["timeLeft"].setText("%0.1f" % (self.robotState.chrono.timeLeft_ms/1000.))
            self.label["x"].setText("%d" %pose.x)
            self.label["y"].setText("%d" %pose.y)
            self.label["h"].setText("%d" % math.degrees(pose.h))
            self.label["state"].setText(self.getMotionStateStr())
            self.label["order"].setText(self.getMotionOrderStr())
            self.label["score"].setText(str(msg.stratInfo.score))
            self.label["stock"].setText(str(len(msg.stratInfo.stock)))
            stock = "h|"
            for c in msg.stratInfo.stock:
                if c == Types_pb2.BICOLOR:
                    stock = stock + "b" + "|"
                elif c == Types_pb2.MONOCOLOR:
                    stock = stock + "m" + "|"
                else:
                    stock = stock + "-" + "|"
            stock = stock + "l"
            self.label["content"].setText(stock)
            
            if chrono > 90.0:
                self.label["chronoMatch"].setStyleSheet("QLabel { color: red }")
            else:
                self.label["chronoMatch"].setStyleSheet("QLabel { color: black }")
            
            self.update()

    @pyqtSlot(RemoteControl_pb2.GraphState)     
    def _updateGraphState(self, msg):
        self.overview.graphState = msg
        #print("New graph state received")
        #print(msg)
        
    @pyqtSlot(RemoteControl_pb2.GraphNodes)     
    def _updateGraphNodes(self, msg):
        self.overview.graphNodes = msg
        #print("New graph nodes received")
        #print(msg)

    @pyqtSlot(RemoteControl_pb2.GraphLinks)     
    def _updateGraphLinks(self, msg):
        self.overview.graphLinks = msg
        #print("New graph links received")
        #print(msg)

    @pyqtSlot(RemoteControl_pb2.Configuration)    
    def _updateConfig(self, msg):
        #print("New config received from robot, updating tab")
        self.robotConfig = msg

    @pyqtSlot(int)
    def _actuatorsChecked(self, state):
        if state == Qt.Checked:
            self.overview.setActuators(True)
        else:
            self.overview.setActuators(False)
            
    @pyqtSlot(int)
    def _mireChecked(self, state):
        if state == Qt.Checked:
            self.overview.setMire(True)
        else:
            self.overview.setMire(False)

    @pyqtSlot(int)
    def _trajChecked(self, state):
        if state == Qt.Checked:
            self.overview.drawTraj = True;
        else:
            self.overview.drawTraj = False;

    @pyqtSlot(int)
    def _graphChecked(self, state):
        if state == Qt.Checked:
            self.overview.drawGraph = True;
            self.teleop.graphActive = True
        else:
            self.overview.drawGraph = False;
            self.teleop.graphActive = False

    @pyqtSlot(int)
    def selectStrat(self, comboId):
        print("New strategy ID selected : " + str(comboId))
        self.uiStrategy = comboId
        settings = QSettings("config.ini", QSettings.IniFormat)
        settings.beginGroup("Strat")
        settings.setValue("id", comboId)
        settings.endGroup()
        
    @pyqtSlot(int)
    def setColor(self, color):
        if color == Types_pb2.PREF:
            print("Match color configured to Prefered (YELLOW) and strategy #" + str(self.uiStrategy))
            self.teleop.configureMatch(self.uiStrategy, Types_pb2.PREF)
            self._setColorButtonState(Types_pb2.PREF)
            self._setStartButtonState("go")
        elif color == Types_pb2.SYM:
            print("Match color configured to Symetric (BLUE) and strategy #" + str(self.uiStrategy))
            self.teleop.configureMatch(self.uiStrategy, Types_pb2.SYM)
            self._setColorButtonState(Types_pb2.SYM)
            self._setStartButtonState("go")
        else:
            assert False
    
    @pyqtSlot()
    def startMatch(self):
        if self.uiColor != Types_pb2.UNKNOWN:
            print("Match start request")  
            self._setColorButtonState(4)
            self._setStartButtonState("reset")
            self.teleop.startMatch()
        else:
            print("Match start request ignored as color is not choosed")
         
    @pyqtSlot()   
    def resetCpu(self):   
        print("Reset CPU request")
        self._setColorButtonState(Types_pb2.UNKNOWN)
        self._setStartButtonState("color")
        self.teleop.resetCpu() 
            
    @pyqtSlot(bool)
    def _colorSwitched(self, pressed):
        if self.uiColor == Types_pb2.UNKNOWN or self.uiColor == Types_pb2.SYM:
            self.setColor(Types_pb2.PREF)
        else:
            self.setColor(Types_pb2.SYM)
        
    @pyqtSlot(bool)
    def _startPressed(self, pressed):
        if self.uiColor != Types_pb2.UNKNOWN and not self.uiStarted:
            self.startMatch()
        elif self.uiStarted:
            self.resetCpu()
        
    @pyqtSlot()
    def copyGhostToClipboard(self):
        print("CGTC : " + str(self.overview.ghost.pose))
        self.clipboard.setText("%d, %d, %d" 
                               %(self.overview.ghost.pose.x, 
                                 self.overview.ghost.pose.y, 
                                 math.degrees(self.overview.ghost.pose.h)))
        
    def _setColorButtonState(self, color):
        if color == Types_pb2.UNKNOWN:
            self.uiColor = Types_pb2.UNKNOWN
            self.buttonColor.setDisabled(False)
            self.buttonColor.setText("Unknown")
            self.buttonColor.setStyleSheet("background: #BDBAAB")
            
        elif color == Types_pb2.PREF:
            self.uiColor = Types_pb2.PREF
            self.buttonColor.setDisabled(False)
            self.buttonColor.setText("PREF")
            self.buttonColor.setStyleSheet("background: #FCBD1F")
            
        elif color == Types_pb2.SYM:
            self.uiColor = Types_pb2.SYM
            self.buttonColor.setDisabled(False)
            self.buttonColor.setText("SYM")
            self.buttonColor.setStyleSheet("background: #1761AB; color: #FFFFFF")
            
        elif color == 4:#disabled during match
            self.buttonColor.setDisabled(True)
            self.buttonColor.setText("running")
        else:
           assert False 
            
        
    def _setStartButtonState(self, state):
        if state == "go":
            self.uiStarted = False
            self.buttonStart.setDisabled(False)
            self.comboStratId.setEnabled(False)
            self.buttonStart.setText("GO !") 
        elif state == "reset":
            self.uiStarted = True
            self.buttonStart.setDisabled(False)
            self.comboStratId.setEnabled(False)
            self.buttonStart.setText("reset")
        elif state == "color":
            self.uiStarted = False
            self.buttonStart.setDisabled(True)
            self.comboStratId.setEnabled(True)
            self.buttonStart.setText("color missing")
        else:
            assert False