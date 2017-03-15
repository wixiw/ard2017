#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from ArdWidgets import *
from core import *

#
# This class is a pre-built widget which is designed to display 
# robot state related to onboard actuators/sensors.
#
class TabRobot(QWidget):
    
    getOsStatsLogs = pyqtSignal()
    getTelemetry = pyqtSignal()
    configureMatch = pyqtSignal(int, int)
    startMatch = pyqtSignal()
    resetCpu = pyqtSignal()
    
    def __init__(self):
        super().__init__()
        
        self.btns = dict()
        self.layout=dict()

        #Navigation order widgets   
        self.navTab = dict()
        self.navTab["setPosition"] = SetPosForm()
        self.navTab["requestGotoCap"] = GotoCapForm()
        self.navTab["requestGoto"] = GotoForm()
        self.navCombo = QComboBox(self)
        for tabName, tab in self.navTab.items():
            self.navCombo.addItem(tabName, tab)
        self.navCombo.setEditable(True)
        self.navCombo.lineEdit().setReadOnly(True)
        self.navCombo.lineEdit().setAlignment(Qt.AlignHCenter)
        self.navCombo.highlighted[int].connect(self._navCmdChanged)
        self.navCombo.currentIndexChanged[int].connect(self._navCmdChanged)
        
        #Buttons widgets
        self.btn_cmds = dict()
        self.btn_cmds["getOsStats"] = QPushButton('Get OS Stats', self)
        self.btn_cmds["getOsStats"].clicked.connect(self._getOsStatsLogs) 
        
        self.btn_cmds["getTelemetry"] = QPushButton('Get Telemetry', self)
        self.btn_cmds["getTelemetry"].clicked.connect(self._getTelemetry) 
        
        self.btn_cmds["configureMatch"] = QPushButton('Configure Match', self)
        self.btn_cmds["configureMatch"].clicked.connect(self._configureMatch) 
        
        self.btn_cmds["startMatch"] = QPushButton('Start Match', self)
        self.btn_cmds["startMatch"].clicked.connect(self._startMatch) 
        
        self.btn_cmds["resetCpu"] = QPushButton('Reset CPU', self)
        self.btn_cmds["resetCpu"].clicked.connect(self._resetCpu) 
        
        #create layouts objects
        self.layout["Top"] = QVBoxLayout(self)
        self.layout["NavOrder"] = QHBoxLayout()
        self.layout["Commands"] = QVBoxLayout()
        for button in self.btn_cmds:    
            self.layout["Commands"].addWidget(self.btn_cmds[button])
        self.layout["Commands"].addStretch()
        
        #populate layouts
        self.layout["NavOrder"].addWidget(self.navCombo)
        self.layout["NavStack"] = QStackedLayout()
        self.layout["NavOrder"].addLayout(self.layout["NavStack"])
        for tabName, tab in self.navTab.items():
            self.layout["NavStack"].addWidget(tab)
        self.layout["NavOrder"].addStretch(1)
        
        self.box_nav = QGroupBox("Navigation")
        self.box_nav.setLayout(self.layout["NavOrder"])
        
        self.box_cmd = QGroupBox("Commands")
        self.box_cmd.setLayout(self.layout["Commands"])
        
        self.layout["Top"].addWidget(self.box_nav)
        self.layout["Top"].addWidget(self.box_cmd)
        self.layout["Top"].addStretch(1)
        
    @pyqtSlot(int)
    def _navCmdChanged(self, comboId):
        self.navTab[self.navCombo.currentText()].reset()
        for name, widget in self.navTab.items():
            widget.reset()
        self.layout["NavStack"].setCurrentIndex(comboId)
        
    @pyqtSlot()
    def _getOsStatsLogs(self): 
       print("Stats request")
       self.getOsStatsLogs.emit()
       
    @pyqtSlot()
    def _getTelemetry(self): 
       print("Telemetry request")
       self.getTelemetry.emit()
       
    @pyqtSlot()
    def _configureMatch(self): 
       print("Configure match request")
       self.configureMatch.emit(0, 2)
       
    @pyqtSlot()
    def _startMatch(self): 
       print("Start match request")
       self.startMatch.emit()
       
    @pyqtSlot()
    def _resetCpu(self): 
       print("Reset CPU request")
       self.resetCpu.emit()
   
   
class SetPosForm(QWidget):
    #signal(pose)
    execute = pyqtSignal(Pose2D)
                        
    def __init__(self):
        super().__init__()
        self.x = IntegerInput(self, -2000, 2000)
        self.y = IntegerInput(self, -1500, 1500)
        self.h = HeadingInput(self)
        self.exe = QPushButton('Execute', self)
        self.exe.clicked.connect(self._execute) 
        
        layout = QHBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.exe)
        layoutForm.addRow("x (mm)", self.x)
        layoutForm.addRow("y (mm)", self.y)
        layoutForm.addRow("h (°)", self.h)
    
    def reset(self):
        self.x.clear()
        self.y.clear()
        self.h.clear()
        
    @pyqtSlot()
    def _execute(self):
        self.execute.emit(Pose2D(self.x.getValue(), self.y.getValue(), self.h.getValue()))
        
class GotoCapForm(QWidget):
    #signal(pose,dir)
    execute = pyqtSignal(Pose2D, int)
                        
    def __init__(self):
        super().__init__()
        self.x = IntegerInput(self, -2000, 2000)
        self.y = IntegerInput(self, -1500, 1500)
        self.h = HeadingInput(self)
        self.dir = DirectionInput(self)
        self.exe = QPushButton('Execute', self)
        self.exe.clicked.connect(self._execute) 
        
        layout = QHBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.exe)
        layoutForm.addRow("x (mm)", self.x)
        layoutForm.addRow("y (mm)", self.y)
        layoutForm.addRow("h (°)", self.h)
        layoutForm.addRow("dir", self.dir)
        
    def reset(self):
        self.x.clear()
        self.y.clear()
        self.h.clear()
        
    @pyqtSlot()
    def _execute(self):
        self.execute.emit(Pose2D(self.x.getValue(), self.y.getValue(), self.h.getValue()), self.dir.getValue())
        
class GotoForm(QWidget):
    #signal(point, dir)
    execute = pyqtSignal(Point, int)
    
    def __init__(self):
        super().__init__()
        self.x = IntegerInput(self, -2000, 2000)
        self.y = IntegerInput(self, -1500, 1500)
        self.dir = DirectionInput(self)
        self.exe = QPushButton('Execute', self)
        self.exe.clicked.connect(self._execute) 
        
        layout = QHBoxLayout(self)
        layoutForm = QFormLayout()
        layout.addLayout(layoutForm)
        layout.addWidget(self.exe)
        layoutForm.addRow("x (mm)", self.x)
        layoutForm.addRow("y (mm)", self.y)
        layoutForm.addRow("dir", self.dir)

    def reset(self):
        self.x.clear()
        self.y.clear()
        
    @pyqtSlot()
    def _execute(self):
        self.execute.emit(Point(self.x.getValue(), self.y.getValue()), self.dir.getValue())
        
        
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    widget = TabRobot()
    widget.show()
    sys.exit(app.exec_())
    
    
    
    